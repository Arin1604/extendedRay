#include <stdexcept>
#include "raytracescene.h"
#include "utils/sceneparser.h"
#include "raytracer.h"
#include<iostream>
#include "glm/gtx/string_cast.hpp"

RayTraceScene::RayTraceScene(int width, int height, const RenderData &metaData):
    cam(metaData.cameraData, width, height)
{
    MetaData = metaData;
    Width = width;
    Height = height;
    camData = metaData.cameraData;



    // Optional TODO: implement this. Store whatever you feel is necessary.
}

const int& RayTraceScene::width() const {
    return Width;
    throw std::runtime_error("not implemented");
}

const int& RayTraceScene::height() const {
    return Height;
    throw std::runtime_error("not implemented");
}

const SceneGlobalData& RayTraceScene::getGlobalData() const {
    return MetaData.globalData;
    throw std::runtime_error("not implemented");
}

const Camera& RayTraceScene::getCamera() const {

    return cam;


    throw std::runtime_error("not implemented");
}

RayTracer::Ray RayTraceScene::convertRaySpace(RayTracer::Ray  worldRay, glm::mat4 p) const{
    glm::vec4 newDir = p * worldRay.dir;
    glm::vec4 newPos = p * worldRay.pos;

    RayTracer::Ray newRay = RayTracer::Ray{newPos, newDir};
    return newRay;

}

RGBA toRGBA(const glm::vec4 &illumination) {

    std::uint8_t r = 255 * fmin(fmax(illumination[0], 0),1);
    std::uint8_t g = 255 * fmin(fmax(illumination[1], 0),1);
    std::uint8_t b = 255 * fmin(fmax(illumination[2], 0),1);

    return RGBA{r, g, b};
}



glm::vec4 RayTraceScene::traceRay(RayTracer::Ray  worldRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap) const {
    std::vector<RayTracer::surfaceStruct> t_vals;
    int c = count + 1;


    for(int i = 0; i < MetaData.shapes.size(); i++){
        glm::mat4 p = glm::inverse(MetaData.shapes[i].ctm);// Coord transformation
        RayTracer::Ray objectRay = convertRaySpace(worldRay, p);

        RayTracer::intersectInfo intInf = RayTraceScene::getIntersection(MetaData.shapes[i], objectRay);
        //std::cout << intInf.normal[0] << "," << intInf.normal[1] << "," << intInf.normal[2] << std::endl;
        RayTracer::surfaceStruct objectProps = {intInf.t, intInf.position, intInf.normal, MetaData.shapes[i], intInf.u, intInf.v};



        t_vals.push_back(objectProps);
    }

    RayTracer::surfaceStruct closestObject = RayTraceScene::getminPos(t_vals);
    // std::cout << closestObject.u << closestObject.v  << std::endl;


    RGBA texture{0,0,0,1};
    if(closestObject.t > 0){
        if(closestObject.shape.primitive.material.textureMap.isUsed){
        texture = RayTraceScene::applyTexture(closestObject, textureMap);
        }

        glm::mat3 ctm = glm::mat3(closestObject.shape.ctm);
        //std::cout << closestObject.normal[0] << "," << closestObject.normal[1] << "," << closestObject.normal[2] << std::endl;
        glm::vec4 normal = glm::vec4(glm::inverse(glm::transpose(ctm)) * glm::vec3(closestObject.normal), 0.f);
        glm::vec3 pos = closestObject.shape.ctm * closestObject.pos;
        glm::vec4 directIllumination = RayTraceScene::phong(pos, normal, - worldRay.dir, closestObject.shape.primitive.material, MetaData.lights, getGlobalData(), doPrint, scene, texture);
        glm::vec4 indirectIllum(0.f);

        if(c < 5 && closestObject.shape.primitive.material.cReflective != glm::vec4(0.f)){
            glm::vec4 posReflected(pos, 1.f);

            glm::vec4 dirIncident = glm::normalize(worldRay.dir);
            glm::vec4 normNorm = glm::normalize(normal);
            glm::vec4 normalFactor = 2.0f * normNorm * glm::dot(normNorm, dirIncident);
            glm::vec4 dirReflected = dirIncident - normalFactor;
            RayTracer::Ray reflectedRay{posReflected + 0.001f * dirReflected, dirReflected};

            indirectIllum = traceRay(reflectedRay, false, scene, c, textureMap);
            indirectIllum[0] *= getGlobalData().ks * closestObject.shape.primitive.material.cReflective[0];
            indirectIllum[1] *=  getGlobalData().ks *closestObject.shape.primitive.material.cReflective[1];
            indirectIllum[2] *= getGlobalData().ks * closestObject.shape.primitive.material.cReflective[2];
        }



        return directIllumination + indirectIllum;//phong(closestObject.shape.ctm * closestObject.pos, normal, - worldRay.dir, closestObject.shape.primitive.material, MetaData.lights, getGlobalData(), doPrint, scene, count);
    }

    else{
        return glm::vec4(0.f, 0.f, 0.f, 0.f);
    }



}

RGBA RayTraceScene::getUpdatedPixel(RayTracer::Ray  worldRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap) const{
    return toRGBA(traceRay(worldRay, doPrint, scene, count, textureMap));



}


