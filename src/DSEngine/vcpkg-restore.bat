@REM Store args...
set _platform=%1%
set _solutionDir=%2%
set _vcpkg=vcpkg

if %_platform%==Win32 ( set _platform=x86 )

WHERE vcpkg >nul 2>nul
IF %ERRORLEVEL% NEQ 0 ( 
    ECHO vcpkg not found in PATH, checking solution folder
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
    set _vcpkg=%_solutionDir%vcpkg\vcpkg.exe
) else (
    ECHO Using pre-installed vcpkg.
)
ECHO Restoring package
%_vcpkg% integrate install
%_vcpkg% install boost:%_platform%-windows
%_vcpkg% install ffmpeg:%_platform%-windows
