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

  Texture *m_texture;
  Texture *m_bumps;
  int bump_channel; //0x1 red, 0x2 green, 0x4 blue.

	PhongMaterial *m_material;
	Primitive *m_primitive;
};
