#ifndef Entity_H
#define Entity_H

#include <vector>

#include "Shader.h"

class Entity
{
public:
	glm::vec3 Position;
	glm::vec3 MaxPosition;
	glm::vec3 MinPosition;
	virtual void Draw(Shader& shader) = 0;

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