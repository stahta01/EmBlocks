cd ..\sdk\resources
md ..\..\build\share\emblocks
zip -jq9 ..\..\build\share\emblocks\manager_resources.zip manifest.xml *.xrc
zip -q9  ..\..\build\share\emblocks\manager_resources.zip images\*.png
zip -q9  ..\..\build\share\emblocks\manager_resources.zip project_img\*.png
zip -q9  ..\..\build\share\emblocks\manager_resources.zip editor_img\*.png
xcopy /I /y /Q .\lexers\lexer_* ..\..\build\share\emblocks\lexers
