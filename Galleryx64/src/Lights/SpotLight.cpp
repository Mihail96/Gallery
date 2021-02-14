#include "SpotLight.h"

SpotLight::SpotLight(float height, float cutOff, float outerCutOff, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->height = height;
	this->CutOff = cutOff;
	this->OuterCutOff = outerCutOff;
	this->Constant = constant;
	this->Linear = linear;
	this->Quadratic = quadratic;
	this->Ambient = ambient;
	this->Diffuse = diffuse;
	this->Specular = specular;
}
