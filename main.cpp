#include "renderer/renderer.h"
#include <iostream>
#include <SFML/Graphics.hpp>

void draw(const Renderer::Screen &screen) {
    int w = screen.w;
    int h = screen.h;
    sf::RenderWindow window(sf::VideoMode(w, h), "Renderer");

    auto *pixels = new sf::Uint8[w * h * 4];

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);


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

void example1() {
    // Инициализируем мир
    Renderer::World world;
    // Создаем и добавляем в мир красный отрезок, зеленую точку и синий треугольник
    auto sector = Renderer::Sector(Eigen::Vector3d {0.1, 0.5, -3},
                                   Eigen::Vector3d {-0.5, -0.5, -3},
                                   {255, 0, 0});
    auto point = Renderer::Point(Eigen::Vector3d {0, 0, -2},
                                 {0, 255, 0});
    auto triangle1 = Renderer::Triangle(Eigen::Vector3d {-1, -10, -5},
                                        Eigen::Vector3d {-1, 7, -5},
                                        Eigen::Vector3d {2, 3, -5},
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
    draw(screen);
}

void example2() {
    Renderer::World world;

    Eigen::Vector3d p1, p2, p3, p4, p5, p6;
    p1 = {1, 0, 0};
    p2 = {0, 1, 0};
    p3 = {-1, 0, 0};
    p4 = {0, -1, 0};
    p5 = {0, 0, -1};
    p6 = {0, 0, 1};

    Renderer::Object obj;
    obj.add_triangle({p1, p2, p6, {255, 255, 0}});
    obj.add_triangle({p2, p3, p6, {255, 0, 255}});
    obj.add_triangle({p3, p4, p6, {0, 255, 255}});
    obj.add_triangle({p1, p4, p6, {255, 0, 0}});
    obj.add_triangle({p1, p2, p5, {0, 255, 0}});
    obj.add_triangle({p2, p3, p5, {0, 0, 255}});
    obj.add_triangle({p3, p4, p5, {50, 255, 255}});
    obj.add_triangle({p1, p4, p5, {255, 70, 50}});

    obj.rotate_global({0, 1, 0}, 3);
    obj.translate({0, 0, -5});

    world.add_object(obj);

    // Переводим все объекты в координаты "куба зрения"
    auto view_box = world.make_view_box();

    // Создаем экран с разрешением 800 * 600 пикселей и отрисовываем все объекты на нем
    int w = 1024;
    int h = 1024;
    Renderer::Screen screen(w, h);
    view_box.map_to_pixels(screen);

    // Рисуем экран одним из доступных нам способов
    draw(screen);
}

void example3() {
    Renderer::World world;

    for (int i = 0; i < 10000; ++i) {
        world.add_triangle({Eigen::Vector3d {-1, -10, -1000},
                               Eigen::Vector3d {-1, 7, -1000},
                               Eigen::Vector3d {2, 3, -1000},
                               {0, 0, 255}});
    }
    // Переводим все объекты в координаты "куба зрения"
    auto view_box = world.make_view_box();

    // Создаем экран с разрешением 800 * 600 пикселей и отрисовываем все объекты на нем
    int w = 800;
    int h = 600;
    Renderer::Screen screen(w, h);
    view_box.map_to_pixels(screen);

    // Рисуем экран одним из доступных нам способов
    draw(screen);
}

void example4() {
    Renderer::World world;
    Renderer::Object overlapping;
    overlapping.add_triangle({Eigen::Vector3d{0.3, 1.0, -2.5},
                       {2.7, 0.85, -2},
                       {2.7, 1.15, -2 },
                       {255, 0, 0}});
    overlapping.add_triangle({Eigen::Vector3d{2.53, 0.71, -2.5},
                        {1.46, 2.86, -2},
                        {1.2, 2.71, -2 },
                        {0, 255, 0}});

    overlapping.add_triangle({Eigen::Vector3d{1.667, 2.79, -2.5},
                        {0.337, 0.786, -2},
                        {0.597, 0.636, -2 },
                        {0, 0, 255}});

    overlapping.translate({-1, -1, -5});
    world.add_object(overlapping);

    auto view_box = world.make_view_box();

    // Создаем экран с разрешением 800 * 600 пикселей и отрисовываем все объекты на нем
    int w = 1024;
    int h = 1024;
    Renderer::Screen screen(w, h);
    view_box.map_to_pixels(screen);

    // Рисуем экран одним из доступных нам способов
    draw(screen);
}

int main() {
    example1();
}
