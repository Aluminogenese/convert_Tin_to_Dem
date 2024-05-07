#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "CTriangle3d.h"

class CTin
{
public:
	/// <summary>
	/// Ĭ�Ϲ��캯��
	/// </summary>
	CTin() :m_min(DBL_MAX, DBL_MAX, DBL_MAX), m_max(DBL_MIN, DBL_MIN, DBL_MIN) {};

	/// <summary>
	/// ��������
	/// </summary>
	~CTin();

	/// <summary>
	/// ����Tin
	/// </summary>
	/// <param name="filePath">Tin�ļ�·��</param>
	/// <returns>true/false</returns>
	bool loadTin(const std::string& filePath);

	/// <summary>
	/// ����Tin���Ƿ�Χ
	/// </summary>
	/// <param name="lf_X">������X����</param>
	/// <param name="lf_Y">������Y����</param>
	/// <param name="lf_Z">������Z����</param>
	void update_area_range(const double& lf_X, const double& lf_Y, const double& lf_Z);
public:
	// ����Χ
	Eigen::Vector3d m_min;
	Eigen::Vector3d m_max;
	// ����������
	std::vector<CTriangle3d> m_vTriangles;
};

