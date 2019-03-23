@REM Store args...
set _outdir=%~1%
set _projdir=%~2%

for /R "%_outdir%" %%f in (*.cso) do copy "%%f" "%_projdir%" /y