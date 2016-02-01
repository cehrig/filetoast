# filetoast
This program pushes contents of files - from configured directories and patterns - to HTTP using POST

**/etc/filetoast.conf**
// The URL where to POST data to
posturl = "https://example.com"

// The Post string. First %s gets replaced with the file contents
poststring = "%s

// Semicolon separated list of directories"
directories = "/media/sf_dev/FileToast/xml;/tmp/"

// Semicolon separated list of file patterns (see glob)
patterns = "*.xml"

// Maximum number of threads to spawn
maxthreads = 20
