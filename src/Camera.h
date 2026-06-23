#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 target;
    float fovDegrees;
    float aspect;
    float nearPlane;
    float farPlane;

    Camera(glm::vec3 pos, glm::vec3 tgt, float fov, float aspect,
           float nearPlane = 0.1f, float farPlane = 100.0f)
        : position(pos), target(tgt), fovDegrees(fov), aspect(aspect),
          nearPlane(nearPlane), farPlane(farPlane) {}

    glm::mat4 view() const {
        return glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 projection() const {
        return glm::perspective(glm::radians(fovDegrees), aspect, nearPlane, farPlane);
    }
};
