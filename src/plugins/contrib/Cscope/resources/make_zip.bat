cd .\resources
zip -jq9 ..\..\..\..\build\share\emblocks\cscope.zip manifest.xml *.xrc
zip -q9  ..\..\..\..\build\share\emblocks\cscope.zip images\*.png

xcopy /I /y ..\Cscope\*.exe ..\..\..\..\build\share\emblocks\Cscope > nul