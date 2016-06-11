@echo off
cls
zip ..\..\..\build\share\EmBlocks\wxsmith.zip manifest.xml
zip ..\..\..\output\share\EmBlocks\wxsmith.zip manifest.xml
md ..\..\..\build\share\EmBlocks\images\wxsmith > nul 2>&1
md ..\..\..\output\share\EmBlocks\images\wxsmith > nul 2>&1
copy wxwidgets\icons\*.png ..\..\..\build\share\EmBlocks\images\wxsmith\ > nul 2>&1
copy wxwidgets\icons\*.png ..\..\..\output\share\EmBlocks\images\wxsmith\ > nul 2>&1
exit 0