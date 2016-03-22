#pragma once

#include <vector>
#include <limits>
#include <thread>
#include <mutex>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Utils.hpp"
#include "Texture.hpp"
#include "SceneNode.hpp"
#include "CSGNode.hpp"
#include "JointNode.hpp"
#include "GeometryNode.hpp"
#include "Primitive.hpp"
#include "Intersection.hpp"
#include "Flags.hpp"
#include "Material.hpp"
#include "PhongMaterial.hpp"
#include "Light.hpp"
#include "FlatPrimitives.hpp"
#include "Image.hpp"

void A5_Render(
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

