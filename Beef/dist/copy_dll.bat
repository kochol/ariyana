FOR /R "..\..\..\fips-build\ariyana\win64-vstudio-debug\" %%i IN (*.lib) DO COPY "%%i" ".\win64\Debug\"
copy ..\..\..\fips-deploy\ariyana\win64-vstudio-debug\cari.dll .\win64\Debug\

FOR /R "..\..\..\fips-build\ariyana\server-win64-vstudio-debug\" %%i IN (*.lib) DO COPY "%%i" ".\win64\ServerDebug\"
copy ..\..\..\fips-deploy\ariyana\server-win64-vstudio-debug\cari.dll .\win64\ServerDebug\

FOR /R "..\..\..\fips-build\ariyana\server-linux-ninja-debug\" %%i IN (*.a) DO COPY "%%i" ".\linux64\ServerDebug\"
copy ..\..\..\fips-deploy\ariyana\server-linux-ninja-debug\libcari.so .\linux64\ServerDebug\
