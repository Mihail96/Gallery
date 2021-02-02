#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <sstream> 
#include "2DBlock.h"
#include "../Renderers/2DBlockRenderer.h"
#include <glm/matrix.hpp>
#include "Player.h"
#include "../World.h"
#include "../Utils/Window.h"
#include <glm/gtc/matrix_transform.hpp>

TwoDBlock::TwoDBlock(std::vector<unsigned int> textureIds, Shader* shader)
{
	this->TextureIds = textureIds;
	this->MinPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->MaxPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->shader = shader;
	this->Collison = false;
    textureCounter = 3;
    World* world = World::GetInstance();

    world->ActiveEntities.push_back(this);
}

void TwoDBlock::Act(double currentFrame)
{
    DeltaTime = currentFrame - LastFrame;
    if (DeltaTime > MaxActTime)
    {
        LastFrame = currentFrame;

        textureCounter++;
        if (textureCounter > 5)
        {
            textureCounter = 3;
        }
    }
}

void TwoDBlock::Draw()
{
	Player* player = Player::GetInstance();
	World* world = World::GetInstance();

    unsigned int TextureId = TextureIds[textureCounter];
    glBindTexture(GL_TEXTURE_2D, TextureId);

	shader->use();
	glm::mat4 projection = glm::perspective(player->Zoom, (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = player->GetViewMatrix();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);

	shader->setMat4("model", model);

    shader->setVec3("viewPos", player->Position);

    shader->setInt("material.diffuse", 0);
    shader->setInt("material.specular", 1);
    shader->setFloat("material.shininess", 32.0f);

    shader->setVec3("dirLight.direction", world->directionalLight->Direction);
    shader->setVec3("dirLight.ambient", world->directionalLight->Ambient);
    shader->setVec3("dirLight.diffuse", world->directionalLight->Diffuse);
    shader->setVec3("dirLight.specular", world->directionalLight->Specular);

    shader->setInt("toggleSpotlight", player->toggleSpotlight);
    if (player->toggleSpotlight)
    {
        shader->setVec3("spotLight[0].position", player->Position);
        shader->setVec3("spotLight[0].direction", player->Front);
        shader->setVec3("spotLight[0].ambient", player->spotLight->Ambient);
        shader->setVec3("spotLight[0].diffuse", player->spotLight->Diffuse);
        shader->setVec3("spotLight[0].specular", player->spotLight->Specular);
        shader->setFloat("spotLight[0].constant", player->spotLight->Constant);
        shader->setFloat("spotLight[0].linear", player->spotLight->Linear);
        shader->setFloat("spotLight[0].quadratic", player->spotLight->Quadratic);
        shader->setFloat("spotLight[0].cutOff", player->spotLight->CutOff);
        shader->setFloat("spotLight[0].outerCutOff", player->spotLight->OuterCutOff);
    }

    if (!this->LightsCalculated)
    {
        this->LightsCalculated = true;
        int xMin = std::fminf(std::fmaxf(Position.x - 8, 0), 49);
        int xMax = std::fminf(std::fmaxf(Position.x + 8, 0), 49);
        int yMin = std::fminf(std::fmaxf(Position.y - 8, 0), 49);
        int yMax = std::fminf(std::fmaxf(Position.y + 8, 0), 49);
        int zMin = std::fminf(std::fmaxf(Position.z - 8, 0), 49);
        int zMax = std::fminf(std::fmaxf(Position.z + 8, 0), 49);
        int i = 0;
        Entity* entity = nullptr;
        for (int x = xMin; x < xMax; x++)
        {
            for (int y = yMin; y < yMax; y++)
            {
                for (int z = zMin; z < zMax; z++)
                {
                    entity = world->Coordinates[x + world->WorldSize * (y + world->WorldSize * z)];
                    if (entity && entity->pointLight)
                    {
                        this->illuminatedByEntities.push_back(entity);
                        i++;
                    }
                    entity = nullptr;
                    if (i == 10)
                    {
                        continue;
                    }
                }
            }
        }
    }

    shader->setInt("numberOfLights", this->illuminatedByEntities.size());
    for (int i = 0; i < this->illuminatedByEntities.size(); i++)
    {
        std::stringstream ss;
        ss << i;
        std::string s;
        ss >> s;
        shader->setVec3("pointLights[" + s + "].position", this->illuminatedByEntities[i]->Position);
        shader->setVec3("pointLights[" + s + "].ambient", this->illuminatedByEntities[i]->pointLight->Ambient);
        shader->setVec3("pointLights[" + s + "].diffuse", this->illuminatedByEntities[i]->pointLight->Diffuse);
        shader->setVec3("pointLights[" + s + "].specular", this->illuminatedByEntities[i]->pointLight->Specular);
        shader->setFloat("pointLights[" + s + "].constant", this->illuminatedByEntities[i]->pointLight->Constant);
        shader->setFloat("pointLights[" + s + "].linear", this->illuminatedByEntities[i]->pointLight->Linear);
        shader->setFloat("pointLights[" + s + "].quadratic", this->illuminatedByEntities[i]->pointLight->Quadratic);
    }

	glBindVertexArray(TwoDBlockRenderer::GetInstance().GetVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}
