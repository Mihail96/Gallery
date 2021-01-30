#include <GLFW/glfw3.h>

#include "Player.h"

#include "Window.h"
#include "World.h"

double lastX = Window::SCR_WIDTH / 2.0f;
double lastY = Window::SCR_HEIGHT / 2.0f;
bool firstMouse = true;

Player* Player::instance = nullptr;

Player* Player::GetInstance()
{
    if (!instance)
    {
        instance = new Player();
    }
    return instance;
}

Player::Player()
{
	this->MinPosition = glm::vec3(-0.3f, -0.3f, -0.3f);
	this->MaxPosition = glm::vec3(0.3f, 0.3f, 0.3f);

	this->camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

	glfwSetCursorPosCallback(Window::window, mouse_callback);
	glfwSetScrollCallback(Window::window, scroll_callback);
	glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Player::SetCamera(glm::vec3* position, glm::vec3* up, float yaw, float pitch)
{
	this->camera.MoveCamera(*position, *up, yaw, pitch);
}

void Player::Draw(Shader& shader)
{

}

void Player::processInput()
{
    if (glfwGetKey(Window::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Window::window, true);

    if (glfwGetKey(Window::window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, World::DeltaTime);
    if (glfwGetKey(Window::window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, World::DeltaTime);
    if (glfwGetKey(Window::window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, World::DeltaTime);
    if (glfwGetKey(Window::window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, World::DeltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    Player::GetInstance()->camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Player::GetInstance()->camera.ProcessMouseScroll(yoffset);
}
