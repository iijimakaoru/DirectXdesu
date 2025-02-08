#include "P_Defined.h"

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cmath>
#include<algorithm>

PHONON_SUPPRESS_WARNINGS_END

bool _Approximately(float a,float b)
{
	constexpr float P_EPSILON = 2.2204460492503131E-16f;

	float tmp = 1e-06f * std::max(std::abs(a),std::abs(b));

	float tmp2 = P_EPSILON * 8.0f;

	if ( std::abs(b - a) < std::max(tmp,tmp2) )
	{
		return true;
	}

	return false;
}