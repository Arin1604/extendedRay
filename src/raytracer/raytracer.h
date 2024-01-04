#pragma once

#include <glm/glm.hpp>
#include <map>
#include "utils/rgba.h"
#include "utils/sceneparser.h"

/// A forward declaration for the RaytraceScene class

class RayTraceScene;

/// A class representing a ray-tracer

class RayTracer
{
public:

    bool dofEnable = true;
    int sample = 6;

    struct Config {
        bool enableShadow        = false;
        bool enableReflection    = false;
        bool enableRefraction    = false;
        bool enableTextureMap    = false;
        bool enableTextureFilter = false;
        bool enableParallelism   = false;
        bool enableSuperSample   = false;
        bool enableAcceleration  = false;
        bool enableDepthOfField  = false;
        int maxRecursiveDepth    = 4;
        float apperture = 0.16f;
        float focalLength = 13.f;
        float planeZ = 1.f;
        bool onlyRenderNormals   = false;
    };

    ///
    /// \brief The Ray struct represents light as a mathematical object
    ///
    struct Ray {
        glm::vec4 pos;
        glm::vec4 dir;

    };

    ///
    /// \brief The intersectInfo struct wraps important information regarding a shape-light intersection
    ///
    struct intersectInfo {
        float t;
        glm::vec4 normal;
        glm::vec4 position;
        float u;
        float v;

    };

    ///
    /// \brief The textureInfo struct wraps information regarding a shape's texture
    ///
    struct textureInfo {
        int width;
        int height;
        RGBA* texture;
    };

    ///
    /// \brief The surfaceStruct captures additional information regarding a shape-light intersection for lighting calculations
    ///
    struct surfaceStruct {
        float t;
        glm::vec4 normal;
        glm::vec4 pos;
        RenderShapeData shape;
        float u;
        float v;


    };

public:
    RayTracer(Config config);

    // Renders the scene synchronously.
    // The ray-tracer will render the scene and fill imageData in-place.
    // @param imageData The pointer to the imageData to be filled.
    // @param scene The scene to be rendered.
    void render(RGBA *imageData, const RayTraceScene &scene);
    glm::vec4 lensOffset();
    RGBA lensMaker(RayTracer::Ray  worldRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo> &textureMap);

    void DOF(RGBA *imageData, const RayTraceScene &scene);

private:
    const Config m_config;
};

