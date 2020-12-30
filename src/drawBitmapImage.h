// 
// Created by Ying Yuan on 2020/12/16.
//

#ifndef DRAW_H
#define DRAW_H
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
/**
 * Defines recordStorageUsage, used for drawing bitmap images.
 */
struct recordStorageUsage {
    bool reg_write[32];
    bool reg_read[32];
    bool memory[16];
    bool stack;
};
/**
 * Draws Bitmap Image
 * @param imageFileNum The number of calls of this function, used for naming bmp image.
 * Includes resetting recordStorageUsage.
 */
void drawBitmapImage (int imageFileNum);

#endif
