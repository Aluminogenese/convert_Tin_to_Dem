#pragma once
#include <string>
#include <fstream>
#include <iomanip>
#include <eigen3/Eigen/Dense>

class CDem
{
public:
	CDem() :m_pDem(nullptr), resolution(0.), rows(0), cols(0), x_min(DBL_MAX), y_min(DBL_MAX) {};
	//CDem(const CDem& source);
	~CDem();
	//void operator=(CDem& source);
	void set_config(const double& lf_resolution, const size_t& n_rows, const size_t& n_cols, const double& lf_minX, const double& lf_minY);
	void set_elevation(const size_t& n_offX, const size_t& n_offY, const double& lf_elevation);
	void save(std::string filePath);
public:
	double* m_pDem;
	double resolution;
	size_t rows;
	size_t cols;
	double x_min;
	double y_min;
};

