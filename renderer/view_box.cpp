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
        std::cout << "Putting pixel " << i << " " << j << " " << z << "\n";
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
        if (i2 <= i1) {
            std::swap(i1, i2);
            std::swap(j1, j2);
        }

        double dz = s.p2_.z() - s.p1_.z();

        int dx = i2 - i1;
        int dy = j2 - j1;
        int j = j1;
        int d = 2 * dy - dx;

        for (int i = i1; i <= i2; ++i) {
            put_pixel(i, j, s.color_, (i - i1) * dz / (i2 - i1) + s.p1_.z());
            if (d > 0) {
                j += 1;
                d -= 2 * dx;
            }
            d += 2 * dy;
        }
    }


    void ViewBox::map_to_pixels(Renderer::Screen &screen) {
        for (const auto &point : points_) {
            screen.rasterize_and_put(point);
        }
        for (const auto &sector : sectors_) {
            screen.rasterize_and_put(sector);
        }
//        for (const auto &triangle : triangles_) {
//            screen.rasterize_and_put(triangle);
//        }
    }
}
