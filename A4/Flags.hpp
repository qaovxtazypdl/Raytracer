#pragma once

#define MACRO_USE_SUPERSAMPLE_TEST false
#define MACRO_USE_SUPERSAMPLE (MACRO_USE_SUPERSAMPLE_TEST || true)
#define MACRO_SUPERSAMPLE_SCALE 3

#define MACRO_NUM_THREADS 12


//must be even
#define MACRO_NUM_PLANAR_LIGHT_SAMPLES 4
#define MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_RADIAL 3
#define MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_ANGULAR 6

#define MACRO_STARFIELD_BACKGROUND_ON false
#define MACRO_RENDER_BOUNDING_BOX false
#define MACRO_REFLECTION_ON true
#define MACRO_REFRACTION_ON false

#define PI 3.1415926535897932384
