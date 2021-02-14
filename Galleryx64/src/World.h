#ifndef World_H
#define World_H

#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Lights/DirectionalLight.h"

class World
{
public:
	static unsigned int WorldSize;
	static float GravityAcceleration;
	static double DeltaTime;
	static double LastFrame;

	Player* Player;
	Entity* spotLightBlock;
	std::vector<Entity*> Entities;
	Entity** Coordinates;
	std::vector<Entity*> ActiveEntities;
	DirectionalLight* directionalLight;

	static World* GetInstance();

	void SetCamera(glm::vec3* position, glm::vec3* up, float yaw, float pitch);

	void Draw();

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