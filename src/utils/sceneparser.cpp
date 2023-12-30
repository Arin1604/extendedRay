#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>


void dfsParser(SceneNode* root, RenderData &renderData, glm::mat4 ctm){
    //    std::vector<RenderShapeData> newSentence = shapes.push_back(root.);
    //    RenderShapeData newObj = RenderShapeData{/*primitive_acc*//*,*/ /*ctm_acc*/};
    //    shapes.push_back(newObj);

    if(root == nullptr){
        return;
    }

    glm::mat4 latestCtm = ctm;

    for(int i = 0; i < root->transformations.size(); i++){
        switch (root->transformations[i]->type) {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            latestCtm = latestCtm * glm::translate(root->transformations[i]->translate);

            break;

        case TransformationType::TRANSFORMATION_ROTATE:
            latestCtm = latestCtm * glm::rotate(root->transformations[i]->angle, root->transformations[i]->rotate);
            break;

        case TransformationType::TRANSFORMATION_SCALE:
            latestCtm = latestCtm * glm::scale(root->transformations[i]->scale) ;
            break;

        case TransformationType::TRANSFORMATION_MATRIX:
            latestCtm = latestCtm * root->transformations[i]->matrix;
            break;

        }

    }

    std::vector<SceneLightData> lightsToAdd;

    for(int i = 0; i < root->lights.size(); i++){
        SceneLight* q = root->lights[i];
        glm::vec4 posUpdate = latestCtm * glm::vec4(0, 0, 0, 1);
        glm::vec4 dirUpdate = latestCtm * q->dir;
        SceneLightData p = SceneLightData{q->id,q->type,q->color, q->function, posUpdate, dirUpdate, q->penumbra, q->angle, q->width, q->height};
        renderData.lights.push_back(p);
    }




    for(int i = 0; i < root->primitives.size(); i++){
        RenderShapeData newObj;
        newObj.ctm = latestCtm;
        newObj.primitive = *root->primitives[i];
        renderData.shapes.push_back(newObj);
    }





    for(int i =0; i < root->children.size(); i++){
        dfsParser(root->children[i], renderData, latestCtm);
    }


}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }




    renderData = {fileReader.getGlobalData(), fileReader.getCameraData()};


    renderData.shapes.clear();
    renderData.lights.clear();
    glm::mat4 identity = glm::mat4(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, 1, 0,
                                   0, 0, 0, 1) ;

    std::cout << "DFS" << std::endl;
    dfsParser(fileReader.getRootNode(), renderData,identity);
    std::cout << "DFS ENDS" << std::endl;


    return true;
}
