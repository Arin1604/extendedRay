#include <stdexcept>
#include "raytracescene.h"
#include "utils/sceneparser.h"
#include "raytracer.h"
#include<iostream>
#include "raytracer.h"
#include "glm/gtx/string_cast.hpp"
#include "qimage.h"
#include "raytracescene.h"
#include <iostream>
#include <map>


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

glm::vec3 locComputer(int identifier, float radius, int incrementer){
    float angle1 = (3 *M_PI/2) + (0.f/240.f) * M_PI;
    float angle;


    ///
    /// \brief frameCoeff
    /// // DETERMINES THE NUMBER OF FRAMES FOR WHICH THE ANIMATION RUNS
    ///
    /// \cond set to 1.f for 240 frames, scale appropriately for higer/lower frames
    ///
    float frameCoeff = 2.f;

    float offset= M_PI/16.f;

    float x;
    float y;
    float z;


    if(identifier == 0){
        z = radius * cos(angle1);
        x = radius * sin(angle1);
        y = (0.25* sin(2 * angle1)) + (0.75f*cos(0.7f * angle1));

        //return glm::vec3(x, y, z );
    }

    else if(identifier == 1){
        float angle = (0 + M_PI/5.5) - (incrementer/(frameCoeff *960.f)) * M_PI;
        float a = -15.524f;
        float b = -15.524f;
        radius = 15.0 * a * cos(angle) + 15.0 * b * sin(angle);

        z =  radius * cos(angle) + 80.f;
        y = ((radius/14)* sin(6 * angle)); /*((radius/10)*cos(0.7f * angle));*/
        x =  radius * sin(angle) - 0.f;

        // return glm::vec3(x, y, z );

    }

    else if(identifier == 2){
        float angle = (0 - M_PI/10.0) + (incrementer/(frameCoeff *960.f)) * M_PI;
        float a = -15.524f;
        float b = -15.524f;
        radius = 30.0 * a * cos(angle) + 30.0 * b * sin(angle);
        //    radius =  radius * sin(6 * angle);
        //    z =  radius * cos(angle);
        //    x = 0.f;
        //    y =  radius * sin(angle);
        z =  radius * cos(angle) + 80.f;
        y = ((radius/14)* sin(6 * angle)); /*((radius/10)*cos(0.7f * angle));*/
        x =  radius * sin(angle) - 0.f;

        //return glm::vec3(x, y, z );

    }

    else if(identifier == 3){
        float angle = (-incrementer/(frameCoeff * 120.f)) * M_PI + (3 * M_PI/2) ;
        radius = 0.5 * (7 + 23 * sin(angle + M_PI));
        z =   radius * sin(angle) + 30.f;
        x = radius * cos(angle) - 1.f;
        y = 0.f;
    }

    else if(identifier == 4){
        float angle = ((-incrementer/(frameCoeff * 120.f)) * M_PI + (3 * M_PI/2) )+ offset;
        radius = 0.5 * (7 + 23 * sin(angle + M_PI));
        z =   radius * sin(angle) + 30.f;
        x = radius * cos(angle) - 1.f;
        y = 0.f;
    }

    else if(identifier == 5){
        float angle = ((-incrementer/(frameCoeff * 120.f)) * M_PI + (3 * M_PI/2) )+ (2 *offset);
        radius = 0.5 * (7 + 23 * sin(angle + M_PI));
        z =   radius * sin(angle) + 30.f;
        x = radius * cos(angle) - 1.f;
        y = 0.f;
    }

    else if(identifier == 6){
        float angle = ((-incrementer/(frameCoeff * 120.f)) * M_PI + (3 * M_PI/2) )- (1 *offset);
        radius = 0.5 * (7 + 23 * sin(angle + M_PI));
        z =   radius * sin(angle) + 30.f;
        x = radius * cos(angle) - 1.f;
        y = 0.f;
    }

    else if(identifier == 7){
        float angle = ((-incrementer/(frameCoeff * 120.f)) * M_PI + (3 * M_PI/2) )- (2 *offset);
        radius = 0.5 * (7 + 23 * sin(angle + M_PI));
        z =   radius * sin(angle) + 30.f;
        x = radius * cos(angle) - 1.f;
        y = 0.f;
    }



    else{
        z =  0.f;
        x = 0.f;
        y =  0.f;
    }

    return glm::vec3(x,y,z);

}

