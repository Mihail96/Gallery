#ifndef SpotLightBlock_H
#define SpotLightBlock_H

#include "Entity.h"

class SpotLightBlock : public Entity
{
public:
    SpotLightBlock(unsigned int textureId, Shader* shader);

    void Act(double currentFrame);

    void Draw();

private:
    unsigned int TextureId;
    glm::vec3 color;
    bool spot = false;
};
#endif