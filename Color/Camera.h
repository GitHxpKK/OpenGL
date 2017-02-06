#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include<vector>
#include<GL\glew.h>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>

enum Camera_Movement
{
	FOERWARD,
	BACKWARD,
	LEFT,
	RIGHNT
};
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      = 0.0f;
const GLfloat SPEED      = 5.0f;
const GLfloat SENSITIVITY = 0.15f;
const GLfloat ZOOM       = 45.0f;

class Camera
{
public:
	//摄像机属性值
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	
	//偏航角
	GLfloat Yaw;
	//俯仰角
	GLfloat Pitch;  

	GLfloat MovementSpeed;          //视角移动速度
	GLfloat MouseSensitivity;       //视角旋转的灵敏度
	GLfloat Zoom;                   //视角缩放值

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix(){
		//std::cout << this->Front.x << " " << this->Front.y << " " << this->Front.z << std::endl;
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime){
		GLfloat velocity = this->MovementSpeed*deltaTime;
		if (direction == FOERWARD) this->Position += this->Front*velocity;
		if (direction == BACKWARD) this->Position -= this->Front*velocity;
		if (direction == LEFT)     this->Position -= this->Right*velocity;
		if (direction == RIGHNT)   this->Position += this->Right*velocity;

		this->Position.y = 0.0f;
	}

	void ProcessMouseMovement(GLdouble xOffset, GLdouble yOffset, GLboolean constrainPitch = true){
		xOffset *= this->MouseSensitivity;
		yOffset *= this->MouseSensitivity;

		this->Yaw   += xOffset;
		this->Pitch += yOffset;
	  
		if (constrainPitch){
			if (this->Pitch > 89.0f)  this->Pitch = 89.0f;
			if (this->Pitch < -89.0f) this->Pitch = -89.0f;
		}

		this->UpdateCameraVectors() ;
	}

	void ProcessMouseScroll(GLfloat yOffset){
	
		if (this->Zoom >= 1.0f&&this->Zoom <= 90.0f) this->Zoom -= yOffset;
		if (this->Zoom <= 1.0f)  this->Zoom = 1.0f;
		if (this->Zoom >= 90.0f) this->Zoom = 90.0f;
	}

	~Camera();

private:
	//更新摄像机向量
	void UpdateCameraVectors(){
		glm::vec3 front;
		//参考球坐标
		front.x = cos(glm::radians(this->Pitch))*cos(glm::radians(this->Yaw));
		front.y = sin(glm::radians(this->Pitch));
		front.z = cos(glm::radians(this->Pitch))*sin(glm::radians(this->Yaw));

		//front.x = cos(this->Pitch)*cos(this->Yaw);
		//front.y = sin(this->Pitch);
		//front.z = cos(this->Pitch)*sin(this->Yaw);


		this->Front = glm::normalize(front);


		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};


Camera::~Camera()
{

}
#endif