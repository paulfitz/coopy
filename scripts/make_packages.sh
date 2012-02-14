#!/bin/bash

if [ "k$1" = "k" ]; then
    echo "Call with any of: mingw linux osx doc src"
    exit 1
fi

cmake_opts="-DUSE_MYSQL=TRUE -DUSE_REMOTE_SQL=TRUE -DUSE_ACCESS=TRUE -DUSE_GNUMERIC=TRUE -DCMAKE_BUILD_TYPE=Release"

cmake_opts_linux="$cmake_opts"
cmake_opts_win="$cmake_opts"

while [ ! "k$1" = "k" ]; do 

    cfg="$HOME/.COOPY_PATHS"
    
    if [ ! -e $cfg ] ; then
	echo "Need: $cfg"
	echo "of form"
	echo "  MINGW_REPO=/home/paulfitz/cvs/coopy/"
	echo "  MINGW_BUILD=/home/paulfitz/cvs/coopy/mingw"
	echo "  LINUX_CHROOT=/scratch/debian-etch-i386"
	echo "  LINUX_CHROOT_REPO=/home/paulfitz/coopy"
	echo "  LINUX_CHROOT_BUILD=/home/paulfitz/coopy_build"
	echo "  OSX_IP=127.0.0.1"
	echo "  OSX_USER=paulfitz"
	echo "  OSX_PATH=coopy/build"
	echo "  OUTPUT=/tmp/coopy"
	exit 1
    fi
    
    source $cfg
    
    OUTPUT="$OUTPUT/coopy-pack"
    mkdir -p $OUTPUT
    
    if [ "k$1" = "kmingw" ]; then
	echo "Working on MINGW"
	cd $MINGW_REPO
	export LDFLAGS="-L$HOME/mingw/install/lib"
	export CFLAGS="-I$HOME/mingw/install/include"
	git pull || exit 1
	cd $MINGW_BUILD || (
	    echo "Creating $MINGW_BUILD"
	    mkdir -p $MINGW_BUILD
	    cd $MINGW_BUILD || exit 1
	    cmake $cmake_opts_win -DCMAKE_TOOLCHAIN_FILE=$MINGW_REPO/conf/mingwin.cmake $MINGW_REPO || exit 1
	)
	cd $MINGW_BUILD || exit 1
	cmake $cmake_opts_win . || exit 1
	make || exit 1
	rm -f *.exe
	make package || exit 1
	cp -v *.exe $OUTPUT
	cp -v *.zip $OUTPUT

	# Give a no-install version
	#cp -v bin/coopy.exe $OUTPUT/`echo coopy-*.exe | sed "s/.exe/-direct.exe/"`
    fi

    if [ "k$1" = "klinux" ]; then
	cd $LINUX_CHROOT || exit 1
	cd ./$LINUX_CHROOT_REPO || exit 1
	echo "Working on linux repo: $PWD"
	sudo git pull
	cd $LINUX_CHROOT
	cd ./$LINUX_CHROOT_BUILD || exit 1
	test -e static_libs || (
	    echo "Static libraries not set up"
	    exit 1
	    )
	sudo rm -rf *.gz *.sh *.Z
	(
	    echo "cd $LINUX_CHROOT_BUILD"
	    echo "cmake $cmake_opts_linux ."
	    echo "make || ( make clean; make )"
	    echo "make package"
	    echo "make package_source"
	    ) | sudo chroot $LINUX_CHROOT
	cp -v *.gz *.sh *.Z $OUTPUT
    fi

    if [ "k$1" = "kosx" ]; then
	scp $OSX_USER@$OSX_IP:$OSX_PATH/$OSX_FILE $OUTPUT
    fi

    if [ "k$1" = "ksrc" ]; then
	rm -rf /tmp/coopysrc
	mkdir -p /tmp/coopysrc
	cd /tmp/coopysrc || exit 1
	git clone git://github.com/paulfitz/coopy.git || {
	    cd coopy ; git pull; cd ..
	    }
	mkdir -p build
	cd build || exit 1
	cmake ../coopy
	make package_source
	cp -v coopy*.tar.gz $OUTPUT
    fi

    if [ "k$1" = "kdoc" ]; then
	cd build
	cmake .
	make latex
	ver=`cat coopy_version.txt`
	echo $ver
	cd gendoc/latex
	## % workaround \hyperlink bug (see Debian #580912)
	sed -i "s|\\\\usepackage{doxygen}|\\\\renewcommand{\\\\hyperlink}[2]{%\n\\\\hyperref[#1]{#2}%\n}\n\\\\usepackage{doxygen}|" refman.tex
	rm -f refman.dvi refman.ps refman.pdf
	latex refman && latex refman && latex refman && dvips refman.dvi && ps2pdf refman.ps
	cp -v refman.pdf $OUTPUT/coopy-$ver.pdf
    fi

    shift

done
