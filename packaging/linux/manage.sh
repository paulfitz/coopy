#!/bin/bash

if [ -e manage.sh ]; then
	echo "Please call from a build directory"
	exit 1
fi

declare -A check_for_associative_arrays || (
	echo "Update cygwin bash version"
	exit 1
)

# set up build and source directory
BUILD_DIR=$PWD
relative_dir="`dirname $0`"
cd "$relative_dir"
SOURCE_DIR=$PWD

# load bundle settings
if [ "k$1" = "k" ]; then
	echo "Please specify a bundle name.  One of these:"
	echo `cd conf; ls -1 bundle*.sh | sed "s/\.sh//"`
	exit 1
fi
BUNDLE_NAME="$1"
BUNDLE_FILENAME="conf/$1.sh"
if [ ! -e "$BUNDLE_FILENAME" ]; then
	echo "Cannot find $BUNDLE_FILENAME"
	exit 1
fi
source $BUNDLE_FILENAME

# remember command
(
	echo "default:"
	echo -e "\t$0 $1\n"
) > $BUILD_DIR/Makefile

# load compiler settings
source $SOURCE_DIR/conf/compilers.sh || exit 1
if [ -e $SOURCE_DIR/conf/compilers_local.sh ]; then
	source $SOURCE_DIR/conf/compilers_local.sh || exit 1
fi

cd $BUILD_DIR
(
	echo "export SOURCE_DIR='$SOURCE_DIR'"
	echo "export BUNDLE_NAME='$BUNDLE_NAME'"
	echo "export BUNDLE_FILENAME='$SOURCE_DIR/$BUNDLE_FILENAME'"
) > settings.sh

if sudo which debootstrap; then
    echo "Good, debootstrap is available"
else
    sudo apt-get install debootstrap
fi

PLATFORM_SCRIPTS=/usr/share/debootstrap/scripts
echo " "
echo "debootstrap scripts are available on this machine for:"
ls $PLATFORM_SCRIPTS
echo " "

for full_platform in $PLATFORMS; do
    platform=`echo $full_platform | sed "s/_.*//"`
    variant=`echo $full_platform | sed "s/^[^_]*//" | sed "s/_//"`
    PLATFORM_FILE="$PLATFORM_SCRIPTS/$platform"
    if [ ! -e "$PLATFORM_FILE" ]; then
	echo "Do not know how to make $platform"
	exit 1
    fi
    echo "Adding targets for: $full_platform"
    {
	PLATFORM_MIRROR_t=${platform}_MIRROR
	PLATFORM_MIRROR=${!PLATFORM_MIRROR_t}
	echo "PLATFORM_KEY=$platform"
	echo "PLATFORM_VARIANT=$variant"
	echo "PLATFORM_MIRROR=$PLATFORM_MIRROR"
	grep -q "ubuntu\.com" $PLATFORM_FILE && echo "PLATFORM_IS_UBUNTU=true"
	grep -q "ubuntu\.com" $PLATFORM_FILE || echo "PLATFORM_IS_DEBIAN=true"
    } > config_$full_platform.sh
    (
	echo "chroot_$full_platform.txt:"
	echo -e "\t$SOURCE_DIR/src/build_chroot.sh $full_platform chroot_$full_platform && touch chroot_$platform.txt\n"
	echo "coopy_$full_platform.txt:"
	echo -e "\t$SOURCE_DIR/src/build_coopy.sh $full_platform coopy_$full_platform && touch coopy_$platform.txt\n"
	echo "test_$full_platform.txt:"
	echo -e "\t$SOURCE_DIR/src/test_coopy.sh $full_platform && touch test_$full_platform.txt\n"
    ) >> $BUILD_DIR/Makefile
    #  --components=main,universe
    # http://ubuntu.media.mit.edu/ubuntu/
done

(
    echo -n "all:"
    for platform in $PLATFORMS; do
	echo -n " chroot_$platform.txt"
    done
    for platform in $PLATFORMS; do
	echo -n " coopy_$platform.txt"
    done
    for platform in $PLATFORMS; do
	echo -n " test_$platform.txt"
    done
    echo -e "\n"
) >> $BUILD_DIR/Makefile

echo " "
echo "Makefile prepared."
