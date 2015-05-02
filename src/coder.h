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
 * Read dimensions of bitmap image.
 * @param char* filename Path to the bmp file.
 * @return unsigned int* Number of pixels in width and height of bmp file.
 */
unsigned int*
get_dim(char* filename);

/**
 * Final wrapper for all supported codecs.
 * @param char* ext File extension of the encoded file.
 * @param char* in Path to input bmp.
 * @param char* outdir Path to output dir.
 * @param char* out Path to output file.
 * @param double bpp Bits per pixel of dest.
 * @return int error (0 is no error), int
 */
int
code_image (char* ext, char* in, char* outdir, char* out, double bpp);

/**
 * Create a BMP with the quality of a JPEG image with specified quality setting.
 * @param char * in Input file
 * @param char * outdir Output directory
 * @param char * out_bmp Output path of bmp file
 * @param int quality Quality of jpeg file
 * @return int Success
 */
int
code_jpeg (char* in, char* outdir, char* out_bmp, int quality);