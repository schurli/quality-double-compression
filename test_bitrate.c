#include <stdio.h>

/**
 * Command to compress JPEG2000
 * 1st wildcard is input file (bmp)
 * 2nd wildcard is output file (j2k)
 * 3rd wildcard is quality setting e.g. 30,40,50. Must be increasing 1st < 2nd < 3rd layer.
 */
char path_jpeg2000_enc[] = "./lib/openjpeg-2.1.0/bin/opj_compress -i %s -o %s -q %s";

/**
 * Command to decompress JPEG2000
 * 1st wildcard is input file (j2k)
 * 2nd wildcard is output file (bmp)
 */
char path_jpeg2000_dec[] = "./lib/openjpeg-2.1.0/bin/opj_decompress -i %s -o %s";

/**
 * Command to compress JPEG XR
 * 1st wildcard is input file (bmp)
 * 2nd wildcard is output file (jxr)
 * 3rd wildcard is quality setting 0.0 - 1.0
 */
char path_jpegxr_enc[] = "./lib/jxrlib-master/JxrEncApp -i %s -o %s -q %f";

/**
 * Command to decompress JPEG XR
 * 1st wildcard is input file (jxr)
 * 2nd wildcard is output file (bmp)
 */
char path_jpegxr_dec[] = "./lib/jxrlib-master/JxrDecApp -i %s -o %s";

/**
 * Command to compress JPEGv9
 * 1st wildcard is quality setting 0 - 100
 * 2nd wildcard is output file (jpg)
 * 3rd wildcard is input file (bmp)
 */
char path_jpeg_enc[] = "./lib/jpeg-9a/cjpeg -quality %d -outfile %s %s";


/**
 * Command to decompress JPEGv9
 * 1st wildcard is output file (bmp)
 * 2nd wildcard is input file (jpg)
 */
char path_jpeg_dec[] = "./lib/jpeg-9a/djpeg -outfile %s %s";

/**
 * JPEG2000: Function to encode a bmp file to a j2k file with configurable quality.
 * @param char* in Path to input bmp file
 * @param char* out Path to output j2k file
 * @param int quality Something between 1 and 100.
 * @return int exist status of encoding process.
 */
int
jpeg_2000_enc(char* in, char* out, int quality)
{
  char command[1000];
  char quality2000[15];

  /**
   * Generate quality setting "a,b,c"
   * a = 5 - 80, quality * 75 / 100 + 5
   * b = 10 - 90, quality * 80 / 100 + 10
   * c = 15 - 100, quality * 85 / 100 + 15
   */
  sprintf(quality2000, "%d,%d,%d", (quality * 75 / 100 + 5), (quality * 80 / 100 + 10), (quality * 85 / 100 + 15));

  sprintf(command, path_jpeg2000_enc, in, out, quality2000);
  printf("exec: %s\n", command);
  return system(command);
}

/**
 * JPEG2000: Function to decode a j2k file to bmp.
 * @param char* in Path to input j2k file
 * @param char* out Path to output bmp file
 * @return int exit status of decoding process.
 */
int
jpeg_2000_dec(char* in, char* out)
{
  char command[1000];
  sprintf(command, path_jpeg2000_dec, in, out);
  printf("exec: %s\n", command);
  return system(command);
}

/**
 * JPEGXR: Function to encode a bmp file to a jxr file with configurable quality.
 * @param char* in Path to input bmp file
 * @param char* out Path to output jxr file
 * @param int quality Something between 1 and 100.
 * @return int exist status of encoding process.
 */
int
jpeg_xr_enc(char* in, char* out, int quality)
{
  char command[1000];

  /**
   * Generate quality setting 0.0-1.0
   * q = (double) quality / 100
   */
  sprintf(command, path_jpegxr_enc, in, out, ((double) quality / 100));
  printf("exec: %s\n", command);
  return system(command);
}

/**
 * JPEGXR: Function to decode a jxr file to bmp.
 * @param char* in Path to input jxr file
 * @param char* out Path to output bmp file
 * @return int exit status of decoding process.
 */
int
jpeg_xr_dec(char* in, char* out)
{
  char command[1000];
  sprintf(command, path_jpegxr_dec, in, out);
  printf("exec: %s\n", command);
  return system(command);
}

/**
 * JPEGv9: Function to encode a bmp file to a jpg file with configurable quality.
 * @param char* in Path to input bmp file
 * @param char* out Path to output jpg file
 * @param int quality Something between 1 and 100.
 * @return int exist status of encoding process.
 */
int
jpeg_enc(char* in, char* out, int quality)
{
  char command[1000];

  sprintf(command, path_jpeg_enc, quality, out, in);
  printf("exec: %s\n", command);
  return system(command);
}

/**
 * JPEGv9: Function to decode a jpg file to bmp.
 * @param char* in Path to input jpg file
 * @param char* out Path to output bmp file
 * @return int exit status of decoding process.
 */
int
jpeg_dec(char* in, char* out)
{
  char command[1000];
  sprintf(command, path_jpeg_dec, out, in);
  printf("exec: %s\n", command);
  return system(command);
}


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

  char out_enc_jpg[] = "out/test.jpg";
  char out_enc_jpg2k[] = "out/test.j2k";
  char out_enc_jpgxr[] = "out/test.jxr";

  char out_dec_jpg[] = "out/test.jpg.bmp";
  char out_dec_jpg2k[] = "out/test.j2k.bmp";
  char out_dec_jpgxr[] = "out/test.jxr.bmp";

  int q = 5;

  jpeg_enc(input_file, out_enc_jpg, q);
  jpeg_dec(out_enc_jpg, out_dec_jpg);

  jpeg_2000_enc(input_file, out_enc_jpg2k, q);
  jpeg_2000_dec(out_enc_jpg2k, out_dec_jpg2k);

  jpeg_xr_enc(input_file, out_enc_jpgxr, q);
  jpeg_xr_dec(out_enc_jpgxr, out_dec_jpgxr);

  return 0;
}