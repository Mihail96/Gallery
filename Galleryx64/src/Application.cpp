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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

    Shader modelShader("res/shaders/ModelVertex.shader", "res/shaders/ModelFragment.shader");
    Shader blockShader("res/shaders/BlockVertex.shader", "res/shaders/BlockFragment.shader");
    Shader lightShader("res/shaders/LightVertex.shader", "res/shaders/LightFragment.shader");

    glm::vec3 cameraPosition = glm::vec3(30.0f, 3.0f, 30.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    World* world = World::GetInstance();
    world->InitEntities(WorldSize);
    world->SetCamera(&cameraPosition, &cameraUp, 90.0f, -45.0f);

    const char* name = "res/textures/container.jpg";
    loadedTextures.push_back(world->LoadTexture(name));
    name = "res/textures/marble.jpg";
    loadedTextures.push_back(world->LoadTexture(name));
    name = "res/textures/floor.jpg";
    loadedTextures.push_back(world->LoadTexture(name));
    name = "res/textures/forrestground.jpg";
    loadedTextures.push_back(world->LoadTexture(name));

    //std::string modelPath = std::string("res/models/backpack/backpack.obj");
    ////Entity* backpackModel = new Model(modelPath, &modelShader);

    Entity* light1 = new Block(loadedTextures[1], &lightShader);
    light1->pointLight = new PointLight();
    world->InsertEntity(light1, 30.5f, 3.5f, 30.5f);

    Entity* light2 = new Block(loadedTextures[1], &lightShader);
    light2->pointLight = new PointLight();
    world->InsertEntity(light2, 20.5f, 2.5f, 20.5f);

    Entity* light3 = new Block(loadedTextures[1], &lightShader);
    light3->pointLight = new PointLight();
    world->InsertEntity(light3, 20.5f, 2.5f, 40.5f);

    Entity* light4 = new Block(loadedTextures[1], &lightShader);
    light4->pointLight = new PointLight();
    world->InsertEntity(light4, 40.5f, 2.5f, 20.5f);

    Entity* light5 = new Block(loadedTextures[1], &lightShader);
    light5->pointLight = new PointLight();
    world->InsertEntity(light5, 40.5f, 2.5f, 40.5f);

    

    for (int x = 20; x < 40; x++)
    {
        for (int y = 20; y < 40; y++)
        {
            int z = 1;
            Entity* block = new Block(loadedTextures[3], &blockShader);
            world->InsertEntity(block, x, z, y);
        }
    }

    while (!glfwWindowShouldClose(Window::window))
    {
        world->ProcessTime();

        world->Player->processInput();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world->Draw();

        glfwSwapBuffers(Window::window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}