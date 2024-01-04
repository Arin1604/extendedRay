#pragma once

#include <cstdint>
///
/// \brief The RGBA struct represnets the red, green and blue values as  uint8s
///
struct RGBA {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a = 255;
};
