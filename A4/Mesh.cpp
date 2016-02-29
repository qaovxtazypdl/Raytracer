#include <iostream>
#include <fstream>
#include <limits>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

using namespace glm;
using namespace std;

Mesh::Mesh( const std::string& fname )
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

  double minx, miny, minz;
  double maxx, maxy, maxz;

  minx = miny = minz = std::numeric_limits<double>::infinity();
  maxx = maxy = maxz = -1 * std::numeric_limits<double>::infinity();

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::dvec3( vx, vy, vz ) );
      minx = std::min(minx, vx);
      maxx = std::max(maxx, vx);
      miny = std::min(miny, vy);
      maxy = std::max(maxy, vy);
      minz = std::min(minz, vz);
      maxz = std::max(maxz, vz);
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}

  boundingBox = new NonhierBox(dvec3(minx, miny, minz), dvec3(std::max(maxx-minx, 0.01), std::max(maxy-miny, 0.01), std::max(maxz-minz, 0.01)));
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

bool Mesh::computeBGT(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Triangle tri, double &beta, double &gamma, double &t) {
  dvec3 v1 = m_vertices[tri.v1], v2 = m_vertices[tri.v2], v3 = m_vertices[tri.v3];
  dvec4 ray_b = ray_origin + ray_dir;
  dvec3 a = dvec3(ray_origin[0], ray_origin[1], ray_origin[2]);
  dvec3 b = dvec3(ray_b[0], ray_b[1], ray_b[2]);

  dvec3 R = a-v1;
  dvec3 x = v2-v1;
  dvec3 y = v3-v1;
  dvec3 z = dvec3(-ray_dir[0], -ray_dir[1], -ray_dir[2]);

  //intersect
  dmat3 A = dmat3(x,y,z);

  double D = determinant(A);
  if (abs(D) < 1E-11) {
    return false;
  }
  D = 1/D;

  A[0] = R;
  double D1 = determinant(A);
  beta = D1*D;
  if (beta < 0.0 || beta > 1.0) {
    return false;
  }

  A[0] = x;
  A[1] = R;
  double D2 = determinant(A);
  gamma = D2*D;
  if (gamma < 0.0 || beta + gamma > 1.0) {
    return false;
  }

  A[1] = y;
  A[2] = R;
  double D3 = determinant(A);
  t = D3*D;

  return true;
}

IntersectionInfo Mesh::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, double max_t) {
  if (MACRO_RENDER_BOUNDING_BOX) {
    return boundingBox->checkRayIntersection(ray_origin, ray_dir, max_t);
  }

  if (!boundingBox->checkRayIntersection(ray_origin, ray_dir, max_t).didIntersect) {
    return IntersectionInfo();
  }

  const double EPSILON = 1E-11;
  double min_t = std::numeric_limits<double>::infinity();
  bool foundOne = false;
  dvec4 intersect_normal;

  int i = 0;
  //return maximum intersection z
  for (Triangle tri : m_faces) {
    i++;

    dvec3 v1 = m_vertices[tri.v1], v2 = m_vertices[tri.v2], v3 = m_vertices[tri.v3];
    double beta, gamma, t;
    if (!computeBGT(ray_origin, ray_dir, tri, beta, gamma, t)) continue;

    //check for intersection
    if (t >= EPSILON) {
      //found! update if t value of intersection is less. (find minimum t)
      if (min_t > t) {
        foundOne = true;
        min_t = t;
        intersect_normal = normalize(dvec4(cross(v2-v1, v3-v1), 0.0f));
      }
    }
  }

  if (foundOne) {
    if (dot(intersect_normal, ray_dir) > 0) intersect_normal *= -1;
    return IntersectionInfo(min_t, min_t*ray_dir + ray_origin, intersect_normal);
  } else {
    return IntersectionInfo();
  }
}

