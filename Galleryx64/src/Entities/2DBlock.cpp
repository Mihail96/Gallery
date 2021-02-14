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

	shader->use();
	glm::mat4 projection = glm::perspective(player->Zoom, (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = player->GetViewMatrix();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
	shader->setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureIds[textureCounter]);

	glBindVertexArray(TwoDBlockRenderer::GetInstance().GetVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}
