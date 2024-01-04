#pragma once

#include "utils/scenedata.h"
#include <glm/glm.hpp>

// A class representing a virtual camera.


class Camera {
public:
    // Returns the view matrix for the current camera settings.

    Camera(const SceneCameraData &cameraData, int width, int height);
    glm::mat4 getViewMatrix() const;
    glm::mat4 getInverseViewMatrix() const;
    float Width;
    float Height;
    glm::vec4 pos;
    glm::vec4 look;
    glm::vec4 up;
    float heightAngle; // The height angle of the camera in RADIANS

    float aperture;
    float focalLength;
    glm::mat4 viewMat;
    glm::mat4 invMat;

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    // Returns the focal length of this camera.

    float getFocalLength() const;

    // Returns the focal length of this camera.
     float getAperture() const;
};
