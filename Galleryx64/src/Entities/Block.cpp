#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <sstream>  

#include "Block.h"

#include "../Renderers/Shader.h"
#include "../Models/Texture.h"
#include "../Models/Vertex.h"
#include "Entity.h"
#include "../Renderers/BlockRenderer.h"
#include "Player.h"
#include "../Utils/Window.h"
#include "../World.h"

Block::Block(unsigned int textureId, Shader* shader)
{
    this->TextureId = textureId;
    this->MinPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    this->MaxPosition = glm::vec3(1.0f, 1.0f, 1.0f);
    this->shader = shader;
}

void Block::Draw()
{
    Player* player = Player::GetInstance();
    World* world = World::GetInstance();

    shader->use();
    glm::mat4 projection = glm::perspective(player->Zoom, (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = player->GetViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    if (this->pointLight)
    {
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        shader->setVec3("Color", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->setMat4("model", model);
    }
    else
    {
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
            shader->setVec3("spotLight.position", player->Position);
            shader->setVec3("spotLight.direction", player->Front);
            shader->setVec3("spotLight.ambient", player->spotLight->Ambient);
            shader->setVec3("spotLight.diffuse", player->spotLight->Diffuse);
            shader->setVec3("spotLight.specular", player->spotLight->Specular);
            shader->setFloat("spotLight.constant", player->spotLight->Constant);
            shader->setFloat("spotLight.linear", player->spotLight->Linear);
            shader->setFloat("spotLight.quadratic", player->spotLight->Quadratic);
            shader->setFloat("spotLight.cutOff", player->spotLight->CutOff);
            shader->setFloat("spotLight.outerCutOff", player->spotLight->OuterCutOff);
        }

        if (!this->LightsCalculated)
        {
            this->LightsCalculated = true;
            int xMin = std::fminf(std::fmaxf(Position.x - 10, 0), 49);
            int xMax = std::fminf(std::fmaxf(Position.x + 10, 0), 49);
            int yMin = std::fminf(std::fmaxf(Position.y - 10, 0), 49);
            int yMax = std::fminf(std::fmaxf(Position.y + 10, 0), 49);
            int zMin = std::fminf(std::fmaxf(Position.z - 10, 0), 49);
            int zMax = std::fminf(std::fmaxf(Position.z + 10, 0), 49);
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
            shader->setVec3("pointLights[" + s + "].position",   this->illuminatedByEntities[i]->Position);
            shader->setVec3("pointLights[" + s + "].ambient",    this->illuminatedByEntities[i]->pointLight->Ambient);
            shader->setVec3("pointLights[" + s + "].diffuse",    this->illuminatedByEntities[i]->pointLight->Diffuse);
            shader->setVec3("pointLights[" + s + "].specular",   this->illuminatedByEntities[i]->pointLight->Specular);
            shader->setFloat("pointLights[" + s + "].constant",  this->illuminatedByEntities[i]->pointLight->Constant);
            shader->setFloat("pointLights[" + s + "].linear",    this->illuminatedByEntities[i]->pointLight->Linear);
            shader->setFloat("pointLights[" + s + "].quadratic", this->illuminatedByEntities[i]->pointLight->Quadratic);
        }
    }

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, TextureId);
    glUniform1i(glGetUniformLocation(shader->ID, "texture_diffuse1"), 0);
    glBindTexture(GL_TEXTURE_2D, TextureId);


    glBindVertexArray(BlockRenderer::GetInstance().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}