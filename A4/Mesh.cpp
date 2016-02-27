#include <iostream>
#include <fstream>
#include <limits>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

using namespace glm;
using namespace std;

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*

  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}

IntersectionInfo Mesh::checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {
  const double EPSILON = 1E-6;
  double min_t = std::numeric_limits<double>::infinity();
  bool foundOne = false;
  vec4 intersect_normal;

  int i = 0, matched_comp = -1;
  //return maximum intersection z
  for (Triangle tri : m_faces) {
    i++;

    vec3 v1 = m_vertices[tri.v1], v2 = m_vertices[tri.v2], v3 = m_vertices[tri.v3];
    vec4 ray_b = ray_origin + ray_dir;
    vec3 a = vec3(ray_origin[0], ray_origin[1], ray_origin[2]);
    vec3 b = vec3(ray_b[0], ray_b[1], ray_b[2]);

    vec3 R = a-v1;
    vec3 x = v2-v1;
    vec3 y = v3-v1;
    vec3 z = a-b;

    //intersect
    mat3 A = mat3(x,y,z);

    double D = determinant(A);
    if (abs(D) < EPSILON) {
      return IntersectionInfo();
    }

    A[0] = R;
    double D1 = determinant(A);

    A[0] = x;
    A[1] = R;
    double D2 = determinant(A);

    A[1] = y;
    A[2] = R;
    double D3 = determinant(A);

    double beta = D1/D;
    double gamma = D2/D;
    double t = D3/D;

    //check for intersection
    if (t >= EPSILON && t <= max_t && beta >= 0 && gamma >= 0 && beta+gamma <= 1) {
      //found! update if t value of intersection is less. (find minimum t)
      if (min_t > t) {
        foundOne = true;
        min_t = t;
        intersect_normal = normalize(vec4(cross(v2-v1, v3-v1), 0.0f));
        matched_comp = i;
      }
    }
  }

  if (foundOne) {
    return IntersectionInfo(min_t, intersect_normal, matched_comp);
  } else {
    return IntersectionInfo();
  }
}
