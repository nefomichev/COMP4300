#include <utility>
#include "SFML/Graphics.hpp"
#include "fstream"
#include "map"
#include "vector"
#include "iostream"


class movingColoredShape
{
    std::shared_ptr<sf::Shape> m_shape;
    sf::Text m_shapeText;
    sf::Color m_shapeColor;

    float m_initPosX;
    float m_initPosY;
    float m_speedX;
    float m_speedY;

public:
    movingColoredShape(std::shared_ptr<sf::Shape> shape,
                       sf::Color color,
                       float initPosX,
                       float initPosY,
                       float speedX,
                       float speedY,
                       sf::Text ShapeText)
    : m_shape(std::move(shape)), m_shapeColor(color),
      m_initPosX(initPosX), m_initPosY(initPosY),
      m_speedX(speedX), m_speedY(speedY), m_shapeText(std::move(ShapeText))
    {
        m_shape->setFillColor(m_shapeColor);
        m_shape->setPosition(m_initPosX, m_initPosY);
        // Same for string
        putTextInShapeCenter();
        std::cout << "Moving ColorShape '" << m_shapeText.getString().toAnsiString() << "' created" << std::endl;
    }

    void moveBySpeed()
    {
        m_shape->move(m_speedX, m_speedY);
        m_shapeText.move(m_speedX, m_speedY);
    };

    void setSpeedX(float newSpeedX)
    {
        m_speedX = newSpeedX;
    };

    void putTextInShapeCenter()
    {
        sf::FloatRect shapeBounds = m_shape->getGlobalBounds();
        sf::FloatRect textBounds = m_shapeText.getGlobalBounds();

        // TODO understand why initial textBound is not in 0;0
        // Circles have the same
        m_shapeText.setPosition(m_initPosX + shapeBounds.width/2.0f - textBounds.width /2.0f - textBounds.left,
                                m_initPosY + shapeBounds.height/2.0f - textBounds.height / 2.0f - textBounds.top);
    };

    void setSpeedY(float newSpeedY)
    {
        m_speedY = newSpeedY;
    };

    [[nodiscard]] auto getSpeedX() const
    {
        return m_speedX ;
    };

    [[nodiscard]] auto getSpeedY() const
    {
        return m_speedY ;
    };

    auto getShape()
    {
        return m_shape;
    };

    auto getShapeText()
    {
        return m_shapeText;
    };
};

std::shared_ptr<sf::Shape> createCircle(std::ifstream& fin){
    float radius;
    fin >> radius;
    std::shared_ptr<sf::Shape> shape = std::make_shared<sf::CircleShape>(radius);
    return shape;
}

std::shared_ptr<sf::Shape> createRectangle(std::ifstream& fin){
    float width;
    float height;
    fin >> height >> width;
    std::shared_ptr<sf::Shape> shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(height, width));
    return shape;
}




class Engine
{
private:
    unsigned int m_windowWidth = 1920;
    unsigned int m_windowHeight = 1080;
    sf::Font m_font;
    sf::Color m_fontColor;
    unsigned int m_fontSize;
    sf::Text m_defaultText;
    std::vector<movingColoredShape> m_shapes;
    std::map<std::string, std::function<std::shared_ptr<sf::Shape>(std::ifstream&)>> m_shapeCreationFunctionsMapping = {
            {"Circle",    createCircle},
            {"Rectangle", createRectangle}
    };
    //TODO save Font information
    // How to pass this font into figure

    void parseWindowSettings(std::ifstream& fin)
    {
        unsigned int windowWidth;
        unsigned int windowHeight;
        fin >> windowWidth >> windowHeight;

        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;
    };

    void parseFontSetting(std::ifstream& fin)
    {
        std::string fontPath;
        unsigned int fontSize;
        int rColor;
        int gColor;
        int bColor;

        fin >> fontPath >> fontSize >> rColor >> gColor >> bColor;
        if (!m_font.loadFromFile(fontPath))
        {
            std::cout << "Font is not found" << std::endl;
        }
        else
        {
            m_fontColor = sf::Color(rColor, gColor, bColor);
            m_fontSize = fontSize;

            m_defaultText.setCharacterSize(m_fontSize);
            m_defaultText.setFillColor(m_fontColor);
            m_defaultText.setFont(m_font);
            m_defaultText.setString("Default String");
        }




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

        fin >> shapeName >> initX >> initY >> initSX >> initSY >> rColor >> gColor >> bColor;

        std::shared_ptr<sf::Shape> shape = m_shapeCreationFunctionsMapping[option_name](fin);
        sf::Color parsedColor(rColor, gColor, bColor);
        sf::Text shapeText = m_defaultText;
        shapeText.setString(shapeName);

        movingColoredShape newShape(shape, parsedColor, initX, initY, initSX, initSY, shapeText);
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

    auto& getDeafaultText()
    {
        return m_defaultText;
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

            if (option_name == "Font")
            {
                std::cout << "Font settings were found" << std::endl;
                parseFontSetting(fin);
            }

            if (m_shapeCreationFunctionsMapping.count(option_name) > 0)
            {
                std::cout << "Shape was found" << std::endl;
                parseShape(fin, option_name);
            }

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
    e.loadFromFile("/Users/nikita.fomichev/repos/COMP4300/lecture_4/config.txt");

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
            window.draw(shape.getShapeText());
            shape.moveBySpeed();
        }
        window.display();
        e.checkBounce();
    }
    return 0;
}
