#include "renderer.h"
#include "objects.h"
#include "view_box.h"

#include <iostream>

namespace Renderer {
    World::World(): frustum_(-1, 1, -1, 1, 1, 10) {}

    void World::add_triangle(Triangle triangle) {
        triangles_.emplace_back(std::move(triangle));
    }

    void World::add_point(Point point) {
        points_.emplace_back(std::move(point));
    }

    void World::add_sector(Sector sector) {
        sectors_.emplace_back(std::move(sector));
    }

    void World::add_object(Object object) {
        objects_.emplace_back(std::move(object));
    }

    ViewBox World::make_view_box() {
        ViewBox view_box;

        for (const auto &object : objects_) {
            auto new_points = object.get_points();
            points_.insert(points_.end(), new_points.begin(), new_points.end());
            auto new_sectors = object.get_sectors();
            sectors_.insert(sectors_.end(), new_sectors.begin(), new_sectors.end());
            auto new_triangles = object.get_triangles();
            triangles_.insert(triangles_.end(), new_triangles.begin(), new_triangles.end());
        }

        for (auto &point : points_) {
            auto p = clip_point(point);
            if (p) {
                view_box.points_.push_back(project(*p));
            }
        }
        for (auto &sector : sectors_) {
            auto s = clip_sector(sector);
            if (s) {
                view_box.sectors_.push_back(project(*s));
            }
        }
        for (auto &triangle : triangles_) {
            auto triangulated = clip_triangle(triangle);
            for (const auto& t : triangulated) {
                view_box.triangles_.push_back(project(t));
            }
        }
        return view_box;
    }

    std::optional<Point> World::clip_point(const Point& p) const {
        for (const auto& plane : frustum_.get_planes()) {
            if (plane.distance(p.p_) < 0) {
                return {};
            }
        }
        return p;
    }

    std::optional<Sector> World::clip_sector(Sector s) const {
        for (const auto& plane : frustum_.get_planes()) {
            if (plane.distance(s.p1_) < 0 && plane.distance(s.p2_) < 0) {
                return {};
            }
            if (plane.distance(s.p1_) >= 0 && plane.distance(s.p2_) >= 0) {
                continue;
            }
            Eigen::Vector4d new_point = plane_line_intersection(Line(s), plane);
            if (plane.distance(s.p1_) < 0) {
                s = Sector(new_point, s.p2_, s.color_);
            } else {
                s = Sector(s.p1_, new_point, s.color_);
            }
        }
        return s;
    }

    std::vector<Triangle> World::clip_triangle(const Triangle& t) const {
        std::vector<Triangle> triangulated{t};
        for (const auto &plane : frustum_.get_planes()) {
            std::vector<Triangle> tmp;
            for (const auto& triangle : triangulated) {
                auto p1_dist = plane.distance(triangle.p1_);
                auto p2_dist = plane.distance(triangle.p2_);
                auto p3_dist = plane.distance(triangle.p3_);
                // check three cases:
                // triangle inside the plane
                if (p1_dist >= 0 && p2_dist >= 0 && p3_dist >= 0) {
                    tmp.push_back(triangle);
                } else if (p1_dist < 0 && p2_dist < 0 && p3_dist < 0) {
                    continue;
                } else if (p1_dist * p2_dist * p3_dist <= 0) { // vertex p1 outside
                    Eigen::Vector4d p1, p2, p3;
                    if (p1_dist < 0) {
                        p1 = triangle.p1_;
                        p2 = triangle.p2_;
                        p3 = triangle.p3_;
                    } else if (p2_dist < 0) {
                        p1 = triangle.p2_;
                        p2 = triangle.p3_;
                        p3 = triangle.p1_;
                    } else if (p3_dist < 0) {
                        p1 = triangle.p3_;
                        p2 = triangle.p1_;
                        p3 = triangle.p2_;
                    }
                    auto ab1 = clip_sector({p3, p1, triangle.color_});
                    auto b2c = clip_sector({p1, p2, triangle.color_});
                    tmp.emplace_back(p3, ab1->p2_, b2c->p1_, triangle.color_);
                    tmp.emplace_back(p3, b2c->p1_, p2, triangle.color_);
                } else {
                    Eigen::Vector4d p1, p2, p3;
                    if (p1_dist >= 0) {
                        p1 = triangle.p1_;
                        p2 = triangle.p2_;
                        p3 = triangle.p3_;
                    } else if (p2_dist >= 0) {
                        p1 = triangle.p2_;
                        p2 = triangle.p3_;
                        p3 = triangle.p1_;
                    } else if (p3_dist >= 0) {
                        p1 = triangle.p3_;
                        p2 = triangle.p1_;
                        p3 = triangle.p2_;
                    }
                    auto ab = clip_sector({p1, p2, triangle.color_});
                    auto ca = clip_sector({p3, p1, triangle.color_});
                    tmp.emplace_back(ab->p1_, ab->p2_, ca->p1_, triangle.color_);
                }
            }
            triangulated = tmp;
        }
        return triangulated;
    }

    void World::change_frustum_params(double l, double r, double b, double t, double n, double f) {
        frustum_.change_params(l, r, b, t, n, f);
    }
}
