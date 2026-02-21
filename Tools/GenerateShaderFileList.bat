@echo off
set sourceDir=%1
set outputDir=%2

type nul > %outputDir%/files_shader.txt

for /R %sourceDir% %%f in (*.sc) do (
    @echo %%f
    @echo %%f>> %outputDir%/files_shader.txt
    )

exit /b
