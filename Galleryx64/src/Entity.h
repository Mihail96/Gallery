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
		return (this->MinPosition.x <= entity->MaxPosition.x && this->MaxPosition.x >= entity->MinPosition.x) &&
			   (this->MinPosition.y <= entity->MaxPosition.y && this->MaxPosition.y >= entity->MinPosition.y) &&
			   (this->MinPosition.z <= entity->MaxPosition.z && this->MaxPosition.z >= entity->MinPosition.z);
	}
};
#endif