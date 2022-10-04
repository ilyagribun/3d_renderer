//
// Created by Илья Григорьев on 28.09.2022.
//

#include "geometry.h"
#include <iostream>

namespace Renderer {
    Eigen::Vector4d normalize(Eigen::Vector4d v) {
        assert(v[3] != 0);
        v[0] /= v[3];
        v[1] /= v[3];
        v[2] /= v[3];
        v[3] = 1;
        return v;
    }

    Eigen::Vector4d transform_and_normalize(const Eigen::Vector4d &v, const Eigen::Matrix4d &m) {
        Eigen::Vector4d new_v = normalize(m * v);
        return new_v;
    }

    Eigen::Vector4d vec3d_to_4d(Eigen::Vector3d v) {
        return Eigen::Vector4d{v[0], v[1], v[2], 1};
    }

    Eigen::Vector3d vec4d_to_3d(Eigen::Vector4d v) {
        v = normalize(v);
        return Eigen::Vector3d{v[0], v[1], v[2]};
    }

    Eigen::Matrix4d rotation_matrix(const Eigen::Vector3d &v, double theta) {
        double c = cos(theta);
        double s = sin(theta);
        double x = v.x();
        double y = v.y();
        double z = v.z();
        return Eigen::Matrix4d {
            {c + (1-c)*x*x, (1-c)*x*y - s*z, (1-c)*x*z + s*y, 0},
            {(1-c)*x*y + s*z, c + (1-c)*y*y, (1-c)*y*z - s*x, 0},
            {(1-c)*x*z - s*y, (1-c)*y*z + s*x, c + (1-c)*z*z, 0},
            {0, 0, 0, 1}
        };
    }
}
