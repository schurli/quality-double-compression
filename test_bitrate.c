#include <stdio.h>

/**
 * Command to compress JPEG2000
 * 1st wildcard is input file (bmp)
 * 2nd wildcard is output file (j2k)
 * 3rd wildcard is quality setting e.g. 30,40,50. Must be increasing 1st < 2nd < 3rd layer.
 */
char path_jpeg2000[] = "./lib/openjpeg-2.1.0/bin/opj_compress -i %s -o %s -q %s";

/**
 * Command to compress JPEG XR
 * 1st wildcard is input file (bmp)
 * 2nd wildcard is output file (jxr)
 * 3rd wildcard is quality setting 0.0 - 1.0
 */
char path_jpegxr[] = "./lib/jxrlib-master/JxrEncApp -i %s -o %s -q %s";

/**
 * Command to compress JPEGv9
 * 1st wildcard is quality setting 0 - 100
 * 2nd wildcard is output file (jpg)
 * 3rd wildcard is input file (bmp)
 */
char path_jpeg[] = "./lib/jpeg-9a/cjpeg -outfile %s %s";// -quality %s

/**
 * The main procedure
 * @param int argc argument count
 * @param char* argv array of command line arguments
 * @return int status
 */
int
main(int argc, const char* argv[])
{
  char* input_file;
  if (argc > 1) {
    input_file = argv[1];
  } else {
    printf("usage: test_bitrate image.bmp\n");
    return 0;
  }

  // create JPEG2000
  char output_file2000[] = "out/test.j2k";
  char quality2000[] = "30,40,50";
  char command2000[1000];

  sprintf(command2000, path_jpeg2000, input_file, output_file2000, quality2000);
  printf("exec: %s\n", command2000);
  system(command2000);

  // create JPEG XR
  char output_filexr[] = "out/test.jxr";
  char qualityxr[] = "0.5";
  char commandxr[1000];

  sprintf(commandxr, path_jpegxr, input_file, output_filexr, qualityxr);
  printf("exec: %s\n", commandxr);
  system(commandxr);

  // create JPEGv9
  char output_file[] = "out/test.jpg";
  char quality[] = "50";
  char command[1000];

  sprintf(command, path_jpeg, output_file, input_file);//quality,
  printf("exec: %s\n", command);
  system(command);

  return 0;
}