#include "renderer/renderer.h"
#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    // Инициализируем мир
    Renderer::World world;
    // Создаем и добавляем в мир красный отрезок, зеленую точку и синий треугольник
    auto sector = Renderer::Sector(Eigen::Vector3d {-0.5, -0.5, -3},
                                   Eigen::Vector3d {0.5, 0.5, -3},
                                   {255, 0, 0});
    auto point = Renderer::Point(Eigen::Vector3d {0, 0, -2},
                                 {0, 255, 0});
    auto triangle = Renderer::Triangle(Eigen::Vector3d {-1, -1, -5},
                                       Eigen::Vector3d {-0.5, 0, -8},
                                       Eigen::Vector3d {1, 1.5, -4},
                                       {0, 0, 255});
    world.add_point(point);
    world.add_sector(sector);
    world.add_triangle(triangle);

    // Переводим все объекты в координаты "куба зрения"
    auto view_box = world.make_view_box();

    // Создаем экран с разрешением 800 * 600 пикселей и отрисовываем все объекты на нем
    int h = 600;
    int w = 800;
    Renderer::Screen screen(w, h);
    view_box.map_to_pixels(screen);
    // Рисуем экран одним из доступных нам способов
    // TODO: screen.draw();
}
