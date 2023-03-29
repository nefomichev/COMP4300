#include <SFML/Graphics.hpp>

int main()
{
    const int windowWidth = 1920;
    const int windowHeight = 1080;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Application");
    window.setFramerateLimit(60);
    sf::CircleShape egg;
    float eggRadius = 40.f;
    egg.setRadius(eggRadius);
    float x = 100.f;
    float y = 100.f;
    float sx = 5.0f;
    float sy = 0.0f;
    egg.setPosition(x, y);
    egg.setFillColor(sf::Color::Cyan);


    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(egg);
        x += sx;
        egg.setPosition(x, y);

        if (x+2*eggRadius >= windowWidth || x < 0)
        {
            sx = -(sx);
        }

        window.display();
    }
}
