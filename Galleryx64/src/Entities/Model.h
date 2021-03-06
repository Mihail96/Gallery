#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Entity.h"
#include "../Renderers/Shader.h"

class Model : public Entity
{
public:
    std::vector<Texture*> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    glm::vec3 color;
    bool rotate;
    int rainbowEffectState = 0;

    Model(std::string const& path, Shader* shader);

    void Setup();

    void Act(double currentFrame);

    void Draw();

private:
    void loadModel(std::string const& path);

    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type);

    void CalculateRainbow();
};
#endif