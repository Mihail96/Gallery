#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

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

	glfwSetCursorPosCallback(Window::window, mouse_callback);
	glfwSetScrollCallback(Window::window, scroll_callback);
	glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Position = glm::vec3(0.0f, 0.0f, 0.0f);
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Right = glm::vec3(0.0f, 0.0f, 1.0f);
    Yaw = YAW;
    Pitch = PITCH;
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    updateCameraVectors();
}

void Player::Draw(Shader& shader)
{

}

void Player::processInput()
{
    if (glfwGetKey(Window::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(Window::window, true);
    }

    if (glfwGetKey(Window::window, GLFW_KEY_W) == GLFW_PRESS)
    {
        ProcessKeyboard(FORWARD, World::DeltaTime);
    }
    if (glfwGetKey(Window::window, GLFW_KEY_S) == GLFW_PRESS)
    {
        ProcessKeyboard(BACKWARD, World::DeltaTime);
    }
    if (glfwGetKey(Window::window, GLFW_KEY_A) == GLFW_PRESS)
    {
        ProcessKeyboard(LEFT, World::DeltaTime);
    }
    if (glfwGetKey(Window::window, GLFW_KEY_D) == GLFW_PRESS)
    {
        ProcessKeyboard(RIGHT, World::DeltaTime);
    }
}

glm::mat4 Player::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Player::ProcessKeyboard(Camera_Movement direction, double deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Player::ProcessMouseMovement(double xoffset, double yoffset,
                                  GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Player::ProcessMouseScroll(double yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    if (Zoom <= 1.0f)
        Zoom = 1.0f;
    if (Zoom >= 45.0f)
        Zoom = 45.0f;
}

void Player::MoveCamera(glm::vec3* position, glm::vec3* up, float yaw, float pitch)
{
    Position = *position;
    WorldUp = *up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

void Player::updateCameraVectors()
{
    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(static_cast<double>(Yaw))) * cos(glm::radians(static_cast<double>(Pitch))));
    front.y = static_cast<float>(sin(glm::radians(static_cast<double>(Pitch))));
    front.z = static_cast<float>(sin(glm::radians(static_cast<double>(Yaw))) * cos(glm::radians(static_cast<double>(Pitch))));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
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

    Player::GetInstance()->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Player::GetInstance()->ProcessMouseScroll(yoffset);
}
