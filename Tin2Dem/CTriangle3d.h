#pragma once
#include <eigen3/Eigen/Dense>

class CTriangle3d
{
public:
	CTriangle3d() {};
	CTriangle3d(const Eigen::Vector3d& node1, const Eigen::Vector3d& node2, const Eigen::Vector3d& node3);

public:
	Eigen::Vector3d nodes[3];
};