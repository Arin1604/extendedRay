#pragma once

#include "utils/rgba.h"
#include "utils/scenedata.h"
#include "utils/sceneparser.h"
#include "camera/camera.h"
#include "raytracer.h"
#include <map>

// A class representing a scene to be ray-traced

// Feel free to make your own design choices for RayTraceScene, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class RayTraceScene
{
public:
    RayTraceScene(int width, int height, const RenderData &metaData);

    int Width;
    int Height;
    RenderData MetaData;
    SceneCameraData camData;
    Camera cam;
    
    glm::vec4 traceRay(RayTracer::Ray  worldRay, bool doPrint, RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap) const;
    RayTracer::Ray convertRaySpace(RayTracer::Ray  worldRay, glm::mat4 p) const;
    RGBA getUpdatedPixel(RayTracer::Ray  worldRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap) const;
    static RayTracer::intersectInfo getIntersection(RenderShapeData shape, RayTracer::Ray ray);
    static RayTracer::surfaceStruct getminPos(std::vector<RayTracer::surfaceStruct> &t_StrucArray);
    bool blocked(SceneLightData light, glm::vec3 intersectPos, bool doPrint);
    static glm::vec4 phong(glm::vec3  position,
                                   glm::vec3  normal,
                                   glm::vec3  directionToCamera,
                                   SceneMaterial  material,
                                   std::vector<SceneLightData> lights,
                                   SceneGlobalData globalData,
                                   bool doPrint,
                                   RayTraceScene &scene, RGBA texture);
    static RGBA applyTexture(RayTracer::surfaceStruct closestObject, std::map<std::string, RayTracer::textureInfo>& textureMap);

    // The getter of the width of the scene
    const int& width() const;

    // The getter of the height of the scene
    const int& height() const;

    // The getter of the global data of the scene
    const SceneGlobalData& getGlobalData() const;

    // The getter of the shared pointer to the camera instance of the scene
    const Camera& getCamera() const;



};
