#include "Cube.h"

Cube::Cube(glm::vec3 colour, int shaderProgram) {

    Cube::colour = colour;

    Cube::shaderProgram = shaderProgram;

    float x = colour.x;
    float y = colour.y;
    float z = colour.z;

    GLfloat vertices[] = {
        // set of vertices for each face
        // front face
        0.05f, 0.0f, 0.05f, x, y, z,
        -0.05f, 0.10f, 0.05f, x, y, z,
        -0.05f, 0.0f, 0.05f, x, y, z,

        0.05f, 0.0f, 0.05f, x, y, z,
        0.05f,0.10f, 0.05f, x, y, z,
        -0.05f, 0.10f, 0.05f, x, y, z,

        // back face
        0.05f, 0.0f, -0.05f, x, y, z,
        -0.05f, 0.10f, -0.05f, x, y, z,
        -0.05f, 0.0f, -0.05f, x, y, z,

        0.05f, 0.0f, -0.05f, x, y, z,
        0.05f, 0.10f, -0.05f, x, y, z,
        -0.05f, 0.10f, -0.05f, x, y, z,

        // bottom face
        0.05f, 0.0f, 0.05f, x, y, z,
        -0.05f, 0.0f, -0.05f, x, y, z,
        -0.05f, 0.0f, 0.05f, x, y, z,

        0.05f, 0.0f, 0.05f, x, y, z,
        0.05f, 0.0f, -0.05f, x, y, z,
        -0.05f, 0.0f, -0.05f, x, y, z,

        // top face
        0.05f, 0.10f, 0.05f, x, y, z,
        -0.05f, 0.10f, -0.05f, x, y, z,
        -0.05f, 0.10f, 0.05f, x, y, z,

        0.05f, 0.10f, 0.05f, x, y, z,
        0.05f, 0.10f, -0.05f, x, y, z,
        -0.05f, 0.10f, -0.05f, x, y, z,

        // right face
        0.05f, 0.0f,-0.05f, x, y, z,
        0.05f, 0.10f, 0.05f, x, y, z,
        0.05f, 0.0f, 0.05f, x, y, z,

        0.05f, 0.0f, -0.05f, x, y, z,
        0.05f, 0.10f,-0.05f, x, y, z,
        0.05f, 0.10f, 0.05f, x, y, z,

        //left face
        -0.05f, 0.0f, -0.05f, x, y, z,
        -0.05f, 0.10f,-0.05f, x, y, z,
        -0.05f, 0.10f, 0.05f, x, y, z,

        -0.05f, 0.0f, -0.05f, x, y, z,
        -0.05f, 0.10f, -0.05f, x, y, z,
        -0.05f, 0.10f, 0.05f, x, y, z
    }; 

    // store vertex data in buffer
    // VAO (vertex array object) stores pointers to one or more VBOs (vertex buffer object)
    // EBO (element buffer object)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
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

void Cube::setBooleans(bool pointsOn, bool linesOn) {
    Cube::pointsOn = pointsOn;
    Cube::linesOn = linesOn;
}

void Cube::cubeRotate(float angle, glm::vec3 rotate) {
    Cube::offsetRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotate);
}

void Cube::cubeTransformation(float angle, glm::vec3 rotate, glm::vec3 translate1, glm::vec3 translate2, glm::vec3 scale) {
    glm::mat4 m = glm::mat4(1.0f);

    Cube::rotationMatrix = glm::rotate(m, glm::radians(angle), rotate);

    glm::mat4 translationMatrix1 = glm::translate(m, translate1);
    glm::mat4 translationMatrix2 = glm::translate(m, translate2);
    Cube::translationMatrix = translationMatrix1 * translationMatrix2;

    glm::mat4 translationMatrixcentre = glm::translate(m, glm::vec3(0.0f, 0.0f, 0.0f));

    Cube::scalingMatrix = glm::scale(m, scale);

    Cube::matrix = offsetRotationMatrix * (translationMatrix * rotationMatrix * translationMatrixcentre * scalingMatrix);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(matrix));
}

int Cube::drawCube() {
    glBindVertexArray(VAO);
    if (Cube::pointsOn) { glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); }
    else if (Cube::linesOn) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    return -1;
}
