#include "CTriangle3d.h"

CTriangle3d::CTriangle3d(const Eigen::Vector3d& node1, const Eigen::Vector3d& node2, const Eigen::Vector3d& node3)
{
	nodes[0] = node1;
	nodes[1] = node2;
	nodes[2] = node3;
}
