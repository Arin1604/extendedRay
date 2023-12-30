#include "utils/scenedata.h"
#include <glm/glm.hpp>

class Shape {
public:
    Shape(glm::mat4 ctm);
    PrimitiveType shapeType;
    // Two Implementations:
    // 1) Use a Struct for holding normal vector and t
    virtual auto getIntersection(glm::vec4 p, glm::vec4 d)->float = 0;
    virtual auto getNormal()->double = 0;// Returns the normal vector in World Coordinates

    virtual ~Shape() = default;

    glm::mat4 ctm;
};
