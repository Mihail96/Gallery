#ifndef Entity_H
#define Entity_H

#include <vector>

#include "../Renderers/Shader.h"
#include "../Lights/PointLight.h"
#include "../Lights/SpotLight.h"

class Entity
{
public:
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 MaxPosition;
	glm::vec3 MinPosition;
	glm::vec3 direction;
	
	Shader* shader;
	PointLight* pointLight;
	SpotLight* spotLight;

	std::vector<Entity*>  illuminatedByEntities;
	bool LightsCalculated = false;
	bool Collison = true;
	bool actionBlock1 = false;
	bool lightaction1 = false;

	double DeltaTime = 0;
	double LastFrame = 0;
	double MaxActTime = 10;
	double CurrentFrame = 0;

	virtual void Draw() = 0;

	virtual void Act(double currentFrame) = 0;

	bool Intersects(Entity* entity)
	{
		glm::vec3 calculatedThisMinPosition = this->MinPosition + this->Position;
		glm::vec3 calculatedThisMaxPosition = this->MaxPosition + this->Position;
		glm::vec3 calculatedEntityMinPosition = entity->MinPosition + entity->Position;
		glm::vec3 calculatedEntityMaxPosition = entity->MaxPosition + entity->Position;

		return (calculatedThisMinPosition.x <= calculatedEntityMaxPosition.x && calculatedThisMaxPosition.x  >= calculatedEntityMinPosition.x) &&
			   (calculatedThisMinPosition.y <= calculatedEntityMaxPosition.y && calculatedThisMaxPosition.y  >= calculatedEntityMinPosition.y) &&
			   (calculatedThisMinPosition.z <= calculatedEntityMaxPosition.z && calculatedThisMaxPosition.z  >= calculatedEntityMinPosition.z);
	}
};
#endif