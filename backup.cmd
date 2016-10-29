@echo off

For /f "tokens=3-3 delims=. " %%a in ('echo %DATE%') do (set year=%%a)
For /f "tokens=2-2 delims=. " %%a in ('echo %DATE%') do (set month=%%a)
For /f "tokens=1-1 delims=. " %%a in ('echo %DATE%') do (set day=%%a)
For /f "tokens=1-1 delims=: " %%a in ('echo %TIME%') do (set hour=%%a)
For /f "tokens=2-2 delims=: " %%a in ('echo %TIME%') do (set minute=%%a)
For /f "tokens=3-3 delims=:. " %%a in ('echo %TIME%') do (set second=%%a)
set year=0000%year%
set year=%year:~-4%
set month=00%month%
set month=%month:~-2%
set day=00%day%
set day=%day:~-2%
set hour=00%hour%
set hour=%hour:~-2%
set minute=00%minute%
set minute=%minute:~-2%
set second=00%second%
set second=%second:~-2%

SET NAME_UID=%year%%month%%day%%hour%%minute%%second%
SET FILE_NAME="..\..\__backup__\%NAME_UID%_deemon.7z"

echo Creating backup %FILE_NAME%
7za a -t7z %FILE_NAME% *

echo Testing backup %FILE_NAME% for archive errors
7za t %FILE_NAME%