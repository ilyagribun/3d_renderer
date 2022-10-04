//
// Created by Илья Григорьев on 28.09.2022.
//

#include "view_box.h"
#include <iostream>

namespace Renderer {
    Screen::Screen(int w, int h): w(w), h(h) {
        r = Eigen::MatrixXi(w, h).setZero();
        g = Eigen::MatrixXi(w, h).setZero();
        b = Eigen::MatrixXi(w, h).setZero();
        z_buffer = Eigen::MatrixXd(w, h).setConstant(2);
    }

    std::pair<int, int> Screen::rasterize_point(double x, double y) const {
        assert(x <= 1 && x >= -1);
        assert(y <= 1 && y >= -1);
        int i, j;
        if (x == 1) {
            i = w - 1;
        } else {
            i = int((x + 1.)/2 * w);
        }
        if (y == 1) {
            j = h - 1;
        } else {
            j = int((y + 1.)/2 * h);
        }
        return std::pair<int, int>(i, j);
    }

    void Screen::put_pixel(int i, int j, Color c, double z) {
        if (z < z_buffer(i, j)) {
            z_buffer(i, j) = z;
            r(i, j) = c.r;
            g(i, j) = c.g;
            b(i, j) = c.b;
        }
    }

    void Screen::rasterize_and_put(const Point &p) {
        auto [i, j] = rasterize_point(p.p_.x(), p.p_.y());
        put_pixel(i, j, p.color_, p.p_.z());
    }

    void Screen::rasterize_and_put(const Sector &s) {
        auto [i1, j1] = rasterize_point(s.p1_.x(), s.p1_.y());
        auto [i2, j2] = rasterize_point(s.p2_.x(), s.p2_.y());
        if (abs(i2 - i1) >= abs(j2 - j1)) {
            if (i2 <= i1) {
                std::swap(i1, i2);
                std::swap(j1, j2);
            }

            double dz = s.p2_.z() - s.p1_.z();

            int dx = i2 - i1;
            int dy = j2 - j1;
            int j = j1;
            int d = 2 * dy - dx;
            int yi = 1;
            if (dy < 0) {
                yi = -1;
                dy = -dy;
            }
            for (int i = i1; i <= i2; ++i) {
                put_pixel(i, j, s.color_, (i - i1) * dz / (i2 - i1) + s.p1_.z());
                if (d > 0) {
                    j += yi;
                    d += 2 * (dy - dx);
                } else {
                    d += 2 * dy;
                }
            }
        } else {
            if (j2 <= j1) {
                std::swap(i1, i2);
                std::swap(j1, j2);
            }

            double dz = s.p2_.z() - s.p1_.z();

            int dx = i2 - i1;
            int dy = j2 - j1;
            int i = i1;
            int d = 2 * dx - dy;
            int xi = 1;
            if (dx < 0) {
                xi = -1;
                dx = -dx;
            }
            for (int j = j1; j <= j2; ++j) {
                put_pixel(i, j, s.color_, (j - j1) * dz / (j2 - j1) + s.p1_.z());
                if (d > 0) {
                    i += xi;
                    d += 2 * (dx - dy);
                } else {
                    d += 2 * dx;
                }
            }
        }
    }

    void Screen::rasterize_and_put(const Triangle &t) {
        auto [i1, j1] = rasterize_point(t.p1_.x(), t.p1_.y());
        auto [i2, j2] = rasterize_point(t.p2_.x(), t.p2_.y());
        auto [i3, j3] = rasterize_point(t.p3_.x(), t.p3_.y());
        Screen tmp_screen(w, h);
        tmp_screen.rasterize_and_put(Sector(t.p1_, t.p2_, t.color_));
        tmp_screen.rasterize_and_put(Sector(t.p2_, t.p3_, t.color_));
        tmp_screen.rasterize_and_put(Sector(t.p3_, t.p1_, t.color_));

        int left = std::min(std::min(i1, i2), i3);
        int right = std::max(std::max(i1, i2), i3);
        int bottom = std::min(std::min(j1, j2), j3);
        int top = std::max(std::max(j1, j2), j3);

        for (int i = left; i < right; ++i) {
            int from = -1;
            int to = -1;
            for (int j = bottom; j < top; ++j) {
                if (tmp_screen.z_buffer(i, j) < 2) {
                    if (from == -1) {
                        from = j;
                    }
                    to = j;
                }
            }
            if (from != -1) {
                for (int j = from; j <= to; ++j) {
                    double dz;
                    if (from == to) {
                        dz = 1;
                    } else {
                        dz = ((j - from) * 1.0) / (from - to);
                    }
                    put_pixel(i, j, {tmp_screen.r(i, from), tmp_screen.g(i, from), tmp_screen.b(i, from)},
                              tmp_screen.z_buffer(i, from) * dz + tmp_screen.z_buffer(i, to) * (1 - dz));
                }
            }
        }
    }


    void ViewBox::map_to_pixels(Renderer::Screen &screen) {
        for (const auto &point : points_) {
            screen.rasterize_and_put(point);
        }
        for (const auto &sector : sectors_) {
            screen.rasterize_and_put(sector);
        }
        for (const auto &triangle : triangles_) {
            screen.rasterize_and_put(triangle);
        }
    }
}
