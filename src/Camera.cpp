#include "Camera.hpp"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed){
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;

    front = glm::vec3(.0f,.0f,-1.0f);

    movementSpeed = startMovementSpeed;
    turnSpeed = startTurnSpeed;

    Update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime){
    GLfloat velocity = movementSpeed*deltaTime;
    glm::vec3 moveDirection = glm::vec3(0.0f);

    if(keys[GLFW_KEY_W]){
        moveDirection += front;
    }
    if(keys[GLFW_KEY_S]){
        moveDirection -= front;
    }
    if(keys[GLFW_KEY_A]){
        moveDirection -= right;
    }
    if(keys[GLFW_KEY_D]){
        moveDirection += right;
    }
    if(keys[GLFW_KEY_SPACE]){
        moveDirection += up;
    }
    if(keys[GLFW_KEY_LEFT_CONTROL]){
        moveDirection -= up;
    }

    if(glm::length(moveDirection) > 0.0f){
        position += glm::normalize(moveDirection) * velocity;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime){
    xChange = xChange*turnSpeed*deltaTime;    
    yChange = yChange*turnSpeed*deltaTime;    

    
    yaw += xChange;
    pitch += yChange;

    if(pitch > 89.0f){
        pitch = 89.0f;
    }
    if(pitch < -89.0f){
        pitch = -89.0f;
    }

    Update();
}

glm::mat4 Camera::calculateViewMatrix(){
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition(){
    return position;    
}

glm::vec3 Camera::getCameraDirection(){
    return glm::normalize(front);
}

void Camera::Update(){
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera(){

}