#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim,
		PhongMaterial *mat = nullptr );

	void setMaterial( PhongMaterial *material );
  virtual IntersectionInfo testNode(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir) const;
  virtual IntersectionInfo testHit(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir) const;

  void setBumpMap(Texture *t, int channels);
  void setTextureMap(Texture *t);

  MaterialPackage m_matpack;
	Primitive *m_primitive;
};
