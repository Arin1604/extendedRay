#include "raytracer.h"
#include "glm/gtx/string_cast.hpp"
#include "qimage.h"
#include "raytracescene.h"
#include <iostream>
#include <map>

using namespace std;

///
/// \brief RayTracer::RayTracer sets up the configuration for the RayTracer
/// \param config: struct contains the configuration parameters for the raytracer
///
RayTracer::RayTracer(Config config) :
    m_config(config)
{}

/*!
    @param:
    t_array: the array of various t values which correspond to the shape's intersection with a light ray

    @return
    float: returns the minimum positive value

    @brief:
    * computes the minimum positive value to determine closest(minimum) valid(positive) intersection


*/
float getminPos(std::vector<float> t_array){
    float t_min_pos = t_array[0];
    for(float t: t_array){
        if(t >= 0.0f && (t < t_min_pos || t_min_pos < 0.0f)){
            t_min_pos = t;
        }
    }

    return t_min_pos;
}

//TESTS getMinPos
void testMinPos(){
    auto y = std::vector{ -1.0f, 2.5f, -3.0f, 6.0f, 1.0f, -8.0f};
    auto z = std::vector{ 1.0f, 0.0f, -1.0f, -2.0f, -3.0f, 2.0f};
    auto x = std::vector{-2.5f, 3.0f, -6.0f, -8.0f};
}

/*!
    @param:
    *QString file: the filepath for the texture

    @return
     RayTracer::textureInfo: returns the information struct for the texture

    @brief:
    * loads the texture and stores its information in a struct for raytracing later

*/
RayTracer::textureInfo loadTextureFromFile(const QString &file) {
    QImage myTexture;

    int width; int height;
    if (!myTexture.load(file)) {
        std::cout<<"Failed to load in image: " << file.toStdString() << std::endl;
        //        return nullptr;
    }
    myTexture = myTexture.convertToFormat(QImage::Format_RGBX8888);
    width = myTexture.width();
    height = myTexture.height();

    RGBA* texture = new RGBA[width*height];
    QByteArray arr = QByteArray::fromRawData((const char*) myTexture.bits(), myTexture.sizeInBytes());

    for (int i = 0; i < arr.size() / 4.f; i++){
        texture[i] = RGBA{(std::uint8_t) arr[4*i], (std::uint8_t) arr[4*i+1], (std::uint8_t) arr[4*i+2], (std::uint8_t) arr[4*i+3]};
    }

    return RayTracer::textureInfo{width, height, texture};
}



/*!
    @param:
    RGBA imageData: Pointer to the first pixel of the viewing plane through which the ray is traced
    RayTraceScene: The scene which is being rendered


    @brief:
    * The main rendering function that carries out the ray generation logic
    * Generates and traces (delegated to raytracescene) light rays through the pixels of the view plane
    * Updates the view plane's color with the output received from RayTraceScene
    * Populates the texture map for each shape in the scene to boost efficiency
    * Added support for Depth of Field and More realistic camera mechanics

*/
void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {

    if(!dofEnable){


        map<string, RayTracer::textureInfo> textureMap;
        if(m_config.enableTextureMap){
            // map<string, RayTracer::textureInfo> textureMap;

            for(int i = 0; i < scene.MetaData.shapes.size(); i++){
                if(scene.MetaData.shapes[i].primitive.material.textureMap.isUsed){
                    string fileName = scene.MetaData.shapes[i].primitive.material.textureMap.filename;
                    RayTracer::textureInfo textureInf = loadTextureFromFile(QString::fromStdString(fileName));
                    textureMap[fileName] = textureInf;
                }
            }
        }
        std::cout << "one" << std::endl;

        auto p = scene.getCamera().getViewMatrix() * scene.getCamera().getInverseViewMatrix();



        //Inverse works
        bool doPrint = false;
        testMinPos();
        for(int j = 0; j < scene.height(); j++){
            for(int i = 0; i < scene.width(); i++){
                float k = 1.f;
                float x = ((i + 0.5)/(float)scene.width()) - 0.5;
                float y = (((float)scene.height() - 1 - j + 0.5)/(float)scene.height()) - 0.5;
                float z = -k;

                float V = 2*k* tan(scene.getCamera().getHeightAngle()/2.f);
                float U = V * scene.getCamera().getAspectRatio();

                glm::vec4 uvk(U*x, V*y, z, 1.f);
                glm::vec4 eye(0.f,0.f,0.f, 1.f);

                glm::vec4 dir = glm::normalize(uvk - eye);
                Ray ray = Ray{eye, dir};
                Ray worldRay = scene.convertRaySpace(ray, scene.getCamera().getInverseViewMatrix());//Ray gets converted to world space



                if ( j == 650 && i == 500) {
                    doPrint = true;
                }
                RayTraceScene scene1 = scene;
                imageData[j * scene.width() + i] = scene.getUpdatedPixel(worldRay, doPrint, scene1, 0, textureMap);
                doPrint = false;


            }
            doPrint = false;


        }
    }

    else{
        DOF(imageData, scene);
    }
}

