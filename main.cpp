#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <gl/glut.h>
#define KEY_ESC 27 /* GLUT doesn't supply this */

#pragma region property
struct imageFile
{
    int width;
    int height;
    unsigned char *data;
};

// Globals.
static GLuint texture[17];
const int numTextures = 17; // Array of texture ids.
static float angle = 0.0;   // Angle to rotate textured square.
// Local storage untuk image data.
imageFile *images[numTextures];

int fullscreen = 0;
int mouseDown = 0;

float xrot = 0;
float yrot = 305;

float xdiff = 100;
float ydiff = 100;

float tra_x = 0;
float tra_y = 0;
float tra_z = 0;

float grow_shrink = 80;
float resize_f = 1;

float windowWidth = 800;
float windowHeight = 800;

float open1 = 0;
float open2 = -.7;

float open3 = 0;
float open4 = 0;

bool startIdle = false;
bool onAnimation = false;

// animation 1 property
float moveBoardGame = -1.5;
float openBoardGame = .1;

// animation 2 property
float moveBook = -2.3;
float openBookRotate = 0;
float openBookRotateX = 0;
float openBookUp = 0;

#pragma endregion

imageFile *getBMP(std::string fileName)
{
    int offset, // No. of bytes to start of image data in input BMP file.
        w,      // Width in pixels of input BMP file.
        h;      // Height in pixels of input BMP file.

    // Initialize imageFile objects.
    imageFile *tempStore = new imageFile; // Temporary storage.
    imageFile *outRGB = new imageFile;    // RGB output file.
    imageFile *outRGBA = new imageFile;   // RGBA output file.

    // Initialize input stream.
    std::ifstream inFile(fileName.c_str(), std::ios::binary);

    // Get start point of image data in input BMP file.
    inFile.seekg(10);
    inFile.read((char *)&offset, 4);

    // Get image width and height.
    inFile.seekg(18);
    inFile.read((char *)&w, 4);
    inFile.read((char *)&h, 4);

    // Determine the length of padding of the pixel rows
    // (each pixel row of a BMP file is 4-byte aligned by padding with zero bytes).
    int padding = (3 * w) % 4 ? 4 - (3 * w) % 4 : 0;

    // Allocate storage for temporary input file, read in image data from the BMP file, close input stream.
    tempStore->data = new unsigned char[(3 * w + padding) * h];
    inFile.seekg(offset);
    inFile.read((char *)tempStore->data, (3 * w + padding) * h);
    inFile.close();

    // Set image width and height and allocate storage for image in output RGB file.
    outRGB->width = w;
    outRGB->height = h;
    outRGB->data = new unsigned char[3 * w * h];

    // Copy data from temporary input file to output RGB file adjusting for padding and performing BGR to RGB conversion.
    int tempStorePos = 0;
    int outRGBpos = 0;
    for (int j = 0; j < h; j++)
        for (int i = 0; i < 3 * w; i += 3)
        {
            tempStorePos = (3 * w + padding) * j + i;
            outRGBpos = 3 * w * j + i;
            outRGB->data[outRGBpos] = tempStore->data[tempStorePos + 2];
            outRGB->data[outRGBpos + 1] = tempStore->data[tempStorePos + 1];
            outRGB->data[outRGBpos + 2] = tempStore->data[tempStorePos];
        }

    // Set image width and height and allocate storage for image in output RGBA file.
    outRGBA->width = w;
    outRGBA->height = h;
    outRGBA->data = new unsigned char[4 * w * h];

    // Copy image data from output RGB file to output RGBA file, setting all A values to 1.
    for (int j = 0; j < 4 * w * h; j += 4)
    {
        outRGBA->data[j] = outRGB->data[(j / 4) * 3];
        outRGBA->data[j + 1] = outRGB->data[(j / 4) * 3 + 1];
        outRGBA->data[j + 2] = outRGB->data[(j / 4) * 3 + 2];
        outRGBA->data[j + 3] = 0xFF;
    }

    // Release temporary storage and the output RGB file and return the RGBA version.
    delete[] tempStore;
    delete[] outRGB;
    return outRGBA;
}

