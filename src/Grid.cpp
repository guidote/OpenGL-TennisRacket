#include "Grid.h"

Grid::Grid(int grid_size, int shaderProgram) {
    Grid::grid_size = grid_size;
    Grid::shaderProgram = shaderProgram;
    Grid::grid_unit = 2.0/grid_size;
    // move y down -0.001f so that the coord lines dont get covered
    // parallel to the z direction starting at (-1,0,-1)(-1,0,1)
    for (int i=0; i<=grid_size; i++) {
        vertices.push_back(glm::vec3((-1.0f + i*grid_unit), -0.001f, -1.0f));
        vertices.push_back(glm::vec3(0.412f, 0.412f, 0.412f)); // grey colour
        vertices.push_back(glm::vec3((-1.0f + i*grid_unit), -0.001f, 1.0f));
        vertices.push_back(glm::vec3(0.412f, 0.412f, 0.412f)); // grey colour
    }

    // parallel to the x direction starting at (1,0,-1)(-1,0,-1)
    for (int i=0; i<=grid_size; i++) {
        vertices.push_back(glm::vec3(1.0f, -0.001f, (-1.0f + i*grid_unit)));
        vertices.push_back(glm::vec3(0.412f, 0.412f, 0.412f)); // grey colour
        vertices.push_back(glm::vec3(-1.0f, -0.001f, (-1.0f + i*grid_unit)));
        vertices.push_back(glm::vec3(0.412f, 0.412f, 0.412f)); // grey colour
    }

    Grid::vertices_length = vertices.size()/2; // half of the values in the vertex are colour assignning

    // vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // vertex buffer object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          2*sizeof(glm::vec3),
                          (void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO
}

int Grid::drawGrid() {    

    glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, 1.0f, 2.5f)); // transform scale so that of unit is actually 0.25
    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(transform));
    glDrawArrays(GL_LINES, 0, vertices_length);

    glBindVertexArray(0);
   
    return -1;
}