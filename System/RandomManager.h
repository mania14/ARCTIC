/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */

#pragma once
#include "../CommonUtil/Singletone.h"
#include "../CommonUtil/Math.h"

class RandomManager : public Singletone<RandomManager>
{
public:
	void InitWELLRNG512a();
	double WELLRNG512a(void);

	acm::float3 RandomNormal();
};