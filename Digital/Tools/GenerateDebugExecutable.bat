wip, needs experimentation

: GenerateBuild.bat
cd..
cd build
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
msbuild.exe ProjectSolution.sln /t:_MainTargets\DigitalGame_
pause