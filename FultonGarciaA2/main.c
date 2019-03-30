/*  Name: Fulton Garcia
*   PantherID: 5464347
*   This program was written manipulate bitmap files
*   This shows how to read and write binary data
*   I affirm that I wrote this program myself without any help form any other people or sources from the internet.
*   Even though the skeleton was written for us I changed some of the variable names and descriptions like cols and rows
*   to height and width so that it would be easier for me to understand what exactly I was working with.
*/

#include <stdlib.h>
#include "bmplib.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>


/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * height     - the original number of height
 * width     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the height and columns, e.g.
 *           if scale=2, then 2* height and 2*width
 *
 * new      - the new array containing the PIXELs, allocated within
 * newcols  - the new number of width (scale*width)
 */

int enlarge(PIXEL* original, int height, int width, int scale, PIXEL** new, int* newrows, int* newcols) {

  int row, col;

  if ((height <= 0) || (width <= 0)) return -1;

  *newrows = scale*height;
  *newcols = scale*width;

  *new = (PIXEL*)malloc(scale*height*scale*width*sizeof(PIXEL));

  for(row = 0; row < height; row++){
    for(col = 0;col < width;col++){
      PIXEL* o = original + row*width + col;
      int newrow,newcol;
	
		for(newrow = row*scale; newrow < (row*scale) + scale; newrow++){
			for(newcol = col*scale; newcol < (col*scale) + scale; newcol++){
				PIXEL* n = (*new) + newrow*scale*width + newcol;
				*n = *o;
			}
		}                
  }        
}
   
  return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read 
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 * 
 * original - an array containing the original PIXELs, 3 bytes per each
 * height     - the number of height
 * width     - the number of columns
 * rotation - a positive or negative rotation, 
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of height
 * newcols  - the new number of width
 */

int rotate(PIXEL* original, int height, int width, int rotation, PIXEL** new, int* newrows, int* newcols){
  //this doesn't work with negative rotations and it was something that I should have looked
  //at the instructures and rubric fully before writing
 int row, col;

  if ((height <= 0) || (width <= 0)){
    return -1;
  }

  *new = (PIXEL*)malloc(height*width*sizeof(PIXEL));

  int numberOfRotations;
  numberOfRotations = (rotation/90) % 4;

  if(numberOfRotations < 0){
	  numberOfRotations += 4;
  }

  int tr = height; 	
  for(row = 0; row < height; row++){
	for(col = 0;col < width;col++){
		PIXEL* o = original + row*width + col;
		int offset;

		switch(numberOfRotations){
			case 1	:
      //I lied i fixed it by subtracting 1
			offset  = col*height + tr - 1;
			break;
			case 2  :
			offset = ((height-row-1)*width) + (width-col-1);
			break;
			case 3	:
			offset = ((width-col-1)*height) + row;
			break;
			default :
			offset = row*width + col;
			break;	
		}
		PIXEL* n = (*new) + offset;
		*n = *o;
	}
	tr--;
  }
   *newcols = height;
   *newrows = width;
   
   return 0;
}

/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile(). 
 * 
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * height     - the number of height
 * width     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int flip (PIXEL *original, PIXEL **new, int height, int width) {
  int row, col;

  if ((height <= 0) || (width <= 0)) return -1;

  *new = (PIXEL*)malloc(height*width*sizeof(PIXEL));

  for (row=0; row < height; row++)
    for (col=0; col < width; col++) {
      PIXEL* o = original + row*width + col;
      PIXEL* n = (*new) + row*width + (width-1-col);
      *n = *o;
    }

  return 0;
}

int main(int argc,char **argv)
{
  extern char *optarg;
  
  extern int optind;
  
  static char usage[] = "usage: bmptool [-s scale | -r degree | -f | -v ] [-o output_file] [input_file] \n";
  
  int c = 0, eFlag = 0, sFlag = 0, rFlag = 0, vFlag = 0, fFlag = 0, oFlag = 0;
  

  int scale;
  int degree;
  char *sFile, *oFile;

  while ( (c = getopt ( argc, argv, "vs:r:vfo:?" ) ) != -1){
	switch(c)
	{
		case 's':
			sFlag = 1;
			scale = atoi ( optarg );
			break;
		case 'r':
			rFlag = 1;
			degree = atoi ( optarg );
			break;
    case 'v':
      vFlag = 1;
      break;
		case 'f':
			fFlag = 1;
			break;
		case 'o':
			oFlag = 1;
			oFile = optarg;
			break;
		case '?':
			eFlag = 1;
			break;
		default:
			eFlag = 1;
			break;		
	}
  }

  if(eFlag){
    fprintf(stderr,usage,argv[0]);
    exit(1);
  }

  sFile = argv[optind];
 
  int height, width;
  PIXEL *b, *nb;

  int fd[2];

  pipe(fd);

  if(oFlag == 0){
	  oFile = NULL;
  } 

  readFile( sFile  , &height, &width, &b);

//if the s flag is passed that means that we need to scale
  if(sFlag){
    enlarge(b, height, width, scale, &nb, &height, &width);
    free(b);
    b = nb;
  }

  //we need to rotate if the r flag is passed
  if(rFlag){   
    rotate(b, height, width, degree, &nb, &height, &width);
    free(b);
    b = nb;
  }

//vertical flip is just a -180 rotation so i just use that instead of writing another
//function
  if(vFlag){
    rotate(b, height, width, -180, &nb, &height, &width);
    free(b);
	  b = nb;
  }

//flip was already prewritten for ys
  if(fFlag){
    flip(b, &nb, height, width);
    free(b);
    b = nb;
  }

  writeFile( oFile , height, width, b);
    
  free(b);
  return 0;
}
