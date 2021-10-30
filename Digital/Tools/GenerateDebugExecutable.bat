@echo off

:: Filepath of script executable location.
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
cd %basepath%
cd ..
RMDIR "%basepath%/build-clean" /S /Q

mkdir %projectpath%
mkdir %execpath%

:: Generate Project
echo CMAKE - Creating Project
cd %basepath%
cd ..
mkdir "%projectpath%"
cd %projectpath%
cmake %basepath% >>%logfilename%

:: Build Compile
cd %projectpath%
echo MSVC - Compiling Project(s)
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
msbuild ProjectSolution.sln /t:_MainTargets\DigitalGame_ >>%logfilename%
echo MSVC - Finished Compiling Project(s)

:: Move Generated Objects
robocopy %projectpath% %execpath% %logfilename%
robocopy %projectpath%\DigitalGame_\Debug %execpath%

pause
