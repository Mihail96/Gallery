#ifndef MESH_H
#define MESH_H

#include <vector>

#include "../Renderers/Shader.h"
#include "../Models/Vertex.h"
#include "../Models/Texture.h"

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(Shader& shader);

private:
    unsigned int VBO, EBO;

    void setupMesh();
};
#endif