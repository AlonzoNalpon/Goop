@echo off
setlocal enabledelayedexpansion

set "outputFile=ScriptNames.txt"
set "sourceDirectory=."
set "keyword=GoopScripts"
set "keywordLength=11"

if exist "%outputFile%" del "%outputFile%"

for /r %sourceDirectory% %%i in (*.cs) do (
    set "folder=%~d0%%~pi"
    if /i not "!folder!"=="%CD%\obj\Debug\" (
    if /i not "!folder!"=="%CD%\obj\Release\" (
    if /i not "!folder!"=="%CD%\Properties\" (
    if /i not "!folder!"=="%CD%\Properties\Mono" (

        set "name=%%~ni"
        set "temp=."
        call :findKeyword 
    ))))
)



endlocal

:findKeyword
if defined folder (
    set temp=!folder:~0,%keywordLength%!
    set "char=!folder:~0,1!"
    if /i "!char!"=="%keyword:~0,1%" (
        set "temp=!folder:~0,%keywordLength%!"
        if /i "!temp!"=="%keyword%" (
            set "position=!folder:~0!"
            set "position=!position:\=.!"
            set "position=!position:~0,-1!"
            echo !position!,!name!>> "%outputFile%
        )
    )
    set "folder=!folder:~1!"
    goto findKeyword
) 



@REM @echo off
@REM setlocal enabledelayedexpansion

@REM set "outputFile=ScriptNames.txt"
@REM set "sourceDirectory=."

@REM if exist "%outputFile%" del "%outputFile%"

@REM for /r %sourceDirectory% %%i in (*.cs) do (
@REM     set "folder=%~d0%%~pi"
@REM         if /i not "!folder!"=="%CD%\obj\Debug\" (
@REM         if /i not "!folder!"=="%CD%\obj\Release\" (
@REM         if /i not "!folder!"=="%CD%\Properties\" (
@REM         if /i not "!folder!"=="%CD%\" (
@REM             echo %%~ni>> "%outputFile%"
@REM         )  
@REM         )
@REM         )
@REM         )
@REM )

@REM endlocal