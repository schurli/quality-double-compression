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


char leg_a[1023];
char leg_b[1023];
char msssim_a[1023];
char msssim_b[1023];
char sfloat[63];

void
wmetric (int pass, char *current_file, char *encoded_bmp)
{
  // LEG
  sprintf(sfloat,"%f,",apply_metric("leg", current_file, encoded_bmp));
  if(pass)
    strcat(leg_b, sfloat);
  else
    strcat(leg_a, sfloat);
  // MS-SSIM
  sprintf(sfloat,"%f,",apply_metric("ms-ssim", current_file, encoded_bmp));
  if(pass)
    strcat(msssim_b, sfloat);
  else
    strcat(msssim_a, sfloat);
}

int
process_dir(char * input_dir, char *output_dir1, char *output_dir2, float *bpps1, float *bpps2, int bppsc1, int bppsc2)
{
  FTS* file_system = NULL;
  FTSENT* child = NULL;
  FTSENT* parent = NULL;

  char *indir[] = { input_dir, NULL };

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

          while (NULL != child)
            {
              printf("%s%s\n", child->fts_path, child->fts_name);
              char current_file[255];
              char encoded_bmp[255];
              strcpy (current_file, child->fts_path);
              strcat (current_file, child->fts_name);

              // start new line and write current file
              strcpy(leg_a, current_file);
              strcat(leg_a, ",");
              strcpy(leg_b, current_file);
              strcat(leg_b, ",");
              strcpy(msssim_a, current_file);
              strcat(msssim_a, ",");
              strcpy(msssim_b, current_file);
              strcat(msssim_b, ",");
              int i = 0;
              while (i < bppsc1)
                {
                  int j = 0;
                  while (j < 3)
                    {
                      code_image (coders[j], current_file, output_dir1, encoded_bmp, bpps1[i]);
                      wmetric(0, current_file, encoded_bmp);
                      if (bppsc2 != 0)
                        {
                          code_image (coders[j], encoded_bmp, output_dir2, encoded_bmp, bpps2[i]);
                          wmetric(1, current_file, encoded_bmp);
                        }
                      j++;
                    }
                  i++;
                }

              // write new line to file
              out = fopen("out/leg_a.csv", "a");
              fprintf(out, "%s\n", leg_a);
              fclose(out);
              out = fopen("out/leg_b.csv", "a");
              fprintf(out, "%s\n", leg_b);
              fclose(out);
              out = fopen("out/ms-ssim_a.csv", "a");
              fprintf(out, "%s\n", msssim_a);
              fclose(out);
              out = fopen("out/ms-ssim_b.csv", "a");
              fprintf(out, "%s\n", msssim_b);
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
  char *metric = NULL;
  float first_pass[50] = { 0 };
  float second_pass[50] = { 0 };

  int passes1 = 0;
  int passes2 = 0;

  int index;
  int c;

  // read arguments
  opterr = 0;
  int i;
  char *token;
  while ((c = getopt (argc, argv, "i:o:m:a:b:")) != -1)
    switch (c)
      {
      case 'i':
        input_dir = optarg;
        break;
      case 'o':
        output_dir = optarg;
        break;
      case 'm':
        metric = optarg;
        break;
      case 'a':
        i = 0;
        token = strtok(optarg, ",");
        while (token)
          {
            if (i > 49)
              {
                fprintf (stderr, "Too many rates.\n");
                return 1;
              }
            first_pass[i] = atof(token);
            token = strtok(NULL, ",");
            i++;
          }
        passes1 = i;
        break;
      case 'b':
        i = 0;
        token = strtok(optarg, ",");
        while (token)
          {
            if (i > 49)
              {
                fprintf (stderr, "Too many rates.\n");
                return 1;
              }
            second_pass[i] = atof(token);
            token = strtok(NULL, ",");
            i++;
          }
        passes2 = i;
        break;
      case '?':
        fprintf (stderr, "./dcprocess -i [input-db] -o [output-dir] -a \"0.1,0.2,0.3\" -b \"0.075,0.175,0.275\"\n");
        return 1;
      default:
        abort ();
      }

  if (passes2 != 0 && passes1 != passes2)
    {
      fprintf (stderr, "Number of rates for first and second pass must be the same.\n");
      return 1;
    }

  if (passes1 == 0 || input_dir == NULL || output_dir == NULL || metric == NULL)
    {
      fprintf (stderr, "./dcprocess -i [input-db] -o [output-dir] -a \"0.1,0.2,0.3\" -b \"0.075,0.175,0.275\"\n");
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

  return process_dir(input_dir, output_pass_1, output_pass_2, first_pass, second_pass, passes1, passes2);
}