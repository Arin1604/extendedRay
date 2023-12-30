#pragma once

#include "utils/scenedata.h"
#include <glm/glm.hpp>

// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
public:
    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.
    Camera(const SceneCameraData &cameraData, int width, int height);
    glm::mat4 getViewMatrix() const;
    glm::mat4 getInverseViewMatrix() const;
    float Width;
    float Height;
    glm::vec4 pos;
    glm::vec4 look;
    glm::vec4 up;
    float heightAngle; // The height angle of the camera in RADIANS

    float aperture;    // Only applicable for depth of field
    float focalLength;
    glm::mat4 viewMat;
    glm::mat4 invMat;

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture() const;
};
