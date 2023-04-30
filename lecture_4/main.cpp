#include "SFML/Graphics.hpp"
#include <ostream>
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
    MovingColoredShapeWithText(std::shared_ptr<sf::Shape>& shape,
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


class Engine
{
    using  ShapeLoaderFunction = std::function<std::shared_ptr<sf::Shape>(std::ifstream&)>;

    static std::shared_ptr<sf::Shape> loadCircle(std::ifstream& fin)
    // ShapeLoaderFunction
    {
        float radius;
        fin >> radius;
        return std::make_shared<sf::CircleShape>(radius);
    }

    static std::shared_ptr<sf::Shape> loadRectangle(std::ifstream& fin)
    // ShapeLoaderFunction
    {
        float width;
        float height;
        fin >> height >> width;
        return std::make_shared<sf::RectangleShape>(sf::Vector2f(height, width));
    }

private:
    uint m_windowWidth = 800;
    uint m_windowHeight = 600;
    sf::Font m_textFont;
    sf::Text  m_WindowDefaultText;
    std::vector<MovingColoredShapeWithText> m_MovingColorShapes;
    std::map<std::string, ShapeLoaderFunction> m_shapeCreationFunctions = {
            {"Circle",    loadCircle},
            {"Rectangle", loadRectangle}
    };


    void parseWindowSettings(std::ifstream& fin)
    {
        fin >> m_windowWidth >> m_windowHeight;
    };

    void parseFont(std::ifstream& fin)
    {
        std::string fontPath;
        fin >> fontPath;

        loadFont(fontPath);
    }

    void loadFont(const std::string& fontPath)
    {
        if (!m_textFont.loadFromFile(fontPath))
        {
            throw std::runtime_error("No Font was found");
        }
        else
        {
            m_WindowDefaultText.setFont(m_textFont);
        }
    }

    void parseFontSettings(std::ifstream& fin)
    {
        uint parsedFontSize, rColor, gColor, bColor;
        fin >> parsedFontSize >> rColor >> gColor >> bColor;
        auto fontColor = sf::Color(rColor, gColor, bColor);

        loadFontSettings(fontColor, parsedFontSize);
    }

    void loadFontSettings(const sf::Color& fontColor, uint fontSize)
    {
        try
        {
            m_WindowDefaultText.setCharacterSize(fontSize);
            m_WindowDefaultText.setFillColor(fontColor);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error setting font settings: " << e.what() << std::endl;
        }
    }

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
      
        std::shared_ptr<sf::Shape> shape = m_shapeCreationFunctions[option_name](fin);
        sf::Color shapeColor(rColor, gColor, bColor);
        m_WindowDefaultText.setString(shapeName);

        MovingColoredShapeWithText newShape(shape, m_WindowDefaultText, shapeColor, {initX, initY}, {initSX, initSY});
        m_WindowDefaultText.getGlobalBounds();
        m_MovingColorShapes.push_back(newShape);
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
        return m_MovingColorShapes;
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
                parseFont(fin);
                parseFontSettings(fin);
            }

            if (m_shapeCreationFunctions.count(option_name) > 0)
            {
                parseShape(fin, option_name);
            }
        }
    };

    void checkWindowCollisionBounce()
    {
        for (auto& shape : m_MovingColorShapes)
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
