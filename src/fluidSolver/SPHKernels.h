#ifndef KERNELS_H
#define KERNELS_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>
#include <thirdparty/easylogging++.h>

inline float KernelPoly6(float x, float h)
{
    if (x < 0 || x > h) {
        return 0;
    }

    if (h <= 0) {
        return 0;
    }
    /// @todo: cache h2 and x2
    float result = (315 * pow(h * h - x * x, 3.0f)) / (64.0f * M_PI * pow(h, 9.0f));
    return result < 0.0f ? 0.0f : result;
}

inline float KernelSpiky(float x, float h)
{
    if (x < 0 || x > h) {
        return 0;
    }

    if (h <= 0) {
        return 0;
    }

    float result = (15.0f * pow(h - x, 3.0f)) / (M_PI * pow(h, 6.0f));
    return result < 0.0 ? 0.0f : result;
}

inline glm::vec3 GradKernelSpiky(glm::vec3 x_vec, float x, float h)
{
    if (x <= 0 || x > h) {
        return glm::vec3(0.0f);
    }

    if (h <= 0) {
        return glm::vec3(0.0f);
    }

    float scalarPart = -45.0f * (h - x) * (h - x) / (M_PI * pow(h, 6.0f));
    return scalarPart * (x_vec) / x;
}

inline float LaplacianKernelViscous(float x, float h)
{
    return 45.0f * (h - x) / (M_PI * pow(h, 6.0f));
}

inline float KernelViscous(float x, float h)
{
    if (x <= 0 || x > h) {
        return 0;
    }

    if (h <= 0) {
        return 0;
    }

    return (15 *
        (
            -(pow(x, 3) / (2 * pow(h, 3))) +
            pow(x, 2) / (pow(h, 2)) +
            h / (2 * x) -
            1
        )) /
        (2 * M_PI * pow(h, 3));
}

#endif
