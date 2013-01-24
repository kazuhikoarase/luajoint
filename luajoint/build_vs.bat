@rem ------------------------------------------------------------
@rem
@rem Build batch for Visual Studio 2005 Command Prompt
@rem
@rem Copyright (c) 2009 Kazuhiko Arase
@rem
@rem URL: http://code.google.com/p/luajoint/
@rem
@rem Licensed under the MIT license:
@rem   http://www.opensource.org/licenses/mit-license.php
@rem
@rem ------------------------------------------------------------

@setlocal

@rem begin for VCS Express
@set PLATFORM_SDK_HOME=C:\Program Files\Microsoft Platform SDK
@set PATH=%PLATFORM_SDK_HOME%\Bin;%PATH%
@set INCLUDE=%PLATFORM_SDK_HOME%\Include;%INCLUDE%
@set LIB=%PLATFORM_SDK_HOME%\Lib;%LIB%
@rem end for VCS Express

@set LUA_SRC_HOME=C:\usr\local\lua-5.1.4
@set SHADE_SDK_HOME=C:\usr\local\Shade 9 Plugin SDK
@rem @set SHADE_SDK_HOME=C:\usr\local\Shade 10 Plugin SDK
@set TARGET_NAME=lua_joint.dll

@set CL_OPTS=/nologo /c /TP /MD /O2 /Ob1 /GR /EHsc /D _CRT_SECURE_NO_DEPRECATE

cl^
 %CL_OPTS%^
 /W3^
 "%LUA_SRC_HOME%\src\l*.c"

del lua.obj luac.obj

cl^
 %CL_OPTS%^
 /W1^
 /GS^
 /Gy^
 /I "%SHADE_SDK_HOME%\include"^
 /I "%LUA_SRC_HOME%\src"^
 /wd4819^
 /D WIN32^
 /D _DEBUG^
 /D _WINDOWS^
 /D _USRDLL^
 /D SXWIN32^
 /D SXWINDOWS^
 /D PLUGIN=1^
 /D STDCALL=__stdcall^
 /D DLLEXPORT=__declspec(dllexport)^
 /D for=if(true)for^
 "%SHADE_SDK_HOME%\include\com.cpp"^
 "%SHADE_SDK_HOME%\include\vectors.cpp"^
 lua_joint.cpp

link /nologo /DLL /OUT:%TARGET_NAME% *.obj

mt^
 -nologo^
 -manifest %TARGET_NAME%.manifest^
 -outputresource:%TARGET_NAME%;#2

del *.obj *.exp *.lib *.manifest
 