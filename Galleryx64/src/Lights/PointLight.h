#ifndef PointLight_H
#define PointLight_H

#include "Light.h"

class PointLight : public Light
{
public:
    float Constant;
    float Linear;
    float Quadratic;

    PointLight(float constant =     1.0f,
               float linear =       0.09f,
               float quadratic =    0.032f,
               glm::vec3 ambient =  glm::vec3(0.05f, 0.05f, 0.05f),
               glm::vec3 diffuse =  glm::vec3(0.8f,  0.8f,  0.8f),
               glm::vec3 specular = glm::vec3(0.9f,  0.9f,  0.9f));
};

#endif
