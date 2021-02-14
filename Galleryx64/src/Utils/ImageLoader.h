#ifndef ImageLoader_H
#define ImageLoader_H

#include <string>

unsigned int TextureFromFile(const char* path, const std::string& directory);

TextureData* TextureFromFileAsync(const char* path, const std::string& directory);

unsigned int SetupTexture(TextureData* textureData);

#endif