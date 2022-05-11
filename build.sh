#!/bin/bash

if (( $EUID != 0 )); then
    echo "
	Please run as root
	"
    exit
fi

echo "

This script compiles libocv2.dll / libocv2.so and gives you the necessary compiler flags.

(tested on UBUNTU21)

Type Target:

1 - This System
2 - 64 BIT WINDOWS

"




read varname


if ((( $varname != "1" ) && ( $varname != "2" ))); then
    echo "
	incorrect. You will enter 1 or 2
	"
    exit
fi








apt update
apt -y install  git cmake unzip gcc g++ wget pkg-config





if apt -y install g++-mingw-w64-x86-64-posix ; then
    echo ""
else
    apt -y install g++-mingw-w64-x86-64
fi



mkdir dependencies/opencv


cd dependencies/opencv



if [ -d "opencv-70bbf17b133496bd7d54d034b0f94bd869e0e810/build" ]; then
    rm -rf opencv-70bbf17b133496bd7d54d034b0f94bd869e0e810/build
else 
	rm -rf opencv-70bbf17b133496bd7d54d034b0f94bd869e0e810
	rm -rf 70bbf17b133496bd7d54d034b0f94bd869e0e810.zip
    wget https://github.com/opencv/opencv/archive/70bbf17b133496bd7d54d034b0f94bd869e0e810.zip
	unzip 70bbf17b133496bd7d54d034b0f94bd869e0e810.zip
fi


cd opencv-70bbf17b133496bd7d54d034b0f94bd869e0e810
mkdir build && cd build





if ((( $varname == "1" ))); then
    env CC="gcc -m64" CFLAGS=" -pipe -O2" CXX="g++ -m64" CXXFLAGS=" -pipe -O2"  cmake -DCMAKE_INSTALL_PREFIX:PATH="$PWD/../../this-system-release" -DCMAKE_SKIP_RPATH="ON" -DCMAKE_EXE_LINKER_FLAGS="-static" -DCMAKE_FIND_LIBRARY_SUFFIXES=".a" -DBUILD_opencv_flann=OFF -DBUILD_opencv_ml=OFF -DBUILD_opencv_video=OFF -DBUILD_opencv_shape=OFF -DBUILD_opencv_videoio=OFF -DBUILD_opencv_highgui=OFF -DBUILD_opencv_objdetect=OFF -DBUILD_opencv_superres=OFF -DBUILD_opencv_ts=OFF -DBUILD_opencv_features2d=OFF -DBUILD_opencv_calib3d=OFF -DBUILD_opencv_stitching=OFF -DBUILD_opencv_videostab=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DBUILD_ZLIB=OFF -DBUILD_TIFF=OFF -DBUILD_JASPER=OFF -DBUILD_JPEG=OFF -DBUILD_PNG=OFF -DBUILD_OPENEXR=OFF -DBUILD_TBB=OFF -DBUILD_WITH_STATIC_CRT=ON -DINSTALL_C_EXAMPLES=OFF -DINSTALL_PYTHON_EXAMPLES=OFF -DWITH_1394=OFF -DWITH_AVFOUNDATION=OFF -DWITH_CARBON=OFF -DWITH_CAROTENE=OFF -DWITH_VTK=OFF -DWITH_CUDA=OFF -DWITH_CUFFT=OFF -DWITH_CUBLAS=OFF -DWITH_NVCUVID=OFF -DWITH_EIGEN=OFF -DWITH_VFW=OFF -DWITH_FFMPEG=OFF -DWITH_GSTREAMER=OFF -DWITH_GSTREAMER_0_10=OFF -DWITH_GTK=OFF -DWITH_GTK_2_X=OFF -DWITH_IPP=OFF -DWITH_JASPER=OFF -DWITH_JPEG=OFF -DWITH_WEBP=OFF -DWITH_OPENEXR=OFF -DWITH_OPENGL=OFF -DWITH_OPENVX=OFF -DWITH_OPENNI=OFF -DWITH_OPENNI2=OFF -DWITH_PNG=OFF -DWITH_GDCM=OFF -DWITH_PVAPI=OFF -DWITH_GIGEAPI=OFF -DWITH_ARAVIS=OFF -DWITH_QT=OFF -DWITH_WIN32UI=OFF -DWITH_QUICKTIME=OFF -DWITH_QTKIT=OFF -DWITH_TBB=OFF -DWITH_OPENMP=OFF -DWITH_CSTRIPES=OFF -DWITH_PTHREADS_PF=OFF -DWITH_TIFF=OFF -DWITH_UNICAP=OFF -DWITH_V4L=OFF -DWITH_LIBV4L=OFF -DWITH_DSHOW=OFF -DWITH_MSMF=OFF -DWITH_XIMEA=OFF -DWITH_XINE=OFF -DWITH_CLP=OFF -DWITH_OPENCL=OFF -DWITH_OPENCL_SVM=OFF -DWITH_OPENCLAMDFFT=OFF -DWITH_OPENCLAMDBLAS=OFF -DWITH_DIRECTX=OFF -DWITH_INTELPERC=OFF -DWITH_IPP_A=OFF -DWITH_MATLAB=OFF -DWITH_VA=OFF -DWITH_VA_INTEL=OFF -DWITH_GDAL=OFF -DWITH_GPHOTO2=OFF -DWITH_LAPACK=OFF -DCMAKE_C_FLAGS="-mno-avx -msse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2" -DCMAKE_CXX_FLAGS="-mno-avx -msse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2" -DINSTALL_CREATE_DISTRIB=ON ..