glm::vec3 locComputer2(int identifier, float radius, int incrementer){
    float angle1 = (3 *M_PI/2) + (0.f/240.f) * M_PI;
    float angle;


    ///
    /// \brief frameCoeff
    /// // DETERMINES THE NUMBER OF FRAMES FOR WHICH THE ANIMATION RUNS
    ///
    /// \cond set to 1.f for 240 frames, scale appropriately for higer/lower frames
    ///
    float frameCoeff = 1.f;

    float offset= M_PI/16.f;

    float x;
    float y;
    float z;


    if(identifier == 3){
        angle = (-0/(frameCoeff *480.f)) * M_PI + (M_PI/6);

        radius =  5 * cos((3) * angle);
        z =  0.f;
        x = radius * sin(angle);
        y =  radius * cos(angle);


    }

    else if(identifier == 0){
        angle = (-incrementer/(frameCoeff *480.f)) * M_PI + (M_PI/2);

        radius =  5 * cos((3) * angle);
        z =  0.f;
        x = radius * sin(angle);
        y =  radius * cos(angle);

    }



    else if(identifier == 1){
        angle = (-incrementer/(frameCoeff *480.f)) * M_PI + (M_PI/6);

        radius =  5 * cos((3) * angle);
        z =  0.f;
        x = radius * sin(angle);
        y =  radius * cos(angle);

    }



    else if(identifier == 2){
        angle = 2.609 + ((-incrementer/(frameCoeff *480.f)) * M_PI);

        radius =  5 * cos((3) * angle);
        z =  0.f;
        x = radius * sin(angle);
        y =  radius * cos(angle);

    }



    return glm::vec3(x,y,z);

}

glm::mat4 translator(glm::vec3 translate){
    return glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, translate[0], translate[1], translate[2], 1);
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

glm::vec4 RayTraceScene::traceRay(RayTracer::Ray  worldRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap,int translate) const {
    std::vector<RayTracer::surfaceStruct> t_vals;
    int c = count + 1;





    //MAIN SHAPE LOOP
    for(int i = 0; i < MetaData.shapes.size(); i++){
        RenderShapeData r = MetaData.shapes[i];

        //LOPSIDES
        r.ctm = translator(locComputer2(i,1, translate)) * r.ctm;
        //r.primitive.material.shininess = 2.f;
        glm::mat4 p = glm::inverse(r.ctm);// Coord transformation

        //glm::mat4 p = glm::inverse(MetaData.shapes[i].ctm);// Coord transformation
        RayTracer::Ray objectRay = convertRaySpace(worldRay, p);

        RayTracer::intersectInfo intInf = RayTraceScene::getIntersection(MetaData.shapes[i], objectRay);
        RayTracer::surfaceStruct objectProps = {intInf.t, intInf.position, intInf.normal, MetaData.shapes[i], intInf.u, intInf.v, i};



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

        //animate shininess here
        if(closestObject.i == 1){
        closestObject.shape.primitive.material.shininess *= abs(7.5f * sin(0.5f * translate/480.f * M_PI));;
        }

        if(closestObject.i == 2){
        //closestObject.shape.primitive.material.shininess = abs(7.5f * cos(0.5f * translate/480.f * M_PI));

        }



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

            indirectIllum = traceRay(reflectedRay, false, scene, c, textureMap,translate);
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
RGBA RayTraceScene::getUpdatedPixel(RayTracer::Ray  worldRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap, int translate) const{
    return toRGBA(traceRay(worldRay, doPrint, scene, count, textureMap, translate));



}


