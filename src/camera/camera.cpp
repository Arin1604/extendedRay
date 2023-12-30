#include <stdexcept>
#include "camera.h"
#include "utils/scenedata.h"


glm::mat4 ViewMatrix(glm::vec3 pos, glm::vec3 look, glm::vec3 up) {
    glm::vec3 posLocal = pos;
    glm::vec3 lookLocal = look;
    glm::vec3 upLocal = up;

    glm::vec4 tcol0(1, 0, 0, 0);
    glm::vec4 tcol1(0, 1, 0, 0);
    glm::vec4 tcol2(0, 0, 1, 0);
    glm::vec4 tcol3(-posLocal[0], -posLocal[1], -posLocal[2], 1);

    glm::mat4 translate(tcol0, tcol1, tcol2, tcol3);

    glm::vec3 w = -glm::normalize(lookLocal);
    glm::vec3 v = glm::normalize(upLocal - (glm::dot(upLocal, w) * w));
    glm::vec3 u = glm::cross(v, w);

    glm::mat4 rotate(u[0], v[0], w[0], 0, u[1], v[1], w[1], 0, u[2], v[2], w[2], 0, 0, 0, 0, 1);
    return rotate * translate;

    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

glm::mat4 inverseMat(glm::mat4 mat){
    return glm::inverse(mat);

}

Camera::Camera(const SceneCameraData &cameraData, int width, int height) {
    Width = width;
    Height = height;
    pos = cameraData.pos;
    look = cameraData.look;
    up = cameraData.up;
    focalLength = cameraData.focalLength;
    aperture = cameraData.aperture;
    heightAngle = cameraData.heightAngle;
    viewMat = ViewMatrix(pos, look, up);
    invMat = inverseMat(viewMat);

};




glm::mat4 Camera::getViewMatrix() const {
    return viewMat;
}

glm::mat4 Camera::getInverseViewMatrix() const {
    return invMat;
}

float Camera::getAspectRatio() const {
    int n = Width;
    int d = Height;
    return (float) n/ (float) d;

    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

float Camera::getHeightAngle() const {
    // Optional TODO: implement the getter or make your own design
    return heightAngle;
    throw std::runtime_error("not implemented");
}

float Camera::getFocalLength() const {
    return focalLength;
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

float Camera::getAperture() const {
    return aperture;
    throw std::runtime_error("not implemented");
}
