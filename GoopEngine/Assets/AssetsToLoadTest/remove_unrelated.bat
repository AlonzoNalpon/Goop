@echo off
setlocal enabledelayedexpansion

rem Set the input file name
set "input_file=bat_file_output.json"

rem Use PowerShell to filter the lines in the input file
powershell -command "(Get-Content %input_file%) | ForEach-Object { if ($_ -notmatch '^ *\"generate_json\" *:|\"bat_file_output\" *:|\"remove_unrelated\" *:') { if ($_ -match ':') { $_ + ',' } else { $_ } } } | ForEach-Object { $_.TrimEnd() } | Set-Content %input_file%.temp"
del %input_file%
rename %input_file%.temp %input_file%

echo Commas have been appended to lines with entries, and lines with "generate_json.bat," "bat_file_output.json," or "remove_unrelated.bat" as the first string value have been removed from %input_file%.
