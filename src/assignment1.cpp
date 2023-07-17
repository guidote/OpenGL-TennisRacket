// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

/*
model maps from an object's local coordinate space into world space, 
view from world space to camera space, 
projection from camera to screen.
*/

#include <iostream>
#include <vector>

#include "Line.h"
#include "Camera.h"
#include "Grid.h"
#include "Cube.h"

#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
bool pointsOn = false;
bool linesOn = false;

glm::vec3 starting_location = glm::vec3(0.0f, 0.0f, 0.0f);

float s = 1.0f;
float r = 5.0f;

const char* getVertexShaderSource()
{
    return "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    "out vec3 vertexColor;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 projection;"
    "void main()"
    "{"
    "   vertexColor = aColor;"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0f);"
    "}";
}

const char* getFragmentShaderSource()
{
    return "#version 330 core\n"
    "in vec3 vertexColor;"
    "out vec4 FragColor;"
    "void main()"
    "{"
    "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
    "}";
}

int compileAndLinkShaders()
{
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;

    return -1;
}

int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Comp371 - assignment1", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // compile and link shaders
    int shaderProgram = compileAndLinkShaders();

    // setting perspective doesnt need to be repeated
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // camera object
	Camera camera(shaderProgram, SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.5f, 3.0f), 45.0f, 0.1f, 100.0f);

    Grid grid(100, shaderProgram);

    // coordinate lines
    Line line1(glm::vec3(0,0,0), glm::vec3(0.25,0.0,0), glm::vec3(1,0,0), shaderProgram); // red x coordinate
    Line line2(glm::vec3(0,0,0), glm::vec3(0,0.25,0), glm::vec3(0,1,0), shaderProgram); // green y coordinate
    Line line3(glm::vec3(0,0,0), glm::vec3(0,0.0,0.25), glm::vec3(0,0,1), shaderProgram); // blue z coordinate

    // cube
    Cube cubeArm(glm::vec3(0.961,0.812,0.62), shaderProgram);

    // tennis racket shape
    Cube cubeRacket(glm::vec3(0.933,0.510,0.933), shaderProgram);

    // enable depth test
    glEnable(GL_DEPTH_TEST); // hidden surface removal
    glDepthFunc(GL_LESS);

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {   
        // input
        camera.procressCameraInputs(window);

        // each frame, reset color and depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // matrix transformations
        camera.updateCameraMatrix();

        // drawing
        line1.drawLine();
        line2.drawLine();
        line3.drawLine();

        grid.drawGrid();


        // -- arm -- //
        // bicep
        cubeArm.cubeTransformation(-45.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3(0.0f, 0.0f, 0.0f)) * s, starting_location,(glm::vec3(0.5f, 2.0f, 0.5f)) * s);
        cubeArm.drawCube();
        // forearm
            // use tan(angle) = opp/adj to find how much we need to translate x
            // increase height by the 0.10f height to be able to stack the cubes
            // -0.025f and +0.025f to make sure tha elbow connects
        cubeArm.cubeTransformation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1) - 0.025f, 0.1f + 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.5f, 2.0f, 0.5f)) * s);
        cubeArm.drawCube();

        // -- tennis racket shape -- //
        // handle
        cubeRacket.cubeTransformation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f, (0.1f) * 3.0f + 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.25f, 2.0f, 0.25f)) * s);
        cubeRacket.drawCube();

        // racket
        cubeRacket.cubeTransformation(-70.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - (0.025f), (0.1f) * 5.0f + 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.25f, 1.25f, 0.25f)) * s);
        cubeRacket.drawCube();

        cubeRacket.cubeTransformation(70.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - (0.025f), (0.1f) * 5.0f + 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.25f, 1.25f, 0.25f)) * s);
        cubeRacket.drawCube();

        cubeRacket.cubeTransformation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f + 0.125f, 0.1f * 5.0f + 0.065f, 0.0f)) * s, starting_location, (glm::vec3(0.25f, 2.0f, 0.25f)) * s);
        cubeRacket.drawCube();

        cubeRacket.cubeTransformation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f - 0.125f, 0.1f * 5.0f + 0.065f, 0.0f)) * s, starting_location, (glm::vec3(0.25f, 2.0f, 0.25f)) * s);
        cubeRacket.drawCube();

        cubeRacket.cubeTransformation(-35.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f - 0.125f, (0.1f * 5.0f + 0.065f) + 2.0f * (0.05f * 2.0f), 0.0f)) * s, starting_location, (glm::vec3(0.25f, 1.25f, 0.25f)) * s);
        cubeRacket.drawCube();

        cubeRacket.cubeTransformation(35.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f + 0.125f, (0.1f * 5.0f + 0.065) + 2.0f * (0.05f * 2.0f), 0.0f)) * s, starting_location, (glm::vec3(0.25f, 1.25f, 0.25f)) * s);
        cubeRacket.drawCube();

        cubeRacket.cubeTransformation(90.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) + 0.035f, (0.1f * 5.0f + 0.065f) + 3.0f * (0.05f * 2.0f), 0.0f)) * s, starting_location, (glm::vec3(0.25f, 1.25f, 0.25f)) * s);
        cubeRacket.drawCube();

        cubeRacket.cubeTransformation(90.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) + 0.1f, (0.1f * 6.0f) - 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.25f, 2.5f, 0.25f)) * s);
        cubeRacket.drawCube();

        // -- tennis strings -- //
        cubeRacket.cubeTransformation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f, (0.1f * 6.0f) - 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.025f, 3.0f, 0.025f)) * s);
        cubeRacket.drawCube();
        for (int i=1; i<= 4; i++) {
            if ( i > 2) {
                cubeRacket.cubeTransformation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f + (i * 0.025f), (0.1f * 6.0f) - 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.025f, 2.5f, 0.025f)) * s);
                cubeRacket.drawCube();
                cubeRacket.cubeTransformation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f - (i * 0.025f), (0.1f * 6.0f) - 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.025f, 2.5f, 0.025f)) * s);
                cubeRacket.drawCube();
            }
            else {
                cubeRacket.cubeTransformation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f + (i * 0.025f), (0.1f * 6.0f) - 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.025f, 3.0f, 0.025f)) * s);
                cubeRacket.drawCube();
                cubeRacket.cubeTransformation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) - 0.025f - (i * 0.025f), (0.1f * 6.0f) - 0.025f, 0.0f)) * s, starting_location, (glm::vec3(0.025f, 3.0f, 0.025f)) * s);
                cubeRacket.drawCube();
            }
        }

        cubeRacket.cubeTransformation(90.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) + 0.1f, (0.1f * 6.0f) + 0.105f, 0.0f)) * s, starting_location, (glm::vec3(0.025f, 2.5f, 0.025f)) * s);
        cubeRacket.drawCube();
        for (int i=1; i<= 4; i++) {
            if (i == 4) { 
                cubeRacket.cubeTransformation(90.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) + 0.1f, (0.1f * 6.0f) + 0.105f - (i * 0.025f), 0.0f)) * s, starting_location, (glm::vec3(0.025f, 2.5f, 0.025f)) * s);
                cubeRacket.drawCube();
                cubeRacket.cubeTransformation(90.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) + 0.075f, (0.1f * 6.0f) + 0.105f + (i * 0.025f), 0.0f)) * s, starting_location, (glm::vec3(0.025f, 2.0f, 0.025f)) * s);
                cubeRacket.drawCube();
            }
            else {
                cubeRacket.cubeTransformation(90.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) + 0.1f, (0.1f * 6.0f) + 0.105f + (i * 0.025f), 0.0f)) * s, starting_location, (glm::vec3(0.025f, 2.5f, 0.025f)) * s);
                cubeRacket.drawCube();
                cubeRacket.cubeTransformation(90.0f, glm::vec3(0.0f, 0.0f, 1.0f), (glm::vec3((tan(45) * 0.1f) + 0.1f, (0.1f * 6.0f) + 0.105f - (i * 0.025f), 0.0f)) * s, starting_location, (glm::vec3(0.025f, 2.5f, 0.025f)) * s);
                cubeRacket.drawCube();
            }
        }

        // end frame
        glfwSwapBuffers(window);
        
        // detect inputs
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwSetWindowShouldClose(window, true);}
        // key ‘P’ for points, key ‘L’ for lines, key ‘T’ for triangles
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
        {
            cubeArm.setBooleans(true, false);
            cubeRacket.setBooleans(true, false);
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) 
        {
            cubeArm.setBooleans(false, true);
            cubeRacket.setBooleans(false, true);
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) 
        {
            cubeArm.setBooleans(false, false);
            cubeRacket.setBooleans(false, false);
        }
        // key 'spacebar' should re-position the model at a random location on the grid
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
        {
            float x_value = rand()%100 * (1.0f/100.0f); // assume the 0,0 is 50,50
            float z_value = rand()%100 * (1.0f/100.0f); // assume the 0,0 is 50,50

            if ((rand() & 1) == 0) { x_value = -x_value; }; // generate number between 1 and 0, if 0 negative
            if ((rand() & 1) == 0) { z_value = -z_value; }; // generate number between 1 and 0, if 0 negative

            starting_location = glm::vec3(x_value, 0.0f, z_value);
        }
        // key 'A' → move left, 'D' → move right, 'W' → move up, 'S' → move down, 
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)))
        {
            float x_value = starting_location.x - (1.0f/100.0f);
            starting_location = glm::vec3(x_value, 0.0f, starting_location.z);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)))
        {
            float x_value = starting_location.x + (1.0f/100.0f);
            starting_location = glm::vec3(x_value, 0.0f, starting_location.z);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)))
        {
            float z_value = starting_location.z - (1.0f/100.0f);
            starting_location = glm::vec3(starting_location.x, 0.0f, z_value);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)))
        {
            float z_value = starting_location.z + (1.0f/100.0f);
            starting_location = glm::vec3(starting_location.x, 0.0f, z_value);
        }
        // key 'a' → rotate left 5 degrees about Y axis, 'd' → rotate right 5 degrees about Y axis
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)))
        {
            cubeArm.cubeRotate(r, glm::vec3(0.0f, 1.0f, 0.0f));
            cubeRacket.cubeRotate(r, glm::vec3(0.0f, 1.0f, 0.0f));
            r += 0.5f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)))
        {
            cubeArm.cubeRotate(-r, glm::vec3(0.0f, 1.0f, 0.0f));
            cubeRacket.cubeRotate(-r, glm::vec3(0.0f, 1.0f, 0.0f));
            r += 0.5f;
        }
        // key ‘U’ for scale-up and ‘J’ for scale-down
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) 
        { // scales by 0.01
            s += 0.01;
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) 
        { // scales by 0.01
            s -= 0.01;
        }

    }   
    
    // shutdown GLFW
    glfwTerminate();
    
	return 0;
}
