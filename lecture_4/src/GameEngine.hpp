#include "MovingColoredShapeWithText.hpp"
#include "fstream"
#include "iostream"

#ifndef MOVINGSHAPES_GAMEENGINE_HPP
#define MOVINGSHAPES_GAMEENGINE_HPP

using uint = unsigned int;

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

    uint m_windowWidth = 800;
    uint m_windowHeight = 600;
    sf::Font m_textFont;
    sf::Text  m_WindowDefaultText;
    std::vector<MovingColoredShapeWithText> m_MovingColorShapes{};
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
        float initX, initY, initSX, initSY;
        uint rColor, gColor, bColor;

        fin >> shapeName >> initX >> initY >> initSX >> initSY >> rColor >> gColor >> bColor;

        m_WindowDefaultText.setString(shapeName);
        auto shape = m_shapeCreationFunctions[option_name](fin);
        auto shapeColor = sf::Color(rColor, gColor, bColor);
        auto newShape = MovingColoredShapeWithText(
                shape,
                m_WindowDefaultText,
                shapeColor,
                {initX, initY},
                {initSX, initSY}
                );

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
    explicit Engine(const std::string& configFileName)
    {
        loadFromFile(configFileName);
    }

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
            // TODO think about the switch statement

            if (option_name == "Window")
            {
                parseWindowSettings(fin);
            }

            if (option_name == "Font")
            {
                parseFont(fin);
                parseFontSettings(fin);
            }

            // TODO this check looks ugly
            if (m_shapeCreationFunctions.count(option_name) > 0)
            {
                parseShape(fin, option_name);
            }
        }
    };
// TODO Should it be here?
    void checkWindowCollisionBounce()
    {
        for (auto& shape : m_MovingColorShapes)
        {
            sf::FloatRect shapeBounds = shape.getShape()->getGlobalBounds();
            auto leftX = shapeBounds.left;
            auto topY =  shapeBounds.top;
            auto rightX =  leftX + shapeBounds.width;
            auto bottomY = topY + shapeBounds.height;
            auto windowRightBound =  static_cast<float>(m_windowWidth);
            auto windowLeftBound =  0.0f;
            auto windowTopBound = 0.0f;
            auto windowBottomBound =  static_cast<float>(m_windowHeight);

            if (leftX < windowLeftBound || rightX  > windowRightBound)  shape.horizontalBounce();
            if (topY < windowTopBound  || bottomY > windowBottomBound) shape.verticalBounce();
        }

    }
};

#endif //MOVINGSHAPES_GAMEENGINE_HPP
