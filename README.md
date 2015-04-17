# quality-double-compression
PS Multimedia Datenformate

## Sources:

* JPEG2000: https://code.google.com/p/openjpeg/
* JPEG XR: https://github.com/curasystems/jxrlib (clone of https://jxrlib.codeplex.com/)
* JPEGv9: http://www.infai.org/jpeg/
* Image Quality Assessment: http://tdistler.com/iqa/
* Visual Quality Index Implementations: http://wavelab.at/sources/VQI/?hc_location=ufi

## Dependencies:

* For building 3rd party libraries: `sudo apt-get install libbsd-dev cmake libfreeimage-dev`
* For documentation: `sudo apt-get install doxygen`

## Building:

To build 3rd party libraries use the global makefile `make` and `make clean` in the root directory or one of the
options below. Once this is done go to the development section below.
Use `make doc` to view documentation and `make doc-clean` to clean the generated doc.

* JXRLIB

    cd lib/jxr-1.1/
    make

* JPEG-9A

    cd lib/jpg-9a/
    ./configure && make

* OPENJPEG-2.1.0

    cd lib/j2k-2.1.0/
    cmake .
    make

* IQA

    cd lib/iqa
    make RELEASE=1

* VQI: Make sure you have downloaded and added the sources manually.

    cd lib/vqi-3.3.0/
    

## De-/encoding:

* JXRLIB

    * `./lib/jxr-1.1/JxrEncApp [options]`
    * `./lib/jxr-1.1/JxrDecApp [options]`
    * Call binaries without options to get more info.

* JPEG-9A
    * `./lib/jpg-9a/cjpeg [switches] [imagefile] > [jpegfile]`
    * `./lib/jpg-9a/djpeg [switches] [jpegfile] > [imagefile]`
    * See `./lib/jpg-9a/usage.txt` for switches.

* OPENJPEG-2.1.0
    * `./lib/j2k-2.1.0/bin/opj_compress -i image.pgm -o image.j2k`
    * `./lib/j2k-2.1.0/bin/opj_decompress -i image.j2k -o image.pgm`
    * See `./lib/j2k-2.1.0/bin/opj_compress -h` for further options.


## Calculate metrics

* VQI
    * `./imagemetric dl/[metric.dl] [original] [compare]`
    * Available metrics are: `ess.dl,leg.dl,lfbvs.dl,lss.dl,mse.dl,nice.dl,psnr.dl,psnry.dl,ssim.dl,sun10a.dl,yao09a.dl`


## Development

Create a standard conform bmp file in the root directory called mikro.bmp. 
Use e.g. GIMP and enable the compatibility export option in the BMP save dialog.
Use `make bitrate` to create test images in `out/`.
