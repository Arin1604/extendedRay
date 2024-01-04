#include <stdexcept>
#include "camera.h"
#include "utils/scenedata.h"


/*!
    @param:
    *glm::vec3 up, pos, and look: three vectors that determine the camera's orientation

    @return
    glm::mat4: the view matrix that determines camera space conversions and the view volume for our scene

    @brief:
    * This function computes the view matrix for our camera that determines the camera space conversions and the viewing volume for our scene
*/
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

    throw std::runtime_error("not implemented");
}

//Computes the inverse of the view matrix for converting back to world space
glm::mat4 inverseMat(glm::mat4 mat){
    return glm::inverse(mat);

}

/*!
 ///CONSTRUCTOR FOR CAMERA CLASS
    @param:
    *SceneCameraData cameraData: The camera data obtained after parsing the scene, passed in as reference to optimize efficiency
    *int width, height: the dimensions of the viewplane

    @brief:
    * The virtual representation of a camera in our scene
*/
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



//GETTERS
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

    throw std::runtime_error("not implemented");
}

float Camera::getHeightAngle() const {
     return heightAngle;
    throw std::runtime_error("not implemented");
}

float Camera::getFocalLength() const {
    return focalLength;
    throw std::runtime_error("not implemented");
}

float Camera::getAperture() const {
    return aperture;
    throw std::runtime_error("not implemented");
}
