#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "CTriangle3d.h"

class CTin
{
public:
	/// <summary>
	/// 默认构造函数
	/// </summary>
	CTin() :m_min(DBL_MAX, DBL_MAX, DBL_MAX), m_max(DBL_MIN, DBL_MIN, DBL_MIN) {};

	/// <summary>
	/// 析构函数
	/// </summary>
	~CTin();

	/// <summary>
	/// 加载Tin
	/// </summary>
	/// <param name="filePath">Tin文件路径</param>
	/// <returns>true/false</returns>
	bool loadTin(const std::string& filePath);

	/// <summary>
	/// 更新Tin覆盖范围
	/// </summary>
	/// <param name="lf_X">格网点X坐标</param>
	/// <param name="lf_Y">格网点Y坐标</param>
	/// <param name="lf_Z">格网点Z坐标</param>
	void update_area_range(const double& lf_X, const double& lf_Y, const double& lf_Z);
public:
	// 区域范围
	Eigen::Vector3d m_min;
	Eigen::Vector3d m_max;
	// 三角形数据
	std::vector<CTriangle3d> m_vTriangles;
};

