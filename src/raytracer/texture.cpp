

#include <stdexcept>
#include "raytracescene.h"
#include "utils/sceneparser.h"
#include "raytracer.h"
#include<iostream>

RGBA RayTraceScene::applyTexture(RayTracer::surfaceStruct closestObject, std::map<std::string, RayTracer::textureInfo>& textureMap){

    float u = closestObject.u;
    float v = closestObject.v;
    //std::cout << u << "   " << v  << std::endl;

    float m = closestObject.shape.primitive.material.textureMap.repeatU;
    float n = closestObject.shape.primitive.material.textureMap.repeatV;
    //std::cout << "two" << std::endl;
    RayTracer::textureInfo textInf = textureMap[closestObject.shape.primitive.material.textureMap.filename];
    //std::cout << "three" << std::endl;
    int w = textInf.width;
    int h = textInf.height;

    int c = (int)(floor(u*m*w)) % w;
    int r = (int)(floor((1-v)*n*h)) % h;

    return textInf.texture[(r * w) + c];
}
