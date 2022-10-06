#include "camera.h"
#include "objects.h"

namespace Renderer {
    Frustum::Frustum(double l, double r, double b, double t, double n, double f) : l_(l), r_(r), b_(b), t_(t),
                                                                                             n_(n), f_(f) {
        calc_planes();
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
        calc_planes();
        calculate_matrix();
    }

    void Frustum::calculate_matrix() {
        proj_matrix_ = Eigen::Matrix4d {
                {2 * n_ / (r_ - l_), 0, (r_ + l_) / (r_ - l_), 0},
                {0, 2 * n_ / (t_ - b_), (t_ + b_) / (t_ - b_), 0},
                {0, 0, -1. * (f_ + n_) / (f_ - n_), -2 * f_ * n_ / (f_ - n_)},
                {0, 0, -1., 0}
        };
    }

    void Frustum::calc_planes() {
        near_ = Plane({l_, b_, -1 * n_},
                      {l_, t_, -1 * n_},
                      {r_, b_, -1 * n_});
        left_ = Plane({0, 0, 0},
                      {l_, b_, -1 * n_},
                      {l_, t_, -1 * n_});
        right_ = Plane({0, 0, 0},
                      {r_, t_, -1 * n_},
                      {r_, b_, -1 * n_});
        top_ = Plane({0, 0, 0},
                      {l_, t_, -1 * n_},
                      {r_, t_, -1 * n_});
        bottom_ = Plane({0, 0, 0},
                      {r_, b_, -1 * n_},
                      {l_, b_, -1 * n_});
        far_ = Plane({l_, b_, -1 * f_},
                      {r_, b_, -1 * f_},
                      {l_, t_, -1 * f_});
    }

    Eigen::Matrix4d Frustum::get_proj_matrix() const {
        return proj_matrix_;
    }

    std::vector<Plane> Frustum::get_planes() const {
        return {near_, left_, right_, top_, bottom_, far_};
    }
}
