//---------------------------------------------------------------
//
// LuaJoint Plugin
//
// Copyright (c) 2013 Kazuhiko Arase
//
// URL: http://code.google.com/p/luajoint/
//
// Licensed under the MIT license:
//   http://www.opensource.org/licenses/mit-license.php
//
//---------------------------------------------------------------

#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "sxsdk.cxx"


using namespace sxsdk::enums;


static const sx::uuid_class LUA_JOINT_ID("4F72050B-B5F1-4800-9C56-35A7CDE0424E");

static const char* LUA_JOINT_PREFIX = "lua";

static const char* __SHADE_PTR_NAME__ = "__SHADE_PTR_NAME__";

static const char* __SCENE_PTR_NAME__ = "__SCENE_PTR_NAME__";

static const char* __CURRENT_PART_PTR_NAME__ = "__CURRENT_PART_PTR_NAME__";


static bool is_lua_joint(sxsdk::part_class* part);

static sxsdk::part_class* get_parent_lua_joint(sxsdk::part_class* part, int parent_index);

static int lua_print(lua_State* L);

static int lua_set_current_joint_value(lua_State* L);

static int lua_get_current_joint_value(lua_State* L);

static int lua_get_joint_position(lua_State* L);


class lua_joint_plugin : public sxsdk::plugin_interface {

public:

	lua_joint_plugin(sxsdk::shade_interface* shade) : _shade(shade) { }

	virtual ~lua_joint_plugin() { }

private:

	sxsdk::shade_interface* _shade;

	virtual sx::uuid_class get_uuid(void *) { return LUA_JOINT_ID; }

	virtual int get_shade_version() const { return SHADE_BUILD_NUMBER; }

	virtual void do_it(sxsdk::shade_interface *shade, sxsdk::scene_interface *scene, void *);

	virtual void current_joint_value_changing(bool &,
		sxsdk::scene_interface *scene, int n, sxsdk::part_class *const *parts, void *);

	virtual void current_joint_value_changed(bool &,
		sxsdk::scene_interface *scene, int n, sxsdk::part_class *const *parts, void *);

	virtual void sequence_changed(bool &,
		float sequence,
		sxsdk::scene_interface *scene, int n, sxsdk::part_class *const *parts, void*);

	virtual void exec_joints(sxsdk::scene_interface *scene, int n, sxsdk::part_class *const *parts);

	virtual void exec_sub_joints(lua_State* L, sxsdk::scene_interface *scene, sxsdk::part_class* part);

	virtual int do_lua_string(lua_State* L, sxsdk::scene_interface *scene, sxsdk::part_class* part);
};


void lua_joint_plugin::do_it(sxsdk::shade_interface *shade, sxsdk::scene_interface *scene, void *) {
}

void lua_joint_plugin::current_joint_value_changing(
	bool &,
	sxsdk::scene_interface *scene, int n, sxsdk::part_class *const *parts, void *
) {
	exec_joints(scene, n, parts);
}

void lua_joint_plugin::current_joint_value_changed(
	bool &,
	sxsdk::scene_interface *scene, int n, sxsdk::part_class *const *parts, void *
) {
	exec_joints(scene, n, parts);
}

void lua_joint_plugin::sequence_changed(
	bool &,
	float sequence,
	sxsdk::scene_interface *scene, int n, sxsdk::part_class *const *parts, void *
) {
	exec_joints(scene, n, parts);
}

void lua_joint_plugin::exec_joints(sxsdk::scene_interface *scene, int n, sxsdk::part_class *const *parts) {

	lua_State* L = lua_open();

	// setup math library
	lua_pushcfunction(L, luaopen_math);
	lua_call(L, 0, 0);

	// setup functions
	lua_register(L, "print", lua_print);
	lua_register(L, "set_v", lua_set_current_joint_value);
	lua_register(L, "get_v", lua_get_current_joint_value);
	lua_register(L, "get_p", lua_get_joint_position);

	// setup globals
	lua_pushlightuserdata(L, (void*)_shade);
	lua_setglobal(L, __SHADE_PTR_NAME__);
	lua_pushlightuserdata(L, (void*)scene);
	lua_setglobal(L, __SCENE_PTR_NAME__);

	for (int i = 0; i < n; i++) {

		if (!is_lua_joint(parts[i]) ) {
			continue;
		}

		exec_sub_joints(L, scene, parts[i]);
	}

	lua_close(L);
}

void lua_joint_plugin::exec_sub_joints(lua_State* L, sxsdk::scene_interface *scene, sxsdk::part_class* part) {

	for (sxsdk::shape_class::iterator it = part->begin(); it != part->end(); it++) {

		if (it->get_type() != sxsdk::enums::part) {
			continue;
		}

		sxsdk::part_class* sub_part = (sxsdk::part_class*)(sxsdk::shape_class*)it;

		if (!is_lua_joint(sub_part) ) {
			continue;
		}

		int opt = sub_part->get_name()[strlen(LUA_JOINT_PREFIX)];

		bool callAfterChildren = (opt == '~');

		if (!callAfterChildren) {
			if (!do_lua_string(L, scene, sub_part) ) {
				continue;
			}
		}

		exec_sub_joints(L, scene, sub_part);

		if (callAfterChildren) {
			if (!do_lua_string(L, scene, sub_part) ) {
				continue;
			}
		}
	}
}

