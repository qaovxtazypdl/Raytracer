#include "GeometryNode.hpp"

using namespace std;
using namespace glm;
//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, PhongMaterial *mat )
	: SceneNode( name )
	, m_primitive( prim )
  , m_matpack(NULL, NULL, 0, mat)
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( PhongMaterial *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_matpack.m_material = mat;
}

IntersectionInfo GeometryNode::testNode(const dvec4 &ray_origin, const dvec4 &ray_dir) const {
  return m_primitive->checkRayIntersection(ray_origin, ray_dir, m_matpack);
}

IntersectionInfo GeometryNode::testHit(const dvec4 &ray_origin, const dvec4 &ray_dir) const {
  IntersectionInfo intersectionInfo;
  dmat4 T = trans;
  dmat4 T_inv = invtrans;
  dmat3 T_invtrans = invtrans_transpose;

  if (m_boundingObject != NULL) {
    IntersectionPoint bounding = m_boundingObject->testHit(T_inv * ray_origin, T_inv * ray_dir).getFirstValidIntersection(std::numeric_limits<double>::infinity());
    if (!bounding.valid) {
      return intersectionInfo;
    }
  }

  for (SceneNode * node : children) {
    intersectionInfo = intersectionInfo.UNION(node->testHit(T_inv * ray_origin, T_inv * ray_dir));
  }

  intersectionInfo = intersectionInfo.UNION(testNode(T_inv * ray_origin, T_inv * ray_dir));
  intersectionInfo.TRANSFORM_UP(T, T_invtrans);
  return intersectionInfo;
}


void GeometryNode::setBumpMap(Texture *t, int channels) {
  m_matpack.m_bumps = t;
  m_matpack.bump_channel = channels;
}

void GeometryNode::setTextureMap(Texture *t) {
  m_matpack.m_texture = t;
}
