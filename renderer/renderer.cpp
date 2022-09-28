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
        for (auto &point : points_) {
            view_box.points_.emplace_back(frustum_.project(point));
        }
        for (auto &sector : sectors_) {
            view_box.sectors_.emplace_back(frustum_.project(sector));
        }
        for (auto &triangle : triangles_) {
            view_box.triangles_.emplace_back(frustum_.project(triangle));
        }

        for (auto object : objects_) {
            object = frustum_.project(object);
            auto new_points = object.get_points();
            view_box.points_.insert(view_box.points_.end(), new_points.begin(), new_points.end());
            auto new_sectors = object.get_sectors();
            view_box.sectors_.insert(view_box.sectors_.end(), new_sectors.begin(), new_sectors.end());
            auto new_triangles = object.get_triangles();
            view_box.triangles_.insert(view_box.triangles_.end(), new_triangles.begin(), new_triangles.end());
        }

        return view_box;
    }
}
