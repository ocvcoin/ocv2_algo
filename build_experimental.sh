#!/bin/bash

if (( $EUID != 0 )); then
    echo "
	Please run as root
	"
    exit
fi







if [ -z "$1" ]; then
  echo "

This script is experimental!!!
It should only be used for mining!!!
100% correct hash calculation is not guaranteed!!!

This script builds the libocv2
(only works on ubuntu) (tested ubuntu versions: 22)

Type Target:

1 - Build & Install on this computer

2 - Build libocv2.dll for x86 based 64 BIT WINDOWS

"




read varname
else
  varname="$1"
fi





if [[( "$varname" != "1" ) && ( "$varname" != "2" )]]; then
    echo "
	
	Incorrect! You must enter 1 or 2
	
	"
    exit
fi


if ((( $varname == "2" ))); then


echo "



Please enter the CPU type of the Windows machine where you intend to run the compiled DLL. While leaving this field blank is an option, we strongly recommend that you input your CPU type for optimal performance optimization. If the Windows machine uses the same model processor as your computer, please type "native."

Some acceptable values: native nocona core2 nehalem corei7 westmere sandybridge corei7-avx ivybridge core-avx-i haswell core-avx2 broadwell skylake skylake-avx512 cannonlake icelake-client icelake-server cascadelake tigerlake cooperlake bonnell atom silvermont slm goldmont goldmont-plus tremont knl knm x86-64 x86-64-v2 x86-64-v3 x86-64-v4 

Check this link (-march=cpu-type section) for all supported values:
https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html

Enter cpu type or leave blank:

"




read cputype

cputype=$(echo $cputype | tr -d '[:space:]')

if [ -z "${cputype// }" ]; then
  
  cputype="x86-64"
  
fi





	echo "
	
	
	!!!!!!
	
	function ocv2_test_algo() has been temporarily disabled! Now always returns true!!!
	
	!!!!!!
	
	
	
	"






    
fi




export DEBIAN_FRONTEND=noninteractive

set -e

apt update
apt -y install  git cmake unzip gcc g++ wget pkg-config

apt -y upgrade ca-certificates

set +e


if apt -y install g++-mingw-w64-x86-64-posix ; then
    echo ""
else
    apt -y install g++-mingw-w64-x86-64
fi



mkdir dependencies/opencv


cd dependencies/opencv



if [ -d "opencv-4.7.0/build" ]; then
    rm -rf opencv-4.7.0/build
else 
	rm -rf opencv-4.7.0
	rm -rf 4.7.0.zip
    wget -O 4.7.0.zip https://github.com/opencv/opencv/archive/refs/tags/4.7.0.zip
	unzip 4.7.0.zip
fi


cd opencv-4.7.0

mkdir build && cd build





