#pragma once

#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/type_ptr.hpp>

class Cube {
    public:
        GLuint VBO, VAO;
        int shaderProgram;

        glm::vec3 colour;

        GLfloat vertices[72];

        glm::mat4 matrix = glm::mat4(1.0f);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        glm::mat4 translationMatrix = glm::mat4(1.0f);
        glm::mat4 scalingMatrix = glm::mat4(1.0f);

        glm::mat4 offsetRotationMatrix = glm::mat4(1.0f);

        void scaleBack(glm::vec3 scale);

        bool pointsOn = false;
        bool linesOn = false;

        Cube(glm::vec3 colour, int shaderProgram); // default constructor

        void setBooleans(bool pointsOn, bool linesOn);
 
        void cubeRotate(float angle, glm::vec3 rotate); // calculates the rotation matrix
        void cubeScale(glm::vec3 scale); // calculates the scaling matrix

        void cubeTransformation(float angle, glm::vec3 rotate, glm::vec3 translate1, glm::vec3 translate2, glm::vec3 scale); // when rotating and using translation we need to translate twice
        
        int drawCube(); // display cube, used for drawing
};