int lua_joint_plugin::do_lua_string(lua_State* L, sxsdk::scene_interface *scene, sxsdk::part_class* part) {

	const char* luaString = part->get_name();

	const char* luaStrPos = strchr(luaString, ':');
	if (luaStrPos == NULL) {
		// NOP
		return 1;
	}

	luaStrPos++;

	// setup current part
	lua_pushlightuserdata(L, (void*)part);
	lua_setglobal(L, __CURRENT_PART_PTR_NAME__);

	int ret = 0;

	if (luaL_dostring(L, luaStrPos) == 0) {

		ret = 1;

	} else {

		// error message
		if (!scene->is_rendering_thread() ) {
			_shade->message(part->get_name() );
			_shade->message(lua_tostring(L, 1) );
		}
	}

	// clear stack.
	lua_settop(L, 0);

	return ret;
}


bool is_lua_joint(sxsdk::part_class* part) {

	switch(part->get_part_type() ) {

	case sxsdk::enums::rotator_joint :
	case sxsdk::enums::slider_joint :
	case sxsdk::enums::scale_joint :
	case sxsdk::enums::uniscale_joint :
	case sxsdk::enums::light_joint :
	case sxsdk::enums::path_joint :
	case sxsdk::enums::morph_joint :
	case sxsdk::enums::custom_joint :
		break;

	default :
		// unsupported part type
		return false;
	}

	if (strncmp(LUA_JOINT_PREFIX, part->get_name(), strlen(LUA_JOINT_PREFIX) ) != 0) {
		// prefix unmatch
		return false;
	}

	return true;
}

sxsdk::part_class* get_parent_lua_joint(sxsdk::part_class* part, int parent_index) {

	while (parent_index-- > 0) {

		part = part->get_dad();

		if (part == NULL) {
			return NULL;
		}

		if (!is_lua_joint(part) ) {
			return NULL;
		}
	}

	return part;
}

int lua_print(lua_State* L) {

	const char* message = lua_tostring(L, 1);

	lua_getglobal(L, __SHADE_PTR_NAME__);
	sxsdk::shade_interface* shade = (sxsdk::shade_interface*)lua_touserdata(L, -1);
	lua_getglobal(L, __SCENE_PTR_NAME__);
	sxsdk::scene_interface* scene = (sxsdk::scene_interface*)lua_touserdata(L, -1);

	if (!scene->is_rendering_thread() ) {
		shade->message(message);
	}

	return 0;
}

int lua_get_current_joint_value(lua_State* L) {

	int parent_index = lua_tointeger(L, 1);

	lua_getglobal(L, __CURRENT_PART_PTR_NAME__);
	sxsdk::part_class* part = (sxsdk::part_class*)lua_touserdata(L, -1);

	part = get_parent_lua_joint(part, parent_index);

	if (part == NULL) {
		lua_pushnumber(L, 0);
		return 1;
	}

	compointer<sxsdk::motion_interface> motion(part->get_motion_interface() );

	lua_pushnumber(L, motion->get_current_joint_value() );

	return 1;
}

int lua_set_current_joint_value(lua_State* L) {

	double value = lua_tonumber(L, 1);

	lua_getglobal(L, __CURRENT_PART_PTR_NAME__);
	sxsdk::part_class* part = (sxsdk::part_class*)lua_touserdata(L, -1);
	compointer<sxsdk::motion_interface> motion(part->get_motion_interface() );

	motion->set_current_joint_value(value);

	return 0;
}

int lua_get_joint_position(lua_State* L) {

	int parent_index = (int)lua_tonumber(L, 1);

	lua_getglobal(L, __CURRENT_PART_PTR_NAME__);
	sxsdk::part_class* part = (sxsdk::part_class*)lua_touserdata(L, -1);

	part = get_parent_lua_joint(part, parent_index);

	if (part == NULL) {
		lua_pushnumber(L, 0);
		return 1;
	}

	compointer<sxsdk::motion_interface> motion(part->get_motion_interface() );
 
	sxsdk::vec3 pos = motion->get_position() * part->get_local_to_world_matrix();

	lua_createtable(L, 0, 3);

	lua_pushstring(L, "x");
	lua_pushnumber(L, pos.x);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, pos.y);
	lua_settable(L, -3);

	lua_pushstring(L, "z");
	lua_pushnumber(L, pos.z);
	lua_settable(L, -3);

	return 1;
}


extern "C" void STDCALL create_interface(const IID &iid, int i, void **p, sxsdk::shade_interface *shade, void *) {
	lua_joint_plugin *u = new lua_joint_plugin(shade);
	u->AddRef();
	*p = (void *)u;
}

extern "C" int STDCALL has_interface(const IID &iid, sxsdk::shade_interface *shade) {
	if (iid == plugin_iid) return 1;
	return 0;
}

extern "C" const char * STDCALL get_name(const IID &iid, int i, sxsdk::shade_interface *shade, void *) {
	if (iid == plugin_iid) return "Lua Joint";
	return NULL;
}

extern "C" sx::uuid_class STDCALL get_uuid(const IID &iid, int i, void *) {
	if (iid == plugin_iid) return LUA_JOINT_ID;
	return sx::uuid_class(0,0,0,0);
}

extern "C" bool STDCALL is_resident(const IID &iid, int i, void *) {
	if (iid == plugin_iid) return true;
	return false;
}
