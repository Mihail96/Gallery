#ifndef World_H
#define World_H

#include "Shader.h"
#include "Entity.h"
#include "Player.h"

class World
{
public:
	static unsigned int WorldSize;
	static float GravityAcceleration;
	static double DeltaTime;
	static double LastFrame;

	Player* Player;
	std::vector<Entity*> Entities;
	Entity** Coordinates;

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

	World();

	World(World const&) = delete;
	void operator=(World const&) = delete;

	void DestoryWorld();
};
#endif