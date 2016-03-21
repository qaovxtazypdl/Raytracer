#pragma once

//anti-aliasing
#define MACRO_USE_SUPERSAMPLE_TEST false
#define MACRO_USE_SUPERSAMPLE (MACRO_USE_SUPERSAMPLE_TEST || false)
#define MACRO_SUPERSAMPLE_SCALE 2

//refl and refr
#define MACRO_REFLECTION_ON true
#define MACRO_REFRACTION_ON true
#define MACRO_GLOSSY_ON false

//threading
#define MACRO_NUM_THREADS 16

//sampling
#define MACRO_NUM_GLOSSY_SAMPLES_RADIAL 2
#define MACRO_NUM_GLOSSY_SAMPLES_ANGULAR 2
#define MACRO_NUM_PLANAR_LIGHT_SAMPLES 3
#define MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_RADIAL 2
#define MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_ANGULAR 4


//Miscellaneous
#define MACRO_STARFIELD_BACKGROUND_ON false
#define MACRO_RENDER_BOUNDING_BOX false
#define PI 3.1415926535897932384
#define MACRO_USE_BG false

//parallax and 3D
#define MACRO_USE_ANAGLYPH false
#define MACRO_3D_SIDE_BY_SIDE (MACRO_USE_ANAGLYPH && false)
#define MACRO_3D_GREYSCALE (MACRO_USE_ANAGLYPH && true)
#define MACRO_3D_PARALLAX 0.015
