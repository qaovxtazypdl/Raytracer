#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
		const std::string & meshId,
		const std::string & name
)
	: SceneNode(name),
	  meshId(meshId),
    falseColor(0.0f,0.0f,0.0f),
    falseColorSel(0.0f,0.0f,0.0f)
{
	m_nodeType = NodeType::GeometryNode;
}
