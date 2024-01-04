#pragma once

#include <glm/glm.hpp>
#include <map>
#include "utils/rgba.h"
#include "utils/sceneparser.h"

// A forward declaration for the RaytraceScene class

class RayTraceScene;

// A class representing a ray-tracer

class RayTracer
{
public:
//    float focalLength = 40.f;
    //float apperture = 0.09f;
//    float planeZ = 1.7f;
    bool dofEnable = false;
    int sample = 6;
    //PUT THIS IN CONFIG TO GET ANIMATIONS

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
        float apperture = 0.2f;
        float focalLength = 0.f;
        float planeZ = 1.f;
        bool onlyRenderNormals   = false;
    };

    struct Ray {
        glm::vec4 pos;
        glm::vec4 dir;

    };

    struct intersectInfo {
        float t;
        glm::vec4 normal;
        glm::vec4 position;
        float u;
        float v;

    };

    struct textureInfo {
        int width;
        int height;
        RGBA* texture;
    };

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

