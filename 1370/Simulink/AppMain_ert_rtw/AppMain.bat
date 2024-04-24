
set MATLAB=D:\MATLAB2021\matlab2021a\R2021a

cd .

if "%1"=="" ("D:\MATLAB2021\matlab2021a\R2021a\bin\win64\gmake"  -f AppMain.mk all) else ("D:\MATLAB2021\matlab2021a\R2021a\bin\win64\gmake"  -f AppMain.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
exit /B 1