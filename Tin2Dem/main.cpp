#include <iostream>
#include <chrono>
#include "CDem.h"
#include "CTin.h"
#include "CTin2Dem.h"
#define _DEBUG

int main() {
	auto startTime = std::chrono::steady_clock::now();//计时开始

	std::string tin_path = "../data/facePointsTin.xyztri";
	std::string dem_path = "../result/dem.ddem";
	double resolution = 1.0;
	CTin c_tin;
	CDem c_dem;
	c_tin.loadTin(tin_path);
	CTin2Dem::initDem(c_dem, c_tin, resolution);
	CTin2Dem::convert2Dem(c_dem, c_tin);
	c_dem.save(dem_path);

	auto endTime = std::chrono::steady_clock::now();//计时结束
	double duration_second = std::chrono::duration<double>(endTime - startTime).count();//计算时间差
	std::cout << "总耗时:" << duration_second << "秒";//计时结果输出

	return 0;
}