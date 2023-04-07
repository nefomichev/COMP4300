#include <utility>
#include "SFML/Graphics.hpp"
#include "fstream"
#include "map"
#include "vector"
#include "iostream"


class movingColoredShape
{
    std::shared_ptr<sf::Shape> m_shape;
    sf::Color m_color;
    float m_initPosX;
    float m_initPosY;
    float m_speedX;
    float m_speedY;
    std::string m_shapeName;
public:
    movingColoredShape(std::shared_ptr<sf::Shape> shape,
                       sf::Color color,
                       float initPosX,
                       float initPosY,
                       float speedX,
                       float speedY,
                       std::string name)
    : m_shape(std::move(shape)), m_color(color),
      m_initPosX(initPosX), m_initPosY(initPosY),
      m_speedX(speedX), m_speedY(speedY), m_shapeName(std::move(name))
    {
       // Filling shape with new properties
        m_shape->setFillColor(m_color);
        m_shape->setPosition(m_initPosX, m_initPosY);
        std::cout << "Moving ColorShape '" << m_shapeName << "' created" << std::endl;
    }

    void moveBySpeed()
    {
        m_shape->move(m_speedX, m_speedY);
    };

    void setSpeedX(float newSpeedX)
    {
        m_speedX = newSpeedX;
    };

    void setSpeedY(float newSpeedY)
    {
        m_speedY = newSpeedY;
    };

    auto getSpeedX() const
    {
        return m_speedX ;
    };

    auto getSpeedY() const
    {
        return m_speedY ;
    };

    auto getShape()
    {
        return m_shape;
    };
};

std::shared_ptr<sf::Shape> createCircle(std::ifstream& fin){
    float radius;
    fin >> radius;
    std::shared_ptr<sf::Shape> shape = std::make_shared<sf::CircleShape>(radius);
    return shape;
};

std::shared_ptr<sf::Shape> createRectangle(std::ifstream& fin){
    float width;
    float height;
    fin >> height >> width;
    std::shared_ptr<sf::Shape> shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(height, width));
    return shape;
};




class Engine
{
private:
    unsigned int m_windowWidth;
    unsigned int m_windowHeight;
    std::vector<movingColoredShape> m_shapes;
    std::map<std::string, std::function<std::shared_ptr<sf::Shape>(std::ifstream&)>> m_shapeCreationFunctionsMapping = {
            {"Circle",    createCircle},
            {"Rectangle", createRectangle}
    };

    void parseWindowSettings(std::ifstream& fin)
    {
        unsigned int windowWidth;
        unsigned int windowHeight;
        fin >> windowWidth >> windowHeight;

        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;
    };


    void parseShape(std::ifstream& fin, const std::string& option_name)
    {
        std::string shapeName;
        float initX;
        float initY;
        float initSX;
        float initSY;
        int rColor;
        int gColor;
        int bColor;
        std::shared_ptr<sf::Shape> shape;

        fin >> shapeName >> initX >> initY >> initSX >> initSY >> rColor >> gColor >> bColor;
        shape = m_shapeCreationFunctionsMapping[option_name](fin);
        sf::Color parsedColor(rColor, gColor, bColor);
        movingColoredShape newShape(shape, parsedColor, initX, initY, initSX, initSY, shapeName);
        m_shapes.push_back(newShape);
    }

public:
    Engine() = default;

    [[nodiscard]] const unsigned int& getWindowWidth() const
    {
        return m_windowWidth;
    }

    [[nodiscard]] const unsigned int& getWindowHeight() const
    {
        return m_windowHeight;
    }

    auto& getShapes()
    {
        return m_shapes;
    }

    void loadFromFile(const std::string& filename)
    {
        std::ifstream fin(filename);
        if (fin.fail())
        {
            std::cout << "Config file was not found" << std::endl;
        }

        std::string option_name;
        while (fin >> option_name)
        // TODO call different parse function depending on the option name in MAP ?
        {

            if (option_name == "Window")
            {
                std::cout << "Windows settings were found" << std::endl;
                parseWindowSettings(fin);
            }

            if (m_shapeCreationFunctionsMapping.count(option_name) > 0)
            {
                std::cout << "Shape was found" << std::endl;
                parseShape(fin, option_name);
            }

            // TODO add font parsing

        }
    };

    void checkBounce()
    {
        for (auto& shape : m_shapes)
        {
            // TODO use alias
            float leftX = shape.getShape()->getGlobalBounds().left;
            float rightX = shape.getShape()->getGlobalBounds().left + shape.getShape()->getGlobalBounds().width;
            float topY = shape.getShape()->getGlobalBounds().top;
            float bottomY = shape.getShape()->getGlobalBounds().top + shape.getShape()->getGlobalBounds().height;

            if (leftX < 0 || rightX  > m_windowWidth)  shape.setSpeedX(shape.getSpeedX() * -1);
            if (topY < 0  || bottomY > m_windowHeight) shape.setSpeedY(shape.getSpeedY() * -1);
        }

    }
};



int main()
{
    Engine e;
    e.loadFromFile("/Users/nefomichev/repos/COMP4300/lecture_4/config.txt");

    sf::RenderWindow window(sf::VideoMode(e.getWindowWidth(), e.getWindowHeight()), "My Window");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
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
            shape.moveBySpeed();
        }
        window.display();
        e.checkBounce();

        // TODO for all figures update positions and check for bounce
    }
    return 0;
}