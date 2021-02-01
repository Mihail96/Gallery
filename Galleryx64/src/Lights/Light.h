#ifndef Light_H
#define Light_H

#include <glm/matrix.hpp>

class Light
{
public:
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
};

#endif