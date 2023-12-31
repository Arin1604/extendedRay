#include "raytracer.h"
#include "glm/gtx/string_cast.hpp"
#include "qimage.h"
#include "raytracescene.h"
#include <iostream>
#include <map>

using namespace std; //is this okay??

RayTracer::RayTracer(Config config) :
    m_config(config)
{}

float getminPos(std::vector<float> t_array){
    float t_min_pos = t_array[0];
    for(float t: t_array){
        if(t >= 0.0f && (t < t_min_pos || t_min_pos < 0.0f)){
            t_min_pos = t;
        }
    }

    return t_min_pos;
}

void testMinPos(){
    auto y = std::vector{ -1.0f, 2.5f, -3.0f, 6.0f, 1.0f, -8.0f};
    auto z = std::vector{ 1.0f, 0.0f, -1.0f, -2.0f, -3.0f, 2.0f};
    auto x = std::vector{-2.5f, 3.0f, -6.0f, -8.0f};

    //    std::cout << y.size() <<std::endl;
    //    std::cout << z.size() <<std::endl;
    //    std::cout << x.size() <<std::endl;

    //    y.erase(y.begin() + 3);





}

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
                // std::cout << imageData[600 * scene.width() + 100].r << " " << imageData[600 * scene.width() + 100].g << "" << imageData[600 * scene.width() + 100].b << std::endl;


            }
            doPrint = false;

            //500/380
        }
    }

    else{
        DOF(imageData, scene);
    }
}

glm::vec4 RayTracer::lensOffset(){



    return glm::vec4(0);

}

std::uint8_t floatToUint8(float x) {
    return round(x * 255.f);
}

RGBA RayTracer::lensMaker(RayTracer::Ray  primaryRay, bool doPrint,RayTraceScene &scene,int count, std::map<std::string, RayTracer::textureInfo>& textureMap){

    float redAcc = 0.f;
    float greenAcc = 0.f;
    float blueAcc = 0.f;

    glm::vec4 focalPoint(glm::vec3(primaryRay.dir) * focalLength, 1.f);

    for(int i = 0; i < sample; i++){

        float newX = -0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5f-(-0.5f))));
        float newY = -0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5f-(-0.5f))));
        float newZ = -0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5f-(-0.5f))));

        glm::vec4 offsetPos(newX, newY, newZ, primaryRay.pos[3]);

        glm::vec3 newPosT = glm::vec3(primaryRay.pos) + apperture * glm::vec3(offsetPos);
        glm::vec4 newPos(newPosT, primaryRay.pos[3]);

        glm::vec4 newDir = glm::normalize(focalPoint - newPos);

        Ray newRay{newPos, newDir};

        RGBA update = scene.getUpdatedPixel(newRay, doPrint, scene, count, textureMap);

        redAcc = update.r + redAcc;
        greenAcc = update.g + greenAcc ;
        blueAcc = update.b + blueAcc;



    }


    return RGBA{static_cast<uint8_t>((redAcc/6.f)), static_cast<uint8_t>((greenAcc/6.f)), static_cast<uint8_t>((blueAcc/6.f)), 255};


}

void RayTracer::DOF(RGBA *imageData, const RayTraceScene &scene){


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




            if ( j == 400 && i == 550) {
                doPrint = true;
            }

            RayTraceScene scene1 = scene;


            imageData[j * scene.width() + i] = lensMaker(worldRay, doPrint, scene1, 0, textureMap);/*RGBA{static_cast<uint8_t>((redAcc/6.f)), static_cast<uint8_t>((greenAcc/6.f)), static_cast<uint8_t>((blueAcc/6.f)), 255};*/

            //            RGBA{static_cast<uint8_t>(imageAcc.r /6.f), static_cast<uint8_t>(imageAcc.g/6.f), static_cast<uint8_t>(imageAcc.b/6.f), static_cast<uint8_t>(imageAcc.a/6.f)};
            //            doPrint = false;

        }




    }




}
