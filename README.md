# quality-double-compression
PS Multimedia Datenformate

## Dependencies:

* `sudo apt-get install libbsd-dev cmake`

## Building:

Use the global makefile `make` and `make clean` in the root directory or one of the options below.

### JXRLIB

* `cd lib/jxrlib-master/`
* `make`

### JPEG-9A

* `cd lib/jpeg-9a/`
* `./configure && make`

### OPENJPEG-2.1.0

* `cd lib/openjpeg-2.1.0/`
* `cmake .`
* `make`

### IQA

* `cd lib/iqa`
* `make RELEASE=1`

## De-/encoding:

### JXRLIB

* `./lib/jxrlib-master/JxrEncApp [options]`
* `./lib/jxrlib-master/JxrDecApp [options]`

Call binaries without options to get more info.

### JPEG-9A

* `./lib/jpeg-9a/cjpeg [switches] [imagefile] > [jpegfile]`
* `./lib/jpeg-9a/djpeg [switches] [jpegfile] > [imagefile]`

See `./lib/jpeg-9a/usage.txt` for switches.

### OPENJPEG-2.1.0


* `./lib/openjpeg-2.1.0/bin/opj_compress -i image.pgm -o image.j2k`
* `./lib/openjpeg-2.1.0/bin/opj_decompress -i image.j2k -o image.pgm`

See `./lib/openjpeg-2.1.0/bin/opj_compress -h` for further options.