// Load external textures.
void loadTextures()
{
    // Nama file tekstur
    const char *textureFiles[numTextures] = {
        "Textures/Texture.bmp",           // 0
        "Textures/Box.bmp",               // 1
        "Textures/Book Vertical 1.bmp",   // 2
        "Textures/Lock.bmp",              // 3
        "Textures/Handle.bmp",            // 4
        "Textures/Book Vertical 2.bmp",   // 5
        "Textures/Book Vertical 3.bmp",   // 6
        "Textures/Book Vertical 4.bmp",   // 7
        "Textures/Book Vertical 5.bmp",   // 8
        "Textures/Book Horizontal 1.bmp", // 9
        "Textures/Lubang Kunci.bmp",      // 10
        "Textures/Board Game 1.bmp",      // 11
        "Textures/Board Game 2.bmp",      // 12
        "Textures/Board Game 3.bmp",      // 13
        "Textures/Board Game 4.bmp",      // 14
        "Textures/Board Game 5.bmp",      // 15
        "Textures/Background.bmp"         // 16

        // Tambahkan lebih banyak nama file tekstur sesuai kebutuhan
    };

    // Loop untuk memuat setiap tekstur
    for (int i = 0; i < numTextures; i++)
    {
        // Memuat gambar
        images[i] = getBMP(textureFiles[i]);

        // Membuat dan mengikat objek tekstur
        // glBindTexture(GL_TEXTURE_2D, texture[i]);

        // Mengatur parameter wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Mengatur parameter filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

void drawBackground()
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glRotatef(40, 0, 1, 0);
    glTranslatef(0, 0, -50);
    glScalef(100, 100, .1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[16]->width, images[16]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[16]->data);
    glutSolidCube(.5);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawBox()
{
    glTranslatef(tra_x, tra_y, tra_z);
    glRotatef(xrot, 1, 0, 0);
    glRotatef(yrot, 0, 1, 0);

#pragma region MAIN SHAPE (Texture Done)

    // BACK VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-3, 0, 0);
    glScalef(.05, 5, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // LEFT VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2, 0, -1.5);
    glScalef(2, 5, .05);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // RIGHT VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2, 0, 1.5);
    glScalef(2, 5, .05);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // TOP VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2, 2.5, 0);
    glScalef(2, .05, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // BUTTON VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2, -2.4, 0);
    glScalef(2, .2, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // MID HORIZON VIEW 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2, 1.8, 0);
    glScalef(1.5, .05, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // MID HORIZON VIEW 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2, 1.1, 0);
    glScalef(1.5, .05, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // MID HORIZON VIEW 3
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2, .5, 0);
    glScalef(2, .05, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // MID VERTICAL VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.2, -1, 0);
    glScalef(1.6, 2.9, .05);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
#pragma endregion

#pragma region DETAIL

#pragma region BOOK

    // ORDERED FROM LEFT TO RIGHT

#pragma region HORIZONTAL BOOK (Texture Done)

    //  BOOK 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.5, 1.85, .55);
    glScalef(.2, .1, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[9]->width, images[9]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[9]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.5, 1.97, .55);
    glScalef(.2, .15, .8);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[9]->width, images[9]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[9]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 3
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.5, 1.85, -1);
    glScalef(.2, .1, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[9]->width, images[9]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[9]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 4
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.5, 1.95, -1);
    glScalef(.2, .1, .8);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[9]->width, images[9]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[9]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 5
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.5, 2.05, -1);
    glScalef(.2, .1, .8);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[9]->width, images[9]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[9]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

#pragma endregion

#pragma region VERTICAL BOOK (Texture Done)

    glPushMatrix();
    glTranslatef(moveBook, 2.1, 1.3); // animation book
    //  BOOK 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glRotatef(openBookRotate, openBookRotateX, 0, 0);
    glTranslatef(0, openBookUp, .05);
    glScalef(.8, .5, .05);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[2]->width, images[2]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[2]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glRotatef(-openBookRotate, openBookRotateX, 0, 0);
    glTranslatef(0, openBookUp, .05);
    glScalef(.8, .5, .05);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[2]->width, images[2]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[2]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();

    //  BOOK 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, 1.15);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[5]->width, images[5]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[5]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 3
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, 1);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[6]->width, images[6]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[6]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 4
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2.05, .8);
    glScalef(.8, .45, .1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[7]->width, images[7]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[7]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 5
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, .65);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[5]->width, images[5]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[5]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 6
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, .5);
    glScalef(.8, .4, .1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[2]->width, images[2]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[2]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 7
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, .45);
    glScalef(.8, .4, .06);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[6]->width, images[6]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[6]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 8
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, .4);
    glScalef(.8, .4, .06);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[5]->width, images[5]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[5]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 9
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, .35);
    glScalef(.8, .4, .06);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[7]->width, images[7]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[7]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 10
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, .2);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[2]->width, images[2]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[2]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 11
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, .05);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[8]->width, images[8]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[8]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 12
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-2.3, 2, -.1);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[5]->width, images[5]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[5]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 13
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.9, .1, .1, 1);
    glTranslatef(-2.3, 2.1, -.25);
    glScalef(.8, .6, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[7]->width, images[7]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[7]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 14
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.9, .9, .1, 1);
    glTranslatef(-2.3, 2.1, -.4);
    glScalef(.8, .6, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[6]->width, images[6]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[6]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 15
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.5, .9, .1, 1);
    glTranslatef(-2.3, 2, -.55);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[8]->width, images[8]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[8]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 16
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.3, .9, .5, 1);
    glTranslatef(-2.3, 2, -.7);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[6]->width, images[6]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[6]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 17
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.3, .7, .4, 1);
    glTranslatef(-2.3, 2, -.85);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[2]->width, images[2]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[2]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 18
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.6, .2, .5, 1);
    glTranslatef(-2.3, 2.1, -1);
    glScalef(.8, .6, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[5]->width, images[5]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[5]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 19
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.2, .2, .5, 1);
    glTranslatef(-2.3, 2.1, -1.15);
    glScalef(.8, .6, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[7]->width, images[7]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[7]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 20
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.2, .2, .2, 1);
    glTranslatef(-2.3, 2, -1.3);
    glScalef(.8, .4, .15);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[6]->width, images[6]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[6]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOOK 21
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.2, .6, .1, 1);
    glTranslatef(-2.3, 2, -1.43);
    glScalef(.8, .4, .1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[8]->width, images[8]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[8]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

#pragma endregion

#pragma endregion

#pragma region BOARD GAME UP (Texture Done)
    //  BOARD GAME 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.75, 2.65, 1);
    glScalef(1.5, .2, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.75, 2.85, 1);
    glScalef(1.3, .2, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 3
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.75, 2.65, -1);
    glScalef(1.3, .2, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 4
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.75, 2.85, -1);
    glScalef(1.3, .2, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 5
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.75, 3.05, -1);
    glScalef(1.3, .2, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 6
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.75, 3.25, -1);
    glScalef(1.3, .2, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
#pragma endregion

#pragma region BOARD GAME INSIDE (Texture Done)

    //  BOARD GAME 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 0, 0, 1);
    glTranslatef(-1.5, 1.2, 1.3);
    glScalef(.2, .2, .3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[11]->width, images[11]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[11]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 0, 0, 1);
    glTranslatef(-1.5, 1.4, 1.3);
    glScalef(.2, .2, .3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[12]->width, images[12]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[12]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 3
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 0, 0, 1);
    glTranslatef(-1.7, 1.6, 1.3);
    glScalef(.2, .2, .3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[13]->width, images[13]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[13]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 5
    glPushMatrix();
    glTranslatef(moveBoardGame, 1.2, .95); // noted : Animation 1 Target

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    // glTranslatef(0, 1.2, .95);
    glScalef(.2, .2, .3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[14]->width, images[14]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[14]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // tutup game board
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(0, openBoardGame, 0);
    glScalef(.2, .05, .3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[14]->width, images[14]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[14]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();

    //  BOARD GAME 6
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.9, 0, 0, 1);
    glTranslatef(-1.5, 1.4, .95);
    glScalef(.2, .2, .3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[11]->width, images[11]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[11]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 7
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 0, 1);
    glTranslatef(-1.7, 1.3, .2);
    glScalef(1, .3, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[12]->width, images[12]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[12]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 8
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, .5, 0, 1);
    glTranslatef(-1.7, 1.6, .2);
    glScalef(1, .3, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[13]->width, images[13]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[13]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 9
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glTranslatef(-1.7, .8, .9);
    glScalef(1, .3, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[14]->width, images[14]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[14]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //  BOARD GAME 10
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0, 0.0, 1.0, 1.0);
    glTranslatef(-1.7, .8, -.5);
    glScalef(1, .3, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[11]->width, images[11]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[11]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

#pragma endregion

#pragma region RAIL GLASS DOOR UP (Texture Done)

    // UP GLASS DOOR TRAIL 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.2, 2.45, 0);
    glScalef(.02, .02, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // UP GLASS DOOR TRAIL 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.1, 2.45, 0);
    glScalef(.02, .02, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

#pragma endregion

#pragma region RAIL GLASS DOOR DOWN (Texture Done)

    // UP GLASS DOOR TRAIL 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.2, .55, 0);
    glScalef(.02, .02, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // UP GLASS DOOR TRAIL 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.1, .55, 0);
    glScalef(.02, .02, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

#pragma endregion

#pragma region GLASS LEFT (Texture Done)

    glPushMatrix();
    glTranslatef(0, 0, open1); // min 0 -- max -1.4

    // HANDLE 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.5, .5, .5, 1);
    glTranslatef(-1.19, 1.5, 1.4);
    glScalef(.05, .25, .1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[4]->width, images[4]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[4]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // HANDLE 2
    // glPushMatrix();
    // glColor4f(.8, .8, .8, 1);
    // glTranslatef(-1.18, 1.5, 1.4);
    // glScalef(.05, .1, .05);
    // glutSolidCube(1.0);
    // glPopMatrix();

    // LOCK
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.19, 1.48, 0);
    glScalef(.05, .05, .05);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[10]->width, images[10]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[10]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS FRAME UP VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.2, 2.4, .7);
    glScalef(.05, .2, 1.6);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS FRAME DOWN VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.2, .6, .7);
    glScalef(.05, .2, 1.6);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS FRAME LEFT VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.2, 1.5, 1.4);
    glScalef(.05, 1.6, .2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS FRAME RIGHT VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.2, 1.5, 0);
    glScalef(.05, 1.6, .2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS LEFT VIEW
    glPushMatrix();
    glColor4f(.90, .90, .90, .4f); // abu-abu
    glTranslatef(-1.2, 1.5, .7);
    glScalef(.05, 1.6, 1.2);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

#pragma endregion

#pragma region GLASS RIGHT (Texture Done)

    glPushMatrix();
    glTranslatef(0, 0, open2); // min 0 -- max -.7

    // HANDLE 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.5, .5, .5, 1);
    glTranslatef(-1.09, 1.5, -.65);
    glScalef(.05, .25, .1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[4]->width, images[4]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[4]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // HANDLE 2
    // glPushMatrix();
    // glColor4f(.8, .8, .8, 1);
    // glTranslatef(-1.08, 1.5, -.65);
    // glScalef(.05, .1, .05);
    // glutSolidCube(1.0);
    // glPopMatrix();

    // LOCK 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.09, 1.5, .65);
    glScalef(.05, .15, .05);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[3]->width, images[3]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[3]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS FRAME UP VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.1, 2.4, 0);
    glScalef(.05, .2, 1.5);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS FRAME DOWN VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.1, .6, .0);
    glScalef(.05, .2, 1.5);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS FRAME LEFT VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.1, 1.5, .65);
    glScalef(.05, 1.6, .2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS FRAME RIGHT VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.1, 1.5, -.65);
    glScalef(.05, 1.6, .2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS RIGHT VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, .4f);
    glTranslatef(-1.1, 1.5, 0);
    glScalef(.05, 1.6, 1.1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

#pragma endregion

#pragma region RAIL DOWN DOOR UP (Texture Done)
    // UP GLASS DOOR TRAIL 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.2, .45, 0);
    glScalef(.02, .02, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // UP GLASS DOOR TRAIL 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.1, .45, 0);
    glScalef(.02, .02, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
#pragma endregion

#pragma region RAIL DOWN DOOR DOWN (Texture Done)
    // UP DOWN DOOR TRAIL 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.2, -2.3, 0);
    glScalef(.02, .02, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // UP DOWN DOOR TRAIL 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.1, -2.3, 0);
    glScalef(.02, .02, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[15]->width, images[15]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[15]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

#pragma endregion

#pragma region DOWN DOOR LEFT (Texture Done)

    glPushMatrix();
    glTranslatef(0, 0, open3); // min 0 -- max -1.5

    // HANDLE 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.5, .5, .5, 1);
    glTranslatef(-1.19, -1, 1.4);
    glScalef(.05, .25, .1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[4]->width, images[4]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[4]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // HANDLE 2
    // glPushMatrix();
    // glColor4f(.8, .8, .8, 1);
    // glTranslatef(-1.18, -1, 1.4);
    // glScalef(.05, .1, .05);
    // glutSolidCube(1.0);
    // glPopMatrix();

    // LOCK
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.19, -1, -.1);
    glScalef(.05, .05, .05);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[10]->width, images[10]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[10]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // FRONT GLASS LEFT VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.2, -1, .67);
    glScalef(.05, 2.9, 1.7);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();
#pragma endregion

#pragma region DOWN DOOR RIGHT (Texture Done)

    glPushMatrix();
    glTranslatef(0, 0, open4); // min 0 -- max -.7

    // HANDLE 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.5, .5, .5, 1);
    glTranslatef(-1.09, -1, -1.4);
    glScalef(.05, .25, .1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[4]->width, images[4]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[4]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // HANDLE 2
    // glPushMatrix();
    // glColor4f(.8, .8, .8, 1);
    // glTranslatef(-1.08, -1, -1.4);
    // glScalef(.05, .1, .05);
    // glutSolidCube(1.0);
    // glPopMatrix();

    // LOCK 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor4f(.8, .8, .8, 1);
    glTranslatef(-1.09, -1, -.1);
    glScalef(.05, .15, .05);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[3]->width, images[3]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[3]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // LOCK 2
    // glPushMatrix();
    // glColor4f(.3, .3, .3, 1);
    // glTranslatef(-1.08, -1, -.1);
    // glScalef(.05, .05, .05);
    // glutSolidCube(1.0);
    // glPopMatrix();

    // FRONT GLASS RIGHT VIEW
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-1.1, -1, -.75);
    glScalef(.05, 2.9, 1.5);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->width, images[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, images[0]->data);
    glutSolidCube(1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();
#pragma endregion

#pragma endregion
}

#pragma region minimaze all
void animation1()
{
    if (!onAnimation)
    {
        onAnimation = true;
        open1 = -1;
        moveBoardGame = 0;
        openBoardGame = .2;
    }
    else
    {
        openBoardGame = .1;
        moveBoardGame = -1.5;
        open1 = 0;
        onAnimation = false;
    }
}

void animation2()
{
    if (!onAnimation)
    {
        onAnimation = true;
        open1 = -1;
        moveBook = 0;
        openBookRotate = 30;
        openBookRotateX = 1;
        openBookUp = .2;
    }
    else
    {
        openBookUp = 0;
        openBookRotate = 0;
        openBookRotateX = 0;
        moveBook = -1.5;
        open1 = 0;
        onAnimation = false;
    }
}

int init(void)
{
    glClearColor(0, .3, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);

    loadTextures();
    glGenTextures(numTextures, texture);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    return 1;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gluLookAt(
        4, 1, 4,
        0, 0, 0,
        0, 1, 0);

    // drawBackground();
    drawBox();

    glFlush();
    glutSwapBuffers();
}

void resize(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(grow_shrink, resize_f * w / h, resize_f, 100 * resize_f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idle(void)
{
    if (!mouseDown && startIdle)
    {
        xrot += 2.01f;
        yrot += 2.015f;
    }

    glutPostRedisplay();
}

void mySpecialFunction()
{
    printf("U -----------> rotate clockwise\n");
    printf("Y -----------> rotate counter clockwise\n");
    printf("W or w ------> Up\n");
    printf("S or s -----> Down\n");
    printf("D or d ------> Right\n");
    printf("A or a ------> Left\n");
    printf("Z or z ------> Shrink\n");
    printf("X or x ------> Grow\n");
    printf("Escape Key ---> exit the program\n\n");
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(1);
        break;

    case 'w':
    case 'W':
        tra_x += 0.1f;
        break;
    case 's':
    case 'S':
        tra_x -= 0.1f;
        break;
    case 'a':
    case 'A':
        tra_z += 0.1f;
        break;
    case 'd':
    case 'D':
        tra_z -= 0.1f;
        break;
    case 'u':
    case 'U':
        xrot += 1.0f;
        yrot += 1.0f;
        xdiff += 1.0f;
        ydiff += 1.0f;
        break;

    case 'y':
    case 'Y':
        xrot -= 1.0f;
        yrot -= 1.0f;
        xdiff += 1.0f;
        ydiff += 1.0f;
        break;

    case 'Z':
    case 'z':
        grow_shrink--;
        resize(800, 800);

        break;
    case 'X':
    case 'x':
        grow_shrink++;
        resize(800, 800);

        break;
    case '1':
        if (startIdle)
            startIdle = false;
        else
            startIdle = true;
        break;
    case 'o':
        if (open1 < -.1 && !onAnimation)
            open1 += .1f;
        break;
    case 'p':
        if (open1 > -1.4 && !onAnimation)
            open1 -= .1f;
        break;
    case 'k':
        if (open2 < .7 && !onAnimation)
            open2 += .1f;
        break;
    case 'l':
        if (open2 > -.6 && !onAnimation)
            open2 -= .1f;
        break;
    case 'O':
        if (open3 < -.1 && !onAnimation)
            open3 += .1f;
        break;
    case 'P':
        if (open3 > -1.4 && !onAnimation)
            open3 -= .1f;
        break;
    case 'K':
        if (open4 < 1.5 && !onAnimation)
            open4 += .1f;
        break;
    case 'L':
        if (open4 > 0 && !onAnimation)
            open4 -= .1f;
        break;
    case '2':
        animation1();
        break;
    case '3':
        animation2();
        break;
    }

    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y)
{
    if (key == GLUT_KEY_F1)
    {
        fullscreen = !fullscreen;

        if (fullscreen)
            glutFullScreen();
        else
        {
            glutReshapeWindow(windowWidth, windowHeight);
            glutPositionWindow(80, 80);
        }
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseDown = 1;

        xdiff = x - yrot;
        ydiff = -y + xrot;
    }
    else
        mouseDown = 0;
}

void mouseMotion(int x, int y)
{
    if (mouseDown)
    {
        yrot = x - xdiff;
        xrot = y + ydiff;

        glutPostRedisplay();
    }
}

void setupLightingAndMaterial()
{
    GLfloat ambient[] = {0.2, 0.2, 0.2, 1};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, .2};
    GLfloat specular[] = {1.0, 1.0, 1.0, 1};
    GLfloat shininess = 100.0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(windowWidth, windowHeight);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("Lemari Game Board");

    setupLightingAndMaterial();
    glutDisplayFunc(display);         // fungsi untuk mengeluarkan jendela panel
    glutKeyboardFunc(keyboard);       // fungsi untuk menangani input keyboard
    glutSpecialFunc(specialKeyboard); // fungsi untuk menangani input khusus di keyboard (contoh f1, f2)
    glutMouseFunc(mouse);             // fungsi untuk menangani input dari mouse
    glutMotionFunc(mouseMotion);      // fungsi untuk menangani pergerakan mouse
    glutReshapeFunc(resize);          // fungsi unutk menangani perubahan jendela panel
    mySpecialFunction();
    glutIdleFunc(idle); // fungsi untuk idle

    glewExperimental = GL_TRUE;
    glewInit();

    if (!init())
        return 1;

    glutMainLoop();

    return 0;
}
#pragma endregion
