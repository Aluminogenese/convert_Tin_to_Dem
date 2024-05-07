#pragma once
#include <string>
#include <fstream>
#include <iomanip>
#include <eigen3/Eigen/Dense>

class CDem
{
public:
	/// <summary>
	/// Ĭ�Ϲ��캯��
	/// </summary>
	CDem() :m_pDem(nullptr), resolution(0.), rows(0), cols(0), x_min(DBL_MAX), y_min(DBL_MAX) {};

	/// <summary>
	/// ��������
	/// </summary>
	~CDem();

	/// <summary>
	/// ������ز���
	/// </summary>
	/// <param name="lf_resolution">DEM�ֱ���</param>
	/// <param name="n_rows">DEM����</param>
	/// <param name="n_cols">DEM����</param>
	/// <param name="lf_minX">��ʼ��X����</param>
	/// <param name="lf_minY">��ʼ��Y����</param>
	void set_config(const double& lf_resolution, const size_t& n_rows, const size_t& n_cols, const double& lf_minX, const double& lf_minY);

	/// <summary>
	/// ���ø�����߳�
	/// </summary>
	/// <param name="n_offX">������X����</param>
	/// <param name="n_offY">������Y����</param>
	/// <param name="lf_elevation">������߳�</param>
	void set_elevation(const size_t& n_offX, const size_t& n_offY, const double& lf_elevation);

	/// <summary>
	/// ����DEM
	/// </summary>
	/// <param name="filePath">����·��</param>
	void save(std::string filePath);
public:
	// DEM����
	double* m_pDem;
	// DEM�ֱ���
	double resolution;
	// DEM�С�����
	size_t rows;
	size_t cols;
	// ��ʼ������
	double x_min;
	double y_min;
};