if ((( $varname == "1" ))); then
    env CC="gcc" CXX="g++" cmake -DCMAKE_INSTALL_PREFIX:PATH="$PWD/../../this-system-release" -DCMAKE_SKIP_RPATH="ON" -DCMAKE_EXE_LINKER_FLAGS="-static" -DCMAKE_FIND_LIBRARY_SUFFIXES=".a" -DBUILD_opencv_flann=OFF -DBUILD_opencv_ml=OFF -DBUILD_opencv_video=OFF -DBUILD_opencv_shape=OFF -DBUILD_opencv_videoio=OFF -DBUILD_opencv_highgui=OFF -DBUILD_opencv_objdetect=OFF -DBUILD_opencv_superres=OFF -DBUILD_opencv_ts=OFF -DBUILD_opencv_features2d=OFF -DBUILD_opencv_calib3d=OFF -DBUILD_opencv_stitching=OFF -DBUILD_opencv_videostab=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DBUILD_ZLIB=OFF -DBUILD_TIFF=OFF -DBUILD_JASPER=OFF -DBUILD_JPEG=OFF -DBUILD_PNG=OFF -DBUILD_OPENEXR=OFF -DBUILD_TBB=OFF -DBUILD_WITH_STATIC_CRT=ON -DINSTALL_C_EXAMPLES=OFF -DINSTALL_PYTHON_EXAMPLES=OFF -DWITH_1394=OFF -DWITH_AVFOUNDATION=OFF -DWITH_CARBON=OFF -DWITH_CAROTENE=OFF -DWITH_VTK=OFF -DWITH_CUDA=OFF -DWITH_CUFFT=OFF -DWITH_CUBLAS=OFF -DWITH_NVCUVID=OFF -DWITH_EIGEN=OFF -DWITH_VFW=OFF -DWITH_FFMPEG=OFF -DWITH_GSTREAMER=OFF -DWITH_GSTREAMER_0_10=OFF -DWITH_GTK=OFF -DWITH_GTK_2_X=OFF -DWITH_IPP=OFF -DWITH_JASPER=OFF -DWITH_JPEG=OFF -DWITH_WEBP=OFF -DWITH_OPENEXR=OFF -DWITH_OPENGL=OFF -DWITH_OPENVX=OFF -DWITH_OPENNI=OFF -DWITH_OPENNI2=OFF -DWITH_PNG=OFF -DWITH_GDCM=OFF -DWITH_PVAPI=OFF -DWITH_GIGEAPI=OFF -DWITH_ARAVIS=OFF -DWITH_QT=OFF -DWITH_WIN32UI=OFF -DWITH_QUICKTIME=OFF -DWITH_QTKIT=OFF -DWITH_TBB=OFF -DWITH_OPENMP=OFF -DWITH_CSTRIPES=OFF -DWITH_PTHREADS_PF=OFF -DWITH_TIFF=OFF -DWITH_UNICAP=OFF -DWITH_V4L=OFF -DWITH_LIBV4L=OFF -DWITH_DSHOW=OFF -DWITH_MSMF=OFF -DWITH_XIMEA=OFF -DWITH_XINE=OFF -DWITH_CLP=OFF -DWITH_OPENCL=OFF -DWITH_OPENCL_SVM=OFF -DWITH_OPENCLAMDFFT=OFF -DWITH_OPENCLAMDBLAS=OFF -DWITH_DIRECTX=OFF -DWITH_INTELPERC=OFF -DWITH_IPP_A=OFF -DWITH_MATLAB=OFF -DWITH_VA=OFF -DWITH_VA_INTEL=OFF -DWITH_GDAL=OFF -DWITH_GPHOTO2=OFF -DWITH_LAPACK=OFF -DCMAKE_C_FLAGS="-pipe -O3 -march=native -mtune=native" -DCMAKE_CXX_FLAGS="-pipe -O3 -march=native -mtune=native" -DINSTALL_CREATE_DISTRIB=ON -DOPENCV_GENERATE_PKGCONFIG=ON -DBUILD_opencv_dnn=OFF -DBUILD_opencv_gapi=OFF -DCPU_BASELINE="DETECT" -DCPU_DISPATCH=""  ..
fi

