@echo off
setlocal enabledelayedexpansion

rem Initialize the JSON array
echo { > bat_file_output.json

rem Iterate through files in the current directory
set first=1
for %%f in (*) do (
    set "file_name=%%~nxf"
    set "file_name_no_ext=%%~nf"
    if !first!==1 (
        echo    "!file_name_no_ext!": "!file_name!" >> bat_file_output.json
        set "first=0"
    ) else (
        echo, >> bat_file_output.json
        echo    "!file_name_no_ext!": "!file_name!" >> bat_file_output.json
    )
)

rem Add the closing bracket to complete the JSON object
echo } >> bat_file_output.json

echo JSON file has been created: bat_file_output.json

rem Call the "remove_unrelated.bat" batch file
call remove_unrelated.bat