fi

if ((( $varname == "2" ))); then
    env CC="x86_64-w64-mingw32-gcc" CFLAGS=" -pipe -O2" CXX="x86_64-w64-mingw32-g++" CXXFLAGS=" -pipe -O2" cmake  -DCMAKE_INSTALL_PREFIX:PATH="$PWD/../../windows-release" -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER_TARGET=x86_64-w64-mingw32 -DCMAKE_CXX_COMPILER_TARGET=x86_64-w64-mingw32 -DCMAKE_SKIP_RPATH="ON" -DCMAKE_EXE_LINKER_FLAGS="-static" -DCMAKE_FIND_LIBRARY_SUFFIXES=".a" -DBUILD_opencv_flann=OFF -DBUILD_opencv_ml=OFF -DBUILD_opencv_video=OFF -DBUILD_opencv_shape=OFF -DBUILD_opencv_videoio=OFF -DBUILD_opencv_highgui=OFF -DBUILD_opencv_objdetect=OFF -DBUILD_opencv_superres=OFF -DBUILD_opencv_ts=OFF -DBUILD_opencv_features2d=OFF -DBUILD_opencv_calib3d=OFF -DBUILD_opencv_stitching=OFF -DBUILD_opencv_videostab=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DBUILD_ZLIB=OFF -DBUILD_TIFF=OFF -DBUILD_JASPER=OFF -DBUILD_JPEG=OFF -DBUILD_PNG=OFF -DBUILD_OPENEXR=OFF -DBUILD_TBB=OFF -DBUILD_WITH_STATIC_CRT=ON -DINSTALL_C_EXAMPLES=OFF -DINSTALL_PYTHON_EXAMPLES=OFF -DWITH_1394=OFF -DWITH_AVFOUNDATION=OFF -DWITH_CARBON=OFF -DWITH_CAROTENE=OFF -DWITH_VTK=OFF -DWITH_CUDA=OFF -DWITH_CUFFT=OFF -DWITH_CUBLAS=OFF -DWITH_NVCUVID=OFF -DWITH_EIGEN=OFF -DWITH_VFW=OFF -DWITH_FFMPEG=OFF -DWITH_GSTREAMER=OFF -DWITH_GSTREAMER_0_10=OFF -DWITH_GTK=OFF -DWITH_GTK_2_X=OFF -DWITH_IPP=OFF -DWITH_JASPER=OFF -DWITH_JPEG=OFF -DWITH_WEBP=OFF -DWITH_OPENEXR=OFF -DWITH_OPENGL=OFF -DWITH_OPENVX=OFF -DWITH_OPENNI=OFF -DWITH_OPENNI2=OFF -DWITH_PNG=OFF -DWITH_GDCM=OFF -DWITH_PVAPI=OFF -DWITH_GIGEAPI=OFF -DWITH_ARAVIS=OFF -DWITH_QT=OFF -DWITH_WIN32UI=OFF -DWITH_QUICKTIME=OFF -DWITH_QTKIT=OFF -DWITH_TBB=OFF -DWITH_OPENMP=OFF -DWITH_CSTRIPES=OFF -DWITH_PTHREADS_PF=OFF -DWITH_TIFF=OFF -DWITH_UNICAP=OFF -DWITH_V4L=OFF -DWITH_LIBV4L=OFF -DWITH_DSHOW=OFF -DWITH_MSMF=OFF -DWITH_XIMEA=OFF -DWITH_XINE=OFF -DWITH_CLP=OFF -DWITH_OPENCL=OFF -DWITH_OPENCL_SVM=OFF -DWITH_OPENCLAMDFFT=OFF -DWITH_OPENCLAMDBLAS=OFF -DWITH_DIRECTX=OFF -DWITH_INTELPERC=OFF -DWITH_IPP_A=OFF -DWITH_MATLAB=OFF -DWITH_VA=OFF -DWITH_VA_INTEL=OFF -DWITH_GDAL=OFF -DWITH_GPHOTO2=OFF -DWITH_LAPACK=OFF -DCMAKE_C_FLAGS="-mno-avx -msse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2" -DCMAKE_CXX_FLAGS="-mno-avx -msse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2" -DINSTALL_CREATE_DISTRIB=ON ..
