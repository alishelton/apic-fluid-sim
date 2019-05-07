#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// SOIL
#include "SOIL2/SOIL2.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "shader.h"
#include "Camera.h"

// Window Dimensions
const GLint WIDTH = 800, HEIGHT = 600;
int screenWidth, screenHeight;

void KeyCall(GLFWwindow *window, int key, int scancode, int action, int mode);
void ScrollCall(GLFWwindow *window, double xOffset, double yOffset);
void MouseCall(GLFWwindow *window, double xPos, double yPos);
void Move();

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
GLfloat prevX = WIDTH/2.0f;
GLfloat prevY = WIDTH/2.0f;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


int main() {
    glfwInit();
    
    // Setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Required for Mac Compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Resizability (optional)
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create a window for GLFW operations
    GLFWwindow *window = glfwCreateWindow(WIDTH, WIDTH, "FluidSim", nullptr, nullptr);
    
    // Required for Mac Compatibility/High res/retina display compatability
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, KeyCall);
    glfwSetCursorPosCallback(window, MouseCall);
    glfwSetScrollCallback(window, ScrollCall);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    
    // Tells GLEW to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    
    // Initialize GLEW to set up OpenGL Function pointers
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }
    
    // Define viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);
    
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Shader myShader("resources/shaders/core.vs", "resources/shaders/core.frag");
    
    
    // Cube object vertices/faces
    GLfloat vertices[] = {
        // Object space coordinates xyz //Texture map coordinates ab
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    // Creating an array of position vectors for multiple object initialization
    glm::vec3 objPositions[] =
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 3.0f, 3.0f),
        glm::vec3(2.0f, -1.0f, -5.5f),
        glm::vec3(5.0f, -3.0f, 1.0f),
        glm::vec3(-3.0f, 3.0f, 7.0f),
        glm::vec3(1.0f, 1.2f, -16.0f),
        glm::vec3(-4.0f, -1.5f, -4.0f),
        glm::vec3(3.0f, -4.0f, -9.0f),
        glm::vec3(-1.3f, 5.0f, -4.0f),
        glm::vec3(1.2f, 4.0f, 5.0f)
    };
    
    // Initializing Vertex Buffer Objects and Vertex Array Objects
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind Vertex Array, then set buffers and attribute pointers
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid * ) 0);
    glEnableVertexAttribArray(0);

    //Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid * )(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    //Unbind VAO
    glBindVertexArray(0);
    
    // Load in textures
    GLuint texture;
    
    int texWidth, texHeight;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    unsigned char *image = SOIL_load_image("resources/images/image1.jpg", &texWidth, &texHeight, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Update per frame
    while (!glfwWindowShouldClose(window)) {
        GLfloat curFrame = glfwGetTime();
        deltaTime = curFrame - lastFrame;
        lastFrame = curFrame;
        
        // Poll for events i.e. mouse clicks, keypresses, etc.
        glfwPollEvents();
        
        Move();
        
        // Render, clear color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture"), 0);
        
        myShader.Use();
        
        // Creating the view frustrum
        glm::mat4 projection = glm::perspective(camera.getFOV(), (GLfloat) screenWidth/ (GLfloat) screenHeight, 0.1f, 1000.0f);
        
        // Making and applying the transformations to our objects
        glm::mat4 model(1);
        glm::mat4 view(1);
        view = camera.getViewMatrix();
        
        GLint modelLocation = glGetUniformLocation(myShader.Program, "model");
        GLint viewLocation = glGetUniformLocation(myShader.Program, "view");
        GLint projectionLocation = glGetUniformLocation(myShader.Program, "projection");
        
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        
        
        // Draw
        glBindVertexArray(VAO);
        for(GLuint i = 0; i < 10; i++) {
            glm::mat4 model(1);
            model = glm::translate(model, objPositions[i]);
            GLfloat angle = 20.0f * i; //Precalculated number
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
        // Swap Screen buffers
        glfwSwapBuffers(window);
    }
    
    // Cleaning
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}

void Move() {
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        camera.processKeys(FORWARD, deltaTime);
    }
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        camera.processKeys(BACKWARD, deltaTime);
    }
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        camera.processKeys(LEFT, deltaTime);
    }
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        camera.processKeys(RIGHT, deltaTime);
    }
}

void KeyCall(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (GLFW_KEY_ESCAPE == key && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (GLFW_PRESS == action) {
            keys[key] = true;
        }
        else if (GLFW_RELEASE == action) {
            keys[key] = false;
        }
    }
}

void MouseCall(GLFWwindow *window, double xPos, double yPos) {
    if (firstMouse) {
        prevX = xPos;
        prevY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - prevX;
    GLfloat yOffset = prevY - yPos;
    
    prevX = xPos;
    prevY = yPos;
    
    camera.processMouse(xOffset, yOffset);
}

void ScrollCall(GLFWwindow *window, double xOffset, double yOffset) {
    camera.processFOV(yOffset);
}
