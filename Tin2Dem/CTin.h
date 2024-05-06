#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "CTriangle3d.h"

class CTin
{
public:
	CTin() :m_min(DBL_MAX, DBL_MAX, DBL_MAX), m_max(DBL_MIN, DBL_MIN, DBL_MIN) {};
	~CTin();
	bool loadTin(const std::string& filePath);
	void update_area_range(const double& lf_X, const double& lf_Y, const double& lf_Z);
public:
	// 区域范围
	Eigen::Vector3d m_min;
	Eigen::Vector3d m_max;
	// 三角形数据
	std::vector<CTriangle3d> m_vTriangles;
};

