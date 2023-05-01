#include "MovingColoredShapeWithText.hpp"
#include "GameEngine.hpp"

int main()
{
    Engine e;
    e.loadFromFile("../config.txt");

    sf::RenderWindow window(sf::VideoMode(e.getWindowWidth(), e.getWindowHeight()), "My Window");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        for (auto& shape : e.getShapes())
        {
            window.draw(*shape.getShape());
            window.draw(shape.getShapeText());
            shape.move();
        }
        window.display();
        e.checkWindowCollisionBounce();
    }
    return 0;
}
