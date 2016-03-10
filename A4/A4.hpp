#pragma once

#include <vector>
#include <limits>
#include <thread>
#include <mutex>
#include <iostream>
#include <random>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneNode.hpp"
#include "JointNode.hpp"
#include "GeometryNode.hpp"
#include "Primitive.hpp"
#include "Intersection.hpp"
#include "Flags.hpp"
#include "Material.hpp"
#include "PhongMaterial.hpp"
#include "Light.hpp"
#include "Image.hpp"

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::dvec3 & eye,
		const glm::dvec3 & view,
		const glm::dvec3 & up,
		double fovy,

		// Lighting parameters
		const glm::dvec3 & ambient,
		const std::list<Light *> & lights
);
