#include "shapeInterface.h"
#include "utils/scenedata.h"

class Sphere: public Shape {
public:
    double radius;
    PrimitiveType shapeType = PrimitiveType::PRIMITIVE_SPHERE;

    auto getIntersection(glm::vec4 P, glm::vec4 D)->float override{
        float t = -1;

        //glm::vec4 q = P + t * D;
        float a = pow(D[0], 2) + pow(D[1], 2) + pow(D[2], 2);
        float b = P[0] * D[0] + P[1]*D[1] + P[2]*D[2];
        float c = pow(P[0], 2) + pow(P[1], 2) + pow(P[2], 2) - pow(radius, 2);

        float det = pow(b, 2) - 4 * a * c;

        if(det < 0){
            return t;
        }

        else{// Since both tangential and full on intersection count as intersection
            float sol1 = (-b + sqrt(det))/ 2*a;
            float sol2 = (-b - sqrt(det))/ 2*a;

            if(sol1 < 0 || sol2 < 0){
                t = fmax(sol1, sol2);
            }

            else if(sol2> 0 && sol1>0){
                t = fmin(sol1, sol2);
            }

            return t;
        }


    }

    auto getNormal()->double override{
        return 2.0;

    }


};
