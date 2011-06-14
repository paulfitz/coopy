#!/bin/bash

BUILD_DIR=$PWD

source settings.sh || exit 1

source $BUNDLE_FILENAME || exit 1

if [ ! -e homebrew ]; then
  git clone -b coopy git://github.com/paulfitz/homebrew.git || {
	rm -rf homebrew
	exit 1
  }
fi

export MACOSX_DEPLOYMENT_TARGET=10.5
cd homebrew || exit 1
for dep in mdbtools mysql-connector-c gettext gtk+ gnumeric gettext wxmac; do
  ./bin/brew install -vd $dep --universal || exit 1
done
./bin/brew install -vd coopy --HEAD || exit 1

cd $BUILD_DIR
export BUNDLE_HOMEBREW=$PWD/homebrew

APP=coopy

rm -rf $APP.app || exit 1
mkdir -p $APP.app || exit 1
cd $APP.app || exit 1
APP_DIR=$PWD
mkdir -p Contents || exit 1
cd Contents || exit 1
mkdir -p MacOS || exit 1
mkdir -p Resources || exit 1

cp $SOURCE_DIR/conf/Info.plist $APP_DIR/Contents || exit 1
cp $SOURCE_DIR/conf/coopy.icns $APP_DIR/Contents/Resources || exit 1

for tool in coopy ssformat csvformat; do

  if [ ! -e "$BUNDLE_HOMEBREW/bin/$tool" ]; then
	echo "Cannot find $BUNDLE_HOMEBREW/bin/$tool"
	exit 1
  fi

  TARGET_DIR=$APP_DIR/Contents/MacOS
  cp $BUNDLE_HOMEBREW/bin/$tool $TARGET_DIR || exit 1
  cd $APP_DIR || exit 1
  chmod u+w $TARGET_DIR/$tool || exit 1
  for f in `cd $TARGET_DIR; otool -L $tool | egrep -v "[[:space:]]/usr/lib" | egrep -v "[[:space:]]/System" | sed "s/(.*//" | grep "/"`; do
	echo "Processing [$f]"
	if [ ! -e $TARGET_DIR/`basename $f` ]; then
	  cp $f $TARGET_DIR || exit 1
	fi
        install_name_tool -change $f @executable_path/`basename $f` $TARGET_DIR/$tool || exit 1
  done
  chmod u-w $TARGET_DIR/$tool || exit 1

done
