#pragma once

#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Grid {
    public:
        GLuint VBO, VAO;
        int shaderProgram;

        int grid_size; // size of grid x size of grid ( grids are squares)
        float grid_unit; // size of a unit on the grid ( units are uniform across grid )
        int vertices_length; // number of vertices

        std::vector<glm::vec3> vertices; // vertices for lines of grid , automatically generated when initializing a grid

        Grid(int grid_size, int shaderProgram); // default constructor
        int drawGrid(); // display grid , used for drawing
};