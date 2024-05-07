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
    // ��ÿ�������ν����ڲ�
    size_t dem_hit_times = 0;
    size_t dem_test_times = 0;
    size_t triangle_test_times = 0;
    size_t triangle_hit_times = 0;
#pragma omp parallel for reduction(+:dem_hit_times, dem_test_times, triangle_test_times, triangle_hit_times)
    for (int iTriangle = 0; iTriangle < nTriangles; iTriangle++)
    {
        triangle_test_times++;

        // ������������������
        CTriangle3d* triangle = pTriangles + iTriangle;
        Eigen::Vector3d* triNodes = triangle->nodes;
        double tri_node_range[] = {
            std::min(triNodes[0].x(), std::min(triNodes[1].x(), triNodes[2].x())),
            std::min(triNodes[0].y(), std::min(triNodes[1].y(), triNodes[2].y())),
            std::max(triNodes[0].x(), std::max(triNodes[1].x(), triNodes[2].x())),
            std::max(triNodes[0].y(), std::max(triNodes[1].y(), triNodes[2].y()))
        }; // �������������� X_MIN Y_MIN X_MAX Y_MAX
        int offset[] = {
            int((tri_node_range[0] - c_tin.m_min.x()) / c_dem.resolution),
            int((tri_node_range[2] - c_tin.m_min.x()) / c_dem.resolution) + 1,
            int((tri_node_range[1] - c_tin.m_min.y()) / c_dem.resolution),
            int((tri_node_range[3] - c_tin.m_min.y()) / c_dem.resolution) + 1
        }; // ƫ��ֵ��˳��Ϊ�� Nx_min Nx_max Ny_min Ny_max
        double area_start[] = {
            c_tin.m_min.x() + c_dem.resolution * offset[0],
            c_tin.m_min.y() + c_dem.resolution * offset[2]
        }; // ���������������������Χ�Ĳ�����ǰ����Ϊ��СX Y����
        size_t area_size[] = {
            offset[1] - offset[0] + 1,
            offset[3] - offset[2] + 1
        };

        // ��������������ߵķ���
        // ������������������������
        Eigen::Vector3d nodes_h[] = {
            Eigen::Vector3d(triNodes[0].x(), triNodes[0].y(), 1.0),
            Eigen::Vector3d(triNodes[1].x(), triNodes[1].y(), 1.0),
            Eigen::Vector3d(triNodes[2].x(), triNodes[2].y(), 1.0)
        };
        // ���������ߵĲ���
        Eigen::Vector3d arcs_h[] = {
            nodes_h[1].cross(nodes_h[2]),
            nodes_h[2].cross(nodes_h[0]),
            nodes_h[0].cross(nodes_h[1])
        };
        // �������η�Χ��ÿһ��������ж�
        bool isHasDem = false;
        for (size_t iy = 0; iy < area_size[1]; iy++)
        {
            for (size_t ix = 0; ix < area_size[0]; ix++)
            {
                dem_test_times++;
                // ��ǰ����������
                Eigen::Vector3d cur(area_start[0] + ix * c_dem.resolution, area_start[1] + iy * c_dem.resolution, 1.0);
                // �жϵ�ǰ���Ƿ�����������
                int flag = 0; // ��ǰ��״̬�ı�����ֻ�е� flag = 7 ʱ���Ŵ���ǰ������������
                flag = ((cur.dot(arcs_h[0]) < 0) == (nodes_h[0].dot(arcs_h[0]) <= 0)) ? flag | 4 : flag;
                flag = ((cur.dot(arcs_h[1]) < 0) == (nodes_h[1].dot(arcs_h[1]) <= 0)) ? flag | 2 : flag;
                flag = ((cur.dot(arcs_h[2]) < 0) == (nodes_h[2].dot(arcs_h[2]) <= 0)) ? flag | 1 : flag;
                // ��������������ڣ��ڲ���߳�ֵ
                switch (flag)
                {
                case 7:
                {
                    isHasDem = true;
                    // �����ڲ����
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
                        // �ڲ�߳�ֵ
                        double elevation = coefficient.dot(Eigen::Vector3d(triangle->nodes[0].z(), triangle->nodes[1].z(), triangle->nodes[2].z()));
                        // DEM �����¼�߳�ֵ
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
            std::cout << "������" << iTriangle << "��û��DEM��" << std::endl;
        }
    }
    std::cout << "���Ե���������Ŀ��     " << triangle_test_times << std::endl;
    std::cout << "����DEM�����������Ŀ��" << triangle_hit_times << std::endl;
    std::cout << "�ܵ�DEM�����Ŀ��      " << c_dem.rows * c_dem.cols << std::endl;
    std::cout << "���Ե�DEM�����Ŀ��    " << dem_test_times << std::endl;
    std::cout << "���е�DEM�����Ŀ��    " << dem_hit_times << std::endl;
}
