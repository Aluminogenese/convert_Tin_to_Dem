#include "CTin2Dem.h"

void CTin2Dem::initDem(CDem& c_dem, const CTin& c_tin, const double& lf_resolution)
{
    size_t rows = (size_t)(c_tin.m_max.y() - c_tin.m_min.y()) / lf_resolution + 1, cols = (size_t)(c_tin.m_max.x() - c_tin.m_min.x()) / lf_resolution + 1;
    c_dem.set_config(lf_resolution,rows, cols,c_tin.m_min.x(), c_tin.m_min.y());
}

void CTin2Dem::convert2Dem(CDem& c_dem, CTin& c_tin)
{
    CTriangle3d* pTriangles = c_tin.m_vTriangles.data();
    size_t nTriangles = c_tin.m_vTriangles.size();
    // 对每个三角形进行内插
    size_t dem_hit_times = 0;
    size_t dem_test_times = 0;
    size_t triangle_test_times = 0;
    size_t triangle_hit_times = 0;
#pragma omp parallel for reduction(+:dem_hit_times, dem_test_times, triangle_test_times, triangle_hit_times)
    for (int iTriangle = 0; iTriangle < nTriangles; iTriangle++)
    {
        triangle_test_times++;

        // 计算三角形所在区域
        CTriangle3d* triangle = pTriangles + iTriangle;
        Eigen::Vector3d* triNodes = triangle->nodes;
        double tri_node_range[] = {
            std::min(triNodes[0].x(), std::min(triNodes[1].x(), triNodes[2].x())),
            std::min(triNodes[0].y(), std::min(triNodes[1].y(), triNodes[2].y())),
            std::max(triNodes[0].x(), std::max(triNodes[1].x(), triNodes[2].x())),
            std::max(triNodes[0].y(), std::max(triNodes[1].y(), triNodes[2].y()))
        }; // 三角形区域坐标 X_MIN Y_MIN X_MAX Y_MAX
        int offset[] = {
            int((tri_node_range[0] - c_tin.m_min.x()) / c_dem.resolution),
            int((tri_node_range[2] - c_tin.m_min.x()) / c_dem.resolution) + 1,
            int((tri_node_range[1] - c_tin.m_min.y()) / c_dem.resolution),
            int((tri_node_range[3] - c_tin.m_min.y()) / c_dem.resolution) + 1
        }; // 偏移值的顺序为： Nx_min Nx_max Ny_min Ny_max
        double area_start[] = {
            c_tin.m_min.x() + c_dem.resolution * offset[0],
            c_tin.m_min.y() + c_dem.resolution * offset[2]
        }; // 代表三角形所在区域最大范围的参数，前两个为最小X Y坐标
        size_t area_size[] = {
            offset[1] - offset[0] + 1,
            offset[3] - offset[2] + 1
        };

        // 拟合三角形三条边的方程
        // 生成三角形三个点的齐次坐标
        Eigen::Vector3d nodes_h[] = {
            Eigen::Vector3d(triNodes[0].x(), triNodes[0].y(), 1.0),
            Eigen::Vector3d(triNodes[1].x(), triNodes[1].y(), 1.0),
            Eigen::Vector3d(triNodes[2].x(), triNodes[2].y(), 1.0)
        };
        // 计算三条边的参数
        Eigen::Vector3d arcs_h[] = {
            nodes_h[1].cross(nodes_h[2]),
            nodes_h[2].cross(nodes_h[0]),
            nodes_h[0].cross(nodes_h[1])
        };
        // 对三角形范围内每一个点进行判断
        bool isHasDem = false;
        for (size_t iy = 0; iy < area_size[1]; iy++)
        {
            for (size_t ix = 0; ix < area_size[0]; ix++)
            {
                dem_test_times++;
                // 当前点的齐次坐标
                Eigen::Vector3d cur(area_start[0] + ix * c_dem.resolution, area_start[1] + iy * c_dem.resolution, 1.0);
                // 判断当前点是否在三角形内
                int flag = 0; // 当前点状态的变量。只有当 flag = 7 时，才代表当前点在三角形内
                flag = ((cur.dot(arcs_h[0]) < 0) == (nodes_h[0].dot(arcs_h[0]) <= 0)) ? flag | 4 : flag;
                flag = ((cur.dot(arcs_h[1]) < 0) == (nodes_h[1].dot(arcs_h[1]) <= 0)) ? flag | 2 : flag;
                flag = ((cur.dot(arcs_h[2]) < 0) == (nodes_h[2].dot(arcs_h[2]) <= 0)) ? flag | 1 : flag;
                // 如果点在三角形内，内插其高程值
                switch (flag)
                {
                case 7:
                {
                    isHasDem = true;
                    // 计算内插参数
                    Eigen::Matrix3d matA = Eigen::Matrix3d::Constant(1.0);
                    matA(0, 0) = triangle->nodes[0].x();
                    matA(0, 1) = triangle->nodes[1].x();
                    matA(0, 2) = triangle->nodes[2].x();
                    matA(1, 0) = triangle->nodes[0].y();
                    matA(1, 1) = triangle->nodes[1].y();
                    matA(1, 2) = triangle->nodes[2].y();
                    Eigen::Vector3d coefficient = matA.lu().solve(cur);
                    if ((coefficient.array() > 0).all())
                    {
                        dem_hit_times++;
                        // 内插高程值
                        double elevation = coefficient.dot(Eigen::Vector3d(triangle->nodes[0].z(), triangle->nodes[1].z(), triangle->nodes[2].z()));
                        // DEM 对象记录高程值
                        #pragma omp critical
                        {
                            c_dem.set_elevation(offset[0] + ix, offset[2] + iy, elevation);
                        }
                        break;
                    }
                }
                default:
                    break;
                }
            }
        }
        if (isHasDem) {
            #pragma omp atomic
            triangle_hit_times++;
        }
        else
        {
            std::cout << "三角形" << iTriangle << "中没有DEM点" << std::endl;
        }
    }
    std::cout << "测试的三角形数目：     " << triangle_test_times << std::endl;
    std::cout << "含有DEM点的三角行数目：" << triangle_hit_times << std::endl;
    std::cout << "总的DEM点的数目：      " << c_dem.rows * c_dem.cols << std::endl;
    std::cout << "测试的DEM点的数目：    " << dem_test_times << std::endl;
    std::cout << "命中的DEM点的数目：    " << dem_hit_times << std::endl;
}
