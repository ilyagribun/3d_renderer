#pragma once
#include <Eigen/Core>
#include <vector>

namespace Renderer {
    struct Color {
        int r, g, b;
    };

    class Point {
    public:
        Eigen::Vector4d p_;
        Color color_;
        Point(Eigen::Vector4d p, Color color);
        Point(Eigen::Vector3d p, Color color);

        Point transform(const Eigen::Matrix4d &m) const;
    };

    class Sector {
    public:
        Eigen::Vector4d p1_, p2_;
        Color color_;
        Sector(Eigen::Vector4d p1, Eigen::Vector4d p2, Color color);
        Sector(Eigen::Vector3d p1, Eigen::Vector3d p2, Color color);

        Sector transform(const Eigen::Matrix4d &m) const;
    };

    class Triangle {
    public:
        Eigen::Vector4d p1_, p2_, p3_;
        Color color_;
        Triangle(Eigen::Vector4d p1, Eigen::Vector4d p2, Eigen::Vector4d p3, Color color);
        Triangle(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d p3, Color color);

        Triangle transform(const Eigen::Matrix4d &m) const;
    };

    class Object {
        Eigen::Matrix4d transform_matrix_;
        std::vector<Point> points_;
        std::vector<Sector> sectors_;
        std::vector<Triangle> triangles_;

        Eigen::Vector3d get_translate();
    public:
        Object();

        void add_point(Point point);
        void add_sector(Sector sector);
        void add_triangle(Triangle triangle);

        void rotate_local(const Eigen::Vector3d &a, double theta);
        void rotate_global(const Eigen::Vector3d &a, double theta);
        void translate(const Eigen::Vector3d &t);
        Eigen::Matrix4d get_transform() const;
        void set_matrix(Eigen::Matrix4d m);

        std::vector<Triangle> get_triangles() const;
        std::vector<Point> get_points() const;
        std::vector<Sector> get_sectors() const;

        void project(const Eigen::Matrix4d &proj);
    };
}
