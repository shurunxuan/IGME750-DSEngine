@REM Store args...
set _platform=%1%
set _solutionDir=%~2%
set _vcpkg=vcpkg

if %_platform%==Win32 ( set _platform=x86 )

WHERE vcpkg >nul 2>nul
IF %ERRORLEVEL% NEQ 0 ( 
    ECHO vcpkg not found in PATH, checking solution folder
    if exist "%_solutionDir%vcpkg" ( 
        if exist "%_solutionDir%vcpkg\vcpkg.exe" (
            echo Found vcpkg
        ) else (
            call "%_solutionDir%vcpkg\bootstrap-vcpkg.bat"
        )
    ) else (
        ECHO vcpkg not found
        git clone https://github.com/Microsoft/vcpkg.git "%_solutionDir%vcpkg"
        call "%_solutionDir%vcpkg\bootstrap-vcpkg.bat"
    )
    set _vcpkg="%_solutionDir%vcpkg\vcpkg.exe"
) else (
    ECHO Using pre-installed vcpkg.
)

ECHO Restoring package
%_vcpkg% integrate install
%_vcpkg% update
%_vcpkg% upgrade --no-dry-run
%_vcpkg% install assimp:%_platform%-windows directxtk:%_platform%-windows boost:%_platform%-windows ffmpeg:%_platform%-windows

exit /b 0