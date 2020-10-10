
#include <stdio.h>
#include <stdlib.h> // For exit() function
#include "my_quadtree.h"



//This first function is use to check if grip of image is a power of 2
//Only images that are power of 2s can be manipulated by the program
//the function is called in the validPic function below during checking
int powerOfTwo (int x){
    while (((x % 2) == 0) && x > 1) /* While x is even and > 1 */
        x /= 2;
    return x;
}

int validPic(int x){
    printf("Your grid is %dx%d\n", x,x);
    //variable declaration sets mins and max pixels
    int widthOfPixels = x;
    int powerofTwo = powerOfTwo(widthOfPixels);
    int minPixelWidth = 2;
    int maxPixelWidth = 16;

    if (powerofTwo != 1){
            puts("The width of the image is not a power of 2\n");
            return 0;
    }if (widthOfPixels<minPixelWidth){
            puts("The image file is too small\n");
            return 0;
    }if (widthOfPixels>maxPixelWidth){
            puts("The image file is too large\n");
            return 0;}
    else{
      return 1; }
}


//function is used to check if another pixel is in grid
int colourCheck(int xAxis, int yAxis, int pixelWidth, int** grid){
    int maxPixelWidth = pixelWidth*pixelWidth; //for total grid size
    int sumOfBlacks = 0; //initialising sum of black pixels to 0
    int x,y= 0;

    //calculates sum of black pixels
    for(x=xAxis; x<xAxis+pixelWidth; x++){
        for(y=yAxis;  y<yAxis+pixelWidth; y++){
            if (grid[x][y] == 0){
              sumOfBlacks++;
            }
        }
    }

    //printf("Total black %d\n",sumOfBlacks);
    //printf("pixel width %d\n",pixelWidth);
    //printf("max pixel width: %d\n",maxPixelWidth);


    //returns wheather another pixel has been found based based on above function
    if (sumOfBlacks==maxPixelWidth){
        //puts("The image is only black pixels\n");
        return 0;

    }else if ((sumOfBlacks<maxPixelWidth) && (sumOfBlacks > 0)){
        //puts("There is another colour present grey\n");
        return 2;

    }else if (sumOfBlacks == 0){
        //puts("The colour is white\n");
        return 1;
      }
}

quadTree displayBlackNodes(quadTree showQuadtree){
  if (showQuadtree->colour==0){
    printf("Black terminal node at position (%d,%d) with width %d:\n",showQuadtree->xAxis, showQuadtree->yAxis, showQuadtree->pixelWidth);

  }else if(showQuadtree->colour==2){
    displayBlackNodes(showQuadtree->NW);
    displayBlackNodes(showQuadtree->NE);
    displayBlackNodes(showQuadtree->SW);
    displayBlackNodes(showQuadtree->SE);

  }return 0;
}


//check the Node
quadTree addNode(int xAxis,int yAxis, int pixelWidth, int** grid){
   int returnedColour = colourCheck(xAxis, yAxis, pixelWidth, grid);

   //0 Represents black
   //1 Represents white
   //2 Represents grey

   quadTree quadtree = malloc(sizeof(struct quadTree));
   if (returnedColour == 0){

     //0 is used to indicate a black pixel, so this sets colour to black
     quadtree -> xAxis = xAxis;
     quadtree -> yAxis = yAxis;
     quadtree -> colour = 0;
     quadtree-> pixelWidth = pixelWidth;

     quadtree -> NW = NULL;
     quadtree -> SW = NULL;
     quadtree -> NE = NULL;
     quadtree -> SE = NULL;


   }else if (returnedColour == 1){
    //1 is used to indicate a white pixel, so this sets colour to white
     quadtree -> xAxis = xAxis;
     quadtree ->  yAxis = yAxis;
     quadtree -> colour = 1;
     quadtree-> pixelWidth = pixelWidth;

     quadtree -> NW = NULL;
     quadtree -> SW = NULL;
     quadtree -> NE = NULL;
     quadtree -> SE = NULL;

   }else if (returnedColour == 2){
      //2 is used to indicate a grey pixel, so this sets colour to grey
     quadtree -> colour = 2;
     pixelWidth = pixelWidth/2;
     quadtree->NW = addNode(xAxis, yAxis, pixelWidth, grid);
     quadtree->SW = addNode(xAxis, yAxis+pixelWidth, pixelWidth, grid);
     quadtree->NE = addNode(xAxis+pixelWidth, yAxis, pixelWidth, grid);
     quadtree->SE = addNode(xAxis+pixelWidth, yAxis+pixelWidth, pixelWidth, grid);
   }return(quadtree);
}




//main function
int main(int argc, char* argv[]){
    //open file
    int widthFromFile;
    int numOfBlackPixels;


    //opening the file
    FILE *fptr;
    if ((fptr = fopen(argv[1], "r")) == NULL){

        // Program exits if file pointer returns NULL.
        printf("Error! opening file");
        exit(1);

    // reads text until newline
    }fscanf(fptr,"%i", &widthFromFile);
    fscanf(fptr,"%i", &numOfBlackPixels);

    //some line of codes used during testing have been commented out
    /*printf("Width from the file: %i\n", widthFromFile);
    printf("Number of black Pixels from the file: %i\n", widthFromFile);
 */
    //checks if width of image is valid
    int picFrame = validPic(widthFromFile); //width oF image is parameter passed to validPic function
    if (picFrame == 1){
        printf("\n");
    }else{
        exit (-1);
    }

    //create grid
    int **grid = malloc(widthFromFile *sizeof(int*));
    int maxPixelWidth = widthFromFile;
    int x, y, xFromFile, yFromFile;

    //dynamiically allocating the array
    for (x=0;x<maxPixelWidth; x++){
      grid[x]=malloc(maxPixelWidth *sizeof(int*));
    }

    //sets the grid as all 1
    for(x = 0; x < maxPixelWidth; x++){
      for(y = 0; y < maxPixelWidth; y++){
            grid[x][y] = 1;
            }
    }

    //adding black pixels in the grid
    for(y = 0; y < numOfBlackPixels; y++){
      fscanf(fptr,"%d %d ", &xFromFile, &yFromFile);
      //printf("%d x %d \n", xFromFile, yFromFile);
      grid[xFromFile][yFromFile] = 0;
    }


    //print grid
    /* for(x = 0; x < maxPixelWidth; x++){
        if (x%8==0){
          printf("\n");
        }for(y = 0; y < maxPixelWidth; y++){
              if (y%8==0){
                printf("\n");
              }
            printf("[%d]", grid[x][y]);
            }
      }  printf("\n \n");

      */


    //creating values for the quadtree
    quadTree showQuadtree = addNode(0,0,maxPixelWidth, grid);
    printf("\n");
    displayBlackNodes(showQuadtree);

    //close file
    fclose(fptr);

    return 0;

}
