@rem ------------------------------------------------------------
@rem
@rem Build batch for Visual Studio 2005 Command Prompt
@rem
@rem Copyright (c) 2013 Kazuhiko Arase
@rem
@rem URL: http://code.google.com/p/luajoint/
@rem
@rem Licensed under the MIT license:
@rem   http://www.opensource.org/licenses/mit-license.php
@rem
@rem ------------------------------------------------------------

@setlocal

@rem begin for VCS Express
@set PLATFORM_SDK_HOME=C:\Program Files\Microsoft SDKs\Windows\v7.0
@set PATH=%PLATFORM_SDK_HOME%\Bin;%PATH%
@set INCLUDE=%PLATFORM_SDK_HOME%\Include;%INCLUDE%
@set LIB=%PLATFORM_SDK_HOME%\Lib;%LIB%
@rem end for VCS Express

@set LUA_SRC_HOME=C:\usr\local\lua-5.1.4
@set SHADE_SDK_HOME=C:\usr\local\Shade 12.0.3 Plugin SDK (453077)
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
 /I "%SHADE_SDK_HOME%\boost_1_43_0"^
 /I "%SHADE_SDK_HOME%\include\openexr-1.6.1"^
 /I "%SHADE_SDK_HOME%\include\opengl"^
 /I "%LUA_SRC_HOME%\src"^
 /wd4819^
 /D SXWINDOWS^
 /D SXWIN32^
 /D DEMO_PLUGIN=0^
 /D NDEBUG^
 /D sxdebug=0^
 /D WIN32^
 /D _WINDOWS^
 /D Windows=1^
 /D PLUGIN=1^
 /D STDCALL=__stdcall^
 /D DLLEXPORT=__declspec(dllexport)^
 /D for=if(true)for^
 /D SXPLUGIN=1^
 /D _SECURE_SCL=0^
 /D PLUGIN_EXPORTS^
 /D SXCORE=^
 /D SXMODEL=^
 "%SHADE_SDK_HOME%\include\sx\core\com.cpp"^
 "%SHADE_SDK_HOME%\include\sx\core\vectors.cpp"^
 "%SHADE_SDK_HOME%\include\sx\core\debug.cpp"^
 ..\lua_joint.cpp

link /nologo /DLL /OUT:%TARGET_NAME% *.obj

mt^
 -nologo^
 -manifest %TARGET_NAME%.manifest^
 -outputresource:%TARGET_NAME%;#2

del *.obj *.exp *.lib *.manifest
 