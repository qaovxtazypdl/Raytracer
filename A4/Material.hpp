#pragma once

#include "Texture.hpp"

class PhongMaterial;

struct MaterialPackage {
  Texture *m_texture;
  Texture *m_bumps;
  int bump_channel; //0x1 red, 0x2 green, 0x4 blue.
  PhongMaterial *m_material;

  MaterialPackage(Texture *m_texture, Texture *m_bumps, int bump_channel, PhongMaterial *m_material)
  : m_texture(m_texture), m_bumps(m_bumps), bump_channel(bump_channel), m_material(m_material) {}

  MaterialPackage() : m_texture(NULL), m_bumps(NULL), bump_channel(0), m_material(NULL) {}
};

class Material {
public:
  virtual ~Material();

protected:
  Material();
};
