#include <glad/glad.h>

#include "BlockRenderer.h"

unsigned int BlockRenderer::VBO = 0;
unsigned int BlockRenderer::VAO = 0;

BlockRenderer::BlockRenderer()
{
    CreateBlock();
}

void BlockRenderer::CreateBlock()
{
    float vertices[] = {
    //Coordinates          Texture       Normal vectors
     0.0f,  0.0f,  0.0f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     1.0f,  1.0f,  0.0f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     1.0f,  1.0f,  0.0f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     0.0f,  1.0f,  0.0f,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     0.0f,  0.0f,  0.0f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
                                         
     0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,
     1.0f,  0.0f,  1.0f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
     0.0f,  1.0f,  1.0f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f,
     0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,
                                         
     0.0f,  1.0f,  1.0f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
     0.0f,  1.0f,  0.0f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
     0.0f,  0.0f,  0.0f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
     0.0f,  0.0f,  0.0f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
     0.0f,  0.0f,  1.0f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
     0.0f,  1.0f,  1.0f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
                                        
     1.0f,  1.0f,  1.0f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     1.0f,  1.0f,  0.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  1.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     1.0f,  1.0f,  1.0f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
                                        
     0.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
     1.0f,  0.0f,  0.0f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
     1.0f,  0.0f,  1.0f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     1.0f,  0.0f,  1.0f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     0.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
                                         
     0.0f,  1.0f,  0.0f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  0.0f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  1.0f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  1.0f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  1.0f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //TextureCoordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}