#include "shapes/Sphere.h"
#include "shapes/Cylinder.h"
#include "shapes/Cone.h"
#include "shapes/Cube.h"
#include "utils/sceneparser.h"
#include "raytracescene.h"
#include "raytracer.h"
#include <iostream>
#include <cmath>

///
/// Intersection File TODOs:
/// *Implement Bounding Volumes to increase efficiency
/// * Look into implementing Octree
///


/*!
    @param:
    t_array: the array of various t values which correspond to the shape's intersection with a light ray

    @return
    float: returns the surface struc which represents the closest object

    @brief:
    * This function is useful in determining the shape that is closest to the camera

*/
RayTracer::surfaceStruct RayTraceScene::getminPos(std::vector<RayTracer::surfaceStruct> &t_StrucArray){
    RayTracer::surfaceStruct t_min_pos_Struc = t_StrucArray[0];
    for(RayTracer::surfaceStruct t : t_StrucArray){
        if(t.t >= 0.0f && (t.t < t_min_pos_Struc.t || t_min_pos_Struc.t < 0.0f)){
            t_min_pos_Struc = t;
        }
    }

    return t_min_pos_Struc;
}


/*!
    @param:
    *RenderShapeData shape: the shape through which the ray's intersection will be checked
    *RayTracer::Ray: the ray that is passed in from the raytracer

    @return
    float: returns the intersection information for color and lighting calculation

    @brief:
    * The shapes are represented as mathematical objects using implicit equations
    * This function checks the type of shape and calls the appropriate intersection methods
    * The intersection information returned is crucial in determining the color output

*/
RayTracer::intersectInfo RayTraceScene::getIntersection(RenderShapeData shape, RayTracer::Ray ray){
    RayTracer::intersectInfo intersectInfo;
    if(shape.primitive.type == PrimitiveType::PRIMITIVE_SPHERE){
        intersectInfo = sphere::getSphereIntersection(ray);
    }
    else if(shape.primitive.type == PrimitiveType::PRIMITIVE_CYLINDER){
        intersectInfo = cylinder::getCylinderIntersection(ray);
    }
    else if(shape.primitive.type == PrimitiveType::PRIMITIVE_CONE){
        intersectInfo = cone::getConeIntersection(ray);
    }
    else{
        intersectInfo = cube::getCubeIntersection(ray);

    }


    return intersectInfo;


}
