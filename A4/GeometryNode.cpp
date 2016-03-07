#include "GeometryNode.hpp"

using namespace std;
using namespace glm;
//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
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

	m_material = mat;
}


IntersectionInfo GeometryNode::testHit(const dvec4 &ray_origin, const dvec4 &ray_dir, double max_t) const {
  IntersectionInfo intersectionInfo;
  dmat4 T = trans;
  dmat4 T_inv = invtrans;
  dmat3 T_invtrans = invtrans_transpose;

  for (SceneNode * node : children) {
    intersectionInfo.UNION(node->testHit(T_inv * ray_origin, T_inv * ray_dir, max_t));
  }

  intersectionInfo.UNION(m_primitive->checkRayIntersection(T_inv * ray_origin, T_inv * ray_dir, m_material));

  intersectionInfo.TRANSFORM_UP(T, T_invtrans);

  return intersectionInfo;
}