/*!
    @param:
    *RayTracer::Ray primaryRay: The main ray around which other rays will be generated
    *bool doPrint: debugging parameter
    *RayTraceSceen:: scene being rendered
    *int count: recursive depth for reflections in ray tracer
    *Map textureMap: the map between the shapes and their textures in the scene

    @return
    RGBA: the color result obtained from averaging the different rays generated from the lens surface

    @brief:
    * Simulates a thin lens by creating a lens surface around the primary ray and generating secondary rays

*/

RGBA RayTracer::lensMaker(RayTracer::Ray  primaryRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap){

    float redAcc = 0.f;
    float greenAcc = 0.f;
    float blueAcc = 0.f;

    glm::vec4 focalPoint(glm::vec3(primaryRay.dir) * m_config.focalLength, 1.f);

    for(int i = 0; i < sample; i++){

        float newX = -0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5f-(-0.5f))));
        float newY = -0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5f-(-0.5f))));
        float newZ = -0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5f-(-0.5f))));

        glm::vec4 offsetPos(newX, newY, newZ, primaryRay.pos[3]);

        glm::vec3 newPosT = glm::vec3(primaryRay.pos) + m_config.apperture * glm::vec3(offsetPos);
        glm::vec4 newPos(newPosT, primaryRay.pos[3]);

        glm::vec4 newDir = glm::normalize(focalPoint - newPos);

        Ray newRay{newPos, newDir};

        RGBA update = scene.getUpdatedPixel(newRay, doPrint, scene, count, textureMap);

        redAcc = update.r + redAcc;
        greenAcc = update.g + greenAcc ;
        blueAcc = update.b + blueAcc;



    }


    return RGBA{static_cast<uint8_t>((redAcc/sample)), static_cast<uint8_t>((greenAcc/sample)), static_cast<uint8_t>((blueAcc/sample)), 255};


}



/*!
    @param:
    RGBA imageData: Pointer to the first pixel of the viewing plane through which the ray is traced
    RayTraceScene: The scene which is being rendered


    @brief:
    * Performs similar functions to render
    * Implements depth of field effect using the lens maker
    * instead of just tracing rays from a point (the camera position) I use randomly generated rays from a lens surface
    * With the focalLength and aperture fields the dof effect is achieved

*/
void RayTracer::DOF(RGBA *imageData, const RayTraceScene &scene){


    map<string, RayTracer::textureInfo> textureMap;
    if(m_config.enableTextureMap){


        for(int i = 0; i < scene.MetaData.shapes.size(); i++){
            if(scene.MetaData.shapes[i].primitive.material.textureMap.isUsed){
                string fileName = scene.MetaData.shapes[i].primitive.material.textureMap.filename;
                RayTracer::textureInfo textureInf = loadTextureFromFile(QString::fromStdString(fileName));
                textureMap[fileName] = textureInf;
            }
        }
    }
    std::cout << "one" << std::endl;

    auto p = scene.getCamera().getViewMatrix() * scene.getCamera().getInverseViewMatrix();



    //Inverse works
    bool doPrint = false;
    testMinPos();
    for(int j = 0; j < scene.height(); j++){
        for(int i = 0; i < scene.width(); i++){
            float k = 1.f;
            float x = ((i + 0.5)/(float)scene.width()) - 0.5;
            float y = (((float)scene.height() - 1 - j + 0.5)/(float)scene.height()) - 0.5;
            float z = -m_config.planeZ;

            float V = 2*k* tan(scene.getCamera().getHeightAngle()/2.f) /**(1.f/m_config.focalLength)*/;
            float U = V * scene.getCamera().getAspectRatio();

            glm::vec4 uvk(U*x, V*y, z, 1.f);
            glm::vec4 eye(0.f,0.f,0.f, 1.f);

            glm::vec4 dir = glm::normalize(uvk - eye);
            Ray ray = Ray{eye, dir};
            Ray worldRay = scene.convertRaySpace(ray, scene.getCamera().getInverseViewMatrix());//Ray gets converted to world space



            RayTraceScene scene1 = scene;


            imageData[j * scene.width() + i] = lensMaker(worldRay, doPrint, scene1, 0, textureMap);/*RGBA{static_cast<uint8_t>((redAcc/6.f)), static_cast<uint8_t>((greenAcc/6.f)), static_cast<uint8_t>((blueAcc/6.f)), 255};*/


        }
    }




}
