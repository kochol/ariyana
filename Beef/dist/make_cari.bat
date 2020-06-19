cd ..\..
fips make cari
cd Beef\dist
copy ..\..\..\fips-build\ariyana\win64-vstudio-debug\src\c99\Debug\cari.lib .\win64\Debug\
copy ..\..\..\fips-deploy\ariyana\win64-vstudio-debug\cari.dll .\win64\Debug\
