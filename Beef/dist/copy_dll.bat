FOR /R "..\..\..\fips-build\ariyana\cari-win64-vstudio-debug\" %%i IN (*.lib) DO COPY "%%i" ".\win64\Debug\"

FOR /R "..\..\..\fips-build\ariyana\cari-win64-vstudio-release\" %%i IN (*.lib) DO COPY "%%i" ".\win64\Release\"

FOR /R "..\..\..\fips-build\ariyana\server-win64-vstudio-debug\" %%i IN (*.lib) DO COPY "%%i" ".\win64\ServerDebug\"

FOR /R "..\..\..\fips-build\ariyana\cari-android-ninja-debug\" %%i IN (*.a) DO COPY "%%i" ".\android\armeabi-v7a\Debug\"

FOR /R "..\..\..\fips-build\ariyana\server-linux-ninja-debug\" %%i IN (*.a) DO COPY "%%i" ".\linux64\ServerDebug\"
