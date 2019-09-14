#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
// #include "../source/SOIL.h"

using namespace std;

class Texture{

  public:
    GLuint texture;
    int width,height;
    int image_type;
    const char* image_fname;

  public:
    void GenerateTexture();
    void ApplyTexture();
};

#endif
