#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#include "World.h"

#include "Entities/Entity.h"
#include "Entities/Block.h"

double World::DeltaTime = 0.0f;
double World::LastFrame = 0.0f;
unsigned int World::WorldSize = 0;
World* World::Instance = nullptr;
float World::GravityAcceleration = 0.0981f;

World* World::GetInstance()
{
	if (!Instance)
	{
		Instance = new World();
	}
	return Instance;
}

World::World()
{
	this->Coordinates = nullptr;
	this->directionalLight = new DirectionalLight();
	this->Player = Player::GetInstance();
}

World::~World()
{
	DestoryWorld();
}

void World::SetCamera(glm::vec3* position, glm::vec3* up, float yaw, float pitch)
{
	this->Player->MoveCamera(position, up, yaw, pitch);
}

void World::Draw()
{
	for (unsigned int i = 0; i < Entities.size(); i++)
	{
		Entities[i]->Draw();
	}
}

void World::InsertEntity(Entity* entity, float x, float y, float z)
{
	entity->Position = glm::vec3(x, y, z);
	this->Entities.push_back(entity);

	int CoordX = floor(x);
	int CoordY = floor(y);
	int CoordZ = floor(z);
	this->Coordinates[CoordX + WorldSize * (CoordY + WorldSize * CoordZ)] = entity;
}

void World::ProcessTime()
{
	double currentFrame = glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;

	int coordX = floor(Player->Position.x);
	int coordY = floor(Player->Position.y);
	int coordZ = floor(Player->Position.z);

	int minCoordY = floor(Player->Position.y + Player->MinPosition.y);

	int maxCoordY = floor(Player->Position.y + Player->MaxPosition.y);

	if (coordX <= 0 || minCoordY <= 0 || coordZ <= 0)
	{
		return;
	}

	Entity* topEntity = Coordinates[coordX + WorldSize * (maxCoordY + WorldSize * coordZ)]; // Y Max
	Entity* bottomEntity = Coordinates[coordX + WorldSize * (minCoordY + WorldSize * coordZ)]; // Y Min

	if (bottomEntity && bottomEntity->Intersects(Player) || topEntity && topEntity->Intersects(Player))
	{
		if (bottomEntity && bottomEntity->Position.y + bottomEntity->MaxPosition.y >= Player->Position.y + Player->MinPosition.y)
		{
			Player->Position.y = bottomEntity->Position.y + bottomEntity->MaxPosition.y + std::fabs(Player->MinPosition.y) - 0.01f;
		}

		if (topEntity && topEntity->Position.y + topEntity->MinPosition.y <= Player->Position.y + Player->MaxPosition.y)
		{
			Player->Position.y = topEntity->Position.y + topEntity->MinPosition.y - std::fabs(Player->MaxPosition.y);
		}

		if (topEntity)
		{
			Player->GravityVelocity = GravityAcceleration;
			Player->ProcessGravity(DeltaTime);
		}

		if (bottomEntity)
		{
			Player->GravityVelocity = 0;
		}
	}
	else
	{
		Player->GravityVelocity += GravityAcceleration;
		Player->ProcessGravity(DeltaTime);
		//std::cout << "No Bottom Entity at : " << coordX << ", " << minCoordY << ", " << coordZ << " | ";
	}
}

void World::InitEntities(unsigned int worldSize)
{
	this->WorldSize = worldSize;
	Coordinates = new Entity * [worldSize * worldSize * worldSize];

	for (int x = 0; x < WorldSize; x++)
	{
		for (int y = 0; y < WorldSize; y++)
		{
			for (int z = 0; z < WorldSize; z++)
			{
				this->Coordinates[x + WorldSize * (y + WorldSize * z)] = nullptr;
			}
		}
	}
}

unsigned int World::LoadTexture(const char* fileName)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	return texture;
}

void World::DestoryWorld()
{
	for (int i = 0; i < Entities.size(); i++)
	{
		delete this->Entities[i];
	}
	delete[] this->Coordinates;
}