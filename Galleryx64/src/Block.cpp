#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Block.h"

#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"
#include "Entity.h"
#include "BlockRenderer.h"

Block::Block(unsigned int textureId)
{
    this->TextureId = textureId;
    this->MinPosition = glm::vec3(-0.5f, -0.5f, -0.5f);
    this->MaxPosition = glm::vec3(0.5f, 0.5f, 0.5f);
}

void Block::Draw(Shader& shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, TextureId);
    glUniform1i(glGetUniformLocation(shader.ID, "texture_diffuse1"), 0);
    glBindTexture(GL_TEXTURE_2D, TextureId);


    glBindVertexArray(BlockRenderer::GetInstance().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}