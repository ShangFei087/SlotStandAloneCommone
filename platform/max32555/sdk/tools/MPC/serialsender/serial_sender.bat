ECHO OFF

::-v -t 1 -e 60 -f 20 -r 1 -s

::---------------------------------------------
::  "-v" Verbose
::---------------------------------------------
set option_verbose=%1
::---------------------------------------------
::  "-t" Default cmd response timeout
::---------------------------------------------
set option_default_cmd_resp_timeout=%2
set option_default_cmd_resp_timeout_value=%3
::---------------------------------------------
::  "-e" Specific cmd erase response timeout
::---------------------------------------------
set option_cmd_erase_resp_timeout=%4
set option_cmd_erase_resp_timeout_value=%5
::---------------------------------------------
::  "-f" Connection request cmd retry number
::---------------------------------------------
set option_conn_request_retry_nb=%6
set option_conn_request_retry_value=%7
::---------------------------------------------
::  "-r" Default cmd retry number
::---------------------------------------------
set option_default_cmd_retry_nb=%8
set option_default_cmd_retry_nb_value=%9
::---------------------------------------------
::  "-s" Serial device
::---------------------------------------------
SHIFT
set option_port_com=%9%
SHIFT
set option_port_com_value=%9%
::---------------------------------------------
::  SCP packets list file path
::---------------------------------------------
SHIFT
set option_scp_packets_list_file=%9%
set scp_packets_folder=%~dp9
set scp_packets_list_file=%~nx9
set current_dir=%CD% 
set install_dir="."

echo current_dir=%current_dir% 

::---------------------------------------------
::  "-v" Verbose
::---------------------------------------------
ECHO option_verbose=%option_verbose%
::---------------------------------------------
::  "-t" Default cmd response timeout
::---------------------------------------------
ECHO option_default_cmd_resp_timeout=%option_default_cmd_resp_timeout%
ECHO option_default_cmd_resp_timeout_value=%option_default_cmd_resp_timeout_value%
::---------------------------------------------
::  "-e" Specific cmd erase response timeout
::---------------------------------------------
ECHO option_cmd_erase_resp_timeout=%option_cmd_erase_resp_timeout%
ECHO option_cmd_erase_resp_timeout_value=%option_cmd_erase_resp_timeout_value%
::---------------------------------------------
::  "-f" Connection request cmd retry number
::---------------------------------------------
ECHO option_conn_request_retry_nb=%option_conn_request_retry_nb%
ECHO option_conn_request_retry_value=%option_conn_request_retry_value%
::---------------------------------------------
::  "-r" Default cmd retry number
::---------------------------------------------
ECHO option_default_cmd_retry_nb=%option_default_cmd_retry_nb%
ECHO option_default_cmd_retry_nb_value=%option_default_cmd_retry_nb_value%
::---------------------------------------------

ECHO option_port_com=%option_port_com%
ECHO option_port_com_value=%option_port_com_value%
ECHO option_scp_packets_list_file=%option_scp_packets_list_file%
echo install_dir =%install_dir% 

echo ** CALL BOOTLOADER ACTIVATOR ** 
echo call %install_dir%\\serialsender\\bootloader_activator\\activate_bootloader.exe %option_verbose% %option_default_cmd_resp_timeout% %option_default_cmd_resp_timeout_value% %option_port_com% %option_port_com_value%
call %install_dir%\\serialsender\\bootloader_activator\\activate_bootloader.exe %option_verbose% %option_default_cmd_resp_timeout% %option_default_cmd_resp_timeout_value% %option_port_com% %option_port_com_value%

if %errorlevel% neq 0 goto :end_of_bootloader_activator_process

echo ** CALL SERIALSENDER ** 
echo %install_dir%\\serialsender\\serial_sender.exe %option_verbose% %option_default_cmd_resp_timeout% %option_default_cmd_resp_timeout_value% %option_cmd_erase_resp_timeout% %option_cmd_erase_resp_timeout_value% %option_conn_request_retry_nb% %option_conn_request_retry_value% %option_default_cmd_retry_nb% %option_default_cmd_retry_nb_value% %option_port_com% %option_port_com_value% %option_scp_packets_list_file%
%install_dir%\\serialsender\\serial_sender.exe %option_verbose% %option_default_cmd_resp_timeout% %option_default_cmd_resp_timeout_value% %option_cmd_erase_resp_timeout% %option_cmd_erase_resp_timeout_value% %option_conn_request_retry_nb% %option_conn_request_retry_value% %option_default_cmd_retry_nb% %option_default_cmd_retry_nb_value% %option_port_com% %option_port_com_value% %option_scp_packets_list_file%

if %errorlevel% neq 0 goto :end_of_serialsender_process

goto :EOF

:end_of_bootloader_activator_process
echo Bootloader activator fails with error: %errorlevel%
exit %errorlevel%

:end_of_serialsender_process
echo Serialsender fails with error: %errorlevel%
exit %errorlevel%