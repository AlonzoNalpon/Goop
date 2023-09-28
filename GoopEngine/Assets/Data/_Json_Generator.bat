@echo off
setlocal enabledelayedexpansion

:: Name of the JSON file
set "jsonfile=_Generated.json"

:: Delete the JSON file if it already exists
if exist %jsonfile% del %jsonfile%

:: Initialize the JSON content
echo { > %jsonfile%

:: Initialize a variable to track the first entry
set "firstEntry=1"

:: Loop over all files in the current directory
for %%F in (*.jpg, *.jpeg, *.png, *.gif, *.bmp) do (
    :: Exclude the batch file itself and the generated output file
    if not "%%~nxF"=="%~nx0" if not "%%~nxF"=="%jsonfile%" (
        :: Get the filename without extension
        set "filename=%%~nF"
        
        :: Get the relative file path
        set "filepath=%%~nxF"
        
        :: Append a comma if it's not the first entry
        if !firstEntry!==0 (
            echo , >> %jsonfile%
        ) else (
            set "firstEntry=0"
        )
        
        :: Add the entry to the JSON content
        <nul set /p dummyName=""!filename!": "!filepath!"" >> %jsonfile%
    )
)

:: Close the JSON content
echo. >> %jsonfile%
echo } >> %jsonfile%

endlocal
