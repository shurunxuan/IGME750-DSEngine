@REM Store args...
set _platform=%1%
set _solutionDir=%2%

if %_platform%==Win32 ( set _platform=x86 )

if exist %_solutionDir%vcpkg ( 
    if exist %_solutionDir%vcpkg\vcpkg.exe (
        echo Found vcpkg
    ) else (
        %_solutionDir%vcpkg\bootstrap-vcpkg.bat
    )
) else (
    ECHO vcpkg not found
    git clone https://github.com/Microsoft/vcpkg.git %_solutionDir%vcpkg
    %_solutionDir%vcpkg\bootstrap-vcpkg.bat
)
ECHO Restoring package
%_solutionDir%vcpkg\vcpkg.exe integrate install
%_solutionDir%vcpkg\vcpkg.exe install boost:%_platform%-windows