if ((( $varname == "2" ))); then
    cmake  -DCMAKE_INSTALL_PREFIX:PATH="$PWD/../../windows-release" -DCMAKE_SKIP_RPATH="ON" -DCMAKE_EXE_LINKER_FLAGS="-static" -DCMAKE_FIND_LIBRARY_SUFFIXES=".a" -DBUILD_opencv_flann=OFF -DBUILD_opencv_ml=OFF -DBUILD_opencv_video=OFF -DBUILD_opencv_shape=OFF -DBUILD_opencv_videoio=OFF -DBUILD_opencv_highgui=OFF -DBUILD_opencv_objdetect=OFF -DBUILD_opencv_superres=OFF -DBUILD_opencv_ts=OFF -DBUILD_opencv_features2d=OFF -DBUILD_opencv_calib3d=OFF -DBUILD_opencv_stitching=OFF -DBUILD_opencv_videostab=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DBUILD_ZLIB=OFF -DBUILD_TIFF=OFF -DBUILD_JASPER=OFF -DBUILD_JPEG=OFF -DBUILD_PNG=OFF -DBUILD_OPENEXR=OFF -DBUILD_TBB=OFF -DBUILD_WITH_STATIC_CRT=ON -DINSTALL_C_EXAMPLES=OFF -DINSTALL_PYTHON_EXAMPLES=OFF -DWITH_1394=OFF -DWITH_AVFOUNDATION=OFF -DWITH_CARBON=OFF -DWITH_CAROTENE=OFF -DWITH_VTK=OFF -DWITH_CUDA=OFF -DWITH_CUFFT=OFF -DWITH_CUBLAS=OFF -DWITH_NVCUVID=OFF -DWITH_EIGEN=OFF -DWITH_VFW=OFF -DWITH_FFMPEG=OFF -DWITH_GSTREAMER=OFF -DWITH_GSTREAMER_0_10=OFF -DWITH_GTK=OFF -DWITH_GTK_2_X=OFF -DWITH_IPP=OFF -DWITH_JASPER=OFF -DWITH_JPEG=OFF -DWITH_WEBP=OFF -DWITH_OPENEXR=OFF -DWITH_OPENGL=OFF -DWITH_OPENVX=OFF -DWITH_OPENNI=OFF -DWITH_OPENNI2=OFF -DWITH_PNG=OFF -DWITH_GDCM=OFF -DWITH_PVAPI=OFF -DWITH_GIGEAPI=OFF -DWITH_ARAVIS=OFF -DWITH_QT=OFF -DWITH_WIN32UI=OFF -DWITH_QUICKTIME=OFF -DWITH_QTKIT=OFF -DWITH_TBB=OFF -DWITH_OPENMP=OFF -DWITH_CSTRIPES=OFF -DWITH_PTHREADS_PF=OFF -DWITH_TIFF=OFF -DWITH_UNICAP=OFF -DWITH_V4L=OFF -DWITH_LIBV4L=OFF -DWITH_DSHOW=OFF -DWITH_MSMF=OFF -DWITH_XIMEA=OFF -DWITH_XINE=OFF -DWITH_CLP=OFF -DWITH_OPENCL=OFF -DWITH_OPENCL_SVM=OFF -DWITH_OPENCLAMDFFT=OFF -DWITH_OPENCLAMDBLAS=OFF -DWITH_DIRECTX=OFF -DWITH_INTELPERC=OFF -DWITH_IPP_A=OFF -DWITH_MATLAB=OFF -DWITH_VA=OFF -DWITH_VA_INTEL=OFF -DWITH_GDAL=OFF -DWITH_GPHOTO2=OFF -DWITH_LAPACK=OFF -DCMAKE_C_FLAGS="-pipe -O3 -march=$cputype -mtune=$cputype -m64" -DCMAKE_CXX_FLAGS="-pipe -O3 -march=$cputype -mtune=$cputype -m64" -DINSTALL_CREATE_DISTRIB=ON -DOPENCV_GENERATE_PKGCONFIG=ON -DBUILD_opencv_dnn=OFF -DBUILD_opencv_gapi=OFF \
	-DCMAKE_C_COMPILER="x86_64-w64-mingw32-gcc" \
	-DCMAKE_C_COMPILER_TARGET=x86_64-w64-mingw32 \
	-DCMAKE_CXX_COMPILER="x86_64-w64-mingw32-g++" \
	-DCMAKE_CXX_COMPILER_TARGET=x86_64-w64-mingw32 \
	-DCMAKE_SYSTEM_NAME=Windows \
	-DCMAKE_SYSTEM_PROCESSOR="x86_64" \
	-DOPENCV_WORKAROUND_CMAKE_20989="ON" \  -DCPU_BASELINE="DETECT" -DCPU_DISPATCH="" ..
fi




make && make install



if ((( $varname == "2" ))); then
	mkdir -p "$PWD/../../windows-release/lib/pkgconfig"
    cp "unix-install/opencv4.pc" "$PWD/../../windows-release/lib/pkgconfig/opencv4.pc"
fi


cd ../../../../


mkdir lib

mkdir tmp



if ((( $varname == "1" ))); then





