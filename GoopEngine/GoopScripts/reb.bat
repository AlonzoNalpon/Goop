@ECHO OFF
set VS_VERSION=%1
set BUILD_CONFIG=%2
set DEVENV_PATH=
if %VS_VERSION%==2019 (
 for %%d in (c d e f g h i j k l m n o p q r s t u v) do (
  for %%I in (
   "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv.exe"
   "%ProgramFiles%\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv.exe"
  ) do (
   if exist "%%~I" (
    set DEVENV_PATH=%%~I
    goto :End
   )
  )
 )
) else if %VS_VERSION% ==2022 (
 for %%d in (c d e f g h i j k l m n o p q r s t u v) do (
  for %%I in (
   "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"
   "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"
  ) do (
   if exist "%%~I" (
    set DEVENV_PATH=%%~I
    goto :End
   )
  )
 )
)



@REM :End

@REM %DEVENV_PATH% %SLN_PATH% /build %BuildConfig% /project %PROJ_PATH%
@REM copy /y %BIN_PATH %EDITOR_ASSET_PATH%
@REM copy /y %BIN_PATH %OUTPUT_ASSET_PATH%

:End
set BIN_PATH="%~dp0bin\GoopScripts.dll"
set SLN_PATH="%~dp0..\GoopEngine.sln"
set PROJ="%~dp0GoopScripts.csproj"
set EDITOR_ASSET_PATH="%~dp0..\Editor\Assets\Scripts\GoopScripts.dll"
set OUTPUT_ASSET_PATH =
if "%BUILD_CONFIG%"=="Debug" (
 set OUTPUT_ASSET_PATH="%~dp0..\x64\Debug\Assets\Scripts"
) else if "%BUILD_CONFIG%"=="Release" (
 set OUTPUT_ASSET_PATH="%~dp0..\x64\Release\Assets\Scripts"
)

@REM echo VS_VERSION: %VS_VERSION%
@REM echo SLN_PATH: %SLN_PATH%
@REM echo PROJ: %PROJ%
@REM echo EDITOR_ASSET_PATH: %EDITOR_ASSET_PATH%
@REM echo BIN_PATH: %BIN_PATH%
@REM echo BUILD_CONFIG: %BUILD_CONFIG%
@REM echo OUTPUT_ASSET_PATH: %OUTPUT_ASSET_PATH%
@REM echo DEVENV_PATH: %DEVENV_PATH%
@REM Set the relative path of the file in Goop/Test

@REM Construct the full path using the batch file's directory






if defined DEVENV_PATH (
    @REM echo DEVENV_PATH : %DEVENV_PATH%
    @REM echo Building...
    "%DEVENV_PATH%" %SLN_PATH% /build %BUILD_CONFIG% /project %PROJ%
    copy /y %BIN_PATH% %EDITOR_ASSET_PATH%
    copy /y %BIN_PATH% %OUTPUT_ASSET_PATH%
    @REM "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" "C:\Users\qindi\Documents\GitHub\Goop\GoopEngine\GoopEngine.sln" /build Debug /project "C:\Users\qindi\Documents\GitHub\Goop\GoopEngine\GoopScripts\GoopScripts.csproj"
    @REM copy /y "C:\Users\qindi\Documents\GitHub\Goop\GoopEngine\GoopScripts\bin\GoopScripts.dll" "C:\Users\qindi\Documents\GitHub\Goop\GoopEngine\Editor\Assets\Scripts\GoopScripts.dll"
    @REM echo Build complete!
) else (
    echo Error: devenv.exe not found!
    pause
)