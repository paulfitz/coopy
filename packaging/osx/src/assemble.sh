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
./bin/brew install -vd pkg-config gawk || exit 1
for dep in mdbtools mysql-connector-c gettext gtk+ goffice libgsf gnumeric gettext wxmac; do
  ./bin/brew install -vd $dep --universal || exit 1
done
./bin/brew install -vd coopy --HEAD || exit 1

cd $BUILD_DIR
export BUNDLE_HOMEBREW=$PWD/homebrew

APP=coopy

rm -rf $APP.app* || exit 1
mkdir -p $APP.app || exit 1
cd $APP.app || exit 1
APP_DIR=$PWD
mkdir -p Contents || exit 1
cd Contents || exit 1
mkdir -p MacOS || exit 1
mkdir -p Resources || exit 1
mkdir -p Frameworks || exit 1

cp $SOURCE_DIR/conf/Info.plist $APP_DIR/Contents || exit 1
cp $SOURCE_DIR/conf/coopy.icns $APP_DIR/Contents/Resources || exit 1

coopy_bins=`cd $BUILD_DIR/homebrew/Cellar/coopy/*/bin/; ls`
gnumeric_bins=`cd $BUILD_DIR/homebrew/Cellar/gnumeric/*/bin/; ls | grep -v 1`

for tool in $coopy_bins $gnumeric_bins; do

  if [ ! -e "$BUNDLE_HOMEBREW/bin/$tool" ]; then
	echo "Cannot find $BUNDLE_HOMEBREW/bin/$tool"
	exit 1
  fi

  TARGET_DIR=$APP_DIR/Contents/MacOS
  cp -L $BUNDLE_HOMEBREW/bin/$tool $TARGET_DIR || exit 1
  cd $APP_DIR || exit 1
  chmod u+w $TARGET_DIR/$tool || exit 1
  for f in `cd $TARGET_DIR; otool -L $tool | egrep -v "[[:space:]]/usr/" | egrep -v "[[:space:]]/System" | sed "s/(.*//" | grep "/"`; do
	fb=`echo $f | sed "s/2.8.0.dylib/2.8.0.7.0.dylib/"`
	echo "Processing [$f]"
        base=`basename $fb`
	f2=$TARGET_DIR/../Frameworks/$base
	if [ ! -e $f2 ]; then
	  cp $f $f2 || exit 1
	fi
        install_name_tool -change $f @executable_path/../Frameworks/$base $TARGET_DIR/$tool || exit 1
	chmod u+w $f2 || exit 1
        install_name_tool -change $f @executable_path/../Frameworks/$base $f2
        install_name_tool -id @executable_path/../Frameworks/$base $f2 || exit 1
	otool -L $f2 | grep $f && exit 1
	chmod u-w $f2 || exit 1
  done
  chmod u-w $TARGET_DIR/$tool || exit 1

done

for rep in 1 2 3 4 5; do
for f0 in `cd $TARGET_DIR/../Frameworks; ls *.dylib`; do
  for f in `cd $TARGET_DIR/../Frameworks; otool -L $f0 | egrep -v "[[:space:]]/usr/" | egrep -v "[[:space:]]/System" | sed "s/(.*//" | grep "/"`; do
	fb=`echo $f | sed "s/2.8.0.dylib/2.8.0.7.0.dylib/"`
	echo "Processing [$f]"
        base=`basename $fb`
	f2=$TARGET_DIR/../Frameworks/$base
	if [ ! -e $f2 ]; then
	  cp $f $f2 || exit 1
	fi
	chmod u+w $TARGET_DIR/../Frameworks/$f0 || exit 1
        install_name_tool -change $f @executable_path/../Frameworks/$base $TARGET_DIR/../Frameworks/$f0 || exit 1
	chmod u-w $TARGET_DIR/../Frameworks/$f0 || exit 1
  done
  chmod u+w  $TARGET_DIR/../Frameworks/$f0 || exit 1
  install_name_tool -id @executable_path/../Frameworks/$f0 $TARGET_DIR/../Frameworks/$f0 || exit 1
  chmod u-w  $TARGET_DIR/../Frameworks/$f0 || exit 1
done
done

cp -R -L $BUNDLE_HOMEBREW/etc $TARGET_DIR/../Frameworks/etc || exit 1
mkdir -p $TARGET_DIR/../Frameworks/lib || exit 1
cp -R -L $BUNDLE_HOMEBREW/lib/gdk-pixbuf-2.0 $TARGET_DIR/../Frameworks/lib || exit 1
cp -R -L $BUNDLE_HOMEBREW/lib/gnumeric $TARGET_DIR/../Frameworks/lib || exit 1
cp -R -L $BUNDLE_HOMEBREW/lib/goffice $TARGET_DIR/../Frameworks/lib || exit 1
cp -R -L $BUNDLE_HOMEBREW/lib/pango $TARGET_DIR/../Frameworks/lib || exit 1

for f0 in `cd $TARGET_DIR/../Frameworks/lib; find . -iname "*.so"`; do
  for f in `cd $TARGET_DIR/../Frameworks/lib; otool -L $f0 | egrep -v "[[:space:]]/usr/" | egrep -v "[[:space:]]/System" | sed "s/(.*//" | grep "/"`; do
	echo "Processing [$f]"
        base=`basename $f`
	chmod u+w $TARGET_DIR/../Frameworks/lib/$f0 || exit 1
        install_name_tool -change $f @executable_path/../Frameworks/$base $TARGET_DIR/../Frameworks/lib/$f0 || exit 1
	chmod u-w $TARGET_DIR/../Frameworks/lib/$f0 || exit 1
  done
done

sed -i "" "s|/.*lib/|@executable_path/../Frameworks/lib/|" $TARGET_DIR/../Frameworks/etc/pango/pango.modules || exit 1
(
echo "[Pango]"
echo "ModuleFiles = @executable_path/../Frameworks/etc/pango/pango.modules"
) > $TARGET_DIR/../Frameworks/etc/pango/pango.rc

cd $BUILD_DIR || exit 1
zip -r coopy.app.zip coopy.app || exit 1
