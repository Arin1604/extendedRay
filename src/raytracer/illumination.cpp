#include <stdexcept>
#include "raytracescene.h"
#include "utils/sceneparser.h"
#include "raytracer.h"
#include<iostream>
#include "glm/gtx/string_cast.hpp"


/*!
    @param:
    *SceneLightData light: The light for which color is being determined
    *glm::vec3 intersectPos: the intersect position
    *bool doPrint: debugging boolen for printing specific pixel values

    @return
    bool: whether a shape is shadowed by another primitive

    @brief:
    * This function implements the shadow effect by generating a reverse "shadow ray" to check if the shape is occluded by another

*/
bool RayTraceScene::blocked(SceneLightData light, glm::vec3 intersectPos, bool doPrint){
    float t = -1.f;
    glm::vec4 dirShadowRay;


    if(light.type == LightType::LIGHT_DIRECTIONAL){
        dirShadowRay = glm::normalize(-light.dir);
    }
    else{
        dirShadowRay = glm::normalize(light.pos - glm::vec4(intersectPos, 1.f));
    }

    RayTracer::Ray shadowRay = RayTracer::Ray{glm::vec4(intersectPos, 1.f) + 0.01f * dirShadowRay , dirShadowRay};

    for(int i = 0; i < MetaData.shapes.size(); i++){

        bool shapeChecker = MetaData.shapes[i].primitive.type == PrimitiveType::PRIMITIVE_CONE;
        glm::mat4 p = glm::inverse(MetaData.shapes[i].ctm);// remove inverse calculations
        RayTracer::Ray objectRay = convertRaySpace(shadowRay, p);

        RayTracer::intersectInfo intInf = RayTraceScene::getIntersection(MetaData.shapes[i], objectRay);

        if(doPrint && shapeChecker){
            std::cout << "T value is :" << intInf.t << " " << "|Shape Intersection: "<< glm::to_string(intInf.position) << "|Direction of Ray: " << glm::to_string(shadowRay.dir) <<std::endl;
        }

        float lightDist = glm::distance(glm::vec4(intersectPos, 1.0f), light.pos);
        float shadowIntersectDist = glm::distance(glm::vec4(intersectPos, 1.0f), intInf.position);



        //Checks if light is either pos/spot so that we ca check the edge case
        if(light.type != LightType::LIGHT_DIRECTIONAL){
            if(intInf.t > 0.f && intInf.t < glm::length(glm::vec3(light.pos) - intersectPos)){ /*&& (intInf.position[1] > light.pos[1] && intInf.position[1] < light.pos[1])*/
                t = intInf.t;//is blocked
                if(light.type == LightType::LIGHT_DIRECTIONAL){
                    t = -1.0f;
                }
                break;

            }



            else{
                t = -1.0f;//not blocked
            }
        }

        // case when light is directional and we don't need to worry about the special case(because directionalLight.pos = INF).
        else{

            if(intInf.t > 0.f){
                t = intInf.t;//is blocked
                break;

            }

            else{
                continue;//not blocked
            }

        }
    }



    if(t < 0){
        return false;
    }

    else{
        return true;
    }

}

