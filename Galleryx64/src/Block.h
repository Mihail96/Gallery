#ifndef Block_H
#define Block_H

#include "Entity.h"

class Block : public Entity
{
public:
    Block(unsigned int textureId);

    void Draw(Shader& shader);

private:
    unsigned int TextureId;
};
#endif