fi




make && make install



if ((( $varname == "2" ))); then
	mkdir -p "$PWD/../../windows-release/lib/pkgconfig"
    cp "unix-install/opencv.pc" "$PWD/../../windows-release/lib/pkgconfig/opencv.pc"
fi


cd ../../../../


mkdir lib

mkdir tmp



if ((( $varname == "1" ))); then





g++ -c -o tmp/this-system-release_ocv2.o include/ocv2.cpp -I$PWD/dependencies/sha `env PKG_CONFIG_LIBDIR=$PWD/dependencies/opencv/this-system-release/lib/pkgconfig pkg-config --static --cflags --libs opencv` -std=c++11 -fPIC


g++ -shared -o lib/libocv2.so tmp/this-system-release_ocv2.o -I$PWD/dependencies/sha `env PKG_CONFIG_LIBDIR=$PWD/dependencies/opencv/this-system-release/lib/pkgconfig pkg-config --static --cflags --libs opencv` -std=c++11

cp lib/libocv2.so /usr/lib/


    echo "
	
	Compiled lib file here: 
	
			lib/libocv2.so
	
	Here are the compiler flags you need to add to any project:

			-I$PWD/include -L$PWD/lib -locv2	

	For example, you can build sample_miner.cpp with this command:
	
			g++ -o sample_miner  sample_miner.cpp -I$PWD/include -L$PWD/lib -locv2 -std=c++11

	"
fi

if ((( $varname == "2" ))); then




x86_64-w64-mingw32-g++ -c -o tmp/windows-release_ocv2.o include/ocv2.cpp -I$PWD/dependencies/sha `env PKG_CONFIG_LIBDIR=$PWD/dependencies/opencv/windows-release/lib/pkgconfig pkg-config --static --cflags --libs opencv` -std=c++11


x86_64-w64-mingw32-g++ -shared -o lib/libocv2.dll tmp/windows-release_ocv2.o -I$PWD/dependencies/sha `env PKG_CONFIG_LIBDIR=$PWD/dependencies/opencv/windows-release/lib/pkgconfig pkg-config --static --cflags --libs opencv` -std=c++11






	echo "
	
	Compiled lib file here: 
	
			lib/libocv2.dll

			( Don't forget to copy this dll to the directory where your application is located!!! )
	
	Here are the compiler flags you need to add to any project:

			-I$PWD/include -L$PWD/lib -locv2
					

	For example, you can build sample_miner.cpp with this command:
	
			x86_64-w64-mingw32-g++ -o sample_miner.exe  sample_miner.cpp -I$PWD/include -L$PWD/lib -locv2 -std=c++11
	
	"
    
fi



echo "
	You can include it in projects that support the \"./configure\" command as follows.
	(it doesn't matter in c or c++ project)
	
			./configure CFLAGS=\"-I$PWD/include -L$PWD/lib -locv2\" LDFLAGS=\"-I$PWD/include -L$PWD/lib -locv2\" LIBS=\"-I$PWD/include -L$PWD/lib -locv2\" CPPFLAGS=\"-I$PWD/include -L$PWD/lib -locv2\"
	
	Now you can use the 4 functions in it by adding the header below.
	
			#include <ocv2.hpp>
"


