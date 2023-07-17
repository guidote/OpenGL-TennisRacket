#include "Line.h"

Line::Line(glm::vec3 p1, glm::vec3 p2, glm::vec3 colour, int shaderProgram) {
    Line::colour = colour;
    Line::p1 = p1;
    Line::p2 = p2;

    Line::shaderProgram = shaderProgram;

    glm::vec3 vertices[] = 
    {
        glm::vec3(p1.x, p1.y, p1.z),  // p1
        glm::vec3(colour.r, colour.g, colour.b), // colour
        glm::vec3(p2.x, p2.y, p2.z),  // p2
        glm::vec3(colour.r, colour.g, colour.b) // colour
    };

    // vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    
    // vertex buffer object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

int Line::drawLine() {

    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2); // 2 vertices, starting at index 0
    glBindVertexArray(0);
    return -1;
}