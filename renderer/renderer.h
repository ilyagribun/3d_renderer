#include "camera.h"
#include "objects.h"
#include "view_box.h"
#include <optional>

namespace Renderer {
    class World {
    public:
        Frustum frustum_;
        std::vector<Object> objects_;
        std::vector<Triangle> triangles_;
        std::vector<Point> points_;
        std::vector<Sector> sectors_;

        World();

        void add_triangle(Triangle triangle);

        void add_point(Point point);

        void add_sector(Sector sector);

        void add_object(Object object);

        ViewBox make_view_box();


        template <typename T>
        T project(T obj) const {
            return (obj.transform(frustum_.get_proj_matrix()));
        }

        std::optional<Point> clip_point(const Point& p) const;

        std::optional<Sector> clip_sector(Sector s) const;

        std::vector<Triangle> clip_triangle(const Triangle &t) const;
    };
}
