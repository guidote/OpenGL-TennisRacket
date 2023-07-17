#pragma once
// https://learnopengl.com/Getting-started/Camera

#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

class Camera {
    
    public:
        int width;
        int height;

        glm::vec3 cameraPos; // where camera is located
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); // where the camera should be looking (centre)
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f); // up vector

        float fov; // fov for perspective
        float near_plane; // near_plane for perspective
        float far_plane; // far_plane for perspective


        float lastXPos; // last known x position , used to calculate delta
        float lastYPos; // last known y position , used to calculate delta

        int shaderProgram;

        float deltay = 0.0f;
        float deltax = 0.0f;
        float deltaz = 0.0f;

        Camera(int shaderProgram, int width, int height, glm::vec3 cameraPos, float fov, float near_plane, float far_plane);
        void updateCameraMatrix();
        void procressCameraInputs(GLFWwindow *window);
        // the user can pan and tilt the camera as follows: while right button is pressed → use mouse movement in x direction to pan; and
                                                        // while middle button is pressed → use mouse movement in y direction to tilt.
        // the user can zoom in and out of the scene - while left button is pressed → use mouse movement to move into/out of the scene. 
};