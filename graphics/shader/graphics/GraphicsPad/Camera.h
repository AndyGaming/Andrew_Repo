#pragma once
#include <glm\glm.hpp>

class Camera
{
	glm::vec2 oldMousePosition;
	glm::vec3 strafeDirection;

public:
	Camera():
		Position(0.0f,5.0f,3.0f),
		ViewDir(0.0f,-1.0f,-1.0f),
		UpDir(0.0f,1.0f,0.0f),
		TengentDir(1.0f,0.0f,0.0f)
	{}

	glm::mat4 getWorldToViewMatrix() const;
	glm::vec3 getPosition() const
	{
		return Position;
	}
	void setPosition(glm::vec3 NewPosition) {
		Position = NewPosition;
	}

	void move_forward();
	void move_backward();
	void move_leftward();
	void move_rightward();
	void move_upward();
	void move_downward();
	void rotate_left();
	void rotate_right();
	void rotate_up();
	void rotate_down();
	void mouseUpdate(const glm::vec2& newMousePosition);

protected:
	glm::vec3 Position;
	glm::vec3 ViewDir;
	const glm::vec3 UpDir;
	static const float Movement_speed;
	static const float Rotation_speed;
	glm::vec3 TengentDir;
};
