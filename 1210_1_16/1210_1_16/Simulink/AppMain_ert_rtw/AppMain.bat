
set MATLAB=D:\Program Files\MATLAB\R2023a

cd .

if "%1"=="" ("D:\PROGRA~1\MATLAB\R2023a\bin\win64\gmake"  -f AppMain.mk all) else ("D:\PROGRA~1\MATLAB\R2023a\bin\win64\gmake"  -f AppMain.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
exit /B 1