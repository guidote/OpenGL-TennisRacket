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

        glm::mat4 worldModelMatrix = glm::mat4(1.0f); // model matrix for cube

        bool pointsOn = false;
        bool linesOn = false;

        Cube(glm::vec3 colour, int shaderProgram); // default constructor

        void setBooleans(bool pointsOn, bool linesOn); // set booleans that determine how model is drawn

        void cubeTransformation(glm::mat4 model_matrix, float angle, glm::vec3 rotate, glm::vec3 translate, glm::vec3 scale);
        void drawCube(); // display cube, used for drawing
};