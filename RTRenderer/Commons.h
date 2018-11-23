#pragma once

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

enum RenderFilter {
	R_STATIC, R_NSTATIC, R_ALL
};

class Commons {
public:
	template<class T>
	static T LSmoothStep(T v1, T v2, float step);
};

template<class T>
inline T Commons::LSmoothStep(T v1, T v2, float step)
{
	return v1 * (1.0f - step) + v2 * step;
}
