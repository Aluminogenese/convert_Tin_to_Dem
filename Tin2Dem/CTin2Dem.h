#pragma once
#include "CDem.h"
#include "CTin.h"
class CTin2Dem
{
public:
	/// <summary>
	/// 初始化DEM
	/// </summary>
	/// <param name="c_dem">CDem对象</param>
	/// <param name="c_tin">CTin对象</param>
	/// <param name="lf_resolution">DEM分辨率</param>
	static void initDem(CDem& c_dem, const CTin& c_tin, const double& lf_resolution);

	/// <summary>
	/// Tin转为DEM
	/// </summary>
	/// <param name="c_dem">CDem对象</param>
	/// <param name="c_tin">CTin对象</param>
	static void convert2Dem(CDem& c_dem, CTin& c_tin);
};

