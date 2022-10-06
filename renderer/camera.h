#pragma once
#include <Eigen/Core>
#include "objects.h"

namespace Renderer {
    class Frustum {
        double l_, r_, b_, t_, n_, f_;
        Plane left_, right_, top_, bottom_, far_, near_;
        Eigen::Matrix4d proj_matrix_;
    public:
        Frustum(double l, double r, double b, double t, double n, double f);

        void change_params(double l, double r, double b, double t, double n, double f);

        void calc_planes();

        void calculate_matrix();

        Eigen::Matrix4d get_proj_matrix() const;

        std::vector<Plane> get_planes() const;
    };
}