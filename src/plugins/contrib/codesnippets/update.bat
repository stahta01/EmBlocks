@echo off
del ..\..\..\output\share\EmBlocks\plugins\codesnippets.exe > nul 2>&1
zip -j9 ..\..\..\build\share\EmBlocks\codesnippets.zip manifest.xml > nul 2>&1
md ..\..\..\build\share\EmBlocks\images\codesnippets > nul 2>&1
copy .\resources\*.png ..\..\..\build\share\EmBlocks\images\codesnippets\ > nul 2>&1
md ..\..\..\output\share\EmBlocks\images\codesnippets > nul 2>&1
copy .\resources\*.png ..\..\..\output\share\EmBlocks\images\codesnippets\ > nul 2>&1