#include "renderer/renderer.h"
#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    // Инициализируем мир
    Renderer::World world;
    // Создаем и добавляем в мир красный отрезок, зеленую точку и синий треугольник
    auto sector = Renderer::Sector(Eigen::Vector3d {0.1, 0.5, -3},
                                   Eigen::Vector3d {-0.5, -0.5, -3},
                                   {255, 0, 0});
    auto point = Renderer::Point(Eigen::Vector3d {0, 0, -2},
                                 {0, 255, 0});
    auto triangle1 = Renderer::Triangle(Eigen::Vector3d {-1, -1, -5},
                                       Eigen::Vector3d {-0.5, 0, -5},
                                       Eigen::Vector3d {1, 1.5, -5},
                                       {0, 0, 255});
    auto triangle2 = Renderer::Triangle(Eigen::Vector3d {-0.5, -0.5, -4},
                                        Eigen::Vector3d {-0.5, 0, -4},
                                        Eigen::Vector3d {1, 1.5, -4},
                                        {255, 255, 0});

    world.add_point(point);
    world.add_sector(sector);

    world.add_triangle(triangle1);
    world.add_triangle(triangle2);


    // Переводим все объекты в координаты "куба зрения"
    auto view_box = world.make_view_box();

    // Создаем экран с разрешением 800 * 600 пикселей и отрисовываем все объекты на нем
    int w = 800;
    int h = 600;
    Renderer::Screen screen(w, h);
    view_box.map_to_pixels(screen);

    // Рисуем экран одним из доступных нам способов
    sf::RenderWindow window(sf::VideoMode(w, h), "Test renderer");
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        auto *pixels = new sf::Uint8[w * h * 4];

        sf::Texture texture;
        texture.create(w, h);

        sf::Sprite sprite(texture); // needed to draw the texture on screen

        for (int i = 0; i < w * h * 4; i += 4) {
            pixels[i] = screen.r(i / 4); // obviously, assign the values you need here to form your color
            pixels[i + 1] = screen.g(i / 4);
            pixels[i + 2] = screen.b(i / 4);
            pixels[i + 3] = 200;
        }

        texture.update(pixels);

        window.draw(sprite);

        window.display();
    }
}
