# filetoast
This program pushes contents of files - from configured directories and patterns - to HTTP using POST

**Configuration**
-
See cfg/ for a sample /etc/filetoast.conf

**Init Script**
See init/ for a sample init script. The binary has to be place in /usr/bin/filetoast

**Binary**
-
A linux compiled binary can be found in bin.

bin/filetoast: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, not stripped

**Dependencies**
-
libconfig
libcurl