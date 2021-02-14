#ifndef Texture_H
#define Texture_H

#include <string>
#include <assimp/material.h>

struct Texture
{
    unsigned int id;
    aiTextureType type;
    std::string path;
};

#endif