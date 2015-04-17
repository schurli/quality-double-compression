#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "metrics.h"
#include "coder.h"
#include "../lib/iqa-1.1.2/include/iqa.h"
#include "bmp.h"

static const struct iqa_ms_ssim_args args_wang = {
    1,  /* Wang */
    1,  /* Gaussian (default) */
    5,  /* Scales (default) */
    0,  /* alphas (default) */
    0,  /* betas (default) */
    0,  /* gammas (default) */
};

void striptrail( char *line )
{
  int i = strlen(line) - 1;
  while ( i > 0 )
    {
      if ( line[i] == '\n' )
        {
          line[i] = '\0';
        }
      else
        {
          break;
        }
      i--;
    }
}

float
vqi (char *metric, char *original, char *compare)
{
  char command[1000];
  // execute VQI metric
  char *execpath = "lib/vqi-0.3.3/imagemetric lib/vqi-0.3.3/dl/%s.dl %s %s > /tmp/out.txt";
  sprintf(command, execpath, metric, original, compare);

  FILE *fp;
  char buf[128];
  float value_float = 0.0;
  system(command);
  int i = 0;
  if ((fp = fopen("/tmp/out.txt", "r")) == NULL)
    {
      printf("Error opening file!\n");
      return -1;
    }

  while (fgets(buf, 1024, fp) != NULL)
    {
      striptrail(buf);
      value_float = atof(buf);
      break;
    }

  fclose(fp);
  remove("/tmp/out.txt");

  return value_float;
}

float
apply_metric (char *metric, char *original, char *compare)
{

  if (strcmp(metric,"leg") == 0 || strcmp(metric,"nice") == 0)
    {
      return vqi (metric, original, compare);
    }
  else if (strcmp(metric,"ms-ssim") == 0)
    {
      struct bmp orig_bmp, comp_bmp;
      load_bmp(original, &orig_bmp);
      load_bmp(compare, &comp_bmp);
      float res = iqa_ms_ssim(orig_bmp.img, comp_bmp.img, orig_bmp.w, orig_bmp.h, orig_bmp.stride, &args_wang);
      free_bmp(&orig_bmp);
      free_bmp(&comp_bmp);
      if(res == INFINITY)
        {
          return (float) 0.0;
        }
      else
        return res;
    }
  else
    {
      return (float) 0.0;
    }
}