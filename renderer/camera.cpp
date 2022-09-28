//
// Created by Илья Григорьев on 27.09.2022.
//

#include "camera.h"
#include "objects.h"

namespace Renderer {
    Frustum::Frustum(double l, double r, double b, double t, double n, double f) : l_(l), r_(r), b_(b), t_(t),
                                                                                             n_(n), f_(f) {
        assert(l_ != r_);
        assert(b_ != t_);
        assert(n_ != f_);
        calculate_matrix();
    }

    void Frustum::change_params(double l, double r, double b, double t, double n, double f) {
        l_ = l;
        r_ = r;
        b_ = b;
        t_ = t;
        n_ = n;
        f_ = f;
    }

    void Frustum::calculate_matrix() {
        proj_matrix_ = Eigen::Matrix4d {
                {2 * n_ / (r_ - l_), 0, (r_ + l_) / (r_ - l_), 0},
                {0, 2 * n_ / (t_ - b_), (t_ + b_) / (t_ - b_), 0},
                {0, 0, -1. * (f_ + n_) / (f_ - n_), -2 * f_ * n_ / (f_ - n_)},
                {0, 0, -1., 0}
        };
    }

    Point Frustum::project(const Point &p) const {
        return p.transform(proj_matrix_);
    }

    Triangle Frustum::project(const Triangle &t) const {
        return t.transform(proj_matrix_);
    }

    Sector Frustum::project(const Sector &s) const {
        return s.transform(proj_matrix_);
    }

    Object Frustum::project(Object o) const {
        o.project(proj_matrix_);
        return o;
    }
}