/*!
    @param:
    *glm::vec3  position: the position of intersection
    *glm::vec3  normal: the normal to the surface at the point of intersection
    *glm::vec3  directionToCamera: the direction from point of intersection to camera
    *SceneMaterial  material: the shape's material
    *std::vector<SceneLightData> lights: the array of lights in the scene
    *SceneGlobalData globalData: Scene's global data
    *bool doPrint: debugging boolean for per pixel debugging
    *RayTraceScene &scene: scene being rendered
    * RGBA texture: texture's pixel

    @return
    glm::vec4: the final output color after the Phong Lighting Computation

    @brief:
    * This function implements the famous Phong Lighting Equation
    * It computes the final color output by factoring the scene, the lights, and the shape's properties
    * Supports spot, directional and point light sources
    * In essence, three separate lighting terms are computed that represent the ambient, diffuse and specular lighting that a shape receives
*/
glm::vec4 RayTraceScene::phong(glm::vec3  position,
                glm::vec3  normal,
                glm::vec3  directionToCamera,
                SceneMaterial  material,
                std::vector<SceneLightData> lights,
                SceneGlobalData globalData,
                bool doPrint,
                RayTraceScene &scene, RGBA texture) {
    // Normalizing directions
    normal            = glm::normalize(normal);
    directionToCamera = glm::normalize(directionToCamera);
    //int count = depth;
    // Output illumination (we can ignore opacity)
    glm::vec4 illumination(0, 0, 0, 1);
    float r = (float)texture.r/255.0f;
    float g = (float)texture.g/255.0f;
    float b = (float)texture.b/255.0f;

    glm::vec4 textureColors(r,g,b,1.0f);

    // add the ambient term
    illumination[0] = globalData.ka * material.cAmbient[0];
    illumination[1] = globalData.ka * material.cAmbient[1];
    illumination[2] = globalData.ka * material.cAmbient[2];




    float redAcc = 0;
    float greenAcc = 0;
    float blueAcc = 0;

    glm::vec3 origin(0.f,0.f,0.f);

    float fatt = 1.f;
    glm::vec3 li;
    float fallOff = 1.f;

    for (const SceneLightData &light : lights) {
        if(scene.blocked(light, position, doPrint)){
            continue;
        }

        else{

            if (light.type == LightType::LIGHT_SPOT) {
                float dist = glm::distance(position, glm::vec3(light.pos));
                float term = 1.f/(light.function[0] + dist * light.function[1]  + pow(dist, 2) * light.function[2] );
                fatt = fmin(1.f, term);
                li = glm::normalize(light.pos - glm::vec4(position, 0.f));

                float outerAngle = light.angle; // inner
                float innerAngle = light.angle - light.penumbra;

                glm::vec3 dir = glm::normalize(light.dir);
                glm::vec3 poi = glm::normalize(position - glm::vec3(light.pos));
                float x = acos(glm::dot(dir, poi));
                float nom = x - innerAngle;
                float denom = outerAngle - innerAngle;
                float res = -2 * pow((nom/denom), 3) + 3 * pow((nom/denom), 2);




                if(x <= innerAngle){
                    fallOff = 1.f;
                }

                else if(x > innerAngle && x <= outerAngle){
                    fallOff = 1 - res;

                }

                else{
                    fallOff = 0;
                }






            }

            else if(light.type == LightType::LIGHT_DIRECTIONAL){
                li = glm::normalize(-light.dir);


            }

            else if(light.type == LightType::LIGHT_POINT){
                float dist = glm::distance(position, glm::vec3(light.pos));

                float term = 1.f/(light.function[0] + dist * light.function[1]  + pow(dist, 2) * light.function[2] );
                fatt = fmin(1.f, term);


                li = glm::normalize(light.pos - glm::vec4(position, 0.f));

            }

            float redInten = light.color[0];
            float greenInten = light.color[1];
            float blueInten = light.color[2];

            float blend = material.blend;

            float KdObr = (globalData.kd * material.cDiffuse[0] * (1 - blend)) + (textureColors[0] * blend) ; // * add r from texture weigh using blend and (1-blend) of reg
            float KdObg = (globalData.kd * material.cDiffuse[1] * (1 - blend)) + (textureColors[1] * blend);
            float KdObb = (globalData.kd * material.cDiffuse[2] * (1 - blend)) + (textureColors[2] * blend);

            //        glm::vec3 li = glm::normalize(-light.dir);
            auto dot = glm::dot(normal, li);
            if(dot < 0){
                dot = 0;
            }



            float redVal1 = fatt * redInten * KdObr * dot;
            float greenVal1 = fatt * greenInten * KdObg * dot;
            float blueVal1 = fatt * blueInten * KdObb * dot;

            float KsOsr = globalData.ks * material.cSpecular[0];
            float KsOsg = globalData.ks * material.cSpecular[1];
            float KsOsb = globalData.ks * material.cSpecular[2];

            glm::vec3 reflected = glm::reflect(-li, normal);/*-directionToCamera - (2.f * glm::dot(normal, directionToCamera) * normal);*/
            auto dotspec = glm::dot(glm::normalize(reflected), directionToCamera);
            if(dotspec < 0){
                dotspec = 0;
            }
            auto dotSpec = pow(dotspec, material.shininess);


            float redVal2 = fatt * redInten * KsOsr * dotSpec;
            float greenVal2 = fatt * greenInten * KsOsg * dotSpec;
            float blueVal2 = fatt * blueInten * KsOsb * dotSpec;

            float redVal = redVal1  + redVal2;
            float greenVal = greenVal1  + greenVal2;
            float blueVal = blueVal1 +  blueVal2;


            redAcc += fallOff * redVal;
            greenAcc +=fallOff * greenVal;
            blueAcc += fallOff *blueVal;
        }
    }

    illumination[0] +=  redAcc;
    illumination[1] +=  greenAcc;
    illumination[2] +=  blueAcc;

    return illumination;
}
