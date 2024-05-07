#pragma once
#include "CDem.h"
#include "CTin.h"
class CTin2Dem
{
public:
	/// <summary>
	/// ��ʼ��DEM
	/// </summary>
	/// <param name="c_dem">CDem����</param>
	/// <param name="c_tin">CTin����</param>
	/// <param name="lf_resolution">DEM�ֱ���</param>
	static void initDem(CDem& c_dem, const CTin& c_tin, const double& lf_resolution);

	/// <summary>
	/// TinתΪDEM
	/// </summary>
	/// <param name="c_dem">CDem����</param>
	/// <param name="c_tin">CTin����</param>
	static void convert2Dem(CDem& c_dem, CTin& c_tin);
};

