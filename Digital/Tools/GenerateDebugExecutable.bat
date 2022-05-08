@echo off

:: Filepath of script executable location.
echo {} - Generating File Locations
cd ..
for %%a in ("%cd%\") do set "basepath=%%~fa"
echo %basepath%

:: Filepath Generated Project
set projectpath="%basepath%build-clean\generated-project"
echo %projectpath%

:: Filepath Generated Executables
set execpath="%basepath%build-clean\executables"
echo %execpath%

set logfilename="buildlog.txt"

:: Clean Space
echo {} - Cleaning Build Space
cd %basepath%
cd ..
RMDIR "%basepath%/build-clean" /S /Q
mkdir %projectpath%
mkdir %execpath%

:: Generate Project
echo CMAKE - Creating Project Files
cd %basepath%
cd ..
mkdir "%projectpath%"
cd %projectpath%
cmake -G "Visual Studio 16 2019" %basepath% >>%logfilename%

:: Build Compile
cd %projectpath%
echo {} - MSVC - Compiling Project(s)
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
msbuild ProjectSolution.sln /t:_MainTargets\DigitalGame_ >>%logfilename% -m
echo {} - MSVC - Finished Compiling Project(s)

:: Move Generated Objects
echo {} - Copying Generated Objects.
robocopy %projectpath% %execpath% %logfilename%
robocopy %projectpath%\DigitalGame_\Debug %execpath%

:: Copy Resources
echo {} - Copying Resources ~ Shaders.
for /f %%d in ('dir  %basepath%\Resources\shaders /b /ad') do (
    robocopy  %basepath%\Resources\shaders\%%d %execpath%\shaders\%%d  *.bin /s
    for /f %%e in ('dir  %basepath%\Resources\shaders%%d /b /ad') do (
        robocopy  %basepath%\Resources\shaders%%d\%%e %execpath%\shaders\%%d\%%e *.bin /s
    )
)

echo {} - Copying Resources ~ Models
for /f %%d in ('dir  %basepath%\Resources\models /b /ad') do (
    robocopy  %basepath%\Resources\models\%%d %execpath%\models\%%d  *.* /s
    for /f %%e in ('dir  %basepath%\Resources\\models%%d /b /ad') do (
        robocopy  %basepath%\Resources\\models%%d\%%e %execpath%\\models\%%d\%%e *.* /s
    )
)

:: Finish
echo {} - Build Completed.

pause
