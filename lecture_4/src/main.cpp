#include "MovingColoredShapeWithText.hpp"
#include "GameEngine.hpp"

int main()
{
    Engine engine("../config.txt");

    sf::RenderWindow window(sf::VideoMode(engine.getWindowWidth(), engine.getWindowHeight()), "My Window");
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
        for (auto& shape : engine.getShapes())
        {
            window.draw(*shape.getShape());
            window.draw(shape.getShapeText());
            shape.move();
        }
        window.display();
        engine.checkWindowCollisionBounce();
    }
    return 0;
}
