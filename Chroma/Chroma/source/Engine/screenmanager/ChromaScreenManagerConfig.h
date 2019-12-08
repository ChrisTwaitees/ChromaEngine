#ifndef _CHROMA_SCREEN_CONSTANTS_
#define _CHROMA_SCREEN_CONSTANTS_

#include "glm/glm.hpp"

// SCREEN
static const unsigned int SCREEN_WIDTH{ 1000 };
static const unsigned int SCREEN_HEIGHT{ 1000 };
static const unsigned int MSAA_SAMPLES{ 4 };

// BG COLOR
static const glm::vec4 SCREEN_DEFAULT_COLOR(0.0f, 0.0f, 0.0f, 0.0f);

// INPUT
static double MOUSE_XOFFSET, MOUSE_YOFFSET;

#endif