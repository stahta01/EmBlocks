@echo off
mkdir ..\..\..\build\share\EmBlocks\lib_finder > nul 2>&1
mkdir ..\..\..\output\share\EmBlocks\lib_finder > nul 2>&1
copy /Y lib_finder\*.xml ..\..\..\build\share\EmBlocks\lib_finder > nul 2>&1
copy /Y lib_finder\*.xml ..\..\..\output\share\EmBlocks\lib_finder > nul 2>&1
zip -j9 ..\..\..\build\share\EmBlocks\lib_finder.zip manifest.xml