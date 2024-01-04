

#include <stdexcept>
#include "raytracescene.h"
#include "utils/sceneparser.h"
#include "raytracer.h"
#include<iostream>


/*!
    @param:
    *RayTracer::surfaceStruct closestObject: the surface information for the closest object to the camera

    @return:
    *RGBA: the correct pixel from the texture corresponding to the appropriate uv coordinate of the point of intersection

     @brief:
    * Uses the shapes UV mapping to determine correct output
    * Finds the appropriate pixel from the texture image for the point of intersection on the shape

*/
RGBA RayTraceScene::applyTexture(RayTracer::surfaceStruct closestObject, std::map<std::string, RayTracer::textureInfo>& textureMap){

    float u = closestObject.u;
    float v = closestObject.v;

    float m = closestObject.shape.primitive.material.textureMap.repeatU;
    float n = closestObject.shape.primitive.material.textureMap.repeatV;

    RayTracer::textureInfo textInf = textureMap[closestObject.shape.primitive.material.textureMap.filename];

    int w = textInf.width;
    int h = textInf.height;

    int c = (int)(floor(u*m*w)) % w;
    int r = (int)(floor((1-v)*n*h)) % h;

    return textInf.texture[(r * w) + c];
}