g++ -c -o tmp/this-system-release_ocv2.o include/ocv2.cpp -I$PWD/dependencies/sha `env PKG_CONFIG_LIBDIR=$PWD/dependencies/opencv/this-system-release/lib/pkgconfig pkg-config --static --cflags --libs opencv4` -std=c++11 -fPIC -O3 -march=native -mtune=native


g++ -shared -o lib/libocv2.so tmp/this-system-release_ocv2.o -I$PWD/dependencies/sha `env PKG_CONFIG_LIBDIR=$PWD/dependencies/opencv/this-system-release/lib/pkgconfig pkg-config --static --cflags --libs opencv4` -std=c++11 -O3 -march=native -mtune=native

cp lib/libocv2.so /usr/local/lib/

cp include/ocv2.hpp /usr/local/include/

ln -s /usr/local/lib/libocv2.so /usr/lib/libocv2.so

ln -s /usr/local/include/ocv2.hpp /usr/include/ocv2.hpp


    echo "
	
	libocv2 successfully installed!
			
			
			
	
	Here are the compiler flags you need to add to any project:

			-I/usr/local/include -L/usr/local/lib -locv2	

	Eg, you can build sample_miner.cpp with this command:
	
			g++ -o sample_miner  sample_miner.cpp -I/usr/local/include -L/usr/local/lib -locv2 -std=c++11

	"
	
	
echo "
	You can include it in projects that support the \"./configure\" command as follows.
	(it doesn't matter in c or c++ project)
	
			./configure CFLAGS=\"-I/usr/local/include -L/usr/local/lib -locv2\" LDFLAGS=\"-I/usr/local/include -L/usr/local/lib -locv2\" LIBS=\"-I/usr/local/include -L/usr/local/lib -locv2\" CPPFLAGS=\"-I/usr/local/include -L/usr/local/lib -locv2\"
	
	Now you can use the 4 functions in it by adding the header below.
	
			#include <ocv2.hpp>
"	
	
	
	
fi

if ((( $varname == "2" ))); then




x86_64-w64-mingw32-g++ -c -o tmp/windows-release_ocv2.o include/ocv2.cpp -I$PWD/dependencies/sha `env PKG_CONFIG_LIBDIR=$PWD/dependencies/opencv/windows-release/lib/pkgconfig pkg-config --static --cflags --libs opencv4` -std=c++11 -O3 -march=$cputype -mtune=$cputype -m64 -DTMP_TEST_ALWAYS_RETURN_TRUE


x86_64-w64-mingw32-g++ -shared -o lib/libocv2.dll tmp/windows-release_ocv2.o -I$PWD/dependencies/sha `env PKG_CONFIG_LIBDIR=$PWD/dependencies/opencv/windows-release/lib/pkgconfig pkg-config --static --cflags --libs opencv4` -std=c++11 -O3 -march=$cputype -mtune=$cputype -m64 -DTMP_TEST_ALWAYS_RETURN_TRUE






	echo "
	
	Compiled lib file here: 
	
			$PWD/lib/libocv2.dll
			
	Required header file here:
			$PWD/include/ocv2.hpp

			( Don't forget to copy this dll to the directory where your application is located!!! )
	
	Here are the compiler flags you need to add to any project:

			-I$PWD/include -L$PWD/lib -locv2
					

	Eg, you can build sample_miner.cpp with this command:
	
			x86_64-w64-mingw32-g++ -o sample_miner.exe  sample_miner.cpp -I$PWD/include -L$PWD/lib -locv2 -std=c++11
	
	"
	
echo "
	You can include it in projects that support the \"./configure\" command as follows.
	(it doesn't matter in c or c++ project)
	
			./configure CFLAGS=\"-I$PWD/include -L$PWD/lib -locv2\" LDFLAGS=\"-I$PWD/include -L$PWD/lib -locv2\" LIBS=\"-I$PWD/include -L$PWD/lib -locv2\" CPPFLAGS=\"-I$PWD/include -L$PWD/lib -locv2\"
	
	Now you can use the 4 functions in it by adding the header below.
	
			#include <ocv2.hpp>
"	
	
    
fi






