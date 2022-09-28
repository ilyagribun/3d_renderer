#include <Eigen/Core>

namespace Renderer {
    Eigen::Vector4d normalize(Eigen::Vector4d v);

    Eigen::Vector4d vec3d_to_4d(Eigen::Vector3d v);

    Eigen::Vector3d vec4d_to_3d(Eigen::Vector4d v);

    Eigen::Vector4d transform_and_normalize(const Eigen::Vector4d &v, const Eigen::Matrix4d &m);

    Eigen::Matrix4d rotation_matrix(const Eigen::Vector3d &v, double theta);
}