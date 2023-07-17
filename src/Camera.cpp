#include "Camera.h"

Camera::Camera(int shaderProgram, int width, int height, glm::vec3 cameraPos, float fov, float near_plane, float far_plane) {
    Camera::width = width;
    Camera::height = height;
    Camera::cameraPos = cameraPos;

    Camera::lastXPos = cameraPos.x;
    Camera::lastYPos = cameraPos.y;

    Camera::fov = fov;
    Camera::near_plane = near_plane;
    Camera::far_plane = far_plane;

    Camera::shaderProgram = shaderProgram;
}

void Camera::updateCameraMatrix() {
    // initializes matrices
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, upVector);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width/ (float)height, near_plane, far_plane);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

// the user can pan and tilt the camera as follows: while right button is pressed → use mouse movement in x direction to pan; and
                                                // while middle button is pressed → use mouse movement in y direction to tilt.
// the user can zoom in and out of the scene - while left button is pressed → use mouse movement to move into/out of the scene. 
void Camera::procressCameraInputs(GLFWwindow *window) {
    float sensitivity = 0.01f;
    double xCursorPos;
    double yCursorPos;
    glfwGetCursorPos(window, &xCursorPos, &yCursorPos);

    float deltaX = xCursorPos - lastXPos;
    float deltaY = yCursorPos - lastYPos;

    Camera::lastXPos = xCursorPos;
    Camera::lastYPos = yCursorPos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) // if the right button is being pressed - x pan
    {   // pan the camera from right to left - lookAt has to be updated with the panning
        float x = (deltaX * sensitivity);
        if (x > 90.0f)
			x = 90.0f;
		if (x < -90.0f)
			x = -90.0f;
        Camera::cameraTarget.x = cameraTarget.x + (x);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) // if the middle button is being pressed - y tilt
    {   // tilt the camera face up and down - camera position remains
        // float camY = cameraPos.y - (deltaY * sensitivity); // inverse because y is inverses
        // constraints to stop flipping and aesthetics
        float y =  (deltaY * sensitivity);
        if (y > 90.0f)
			y = 90.0f;
		if (y < -90.0f)
			y = -90.0f;
        Camera::cameraTarget.y = cameraTarget.y - (y);
		// Camera::cameraPos = glm::vec3(cameraPos.x, camY, cameraPos.z);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) // if the left button is being pressed - zoom
    {   // zoom camera based on y axis changes fov - brings items closer to camera
        Camera::fov = fov + (deltaY);
        // constraints to stop flipping and aesthetics
		if (fov < 15.0f)
			Camera::fov = 15.0f;
		if (fov > 90.0f)
			Camera::fov = 90.0f;
    }
    float distance_from_target = cameraPos.z - cameraTarget.z;
    float r = 3.0f;
    // https://stackoverflow.com/questions/14121377/opengl-rotate-camera-around-center-of-scene
    // key left arrow → Rx, right arrow → R-x, up arrow → Ry, down arrow → R-y. 
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) 
    {   
        deltax += 0.01f;
        deltaz += 0.01f;
        Camera::cameraPos.x = r*sin(deltax);
        Camera::cameraPos.z = r*cos(deltaz);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
    {     
        deltax -= 0.01f;
        deltaz -= 0.01f;
        Camera::cameraPos.x = r*sin(deltax);
        Camera::cameraPos.z = r*cos(deltaz);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
    {   
        deltay += 0.01f;
        deltaz += 0.01f;
        Camera::cameraPos.y = r*sin(deltay);
        Camera::cameraPos.z = r*cos(deltaz);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
    {     
        deltay -= 0.01f;
        deltaz -= 0.01f;
        Camera::cameraPos.y  = r*sin(deltay);
        Camera::cameraPos.z = r*cos(deltaz);
    }
    // key 'H' reset to the initial world position and orientation / camera postion
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) 
    {     
        Camera::cameraPos = glm::vec3(0.0f, 0.5f, 3.0f);
        Camera::cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); // where the camera should be looking (centre)
        Camera::fov = 45.0f;
    }

}