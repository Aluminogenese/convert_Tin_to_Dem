#pragma once
#include "CDem.h"
#include "CTin.h"
class CTin2Dem
{
public:
	static void initDem(CDem& c_dem, const CTin& c_tin, const double& lf_resolution);
	static void convert2Dem(CDem& c_dem, CTin& c_tin);
};

