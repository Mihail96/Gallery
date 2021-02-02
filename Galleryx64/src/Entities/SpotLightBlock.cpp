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

SpotLightBlock::SpotLightBlock(unsigned int textureId, Shader* shader)
{
    this->TextureId = textureId;
    this->MinPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    this->MaxPosition = glm::vec3(1.0f, 1.0f, 1.0f);
    this->shader = shader;
    MaxActTime = 2;
    spotLight = new SpotLight();
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    direction = glm::vec3(1.0f, 0.0f, 0.0f);
}

void SpotLightBlock::Act(double currentFrame)
{
    CurrentFrame = currentFrame;
    DeltaTime = currentFrame - LastFrame;

    if (DeltaTime > MaxActTime)
    {
        LastFrame = currentFrame;

        color = glm::vec3((float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX);
        spotLight->Diffuse = color;
        spotLight->Specular = color;
        if (spot)
        {
            direction = glm::vec3(-0.338415712, 0.645457625, -0.684732974);
        }
        else
        {
            direction = glm::vec3(-0.734798312, 0.675590098, 0.0604107864);
        }
        spot = !spot;
    }
}

void SpotLightBlock::Draw()
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

    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    shader->setVec3("Color", color);
    shader->setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, TextureId);

    glBindVertexArray(BlockRenderer::GetInstance().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}