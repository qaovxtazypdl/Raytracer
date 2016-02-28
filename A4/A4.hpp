#pragma once

#include <vector>
#include <limits>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneNode.hpp"
#include "JointNode.hpp"
#include "GeometryNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "PhongMaterial.hpp"
#include "Light.hpp"
#include "Image.hpp"

struct HierarchicalNodeInfo {
  GeometryNode *geoNode;
  glm::mat4 mat;
  glm::mat4 inv;
  glm::mat3 invTranspose;

  HierarchicalNodeInfo(GeometryNode *geoNode, glm::mat4 mat) : geoNode(geoNode), mat(mat) {
    inv = glm::inverse(mat);
    invTranspose = glm::transpose(glm::inverse(glm::mat3(mat)));
  }
};

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);
