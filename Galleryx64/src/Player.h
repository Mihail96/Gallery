#ifndef Player_H
#define Player_H

#include "Entity.h"
#include "Camera.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

class Player : public Entity
{
public:
	Camera camera;

	static Player* GetInstance();

	void SetCamera(glm::vec3* position, glm::vec3* up, float yaw, float pitch);

	void Draw(Shader& shader);

	void processInput();

private:
	static Player* instance;

	Player();

	Player(Player const&) = delete;
	void operator=(Player const&) = delete;
};
#endif
