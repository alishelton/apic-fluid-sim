//
//  Camera.h
//  simulator
//
//  Created by Raymond Ly on 5/7/19.
//  Copyright © 2019 Raymond Ly. All rights reserved.
//

#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// Movement Controls
enum Camera_Controls
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
    
};

// Left/right angle
const GLfloat YAW = -90.0f;
// Up/down angle
const GLfloat PITCH = 0.0f;

const GLfloat SPEED = 6.0f;
// Mouse movement sensitivity
const GLfloat SENSITIVITY = 0.25f;
//Field of View
const GLfloat FOV = 45.0f;

class Camera {
public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), //Relation between world and camera
           GLfloat yaw = YAW,
           GLfloat pitch = PITCH) :
    // Initialization list
    // Understanding reference: https://stackoverflow.com/questions/2785612/c-what-does-the-colon-after-a-constructor-mean
    front(glm::vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->update();
    }
    
    Camera(GLfloat posX,
           GLfloat posY,
           GLfloat posZ,
           GLfloat upX,
           GLfloat upY,
           GLfloat upZ,
           GLfloat yaw,
           GLfloat pitch) :
    front(glm::vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV)
    {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->update();
    }
    
    glm::mat4 getViewMatrix() {
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }
    
    // Processing Keypresses
    void processKeys(Camera_Controls direction, GLfloat deltaTime) {
        GLfloat velocity = moveSpeed * deltaTime;
        
        if (FORWARD == direction) {
            this->position += this->front * velocity;
        }
        if (BACKWARD == direction) {
            this->position -= this->front * velocity;
        }
        if (LEFT == direction) {
            this->position -= this->right * velocity;
        }
        if (RIGHT == direction) {
            this->position += this->right * velocity;
        }
    }
    
    // Processing Mouse Movement
    void processMouse( GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true )
    {
        xOffset *= this->mouseSensitivity;
        yOffset *= this->mouseSensitivity;
        
        this->yaw   += xOffset;
        this->pitch += yOffset;
        
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if ( constrainPitch )
        {
            if ( this->pitch > 89.0f )
            {
                this->pitch = 89.0f;
            }
            
            if ( this->pitch < -89.0f )
            {
                this->pitch = -89.0f;
            }
        }
        
        // Update Front, Right and Up Vectors using the updated Eular angles
        this->update( );
    }
    
    
    void processFOV(GLfloat yOffset) {
        if (this->fov >= 1.0f && this->fov <= 90.0f) {
            this->fov -= yOffset;
        }
        
        if (this->fov <= 1.0f) {
            this->fov = 1.0f;
        }
        
        if (this->fov > 90.0f) {
            this->fov = 90.0f;
        }
    }
    
    GLfloat getFOV() {
        return this->fov;
    }
    
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    GLfloat yaw;
    GLfloat pitch;
    GLfloat moveSpeed;
    GLfloat mouseSensitivity;
    GLfloat fov;
    
    void update() {
        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        
        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
};








