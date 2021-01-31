#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "Player.h"

#include "Window.h"
#include "World.h"
#include <iostream>

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
	this->MaxPosition = glm::vec3(0.2f, 0.1f, 0.2f);
    this->MinPosition = glm::vec3(-0.2f, -0.6f, -0.2f);

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
    GravityVelocity = 0;
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
    if (glfwGetKey(Window::window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        ProcessKeyboard(JUMP, World::DeltaTime);
    }
}

glm::mat4 Player::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Player::ProcessKeyboard(Camera_Movement direction, double deltaTime)
{
    float velocity = MovementSpeed * deltaTime;

    int coordX = floor(Position.x);
    int coordY = floor(Position.y);
    int coordZ = floor(Position.z);

    int minCoordX = floor(Position.x + MinPosition.x);
    int minCoordZ = floor(Position.z + MinPosition.z);

    int maxCoordX = floor(Position.x + MaxPosition.x);
    int maxCoordZ = floor(Position.z + MaxPosition.z);

    Entity** coordinates = World::GetInstance()->Coordinates;
    unsigned int worldSize = World::WorldSize;

    Entity* northEntity = coordinates[maxCoordX + worldSize * (coordY + worldSize * coordZ)]; // X Max
    Entity* southEntity = coordinates[minCoordX + worldSize * (coordY + worldSize * coordZ)]; // X Min
    Entity* eastEntity = coordinates[coordX + worldSize * (coordY + worldSize * maxCoordZ)]; // Z Max
    Entity* westEntity = coordinates[coordX + worldSize * (coordY + worldSize * minCoordZ)]; // Z Min

    if (direction == FORWARD)
    {
        if (!northEntity && Front.x >= 0 || !southEntity && Front.x < 0)
        {
            Position.x += Front.x * velocity;
        }
        if (!eastEntity && Front.z >= 0 || !westEntity && Front.z < 0)
        {
            Position.z += Front.z * velocity;
        }
    }
    if (direction == BACKWARD)
    {
        if (!northEntity && Front.x < 0 || !southEntity && Front.x >= 0)
        {
            Position.x -= Front.x * velocity;
        }
        if (!eastEntity && Front.z < 0 || !westEntity && Front.z >= 0)
        {
            Position.z -= Front.z * velocity;
        }
    }
    if (direction == LEFT)
    {
        if (!northEntity && Right.x < 0 || !southEntity && Right.x >= 0)
        {
            Position.x -= Right.x * velocity;
        }
        if (!eastEntity && Right.z < 0 || !westEntity && Right.z >= 0)
        {
            Position.z -= Right.z * velocity;
        }
    }
    if (direction == RIGHT)
    {
        if (!northEntity && Right.x >= 0 || !southEntity && Right.x < 0)
        {
            Position.x += Right.x * velocity;
        }
        if (!eastEntity && Right.z >= 0 || !westEntity && Right.z < 0)
        {
            Position.z += Right.z * velocity;
        }
    }
    if (direction == JUMP && GravityVelocity == 0)
    {
        GravityVelocity = -2;
        Position.y += 0.1;
    }

    std::cout << "Player Position: " << Position.x << ", " << Position.y << ", " << Position.z << std::endl;
}

void Player::ProcessGravity(double deltaTime)
{
    float velocity = GravityVelocity * deltaTime;
    Position.y -= velocity;

    std::cout << "Player Position: " << Position.x << ", " << Position.y << ", " << Position.z << std::endl;
}

void Player::ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch)
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

    Position = glm::vec3(3.0f, 4.0f, 4.0f);
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
