#pragma once

#include "Material.hpp"
#include "Primitive.hpp"

#include <glm/glm.hpp>

#include <list>
#include <string>
#include <iostream>
#include <limits>

enum class NodeType {
	SceneNode,
	GeometryNode,
  JointNode,
	CSGNode
};

class SceneNode {
public:
  SceneNode(const std::string & name);

	SceneNode(const SceneNode & other);

    virtual ~SceneNode();

	int totalSceneNodes() const;

    const glm::dmat4& get_transform() const;
    const glm::dmat4& get_inverse() const;

    void set_transform(const glm::dmat4& m);

    void add_child(SceneNode* child);

    void remove_child(SceneNode* child);

	//-- Transformations:
    void rotate(char axis, float angle);
    void scale(const glm::vec3& amount);
    void translate(const glm::vec3& amount);

  virtual IntersectionInfo testNode(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir) const;
  virtual IntersectionInfo testHit(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir) const;

	friend std::ostream & operator << (std::ostream & os, const SceneNode & node);

    // Transformations
    glm::dmat4 trans;
    glm::dmat4 invtrans;
    glm::dmat3 invtrans_transpose;

    std::list<SceneNode*> children;

	NodeType m_nodeType;
	std::string m_name;
	unsigned int m_nodeId;

private:
	// The number of SceneNode instances.
	static unsigned int nodeInstanceCount;
};
