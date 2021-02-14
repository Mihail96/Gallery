#include <iostream>
#include <sstream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <limits>

#include "Model.h"

#include "../Utils/ImageLoader.h"
#include "../Models/Vertex.h"
#include "../Models/Texture.h"
#include "Player.h"
#include "../Utils/Window.h"
#include "../World.h"

Model::Model(std::string const& path, Shader* shader)
{
    this->MinPosition = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    this->MaxPosition = glm::vec3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    this->shader = shader;
    Collison = false;
    Scale = glm::vec3(1, 1, 1);
    direction = glm::vec3(1.0f, 0.0f, 1.0f);
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    rotate = false;
    spotLight = new SpotLight();
    spotLight->CutOff = glm::cos(glm::radians(12.5f));
    spotLight->OuterCutOff = glm::cos(glm::radians(25.0f));

    World* world = World::GetInstance();

    world->ActiveEntities.push_back(this);

    loadModel(path);
}

void Model::Act(double currentFrame)
{
    CurrentFrame = currentFrame;
    DeltaTime = currentFrame - LastFrame;

    CalculateRainbow();

    if (DeltaTime > MaxActTime)
    {
        LastFrame = currentFrame;
    }
}

void Model::CalculateRainbow()
{
    float changeSpeed = 0.05f;
    if (rainbowEffectState == 0)
    {
        //Increase Red and Blue untill 1 1 1 then 1
        color.x += changeSpeed;
        color.z += changeSpeed;

        if (color.x >= 1.0f || color.z >= 1.0f)
        {
            color.x = 1.0f;
            color.y = 1.0f;
            color.z = 1.0f;
            rainbowEffectState = 1;
        }
    }
    if (rainbowEffectState == 1)
    {
        //Decrease Blue untill 1 1 0 then 2
        color.z -= changeSpeed;

        if (color.z <= 0.0f)
        {
            color.x = 1.0f;
            color.y = 1.0f;
            color.z = 0.0f;
            rainbowEffectState = 2;
        }
    }
    if (rainbowEffectState == 2)
    {
        //Decrease Green untill 1 0 0 then 3
        color.y -= changeSpeed;

        if (color.y <= 0.0f)
        {
            color.x = 1.0f;
            color.y = 0.0f;
            color.z = 0.0f;
            rainbowEffectState = 3;
        }
    }
    if (rainbowEffectState == 3)
    {
        //Increase Blue untill 1 0 1 then 4
        color.z += changeSpeed;

        if (color.z >= 1.0f)
        {
            color.x = 1.0f;
            color.y = 0.0f;
            color.z = 1.0f;
            rainbowEffectState = 4;
        }
    }
    if (rainbowEffectState == 4)
    {
        //Decrease Red untill 0 0 1 then 5
        color.z -= changeSpeed;

        if (color.z <= 0.0f)
        {
            color.x = 0.0f;
            color.y = 0.0f;
            color.z = 1.0f;
            rainbowEffectState = 5;
        }
    }
    if (rainbowEffectState == 5)
    {
        //Increase Green untill 0 1 1 then 6
        color.y += changeSpeed;

        if (color.y >= 1.0f)
        {
            color.x = 0.0f;
            color.y = 1.0f;
            color.z = 1.0f;
            rainbowEffectState = 6;
        }
    }
    if (rainbowEffectState == 6)
    {
        //Decrease Blue untill 0 1 0 then 0
        color.z -= changeSpeed;

        if (color.z <= 0.0f)
        {
            color.x = 0.0f;
            color.y = 1.0f;
            color.z = 0.0f;
            rainbowEffectState = 0;
        }
    }

    spotLight->Diffuse = color;
    spotLight->Specular = color;
}

void Model::Draw()
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
    model = glm::scale(model, glm::vec3(Scale));

    if (rotate)
    {
        float angle = CurrentFrame * 20;
        model = glm::rotate(model, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
        direction.x = static_cast<float>(cos(glm::radians(static_cast<double>(angle + 90.0f))));
        direction.z = static_cast<float>(sin(glm::radians(static_cast<double>(angle + 90.0f))));
    }

    shader->setMat4("model", model);

    shader->setVec3("viewPos", player->Position);

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

    glm::vec3 spotLightPosition = glm::vec3(world->spotLightBlock->Position.x,
                                            world->spotLightBlock->Position.y + world->spotLightBlock->spotLight->height,
                                            world->spotLightBlock->Position.z);
    shader->setVec3("spotLight[1].position", spotLightPosition);
    shader->setVec3("spotLight[1].direction", world->spotLightBlock->direction);
    shader->setVec3("spotLight[1].ambient", world->spotLightBlock->spotLight->Ambient);
    shader->setVec3("spotLight[1].diffuse", world->spotLightBlock->spotLight->Diffuse);
    shader->setVec3("spotLight[1].specular", world->spotLightBlock->spotLight->Specular);
    shader->setFloat("spotLight[1].constant", world->spotLightBlock->spotLight->Constant);
    shader->setFloat("spotLight[1].linear", world->spotLightBlock->spotLight->Linear);
    shader->setFloat("spotLight[1].quadratic", world->spotLightBlock->spotLight->Quadratic);
    shader->setFloat("spotLight[1].cutOff", world->spotLightBlock->spotLight->CutOff);
    shader->setFloat("spotLight[1].outerCutOff", world->spotLightBlock->spotLight->OuterCutOff);

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

    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(*shader);
}

void Model::loadModel(std::string const& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;

        MinPosition.x = std::fminf(MinPosition.x, vector.x);
        MinPosition.y = std::fminf(MinPosition.y, vector.y);
        MinPosition.z = std::fminf(MinPosition.z, vector.z);

        MaxPosition.x = std::fmaxf(MaxPosition.x, vector.x);
        MaxPosition.y = std::fmaxf(MaxPosition.y, vector.y);
        MaxPosition.z = std::fmaxf(MaxPosition.z, vector.z);

        vertex.Position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = 1-mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> maps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_HEIGHT);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_AMBIENT);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_EMISSIVE);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_NORMALS);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_SHININESS);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_OPACITY);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_DISPLACEMENT);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_LIGHTMAP);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_REFLECTION);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_BASE_COLOR);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_NORMAL_CAMERA);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_EMISSION_COLOR);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_METALNESS);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS);
    textures.insert(textures.end(), maps.begin(), maps.end());

    maps = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION);
    textures.insert(textures.end(), maps.begin(), maps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = type;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}