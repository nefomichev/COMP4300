#include "SFML/Graphics.hpp"
#include <utility>
#include "fstream"
#include "map"
#include "vector"
#include "iostream"
#include <stdexcept>

class MovingColoredShapeWithText
{
    std::shared_ptr<sf::Shape> m_shape;
    sf::Text m_shapeText;
    sf::Vector2f m_shapeSpeed;

    void setInitShapePosition(sf::Vector2f shapeInitPos)
    {
        m_shape->setPosition(shapeInitPos);
    }

    void setInitTextPosition(sf::Vector2f textInitPos)
    {
        m_shapeText.setPosition(textInitPos);
    }

    void setShapeColor(const sf::Color& parsedShapeColor)
    {
        m_shape->setFillColor(parsedShapeColor);
    }

    void putTextInShapeCenter(sf::Vector2f shapeInitPos)
    {
        auto indentToFitCenter = calculateTextCenteredIndent();
        auto centeredTextPosition = calculateCenteredTextPosition(shapeInitPos, indentToFitCenter);
        setInitTextPosition(centeredTextPosition);
    }

    sf::Vector2f calculateTextCenteredIndent()
    {
        auto shapeGlobalBounds = getShapeGlobalBounds();
        auto textGlobalBounds = getTextGlobalBounds();
        float xAxisIndent = shapeGlobalBounds.width/2.0f - textGlobalBounds.width /2.0f - textGlobalBounds.left;
        float yAxisIndent =  shapeGlobalBounds.height/2.0f - textGlobalBounds.height / 2.0f - textGlobalBounds.top;
        return {xAxisIndent, yAxisIndent};
    }

    sf::Vector2f calculateCenteredTextPosition(sf::Vector2f shapeInitPos, sf::Vector2f indentToFitCenter)
    {
        return {shapeInitPos.x + indentToFitCenter.x,shapeInitPos.y + indentToFitCenter.y};

    }

    sf::FloatRect getShapeGlobalBounds()
    {
        return m_shape->getGlobalBounds();
    }

    sf::FloatRect getTextGlobalBounds()
    {
        return m_shapeText.getGlobalBounds();
    }

    void moveShape()
    {
        m_shape->move(m_shapeSpeed.x, m_shapeSpeed.y);
    }

    void moveText()
    {
        m_shapeText.move(m_shapeSpeed.x, m_shapeSpeed.y);
    }

public:
    MovingColoredShapeWithText(std::shared_ptr<sf::Shape> shape,
                               sf::Text shapeText,
                               sf::Color parsedShapeColor,
                               sf::Vector2f shapeInitPos,
                               sf::Vector2f shapeInitSpeed)
    : m_shape(std::move(shape)),
      m_shapeText(std::move(shapeText)),
      m_shapeSpeed(shapeInitSpeed)
    {
        setInitShapePosition(shapeInitPos);
        setShapeColor(parsedShapeColor);
        putTextInShapeCenter(shapeInitPos);
    }

    void move()
    {
        moveShape();
        moveText();
    }

    auto getShape()
    {
        return m_shape;
    }

    auto getShapeText()
    {
        return m_shapeText;
    }

    void horizontalBounce()
    {
        m_shapeSpeed.x *= -1;
    }

    void verticalBounce()
    {
        m_shapeSpeed.y *= -1;
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
    uint m_fontSize = 14;
    sf::Text  m_WindowDefaultText;
    sf::Font m_loadedFont;
    sf::Color m_fontColor;

    std::vector<MovingColoredShapeWithText> m_MovingColorShapesVector;

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

        MovingColoredShapeWithText newShape(shape, shapeText, shapeColor, {initX, initY}, {initSX, initSY});
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
            if (topY < windowTopBound  || bottomY > windowBottomBound) shape.verticalBounce();
        }

    }
};



int main()
{
    Engine e;
    e.loadFromFile("../config.txt");

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
            shape.move();
        }
        window.display();
        e.checkWindowCollisionBounce();
    }
    return 0;
}
