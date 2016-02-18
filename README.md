# filetoast
This program pushes contents of files - from configured directories and patterns - to HTTP using POST and vice versa.

**Configuration**
-
See cfg/ for a sample /etc/filetoast.conf

**Init Script**
-
See init/ for a sample init script. The binary has to be place in /usr/bin/filetoast

**Installation**
-
autogen.sh is provided to build through autotools. The steps in order: autoreconf, build tree, configure, make, make install to /usr/bin/filetoast

**Dependencies**
-
libconfig
libcurl