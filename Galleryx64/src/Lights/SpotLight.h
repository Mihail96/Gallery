#ifndef SpotLight_H
#define SpotLight_H

#include "Light.h"
#include <glm/detail/func_trigonometric.hpp>

class SpotLight : public Light
{
public:
    float CutOff;
    float OuterCutOff;
    float Constant;
    float Linear;
    float Quadratic;

    SpotLight(float cutOff =       glm::cos(glm::radians(12.5f)),
              float outerCutOff =  glm::cos(glm::radians(15.0f)),
              float constant =     1.0f,
              float linear =       0.06f,
              float quadratic =    0.032f,
              glm::vec3 ambient =  glm::vec3(0.05f, 0.05f, 0.05f),
              glm::vec3 diffuse =  glm::vec3(0.4f, 0.4f, 0.4f),
              glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f));
};

#endif
