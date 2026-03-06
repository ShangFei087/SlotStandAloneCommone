
@echo off

if "%~2" == "" goto Usage
if "%~3" == "" goto KeepGoing
goto Usage
:KeepGoing

SET PROJ_NAME=%~1
SET DEVICE_NAME=%2

SET BUILD_PATH_SHORT=%~s1
SET PROJ_NAME_SHORT=%~s1

SET CA_SIGN_BUILD="%~dp0ca_sign_build\ca_sign_build.exe"
SET SESSION_BUILD="%~dp0session_build\session_build.exe"
SET HEXCONVERTER="%~dp0hexconverter\hexconverter.exe"
SET KEY_FILE="%~dp0ca_sign_build\crk_ecdsa_MAX3255X_my.key"

echo Removing old build files
IF exist "scp_packets" (
IF exist "scp_packets\scp_packets*" del "scp_packets\scp_packets*" /Q
IF exist "scp_packets\packet.list" del "scp_packets\packet.list" /Q
rmdir "scp_packets"
)
IF exist "%PROJ_NAME%.sbin" del "%PROJ_NAME%.sbin" /Q
IF exist "signed.s19" del "signed.s19" /Q


echo Sign binary file
IF %DEVICE_NAME% == MAX32550 (
echo MAX32550 Signing Tool Selected
%CA_SIGN_BUILD% version=01000003 arguments= load_address=10000000 jump_address=10000020 algo=ecdsa ecdsa_file=%KEY_FILE% ca="%PROJ_NAME%.bin" sca="%PROJ_NAME%.sbin" verbose=no
)
IF %DEVICE_NAME% == MAX32555 (
echo MAX32555 Signing Tool Selected
%CA_SIGN_BUILD% version=01010003 arguments= load_address=10000000 jump_address=10000020 algo=ecdsa ecdsa_file=%KEY_FILE% ca="%PROJ_NAME%.bin" sca="%PROJ_NAME%.sbin" verbose=no
)
if errorlevel=1 goto Error

REM Generate signed packet stream for loading over SCP
echo Convert signed binary to packet stream for SCP loading.

IF exist "scp_packets" (
IF exist "scp_packets\scp_packets*" del "scp_packets\scp_packets*" /Q
IF exist "scp_packets\packet.list" del "scp_packets\packet.list" /Q
rmdir "scp_packets"
)
mkdir "scp_packets"

REM Convert signed binary to srecord for session_build.exe
%HEXCONVERTER% -r bin -F srec -i "%PROJ_NAME%.sbin" -o "signed.s19" -e -3 -s 0x10000000
if errorlevel=1 goto Error

echo write-file signed.s19 > "sb_script.txt"
%SESSION_BUILD% pp=ECDSA session_mode=SCP_ANGELA_ECDSA ecdsa_file=%KEY_FILE% script_file="sb_script.txt" output_file="scp_packets\scp_packets.output" chunk_size=2074 verbose=no
if errorlevel=1 goto Error

REM Build packet list file
cd scp_packets
dir /b /on scp_packets*.packet > packet.list

echo Build Succeeded.
exit /B 0

:Error
echo Build Failed!
exit /B 1

:Usage
echo usage: signit param1 param2
echo.
echo param1    Project name
echo param2    Device name (MAX32550 or MAX32555)

exit /B 2
