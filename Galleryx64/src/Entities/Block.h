#ifndef Block_H
#define Block_H

#include "Entity.h"

class Block : public Entity
{
public:
    Block(unsigned int textureId, Shader* shader);

    void Draw();

private:
    unsigned int TextureId;
};
#endif