set WORKSPACE=..

set LUBAN_DLL=%WORKSPACE%\Tools\Luban\Luban.dll
set CONF_ROOT=%WORKSPACE%\DataTables

dotnet %LUBAN_DLL% ^
    -t all ^
    -c cpp-sharedptr-bin ^
    -d bin  ^
    --conf %CONF_ROOT%\luban.conf ^
    -x outputCodeDir=..\UELuban\Source\UELuban\LubanGen ^
    -x outputDataDir=..\UELuban\Content\Luban\ConfigBytes ^
    -x pathValidator.rootDir=%WORKSPACE%\UELuban\Content\Luban 

pause