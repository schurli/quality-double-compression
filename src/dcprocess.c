#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <errno.h>
#include "coder.h"
#include "metrics.h"


int compare (const FTSENT**, const FTSENT**);


char leg[32767];
char msssim[32767];
char vif[32767];
char sfloat[63];

/**
 * Calculate and write metric log.
 * @param char * current_file Path to reference file
 * @param char * encoded_bmp Path to calculated file
 */
void
wmetric (char *current_file, char *encoded_bmp)
{
  // LEG
  sprintf(sfloat,"%f,",apply_metric("leg", current_file, encoded_bmp));
  strcat(leg, sfloat);

  // MS-SSIM
  sprintf(sfloat,"%f,",apply_metric("ms-ssim", current_file, encoded_bmp));
  strcat(msssim, sfloat);

  // VIF
  sprintf(sfloat,"%f,",apply_metric("vif", current_file, encoded_bmp));
  strcat(vif, sfloat);
}

/**
 * Process a directory filled with reference images.
 * @param char * input_dir Path to directory to process
 * @param char * output_dir1 Path for first encoding pass output
 * @param char * output_dir2 Path for second encoding pass output, the calculated and compared files.
 * @param float * bpps List of bpp values
 * @param int bppsc Length of bpp list
 * @return int Success
 */
int
process_dir(char * input_dir, char *output_dir1, char *output_dir2, float *bpps, int bppsc)
{
  FTS* file_system = NULL;
  FTSENT* child = NULL;
  FTSENT* parent = NULL;

  char *indir[] = { input_dir, NULL };

  // list of valid coders to use
  char *coders[] = { "jpg","j2k","jxr" };
  FILE *out;

  file_system = fts_open(indir,FTS_COMFOLLOW | FTS_NOCHDIR, &compare);

  if (NULL != file_system)
    {
      while ((parent = fts_read(file_system)) != NULL)
        {
          child = fts_children(file_system,0);

          if (errno != 0)
            {
              perror("fts_children");
            }

          // process one reference image
          while (NULL != child)
            {
              printf("%s%s\n", child->fts_path, child->fts_name);
              char current_file[255];
              char encoded_bmp[255];

              strcpy (current_file, child->fts_path);
              strcat (current_file, child->fts_name);

              // start new line and write current file
              strcpy(leg, current_file);
              strcat(leg, ",");
              strcpy(msssim, current_file);
              strcat(msssim, ",");
              strcpy(vif, current_file);
              strcat(vif, ",");

              // create jpegs with 50%, 75%, 85%
              char source_file[4][255];
              strcpy(source_file[0], current_file);
              code_jpeg(current_file, output_dir1, source_file[1], 50);
              code_jpeg(current_file, output_dir1, source_file[2], 75);
              code_jpeg(current_file, output_dir1, source_file[3], 85);

              // process each bpp setting
              int i = 0;
              while (i < bppsc)
                {
                  //printf("<tr><td>%f</td>", bpps[i]);
                  // apply each coder
                  int j = 0;
                  while (j < 3)
                    {
                      // encode each probably previously encoded file in source file list
                      int si = 0;
                      while (si < 4)
                        {
                          code_image (coders[j], source_file[si], output_dir2, encoded_bmp, bpps[i]);
                          // compare with the reference image
                          wmetric (current_file, encoded_bmp);
                          // unlink(encoded_bmp); // remove encoded bmp
                          /*if (si > 0)
                            {
                              unlink(source_file[si]); // remove first pass encoded bmp
                            }*/
                          si++;
                        }
                      //printf("<td>");

                      j++;
                      //printf("</td>");
                    }
                  //printf("</tr>\n");
                  printf("\n");
                  i++;
                }

              // write new line to file
              out = fopen("out/leg.csv", "a");
              fprintf(out, "%s\n", leg);
              fclose(out);
              out = fopen("out/ms-ssim.csv", "a");
              fprintf(out, "%s\n", msssim);
              fclose(out);
              out = fopen("out/vif.csv", "a");
              fprintf(out, "%s\n", vif);
              fclose(out);
              // next file
              child = child->fts_link;
            }
        }
      fts_close(file_system);
    }
  return 0;
}

int compare(const FTSENT** one, const FTSENT** two)
{
    return (strcmp((*one)->fts_name, (*two)->fts_name));
}

/**
 * The main procedure
 * @param int argc argument count
 * @param char* argv array of command line arguments
 * @return int status
 */
int
main(int argc, char* argv[])
{
  char *input_dir = NULL;
  char *output_dir = NULL;
  float first_pass[500] = { 0 };

  int first_passc = 0;

  int index;
  int c;

  // read arguments
  opterr = 0;
  int i;
  char *token;
  while ((c = getopt (argc, argv, "i:o:a:")) != -1)
    switch (c)
      {
      case 'i':
        input_dir = optarg;
        break;
      case 'o':
        output_dir = optarg;
        break;
      case 'a':
        i = 0;
        token = strtok(optarg, ",");
        while (token)
          {
            if (i > 499)
              {
                fprintf (stderr, "Too many rates.\n");
                return 1;
              }
            first_pass[i] = atof(token);
            token = strtok(NULL, ",");
            i++;
          }
        first_passc = i;
        break;
      case '?':
        fprintf (stderr, "./dcprocess -i [input-db] -o [output-dir] -a \"0.1,0.2,0.3\"\n");
        return 1;
      default:
        abort ();
      }

  if (first_passc == 0 || input_dir == NULL || output_dir == NULL)
    {
      fprintf (stderr, "./dcprocess -i [input-db] -o [output-dir] -a \"0.1,0.2,0.3\"\n");
      return 1;
    }

  // create output directory for 1st and 2nd pass
  char output_pass_1[255];
  strcpy (output_pass_1, output_dir);
  strcat (output_pass_1, "/1st/");
  mkdir (output_pass_1, 0777);
  char output_pass_2[255];
  strcpy (output_pass_2, output_dir);
  strcat (output_pass_2, "/2nd/");
  mkdir (output_pass_2, 0777);

  int error = 0;

  return process_dir(input_dir, output_pass_1, output_pass_2, first_pass, first_passc);
}