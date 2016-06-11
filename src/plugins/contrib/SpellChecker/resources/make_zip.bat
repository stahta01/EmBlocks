cd .\resources
zip -jq9 ..\..\..\..\build\share\emblocks\SpellChecker.zip manifest.xml *.xrc
zip -q9  ..\..\..\..\build\share\emblocks\SpellChecker images\SpellChecker.png

xcopy /I /y ..\SpellChecker\*.* ..\..\..\..\build\share\emblocks\SpellChecker > nul
