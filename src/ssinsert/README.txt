
This directory is no longer needed.  Functionality has been
integrated into sspatch command.
-P

===========

How to compile:

* Limitation: be on a Linux machine
* Install gnumeric (ideally through your package manager)
* Install gnumeric source (ideally through your package manager)
* Run "./configure" on your gnumeric source, do anything it needs
  you to do in order to get configured (e.g. installing extra -dev
  packages).
* In the "src" subdirectory of gnumeric source, do 
  "make gnumeric-paths.h".  If having trouble, see next step.
* You may need to take other steps to bring all header files into
  existence.  If you're having trouble, just do "make" in the
  gnumeric source directory.
* When compiling Coopy, turn on "GNUMERIC" flag in ccmake/cmake-gui
* Give path to Gnumeric source in "GNUMERIC_ROOT" in ccmake/cmake-gui


