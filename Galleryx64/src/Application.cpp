#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Renderers/Shader.h"
#include "World.h"
#include "Entities/Model.h"
#include "Entities/Block.h"
#include "Utils/Window.h"
#include "Entities/2DBlock.h"
#include <thread>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void LoadFlorian(World* world, Shader* shader);
void LoadPearl(World* world, Shader* shader);
void LoadSpotLight(World* world, Shader* shader);
void LoadHorse(World* world, Shader* shader);
void LoadGirls(World* world, Shader* shader);
void LoadGaul(World* world, Shader* shader);
void LoadUtilityBox(World* world, Shader* shader);

const unsigned int WorldSize = 50;

int main()
{
    glfwInit();

    if (Window::GetInstance().Result == -1)
    {
        return -1;
    }

    glfwMakeContextCurrent(Window::window);
    glfwSetFramebufferSizeCallback(Window::window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    std::vector<unsigned int> loadedTextures;

    Shader blockShader("res/shaders/BlockVertex.shader", "res/shaders/BlockFragment.shader");
    Shader lightShader("res/shaders/LightVertex.shader", "res/shaders/LightFragment.shader");
    Shader twoDShader("res/shaders/2DBlockVertex.shader", "res/shaders/2DBlockFragment.shader");

    glm::vec3 cameraPosition = glm::vec3(30.0f, 3.0f, 30.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    World* world = World::GetInstance();
    world->InitEntities(WorldSize);
    world->SetCamera(&cameraPosition, &cameraUp, 90.0f, -45.0f);

    const char* name = "res/textures/marble.jpg";
    loadedTextures.push_back(world->LoadTexture(name));//0
    name = "res/textures/floor.jpg";
    loadedTextures.push_back(world->LoadTexture(name));//1
    name = "res/textures/plaster.jpg";
    loadedTextures.push_back(world->LoadTexture(name));//2
    name = "res/textures/fabric1.jpg";
    loadedTextures.push_back(world->LoadTexture(name));//3
    name = "res/textures/fabric2.jpg";
    loadedTextures.push_back(world->LoadTexture(name));//4
    name = "res/textures/fabric3.jpg";
    loadedTextures.push_back(world->LoadTexture(name));//5

    std::thread threadFlorian(LoadFlorian, world, &blockShader);
    std::thread threadPearl(LoadPearl, world, &blockShader);
    std::thread threadSpotLight(LoadSpotLight, world, &blockShader);

    std::thread threadHorse(LoadHorse, world, &blockShader);
    std::thread threadGirls(LoadGirls, world, &blockShader);
    std::thread threadGaul(LoadGaul, world, &blockShader);
    std::thread threadUtilityBox(LoadUtilityBox, world, &blockShader);

    Entity* light2 = new Block(loadedTextures[0], &lightShader);
    light2->pointLight = new PointLight();
    light2->Collison = false;
    world->InsertEntity(light2, 27.5f, 3.5f, 27.5f);

    Entity* light3 = new Block(loadedTextures[0], &lightShader);
    light3->pointLight = new PointLight();
    light3->Collison = false;
    world->InsertEntity(light3, 27.5f, 3.5f, 32.5f);

    Entity* light4 = new Block(loadedTextures[0], &lightShader);
    light4->pointLight = new PointLight();
    light4->Collison = false;
    world->InsertEntity(light4, 32.5f, 3.5f, 27.5f);

    Entity* light5 = new Block(loadedTextures[0], &lightShader);
    light5->pointLight = new PointLight();
    light5->Collison = false;
    world->InsertEntity(light5, 32.5f, 3.5f, 32.5f);

    Entity* twoDBlock = new TwoDBlock(loadedTextures, &twoDShader);
    twoDBlock->pointLight = new PointLight();
    twoDBlock->Collison = false;
    world->InsertEntity(twoDBlock, 32.5f, 3.0f, 26.0001f);

    //Picture Stand
    Entity* block = new Block(loadedTextures[2], &blockShader);
    world->InsertEntity(block, 32, 2, 25);
    block = new Block(loadedTextures[2], &blockShader);
    world->InsertEntity(block, 32, 3, 25);

    //ARCH
    //1
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 2, 32);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 3, 32);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 4, 32);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 5, 32);
    //2
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 2, 27);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 3, 27);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 4, 27);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 5, 27);
    //arch
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 5, 28);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 5, 29);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 5, 30);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 34, 5, 31);

    // Rotating Column With light
    block = new Block(loadedTextures[1], &blockShader);
    world->InsertEntity(block, 22.0f, 3.0f, 33.0f);
    world->ActiveEntities.push_back(block);
    block->actionBlock1 = true;
    block = new Block(loadedTextures[1], &blockShader);
    world->InsertEntity(block, 22.0f, 4.0f, 33.0f);
    world->ActiveEntities.push_back(block);
    block->actionBlock1 = true;
    block = new Block(loadedTextures[1], &blockShader);
    world->InsertEntity(block, 22.0f, 5.0f, 33.0f);
    world->ActiveEntities.push_back(block);
    block->actionBlock1 = true;
    block = new Block(loadedTextures[1], &blockShader);
    world->InsertEntity(block, 22.0f, 6.0f, 33.0f);
    world->ActiveEntities.push_back(block);
    block->actionBlock1 = true;

    Entity* light6 = new Block(loadedTextures[0], &lightShader);
    light6->pointLight = new PointLight();
    light6->Collison = false;
    light6->lightaction1 = true;
    world->ActiveEntities.push_back(light6);
    world->InsertEntity(light6, 22.0f, 3.5f, 31.0f);

    // Pulsating light
    Entity* pulsingLight = new Block(loadedTextures[0], &lightShader);
    pulsingLight->pointLight = new PointLight();
    pulsingLight->Collison = false;
    pulsingLight->lightactionPulse = true;
    world->ActiveEntities.push_back(pulsingLight);
    world->InsertEntity(pulsingLight, 15.5f, 3.5f, 34.5f);

    //Spot Light Cover
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 16.0f, 3.0f, 25.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 16.0f, 4.0f, 25.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 16.0f, 5.0f, 25.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 16.0f, 6.0f, 25.0f);

    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 17.0f, 3.0f, 25.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 17.0f, 4.0f, 25.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 17.0f, 5.0f, 25.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 17.0f, 6.0f, 25.0f);

    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 15.0f, 3.0f, 27.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 15.0f, 4.0f, 27.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 15.0f, 5.0f, 27.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 15.0f, 6.0f, 27.0f);

    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 15.0f, 3.0f, 28.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 15.0f, 4.0f, 28.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 15.0f, 5.0f, 28.0f);
    block = new Block(loadedTextures[0], &blockShader);
    world->InsertEntity(block, 15.0f, 6.0f, 28.0f);


    for (int x = 25; x < 35; x++)
    {
        for (int y = 25; y < 35; y++)
        {
            int z = 1;
            block = new Block(loadedTextures[1], &blockShader);
            world->InsertEntity(block, x, z, y);
        }
    }

    for (int x = 15; x < 25; x++)
    {
        for (int y = 25; y < 35; y++)
        {
            int z = 2;
            Entity* block = new Block(loadedTextures[0], &blockShader);
            world->InsertEntity(block, x, z, y);
        }
    }

    while (!glfwWindowShouldClose(Window::window))
    {
        world->ProcessTime();

        world->Player->processInput();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world->SetupModels();

        world->Draw();

        glfwSwapBuffers(Window::window);
        glfwPollEvents();
    }

    threadFlorian.detach();
    threadPearl.detach();
    threadSpotLight.detach();
    threadHorse.detach();
    threadGirls.detach();
    threadGaul.detach();
    threadUtilityBox.detach();

    glfwTerminate();
    return 0;
}

