#include <stdexcept>
#include "raytracescene.h"
#include "utils/sceneparser.h"
#include "raytracer.h"
#include<iostream>


/*!
 * //CONSTRUCTOR FOR RayTraceScene
    @param:
    *Camera cam: the camera object for the scene, populated with data from metaData
    *int width,height: dimensions of viewplane
    *Renderdata metaData: the scene's data obtained after parsing, passed in as reference for optimization


    @brief:
    * Class responsible for handling the bulk of the raytracing logic
    * Handles ray intersection with all the shapes in the scene
    * Computes the pixel color using the phong illumination model
    * Handles reflection using recursive raytracing


*/
RayTraceScene::RayTraceScene(int width, int height, const RenderData &metaData):
    cam(metaData.cameraData, width, height)
{
    MetaData = metaData;
    Width = width;
    Height = height;
    camData = metaData.cameraData;

}
///GETTERS

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
///
///


/*!
    @param:
    *RayTracer::Ray  worldRay: the ray in world space
    *glm::mat4 p: matrix to convert the ray from world space to a specific space (camera/object space)

    @return
    RayTracer::Ray: in the new transform space

    @brief:
    * In this project, the light ray is converted between different transform spaces (mainly object <-> world <-> camera)
    * This function carries out the matrix transformations


*/
RayTracer::Ray RayTraceScene::convertRaySpace(RayTracer::Ray  worldRay, glm::mat4 p) const{
    glm::vec4 newDir = p * worldRay.dir;
    glm::vec4 newPos = p * worldRay.pos;

    RayTracer::Ray newRay = RayTracer::Ray{newPos, newDir};
    return newRay;

}


/*!
    @param:
    glm::vec4 &illumination: receives the RGBA color as a vec4 of floats [0,1]

    @return
    RGBA: the RGBA values

    @brief:
    * Converts color values from floats to the RGBA values


*/
RGBA toRGBA(const glm::vec4 &illumination) {

    std::uint8_t r = 255 * fmin(fmax(illumination[0], 0),1);
    std::uint8_t g = 255 * fmin(fmax(illumination[1], 0),1);
    std::uint8_t b = 255 * fmin(fmax(illumination[2], 0),1);

    return RGBA{r, g, b};
}

glm::mat4 translator(float dx, float dy, float dz){
    return glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, dx, dy, dz, 1);
}


/*!
    @param:
    *RayTracer::Ray  worldRay: the ray in world space
    *bool doPrint: boolean for per pixel debugging, can be set to true for specific pixel locations for testing
    *RayTraceScene scene: the scene being rendered passed in as reference
    *int count: sets the maximu recursive depth for recursion
    *map textureMap: a map for the different shapes and their textures, pre-computed to increase efficiency

    @return
    glm::vec4: the final color of a pixel obtained after raytracing as floats

    @brief:
    * This is the main function that handles the ray-tracing
    * It loops through all the shapes in the scene and checks for intersection with the world ray that is passed in
    * Computes color determination using texture mapping and phong lighting
    * Handles reflection mechanics for reflective surfaces using recursion


*/
//NOTE ON "CLOSEST OBJECT": it refers to the object that is closest to the camera, and is hence visible

glm::vec4 RayTraceScene::traceRay(RayTracer::Ray  worldRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap) const {
    std::vector<RayTracer::surfaceStruct> t_vals;
    int c = count + 1;


    //MAIN SHAPE LOOP
    for(int i = 0; i < MetaData.shapes.size(); i++){
        RenderShapeData r = MetaData.shapes[i];
        glm::mat4 o = translator(0.f,0.f,0.2f) * r.ctm;
        glm::mat4 p = glm::inverse(o);// Coord transformation

        //glm::mat4 p = glm::inverse(MetaData.shapes[i].ctm);// Coord transformation
        RayTracer::Ray objectRay = convertRaySpace(worldRay, p);

        RayTracer::intersectInfo intInf = RayTraceScene::getIntersection(MetaData.shapes[i], objectRay);
        RayTracer::surfaceStruct objectProps = {intInf.t, intInf.position, intInf.normal, MetaData.shapes[i], intInf.u, intInf.v};



        t_vals.push_back(objectProps);
    }

    RayTracer::surfaceStruct closestObject = RayTraceScene::getminPos(t_vals);


    //Texture Mapping for Closest Object
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

        //Recursive raytracing if the shape's surface is reflective
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



        //Combination of the color obtained from:
        //directly incident ray
        //indirect rays from reflections
        return directIllumination + indirectIllum;
    }

    else{
        return glm::vec4(0.f, 0.f, 0.f, 0.f);
    }



}


///
/// Converts results from traceRay into an RGBA struct and returns it to the raytracer
///
RGBA RayTraceScene::getUpdatedPixel(RayTracer::Ray  worldRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap) const{
    return toRGBA(traceRay(worldRay, doPrint, scene, count, textureMap));



}


