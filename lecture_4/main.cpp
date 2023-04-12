#include <utility>
#include "SFML/Graphics.hpp"
#include "fstream"
#include "map"
#include "vector"
#include "iostream"
#include <stdexcept>


class movingColoredShape
{
    std::shared_ptr<sf::Shape> m_shape;
    sf::Text m_shapeText;
    sf::Color m_shapeColor;

    float m_initPosX;
    float m_initPosY;
    float m_speedX;
    float m_speedY;

    void setInitShapePositionAndColor()
    {
        m_shape->setFillColor(m_shapeColor);
        m_shape->setPosition(m_initPosX, m_initPosY);
    };

    void putTextInShapeCenter()
    {
        sf::FloatRect shapeBounds = m_shape->getGlobalBounds();
        sf::FloatRect textBounds = m_shapeText.getGlobalBounds();

        // TODO understand why initial textBound is not in 0;0
        m_shapeText.setPosition(m_initPosX + shapeBounds.width/2.0f - textBounds.width /2.0f - textBounds.left,
                                m_initPosY + shapeBounds.height/2.0f - textBounds.height / 2.0f - textBounds.top);
    };

public:
    movingColoredShape(std::shared_ptr<sf::Shape> shape,
                       sf::Text ShapeText,
                       sf::Color color,
                       float initPosX,
                       float initPosY,
                       float speedX,
                       float speedY)
    : m_shape(std::move(shape)),
      m_shapeText(std::move(ShapeText)),
      m_shapeColor(color),
      m_initPosX(initPosX),
      m_initPosY(initPosY),
      m_speedX(speedX),
      m_speedY(speedY)
    {
        setInitShapePositionAndColor();
        putTextInShapeCenter();
    }

    void moveBySpeed()
    {
        m_shape->move(m_speedX, m_speedY);
        m_shapeText.move(m_speedX, m_speedY);
    };

    auto getShape()
    {
        return m_shape;
    };

    auto getShapeText()
    {
        return m_shapeText;
    };

    void horizontalBounce()
    {
        m_speedX *= -1;
    }

    void verticalBounce()
    {
        m_speedY *= -1;
    }
};

using  ShapeLoaderFunction = std::function<std::shared_ptr<sf::Shape>(std::ifstream&)>;

class Engine
{
    static std::shared_ptr<sf::Shape> createCircle(std::ifstream& fin){
        float radius;
        fin >> radius;
        return std::make_shared<sf::CircleShape>(radius);
    }

    static std::shared_ptr<sf::Shape> createRectangle(std::ifstream& fin){
        float width;
        float height;
        fin >> height >> width;
        return std::make_shared<sf::RectangleShape>(sf::Vector2f(height, width));
    }

private:
    uint m_windowWidth  = 800; //default
    uint m_windowHeight = 600;
    sf::Text  m_WindowDefaultText;
    sf::Font m_loadedFont;
    sf::Color m_fontColor;
    unsigned int m_fontSize;

    std::vector<movingColoredShape> m_MovingColorShapesVector;

    std::map<std::string, ShapeLoaderFunction> m_shapeCreationFunctionsMapping = {
            {"Circle",    createCircle},
            {"Rectangle", createRectangle}
    };

    void parseWindowSettings(std::ifstream& fin)
    {
        fin >> m_windowWidth >> m_windowHeight;
    };

    void parseFontSetting(std::ifstream& fin)
    {
        std::string fontPath;

        fin >> fontPath;
        if (!m_loadedFont.loadFromFile(fontPath))

        {
            throw std::runtime_error("No Font was found");
        }

        else

        {
            uint parsedFontSize;
            uint rColor;
            uint gColor;
            uint bColor;

            fin >> m_fontSize >> rColor >> gColor >> bColor;
            m_fontColor = sf::Color(rColor, gColor, bColor);
            // todo refactor
            m_WindowDefaultText.setFont(m_loadedFont);
            m_WindowDefaultText.setFillColor(m_fontColor);
            m_WindowDefaultText.setCharacterSize(m_fontSize);
        }

    };

    void parseShape(std::ifstream& fin, const std::string& option_name)
    {
        std::string shapeName;
        float initX;
        float initY;
        float initSX;
        float initSY;
        uint rColor;
        uint gColor;
        uint bColor;


        fin >> shapeName >> initX >> initY >> initSX >> initSY >> rColor >> gColor >> bColor;

        std::shared_ptr<sf::Shape> shape = m_shapeCreationFunctionsMapping[option_name](fin);
        sf::Color shapeColor(rColor, gColor, bColor);
        sf::Text shapeText = m_WindowDefaultText;
        shapeText.setString(shapeName);

        movingColoredShape newShape(shape, shapeText, shapeColor, initX, initY, initSX, initSY);
        m_MovingColorShapesVector.push_back(newShape);
    }

    static auto readConfigFromFile(const std::string& filename)
    {
        std::ifstream fin(filename);

        if (fin.fail())
        {
            throw std::runtime_error("Config was not found");
        }
        else
        {
            return fin;
        }
    }
public:
    Engine() = default;

    auto getWindowWidth() const
    {
        return m_windowWidth;
    }

    auto getWindowHeight() const
    {
        return m_windowHeight;
    }

    auto& getShapes()
    {
        return m_MovingColorShapesVector;
    }

    void loadFromFile(const std::string& filename)
    {
        std::string option_name;
        std::ifstream fin = readConfigFromFile(filename);

        while (fin >> option_name)
        {

            if (option_name == "Window")
            {
                parseWindowSettings(fin);
            }

            if (option_name == "Font")
            {
                parseFontSetting(fin);
            }

            if (m_shapeCreationFunctionsMapping.count(option_name) > 0)
            {
                parseShape(fin, option_name);
            }
        }
    };

    void checkWindowCollisionBounce()
    {
        for (auto& shape : m_MovingColorShapesVector)
        {
            sf::FloatRect shapeBounds = shape.getShape()->getGlobalBounds();
            float leftX = shapeBounds.left;
            float topY =  shapeBounds.top;
            float rightX =  leftX + shapeBounds.width;
            float bottomY = topY + shapeBounds.height;

            auto windowRightBound =  static_cast<float>(m_windowWidth);
            auto windowLeftBound =  0.0f;
            auto windowTopBound = 0.0f;
            auto windowBottomBound =  static_cast<float>(m_windowHeight);

            if (leftX < windowLeftBound || rightX  > windowRightBound)  shape.horizontalBounce();
            if (topY < windowBottomBound  || bottomY > windowTopBound) shape.verticalBounce();
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
        e.checkWindowCollisionBounce();
    }
    return 0;
}
