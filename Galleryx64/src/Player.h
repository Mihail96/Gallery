#ifndef Player_H
#define Player_H

#include "Entity.h"

enum Camera_Movement
{
	FORWARD, BACKWARD, LEFT, RIGHT, JUMP
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

class Player : public Entity
{
public:
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	float GravityVelocity;

	static Player* GetInstance();

	void Draw(Shader& shader);

	void processInput();

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, double deltaTime);

	void ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch = true);

	void ProcessMouseScroll(double yoffset);

	void MoveCamera(glm::vec3* position, glm::vec3* up, float yaw = YAW, float pitch = PITCH);

	void ProcessGravity(double deltaTime);

private:
	static Player* instance;

	Player();

	Player(Player const&) = delete;
	void operator=(Player const&) = delete;

	void updateCameraVectors();
};
#endif
