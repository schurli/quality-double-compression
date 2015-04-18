# quality-double-compression
PS Multimedia Datenformate

## Sources:

* JPEG2000: https://code.google.com/p/openjpeg/
* JPEG XR: https://github.com/curasystems/jxrlib (clone of https://jxrlib.codeplex.com/)
* JPEGv9: http://www.infai.org/jpeg/
* Image Quality Assessment: http://tdistler.com/iqa/
* Visual Quality Index Implementations: http://wavelab.at/sources/VQI/?hc_location=ufi
* Video Quality Metrics: https://github.com/aizvorski/video-quality

## Dependencies:

* For building 3rd party libraries: `sudo apt-get install libbsd-dev cmake libfreeimage-dev`
* `apt-get install python python-scipy`
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

    cd lib/iqa-1.1.2/
    make RELEASE=1

* VQI: Make sure you have downloaded and added the sources manually.

    cd lib/vqi-3.3.0/
    make all

* VQM: Video Quality Metrics, Python


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

* IQA
    *

* VQI
    * `./imagemetric dl/[metric.dl] [original] [compare]`
    * Available metrics are: `ess.dl,leg.dl,lfbvs.dl,lss.dl,mse.dl,nice.dl,psnr.dl,psnry.dl,ssim.dl,sun10a.dl,yao09a.dl`

* FQM
    * `python runfiv [origial] [compare]`

## Development

Create a standard conform bmp file in the root directory called cat.bmp.
Use e.g. GIMP and enable the compatibility export option in the BMP save dialog.
Use `make dcprocess` to compile the main program.

## Usage and output

Output is one CSV file per metric (e.g. `leg_a.csv`). Each row stores the data for one image resource. The first column
contains an unique identifier (e.g. [db]/[filename]). The other columns are divided into groups, where each group
contains the metric result for jpg, j2k and jxr. There are `n` groups in total, one for each specified target rate.

The target rates can be specified as command line argument in the form of a comma separated list. The program expects
two rate lists, if double compression should be applied. Following command line parameters are recognized:

    ./dcprocess -i [input-db] -o [output-dir] -a "0.1,0.2,0.3" -b "0.075,0.175,0.275"

    -i Path to a directory containing sample images
    -o Output directory for all temporary images and the output CSV file
    -a List of bits per pixel for first pass. The output is stored in `[metric]_a.csv`
    -b List of bits per pixel for second pass. The output is stored in `[metric]_b.csv`

How to read the CSV files:

    test-db/cat.bmp,0.931577,0.687969,0.977865,0.972277,0.982529,0.965975,0.970626,0.913864,0.985756,0.976757,0.986396,0.973639,0.978300,0.933185,0.985756,0.983548,0.988546,0.980116,

    [identifier],[JPG with 1st bpp setting],[J2K with 1st bpp setting],[JXR with 1st bpp setting],[JPG with 2nd bpp setting],...

Pass 1 goes into `[metric]_a.csv`, pass 2 into `[metric]_b.csv`. The metrics are applied to the original file from the database
ant the result of the current pass.