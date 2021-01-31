#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Shader.h"
#include "World.h"
#include "Model.h"
#include "Block.h"
#include "Window.h"

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

    Shader ourShader("res/shaders/vertex.shader", "res/shaders/fragment.shader");

    glm::vec3 cameraPosition = glm::vec3(3.0f, 4.0f, 4.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    World* world = World::GetInstance();
    world->InitEntities(WorldSize);
    world->SetCamera(&cameraPosition, &cameraUp, 90.0f, -45.0f);

    const char* name = "res/textures/container.jpg";
    loadedTextures.push_back(world->LoadTexture(name));

    //std::string modelPath = std::string("res/models/backpack/backpack.obj");
    //Entity* backpackModel = new Model(modelPath);
    Entity* block1 = new Block(loadedTextures[0]);
    Entity* block2 = new Block(loadedTextures[0]);
    Entity* block3 = new Block(loadedTextures[0]);
    Entity* block4 = new Block(loadedTextures[0]);
    Entity* block5 = new Block(loadedTextures[0]);
    Entity* block6 = new Block(loadedTextures[0]);
    Entity* block7 = new Block(loadedTextures[0]);
    Entity* block8 = new Block(loadedTextures[0]);
    Entity* block9 = new Block(loadedTextures[0]);

    Entity* blockw1 = new Block(loadedTextures[0]);
    Entity* blockw2 = new Block(loadedTextures[0]);
    Entity* blockw3 = new Block(loadedTextures[0]);
    Entity* blockw4 = new Block(loadedTextures[0]);
    Entity* blockw5 = new Block(loadedTextures[0]);
    Entity* blockw6 = new Block(loadedTextures[0]);
    Entity* blockw7 = new Block(loadedTextures[0]);
    Entity* blockw8 = new Block(loadedTextures[0]);
    Entity* blockw9 = new Block(loadedTextures[0]);
    Entity* blockw10 = new Block(loadedTextures[0]);
    Entity* blockw11 = new Block(loadedTextures[0]);
    Entity* blockw12 = new Block(loadedTextures[0]);

    Entity* blockr1 = new Block(loadedTextures[0]);

    //world->InsertEntity(backpackModel, 3.0f, 3.0f, 3.0f);

    world->InsertEntity(block1, 2.0f, 2.0f, 4.0f);
    world->InsertEntity(block2, 3.0f, 2.0f, 4.0f);
    world->InsertEntity(block3, 4.0f, 2.0f, 4.0f);

    world->InsertEntity(block4, 2.0f, 2.0f, 3.0f);
    world->InsertEntity(block5, 3.0f, 2.0f, 3.0f);
    world->InsertEntity(block6, 4.0f, 2.0f, 3.0f);

    world->InsertEntity(block7, 2.0f, 2.0f, 2.0f);
    world->InsertEntity(block8, 3.0f, 2.0f, 2.0f);
    world->InsertEntity(block9, 4.0f, 2.0f, 2.0f);


    world->InsertEntity(blockw1,  2.0f, 3.0f, 5.0f);
    world->InsertEntity(blockw2,  3.0f, 3.0f, 5.0f);
    world->InsertEntity(blockw3,  4.0f, 3.0f, 5.0f);
                                        
    world->InsertEntity(blockw4,  5.0f, 3.0f, 4.0f);
    world->InsertEntity(blockw5,  5.0f, 3.0f, 3.0f);
    world->InsertEntity(blockw6,  5.0f, 3.0f, 2.0f);
                                        
    world->InsertEntity(blockw7,  2.0f, 3.0f, 1.0f);
    world->InsertEntity(blockw8,  3.0f, 3.0f, 1.0f);
    world->InsertEntity(blockw9,  4.0f, 3.0f, 1.0f);
                                        
    world->InsertEntity(blockw10, 1.0f, 3.0f, 2.0f);
    world->InsertEntity(blockw11, 1.0f, 3.0f, 3.0f);
    world->InsertEntity(blockw12, 1.0f, 3.0f, 4.0f);

    world->InsertEntity(blockr1, 2.0f, 4.0f, 4.0f);

    while (!glfwWindowShouldClose(Window::window))
    {
        world->ProcessTime();

        world->Player->processInput();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(world->Player->Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = world->Player->GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        world->Draw(ourShader);

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