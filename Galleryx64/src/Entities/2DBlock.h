#ifndef TwoDBlock_H
#define TwoDBlock_H

#include "Entity.h"

class TwoDBlock : public Entity
{
public:
    TwoDBlock(std::vector<unsigned int> textureIds, Shader* shader);

    void Act(double currentFrame);

    void Draw();

private:
    std::vector<unsigned int> TextureIds;
    unsigned int textureCounter;
};
#endif