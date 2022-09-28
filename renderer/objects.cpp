#include "objects.h"
#include "geometry.h"

#include <utility>

namespace Renderer {


    Triangle::Triangle(Eigen::Vector4d p1, Eigen::Vector4d p2, Eigen::Vector4d p3,
                       Color color)
            : p1_(normalize(std::move(p1))),
              p2_(normalize(std::move(p2))),
              p3_(normalize(std::move(p3))),
              color_(color) {}


    Triangle Triangle::transform(const Eigen::Matrix4d &m) const {
        return Triangle(transform_and_normalize(p1_, m),
                        transform_and_normalize(p2_, m),
                        transform_and_normalize(p3_, m),
                        color_);
    }

    Triangle::Triangle(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d p3, Color color)
            : p1_(vec3d_to_4d(std::move(p1))), p2_(vec3d_to_4d(std::move(p2))), p3_(vec3d_to_4d(std::move(p3))),
              color_(color) {}


    Point::Point(Eigen::Vector4d p, Color color) : p_(normalize(std::move(p))), color_(color) {}

    Point Point::transform(const Eigen::Matrix4d &m) const {
        return Point(transform_and_normalize(p_, m), color_);
    }

    Point::Point(Eigen::Vector3d p, Color color) : p_(vec3d_to_4d(std::move(p))), color_(color) {}

    Sector::Sector(Eigen::Vector4d p1, Eigen::Vector4d p2, Color color) : p1_(normalize(std::move(p1))),
                                                                                 p2_(normalize(std::move(p2))),
                                                                                 color_(color) {}

    Sector Sector::transform(const Eigen::Matrix4d &m) const {
        return Sector(transform_and_normalize(p1_, m),
                      transform_and_normalize(p2_, m),
                      color_);
    }

    Sector::Sector(Eigen::Vector3d p1, Eigen::Vector3d p2, Color color) : p1_(vec3d_to_4d(std::move(p1))),
                                                                                 p2_(vec3d_to_4d(std::move(p2))),
                                                                                 color_(color) {}


    Eigen::Vector3d Object::get_translate() {
        return Eigen::Vector3d {transform_matrix_(0, 3),
                                transform_matrix_(1, 3),
                                transform_matrix_(2, 3)};
    }

    Object::Object() {
        transform_matrix_.setIdentity();
    }

    void Object::add_point(Point point) {
        points_.emplace_back(std::move(point));
    }

    void Object::add_sector(Sector sector) {
        sectors_.emplace_back(std::move(sector));
    }

    void Object::add_triangle(Triangle triangle) {
        triangles_.emplace_back(std::move(triangle));
    }

    void Object::rotate_global(const Eigen::Vector3d &a, double theta) {
        transform_matrix_ = rotation_matrix(a, theta) * transform_matrix_;
    }

    void Object::rotate_local(const Eigen::Vector3d &a, double theta) {
        Eigen::Vector3d translate_vector = get_translate();
        transform_matrix_(0, 3) = 0;
        transform_matrix_(1, 3) = 0;
        transform_matrix_(2, 3) = 0;
        transform_matrix_ = rotation_matrix(a, theta) * transform_matrix_;
        transform_matrix_(0, 3) = translate_vector[0];
        transform_matrix_(1, 3) = translate_vector[1];
        transform_matrix_(2, 3) = translate_vector[2];
    }

    void Object::translate(const Eigen::Vector3d &t) {
        transform_matrix_(0, 3) += t[0];
        transform_matrix_(1, 3) += t[1];
        transform_matrix_(2, 3) += t[2];
    }

    Eigen::Matrix4d Object::get_transform() const {
        return transform_matrix_;
    }

    void Object::set_matrix(Eigen::Matrix4d m) {
        transform_matrix_ = std::move(m);
    }

    std::vector<Triangle> Object::get_triangles() const {
        std::vector<Triangle> result;
        for (auto & triangle : triangles_) {
            result.push_back(triangle.transform(transform_matrix_));
        }
        return result;
    }

    std::vector<Point> Object::get_points() const {
        std::vector<Point> result;
        for (auto & point : points_) {
            result.push_back(point.transform(transform_matrix_));
        }
        return result;
    }

    std::vector<Sector> Object::get_sectors() const {
        std::vector<Sector> result;
        for (auto & sector : sectors_) {
            result.push_back(sector.transform(transform_matrix_));
        }
        return result;
    }

    void Object::project(const Eigen::Matrix4d &proj) {
        transform_matrix_ = proj * transform_matrix_;
    }
}