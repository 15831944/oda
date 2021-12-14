To build ODAFileConverter, ODADrawingsExplorer please follow steps described below:
1. Download and install Qt 5.3 or later. 
   In case you have Qt 5.2 - libraries for iOS should be rebuild with c++11 compiler option.
2. Export QTDIR environment variable.
   The path should point to the required platform.
3. Export ODADIR environment variable.
   It's the path to the top directory with ODA SDK.
4. Run make_ios.sh script:
   > bash make_ios.sh <current iphone configuration>
   E.g.
   > bash make_ios.sh iphone_7.1     #For device configuration
   or
   > bash make_ios.sh iphone_7.1sim  #For simulator

5. After step 4 following libraries were build:
   libOdaQtViewSystem.a, libOdaQtOpenGL.a, libOdaConsole.a and will be copied to lib/iphone* folder.
6. Project files for ODADrawingsExplorer will be located into ../OdaQtApp folder.
7. Project files for ODAFileConverter will be located into ../OdaFileConverter folder.

