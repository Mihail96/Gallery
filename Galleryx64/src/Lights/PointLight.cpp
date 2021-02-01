#include "PointLight.h"

PointLight::PointLight(float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->Constant = constant;
	this->Linear = linear;
	this->Quadratic = quadratic;
	this->Ambient = ambient;
	this->Diffuse = diffuse;
	this->Specular = specular;
}
