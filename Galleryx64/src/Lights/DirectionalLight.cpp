#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->Direction = direction;
	this->Ambient = ambient;
	this->Diffuse = diffuse;
	this->Specular = specular;
}