void LoadFlorian(World* world, Shader* shader)
{
    std::string modelPath = std::string("res/models/florian/model.obj");
    Model* florianModel = new Model(modelPath, shader);
    florianModel->rotate = true;
    florianModel->Scale = glm::vec3(0.1f, 0.1f, 0.1f);
    florianModel->Position = glm::vec3(30.5f, 2.0f, 33.5f);

    world->modelsToLoad.push(florianModel);
}

void LoadPearl(World* world, Shader* shader)
{
    std::string modelPath = std::string("res/models/pearl/model.obj");
    Model* pearlModel = new Model(modelPath, shader);
    pearlModel->Position = glm::vec3(28.0f, 2.0f, 33.5f);
    pearlModel->rotate = true;

    world->modelsToLoad.push(pearlModel);
}

void LoadSpotLight(World* world, Shader* shader)
{
    std::string modelPath = std::string("res/models/spotLight/model.obj");
    Model* spotLightModel = new Model(modelPath, shader);
    world->ActiveEntities.push_back(spotLightModel);
    spotLightModel->Scale = glm::vec3(0.5f, 0.5f, 0.5f);
    spotLightModel->rotate = true;
    spotLightModel->spotLight->height = 0.85f;
    spotLightModel->Position = glm::vec3(19.5f, 3.0f, 28.5f);
    world->spotLightBlock = spotLightModel;

    world->modelsToLoad.push(spotLightModel);
}

void LoadGaul(World* world, Shader* shader)
{
    std::string modelPath = std::string("res/models/gaul/model.obj");
    Model* gaulModel = new Model(modelPath, shader);
    gaulModel->Position = glm::vec3(33.0f, 2.0f, 33.5f);
    gaulModel->rotate = true;

    world->modelsToLoad.push(gaulModel);
}

void LoadHorse(World* world, Shader* shader)
{
    std::string modelPath = std::string("res/models/horse/model.obj");
    Model* horseModel = new Model(modelPath, shader);
    horseModel->Position = glm::vec3(34.0f, 2.0f, 30.0f);
    world->modelsToLoad.push(horseModel);
}

void LoadGirls(World* world, Shader* shader)
{
    std::string modelPath = std::string("res/models/girlsOnBench/model.obj");
    Model* girlslModel = new Model(modelPath, shader);
    girlslModel->Position = glm::vec3(29.0f, 2.0f, 27.0f);

    world->modelsToLoad.push(girlslModel);
}

void LoadUtilityBox(World* world, Shader* shader)
{
    std::string modelPath = std::string("res/models/utilityBox/model.obj");
    Model* utilityBoxModel = new Model(modelPath, shader);
    utilityBoxModel->Position = glm::vec3(27.0f, 1.95f, 27.0f);

    world->modelsToLoad.push(utilityBoxModel);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}