#ifndef World_H
#define World_H

#include "Shader.h"
#include "Camera.h"
#include "Entity.h"
#include "Player.h"

class World
{
public:
	Player* Player;
	static double DeltaTime;
	static double LastFrame;

	static World* GetInstance();

	void SetCamera(glm::vec3* position, glm::vec3* up, float yaw, float pitch);

	void Draw(Shader& shader);

	void InsertEntity(Entity* entity, float x, float y, float z);

	void ProcessTime();

	void InitEntities(unsigned int worldSize);

	unsigned int LoadTexture(const char* fileName);

	~World();

private:
	static World* Instance;
	static unsigned int WorldSize;

	Entity** Coordinates;
	std::vector<Entity*> Entities;

	World();

	World(World const&) = delete;
	void operator=(World const&) = delete;

	void DestoryWorld();
};
#endif