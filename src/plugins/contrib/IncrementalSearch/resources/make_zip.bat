cd .\resources
del ..\..\..\..\build\share\emblocks\IncrementalSearch.zip
zip -jq9 ..\..\..\..\build\share\emblocks\IncrementalSearch.zip manifest.xml *.xrc
zip -q9  ..\..\..\..\build\share\emblocks\IncrementalSearch.zip images\*.png
zip -q9  ..\..\..\..\build\share\emblocks\IncrementalSearch.zip images\16x16\*.png