/**
 * @file
 * @brief Main file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

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
char path_jpeg_dec[] = "./lib/jpeg-9a/djpeg -bmp -outfile %s %s";

/**
 * Directory for output.
 */
char output_dir[] = "out/";

/**
 * Bitmap header.
 */
typedef struct
{
  char magic[2];
  char size[4];
  char reserved[4];
  char offset[4];
  char dibbytes[4];
  char width[4];
  char height[4];
  char colorplanes[2];
  char bpp[2];
  char rawsize[4];
  char hor_res[4];
  char ver_res[4];
  char colors[4];
  char important[4];
} bmp_t;

/**
 * Type definition of an encode function pointer.
 * (Interface)
 */
typedef int (*encodeFunction)(char*, char*, int);

/**
 * Type definition of an decode function pointer.
 * (Interface)
 */
typedef int (*decodeFunction)(char*, char*);

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
  // printf("exec: %s\n", command);
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
  // printf("exec: %s\n", command);
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
  // printf("exec: %s\n", command);
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
  // printf("exec: %s\n", command);
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
  // printf("exec: %s\n", command);
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
  // printf("exec: %s\n", command);
  return system(command);
}

/**
 * Convert little endian char array to unsigned int.
 * @param char* arr Char array with length 4
 * @return unsinged int Number representation
 */
unsigned int
char_to_uint(char* arr)
{
  return arr[0] + arr[1]*256 + arr[2]*256*256 + arr[3]*256*256*256;
}

/**
 * Read number of pixels from bmp file.
 * @param char* filename Path to the bmp file.
 * @return unsigned int Number of pixels in bmp file.
 */
unsigned int
get_number_of_pixels_bmp(char* filename)
{
  char * buffer;
  FILE * file = fopen(filename, "rb");
  buffer = (char *) malloc(sizeof(bmp_t));
  fread(buffer, 1, sizeof(bmp_t), file);
  bmp_t * bmp = (bmp_t *) buffer;
  fclose(file);
  return char_to_uint(bmp->width)*char_to_uint(bmp->height);
}

/**
 * Read filesize of file.
 * @param char* filename Path to file
 * @return unsigned int Size
 */
unsigned int
filesize(char* filename)
{
  int size;
  FILE *fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END);
  size = ftell(fp);
  fclose(fp);
  return size;
}

/**
 * JPEG2000: Encode input image (bmp) to jpeg2000. The result will habe bpp bits per pixel.
 * The result will also be decoded to a bmp file for further analysis.
 * @param encodeFunction enc Function pointer to encoder function.
 * @param decodeFunction dec Function pointer to decoder function.
 * @param char* ext File extension for the encoded file.
 * @param char* in Path to input bmp.
 * @param double bpp Bits per pixel of dest.
 * @return int error (0 is no error), int
 */
int
encode_image(encodeFunction enc, decodeFunction dec, char* ext, char* in, double bpp)
{
  char out_jpeg[1000];
  char out_bmp[1000];

  // create jpeg file path
  strcpy(out_jpeg, output_dir);
  strcat(out_jpeg, basename(in));
  // remove .bmp
  out_jpeg[(strlen(out_jpeg) - 4)] = '\0';
  strcat(out_jpeg, ".");
  strcat(out_jpeg, ext);
  // derive bmp path from jpeg path
  strcpy(out_bmp, out_jpeg);
  strcat(out_bmp, ".bmp");

  // Get total number of pixels in image
  unsigned int pixels = get_number_of_pixels_bmp(in);

  // Calc desired size
  unsigned int des_size = (int) ((double) pixels * bpp);

  // Perform a binary search to find optimal quality setting.
  // Quality ranges from 0 to 100 and 2 raised by the pow of 6 is 64. Quality is an integer, thus 6 steps in
  // the binary search is the max number of steps to find the perfect setting to get the desired bpp.
  double quality = 50.0;
  double step = 50.0;
  int size = 0;
  int i;
  for (i = 0; i < 6; i++)
    {
      (*enc)(in, out_jpeg, (int) quality);
      size = filesize(out_jpeg);
      // printf("Type %s (size: %d, target size: %d, quality: %f)\n", ext, size, des_size, quality);
      step /= 2.0;
      if (size < des_size)
        { // increase quality
          quality += step;
        }
      else
        { // decrease quality
          quality -= step;
        }
      if (quality <= 0.0)
        {
          quality = 1.0;
        }
      if (quality >= 100.0)
        {
          quality = 100.0;
        }
    }

  // Decode and store result as bmp.
  (*dec)(out_jpeg, out_bmp);

  printf("Type %s (size: %d, target size: %d, quality: %f)\n", ext, size, des_size, quality);
  return 0;
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
  char input_file[1000];
  float bppf;
  if (argc > 2) {
    strcpy(input_file, argv[1]);
    bppf = atof(argv[2]);
  } else {
    printf("usage: test_bitrate image.bmp 0.08\n");
    return 0;
  }

  double bpp = (double) bppf;
  printf("Parameters: %f bpp and input file %s\n", bpp, input_file);
  encode_image(&jpeg_2000_enc, &jpeg_2000_dec, "j2k", input_file, bpp);
  
  encode_image(&jpeg_xr_enc, &jpeg_xr_dec, "jxr", input_file, bpp);
  encode_image(&jpeg_enc, &jpeg_dec,"jpg", input_file, bpp);

  return 0;
}