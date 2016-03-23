#ifndef KERNELS_H
#define KERNELS_H

#define _USE_MATH_DEFINES
#include <cmath>

inline float KernelPoly6(float x, float h)
{
	if (x < 0 || x > h) {
		return 0;
	}

	return (315 * pow(pow(h, 2) - pow(x, 2), 3)) / (64 * M_PI * pow(h, 9));
}

inline float KernelSpiky(float x, float h)
{

}

inline float KernelViscous(float x, float h)
{

}

#endif
