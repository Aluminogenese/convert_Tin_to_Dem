#include "CDem.h"

CDem::~CDem()
{
	delete[] m_pDem;
	m_pDem = nullptr;
}

void CDem::set_config(const double& lf_resolution, const size_t& n_rows, const size_t& n_cols, const double& lf_minX, const double& lf_minY)
{
	this->resolution = lf_resolution;
	this->rows = n_rows;
	this->cols = n_cols;
	this->x_min = lf_minX;
	this->y_min = lf_minY;
	if (!m_pDem) {
		delete[] m_pDem;
	}
	m_pDem = new double[rows * cols];
	for (size_t i = 0; i < rows * cols; i++) {
		*(m_pDem + i) = -99999.0;
	}
}

void CDem::set_elevation(const size_t& n_offX, const size_t& n_offY, const double& lf_elevation)
{
	if (n_offX < cols && n_offY < rows) {
		double* pCurrent = m_pDem + n_offY * cols + n_offX;
		*pCurrent = lf_elevation;
	}
}

void CDem::save(std::string filePath)
{
	std::ofstream fout(filePath);
	fout.setf(fout.fixed);
	fout.fill(' ');
	fout.precision(6);
	// 文件头
	fout << std::setw(9) << x_min << " " << y_min << " ";
	fout.setf(fout.right);
	fout << 0.0 << " " << resolution << " " << resolution << " " << cols << " " << rows << std::endl;
	// 数据体
	fout.fill(' ');
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			fout << std::setw(12) << std::setprecision(3) << *(m_pDem + i * cols + j);
		}
		fout << std::endl;
	}
	fout.close();

}
