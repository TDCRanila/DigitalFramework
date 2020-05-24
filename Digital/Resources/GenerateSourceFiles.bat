@echo off
set sourceDir=%1
set outputDir=%2

type nul > %outputDir%/files_source.txt
type nul > %outputDir%/files_header.txt
type nul > %outputDir%/files_all.txt

for /R %sourceDir% %%f in (*.h) do (
    @echo %%f
    @echo %%f>> %outputDir%/files_header.txt
    @echo %%f>> %outputDir%/files_all.txt
    )

for /R %sourceDir% %%f in (*.cpp) do (
    @echo %%f
    @echo %%f>> %outputDir%/files_source.txt
    @echo %%f>> %outputDir%/files_all.txt
    )

exit /b