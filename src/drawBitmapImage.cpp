//
// Created by Ying Yuan on 2020/12/16.
//
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct recordStorageUsage {
    bool reg_write[32];
    bool reg_read[32];
    bool memory[16];
    bool stack;
};

extern recordStorageUsage record;

const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

void generateBitmapImage(unsigned char* image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int stride);
unsigned char* createBitmapInfoHeader(int height, int width);
void drawBitmapImage(int imageFileNum);

/**
 * Draws Bitmap Image
 * @param imageFileNum The number of calling this function, used for naming bmp image.
 * Includes resetting recordStorageUsage.
 */
void drawBitmapImage (int imageFileNum)
{
    int height = 50;
    int width = 400;
    unsigned char image[height][width][BYTES_PER_PIXEL];
    stringstream ss;
    ss << imageFileNum << ".bmp";
    char * imageFileName = (char *)ss.str().data();

    int i, j, k;
    //initialize the image to be white
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++) {
            image[i][j][2] = (unsigned char) (255); //red
            image[i][j][1] = (unsigned char) (255); //green
            image[i][j][0] = (unsigned char) (255); //blue
        }
    //initialize the grid to be black
    for (i = 5; i <= 45; i += 10)
        for (j = 50; j <= 270; j++)
            for (k = 0; k <= 2; k++) {
                image[i][j][k] = (unsigned char) (0);
            }        
    for (i = 5; i <= 45; i++)
        for (j = 50; j <= 210; j += 20)
            for (k = 0; k <= 2; k++) {
                image[i][j][k] = (unsigned char) (0);
            } 
    for (i = 5; i <= 45; i++)
        for (j = 225; j <= 270; j += 15)
            for (k = 0; k <= 2; k++) {
                image[i][j][k] = (unsigned char) (0);
            }
    for (i = 5; i <= 45; i++)
        for (k = 0; k <= 2; k++) {
            image[i][350][k] = (unsigned char) (0);
            }
    for (j = 271; j <= 350; j++)
        for (k = 0; k <= 2; k++) {
            image[5][j][k] = (unsigned char) (0);
            image[45][j][k] = (unsigned char) (0);
        }
    //draw register
    for (int reg = 0; reg < 32; reg++) {
        if (record.reg_read[reg] || record.reg_write[reg]) {
            int a = (3 - (reg / 8)) * 10 + 6;
            int b = (reg % 8) * 20 + 51;
            for (i = a; i <= a + 8; i++)
                for (j = b; j <= b + 18; j++) {
                    if (record.reg_write[reg] && !record.reg_read[reg]) {
                        image[i][j][0] = (unsigned char)0;
                        image[i][j][1] = (unsigned char)0;
                    }
                    else if (record.reg_read[reg] && !record.reg_write[reg]) {
                        image[i][j][1] = (unsigned char)0;
                        image[i][j][2] = (unsigned char)0;
                    }
                    else {
                        image[i][j][1] = (unsigned char)0;
                    }                         
            }
        }
    }
    //draw memory    
    for (int mem = 0; mem <= 15; mem++) {
        if (record.memory[mem]) {
            int a = (3 - (mem / 4)) * 10 + 6;
            int b = (mem % 4) * 15 + 211;
            for (i = a; i <= a + 8; i++)
                for (j = b; j <= b + 13; j++) {
                    image[i][j][0] = (unsigned char)0;
                    image[i][j][2] = (unsigned char)0;
                }
        }
    }
    //draw stack
    if (record.stack) {
        for (i = 6; i <= 44; i++)
            for (j = 271; j <= 349; j++) {
                image[i][j][1] = (unsigned char)192;
                image[i][j][0] = (unsigned char)0;
            }
    }
   
    generateBitmapImage((unsigned char*) image, height, width, imageFileName);
    //printf("Image generated!!");

     //reset recordStorageUsage
    for (i = 0; i < 32; i++) {
        record.reg_read[i] = false;
        record.reg_write[i] = false;
    }
    for (i = 0; i < 16; i++) {
        record.memory[i] = false;
    }
    record.stack = false;
}

void generateBitmapImage (unsigned char* image, int height, int width, char* imageFileName)
{
    int widthInBytes = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(height, stride);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    int i;
    for (i = 0; i < height; i++) {
        fwrite(image + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader (int height, int width)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}
