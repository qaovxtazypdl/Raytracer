#pragma once

//parallax and 3D
#define MACRO_USE_ANAGLYPH false
#define MACRO_3D_SIDE_BY_SIDE (MACRO_USE_ANAGLYPH && false)
#define MACRO_3D_PARALLAX 0.021

//anti-aliasing
#define MACRO_USE_SUPERSAMPLE_TEST false
#define MACRO_USE_SUPERSAMPLE (MACRO_USE_SUPERSAMPLE_TEST || true)
#define MACRO_SUPERSAMPLE_SCALE 3

//refl and refr
#define MACRO_REFLECTION_ON false
#define MACRO_REFRACTION_ON true

//threading
#define MACRO_NUM_THREADS 12

//area lights
#define MACRO_NUM_PLANAR_LIGHT_SAMPLES 4
#define MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_RADIAL 3
#define MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_ANGULAR 6


//Miscellaneous
#define MACRO_STARFIELD_BACKGROUND_ON false
#define MACRO_RENDER_BOUNDING_BOX false
#define PI 3.1415926535897932384
