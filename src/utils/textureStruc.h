#ifndef TEXTURESTRUC_H
#define TEXTURESTRUC_H

#include "utils/rgba.h"
#endif // TEXTURESTRUC_H

///
/// \brief The textureInfo struct represents the information relevant for
///        texture application
///

struct textureInfo {
    int width;
    int height;
    RGBA texture;
};
