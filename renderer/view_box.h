#pragma once

#include "objects.h"
namespace Renderer {
    class Screen {
    public:
        Eigen::MatrixXi r, g, b;
        Eigen::MatrixXd z_buffer;
        int w, h;
        Screen(int w, int h);

        void put_pixel(int i, int j, Color c, double z);
        std::pair<int, int> rasterize_point(double x, double y) const;

        void rasterize_and_put(const Point &p);
        void rasterize_and_put(const Sector &s);
        void rasterize_and_put(const Triangle &t);
    };

    struct ViewBox {
        std::vector<Object> objects_;
        std::vector<Triangle> triangles_;
        std::vector<Point> points_;
        std::vector<Sector> sectors_;

    public:
        void map_to_pixels(Screen &screen);
    };

}
