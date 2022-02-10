@ECHO OFF
REM -- Copy this file to the root of your boost repository and run it from an administrative command prompt
REM -- Libraries will be built in C:\boost\lib and C:\boost\lib64
REM -- Update your Visual Studio paths.
REM -- Tested with Boost 1.51.0 and higher

call bootstrap.bat

SET INCLUDE_LIBRARIES=--with-test
SET BOOST_OPTIONS=toolset=msvc-11.0 define=_SECURE_SCL=0 define=arch:SSE2 threading=multi runtime-link=static --layout=versioned 


SET BUILD_32BIT_OPTIONS=%BOOST_OPTIONS% %INCLUDE_LIBRARIES% address-model=32 --prefix=C:\boost
SET BUILD_64BIT_OPTIONS=%BOOST_OPTIONS% %INCLUDE_LIBRARIES% address-model=64 --prefix=C:\boost --libdir=C:\boost\lib64

SET BUILD_LINK_OPTIONS=link=static runtime-link=shared

SET OPTIONS_32=%BUILD_32BIT_OPTIONS% %BUILD_LINK_OPTIONS% variant=debug,release install
SET OPTIONS_64=%BUILD_64BIT_OPTIONS% %BUILD_LINK_OPTIONS% variant=debug,release install

.\b2 --clean

echo =============================================
echo %OPTIONS_32%
echo Output is in log32.txt...
echo =============================================

.\b2 %OPTIONS_32% > log32.txt
del bin.v2 /s /q 

.\b2 --clean

echo =============================================
echo %OPTIONS_64%
echo Output is in log64.txt...
echo =============================================


.\b2 %OPTIONS_64% > log64.txt
del bin.v2 /s /q 

