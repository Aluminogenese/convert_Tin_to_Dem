#pragma once
#include <string>
#include <fstream>
#include <iomanip>
#include <eigen3/Eigen/Dense>

class CDem
{
public:
	/// <summary>
	/// 默认构造函数
	/// </summary>
	CDem() :m_pDem(nullptr), resolution(0.), rows(0), cols(0), x_min(DBL_MAX), y_min(DBL_MAX) {};

	/// <summary>
	/// 析构函数
	/// </summary>
	~CDem();

	/// <summary>
	/// 设置相关参数
	/// </summary>
	/// <param name="lf_resolution">DEM分辨率</param>
	/// <param name="n_rows">DEM行数</param>
	/// <param name="n_cols">DEM列数</param>
	/// <param name="lf_minX">起始点X坐标</param>
	/// <param name="lf_minY">起始点Y坐标</param>
	void set_config(const double& lf_resolution, const size_t& n_rows, const size_t& n_cols, const double& lf_minX, const double& lf_minY);

	/// <summary>
	/// 设置格网点高程
	/// </summary>
	/// <param name="n_offX">格网点X坐标</param>
	/// <param name="n_offY">格网点Y坐标</param>
	/// <param name="lf_elevation">格网点高程</param>
	void set_elevation(const size_t& n_offX, const size_t& n_offY, const double& lf_elevation);

	/// <summary>
	/// 保存DEM
	/// </summary>
	/// <param name="filePath">保存路径</param>
	void save(std::string filePath);
public:
	// DEM数据
	double* m_pDem;
	// DEM分辨率
	double resolution;
	// DEM行、列数
	size_t rows;
	size_t cols;
	// 起始点坐标
	double x_min;
	double y_min;
};

