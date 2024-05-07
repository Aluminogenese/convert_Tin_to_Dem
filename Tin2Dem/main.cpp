#include <iostream>
#include <chrono>
#include "CDem.h"
#include "CTin.h"
#include "CTin2Dem.h"
#define _DEBUG

int main() {
	auto startTime = std::chrono::steady_clock::now();

	std::string tin_path = "../data/facePointsTin.xyztri";
	std::string dem_path = "../result/dem.ddem";
	double resolution = 1.0;
	CTin c_tin;
	CDem c_dem;
	if (c_tin.loadTin(tin_path)) {
		CTin2Dem::initDem(c_dem, c_tin, resolution);
		CTin2Dem::convert2Dem(c_dem, c_tin);
		c_dem.save(dem_path);
	}

	auto endTime = std::chrono::steady_clock::now();
	double duration_second = std::chrono::duration<double>(endTime - startTime).count();
	std::cout << "×ÜºÄÊ±:" << duration_second << "Ãë";

	return 0;
}