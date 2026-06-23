#include <glad/glad.h> // must come before GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Shader.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "Camera.h"

static int WIDTH  = 1280;
static int HEIGHT = 720;

static void onResize(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
    WIDTH = w; HEIGHT = h;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onResize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed\n";
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/phong.vert", "shaders/phong.frag");
    auto mesh = loadObj("assets/cube.obj");
    if (!mesh) return -1;

    Camera camera(
        glm::vec3(3.0f, 2.5f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        45.0f,
        (float)WIDTH / HEIGHT
    );

    glm::vec3 lightPos   (3.0f, 5.0f, 3.0f);
    glm::vec3 lightColor (1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(0.4f, 0.6f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float t = (float)glfwGetTime();
        glm::mat4 model = glm::rotate(glm::mat4(1.0f),
                                      t * glm::radians(30.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f));

        // inverse-transpose keeps normals correct under non-uniform scale
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

        camera.aspect = (float)WIDTH / HEIGHT;

        shader.use();
        shader.setMat4("model",        model);
        shader.setMat4("view",         camera.view());
        shader.setMat4("projection",   camera.projection());
        shader.setMat3("normalMatrix", normalMatrix);
        shader.setVec3("lightPos",     lightPos);
        shader.setVec3("lightColor",   lightColor);
        shader.setVec3("objectColor",  objectColor);
        shader.setVec3("viewPos",      camera.position);

        mesh->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
