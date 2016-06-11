@echo off
md ..\..\..\build\share\EmBlocks\images\ThreadSearch > nul 2>&1
md ..\..\..\output\share\EmBlocks\images\ThreadSearch > nul 2>&1
md ..\..\..\build\share\EmBlocks\images\ThreadSearch\16x16 > nul 2>&1
md ..\..\..\output\share\EmBlocks\images\ThreadSearch\16x16 > nul 2>&1
md ..\..\..\build\share\EmBlocks\images\ThreadSearch\22x22 > nul 2>&1
md ..\..\..\output\share\EmBlocks\images\ThreadSearch\22x22 > nul 2>&1
md ..\..\..\build\share\EmBlocks\images\settings > nul 2>&1
md ..\..\..\output\share\EmBlocks\images\settings > nul 2>&1
copy .\resources\images\*.png ..\..\..\build\share\EmBlocks\images\ThreadSearch\ > nul 2>&1
copy .\resources\images\16x16\*.png ..\..\..\build\share\EmBlocks\images\ThreadSearch\16x16\ > nul 2>&1
copy .\resources\images\22x22\*.png ..\..\..\build\share\EmBlocks\images\ThreadSearch\22x22\ > nul 2>&1
copy .\resources\images\*.png ..\..\..\output\share\EmBlocks\images\ThreadSearch\ > nul 2>&1
copy .\resources\images\16x16\*.png ..\..\..\output\share\EmBlocks\images\ThreadSearch\16x16\ > nul 2>&1
copy .\resources\images\22x22\*.png ..\..\..\output\share\EmBlocks\images\ThreadSearch\22x22\ > nul 2>&1
copy .\resources\*.png ..\..\..\build\share\EmBlocks\images\settings\ > nul 2>&1
copy .\resources\*.png ..\..\..\output\share\EmBlocks\images\settings\ > nul 2>&1
exit 0
