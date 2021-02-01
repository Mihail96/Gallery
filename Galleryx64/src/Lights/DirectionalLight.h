#ifndef DirectionalLight_H
#define DirectionalLight_H

#include <glm/matrix.hpp>
#include "Light.h"

class DirectionalLight : public Light
{
public: 
	glm::vec3 Direction;

	DirectionalLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f,  0.0f), 
				     glm::vec3 ambient =   glm::vec3(0.00f, 0.00f, 0.00f),
					 glm::vec3 diffuse =   glm::vec3(0.0f,  0.0f,  0.0f),
					 glm::vec3 specular =  glm::vec3(0.0f,  0.0f,  0.0f));
};

#endif
