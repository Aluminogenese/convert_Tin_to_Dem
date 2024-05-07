#include "CTin.h"

CTin::~CTin()
{
	m_vTriangles.clear();
	m_vTriangles.shrink_to_fit();
}

bool CTin::loadTin(const std::string& filePath)
{
    std::ifstream fin(filePath);

    std::string s_datatype;
    fin >> s_datatype;
    if (s_datatype != "xyztri")
    {
        std::cout << "文件格式错误！" << std::endl;
        return false;
    }
    // 读取三角点
    size_t n_nodeNum = 0;
    fin >> n_nodeNum;
    if (n_nodeNum < 3)
    {
        std::cout << "数据有误！没有角点" << std::endl;
        return false;
    }
    std::vector<Eigen::Vector3d> v_points;
    for (size_t i = 0; i < n_nodeNum; i++)
    {
        Eigen::Vector3d lf_coordinates;
        fin >> lf_coordinates[0] >> lf_coordinates[1] >> lf_coordinates[2];
        update_area_range(lf_coordinates[0], lf_coordinates[1], lf_coordinates[2]);
        v_points.push_back(lf_coordinates);
    }
    // 读取三角形
    size_t n_triangleNum = 0;
    fin >> n_triangleNum;
    if (n_triangleNum < 1)
    {
        std::cout << "数据有误！缺少三角形" << std::endl;
        return false;
    }
    Eigen::Vector3d* p_points = v_points.data();
    for (size_t i = 0; i < n_triangleNum; i++)
    {
        size_t n_pointIndex[3] = { 0 };
        fin >> n_pointIndex[0] >> n_pointIndex[1] >> n_pointIndex[2];
        m_vTriangles.push_back(CTriangle3d(v_points[n_pointIndex[0]], v_points[n_pointIndex[1]], v_points[n_pointIndex[2]]));
    }
    return true;
}

void CTin::update_area_range(const double& lf_X, const double& lf_Y, const double& lf_Z)
{
    m_min.x() = std::min(lf_X, m_min.x());
    m_min.y() = std::min(lf_Y, m_min.y());
    m_min.z() = std::min(lf_Z, m_min.z());

    m_max.x() = std::max(lf_X, m_max.x());
    m_max.y() = std::max(lf_Y, m_max.y());
    m_max.z() = std::max(lf_Z, m_max.z());
}
