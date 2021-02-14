#ifndef Texture_H
#define Texture_H

#include <string>
#include <assimp/material.h>
#include "TextureData.h"

struct Texture
{
    unsigned int id;
    aiTextureType type;
    std::string path;
    TextureData* textureData;
};

#endif