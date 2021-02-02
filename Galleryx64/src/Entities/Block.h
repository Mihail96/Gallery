#ifndef Block_H
#define Block_H

#include "Entity.h"

class Block : public Entity
{
public:
    Block(unsigned int textureId, Shader* shader);

    void Act(double currentFrame);

    void Draw();

private:
    unsigned int TextureId;
    bool action1 = false;
    glm::vec3 color;
};
#endif