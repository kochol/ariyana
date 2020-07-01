cd ..\..
fips make cari server-win64-vstudio-debug
cd Beef\dist
copy ..\..\..\fips-build\ariyana\server-win64-vstudio-debug\src\c99\Debug\cari.lib .\win64\ServerDebug\
copy ..\..\..\fips-deploy\ariyana\server-win64-vstudio-debug\cari.dll .\win64\ServerDebug\
