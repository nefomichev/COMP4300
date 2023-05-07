#include "SFML/Graphics.hpp"

#ifndef MOVINGSHAPES_SHAPE_HPP
#define MOVINGSHAPES_SHAPE_HPP

class MovingColoredShapeWithText
{
    std::shared_ptr<sf::Shape> m_shape;
    std::shared_ptr<sf::Text> m_shapeText;
    sf::Vector2f m_shapeSpeed;

    void setInitShapePosition(sf::Vector2f shapeInitPos)
    {
        m_shape->setPosition(shapeInitPos);
    }

    void setInitTextPosition(sf::Vector2f textInitPos)
    {
        m_shapeText->setPosition(textInitPos);
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

    static sf::Vector2f calculateCenteredTextPosition(sf::Vector2f shapeInitPos, sf::Vector2f indentToFitCenter)
    {
        return {shapeInitPos.x + indentToFitCenter.x,shapeInitPos.y + indentToFitCenter.y};

    }

    sf::FloatRect getShapeGlobalBounds()
    {
        return m_shape->getGlobalBounds();
    }

    sf::FloatRect getTextGlobalBounds()
    {
        return m_shapeText->getGlobalBounds();
    }

    void moveShape()
    {
        m_shape->move(m_shapeSpeed.x, m_shapeSpeed.y);
    }

    void moveText()
    {
        m_shapeText->move(m_shapeSpeed.x, m_shapeSpeed.y);
    }

public:
    MovingColoredShapeWithText(std::shared_ptr<sf::Shape>& shape,
                               std::shared_ptr<sf::Text>& shapeText,
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

    void windowBounce(const sf::Vector2u& windowSize)
    {
        sf::FloatRect shapeBounds = m_shape->getGlobalBounds();
        auto leftX = shapeBounds.left;
        auto topY =  shapeBounds.top;
        auto rightX =  leftX + shapeBounds.width;
        auto bottomY = topY + shapeBounds.height;
        auto windowRightBound =  static_cast<float>(windowSize.x);
        auto windowLeftBound =  0.0f;
        auto windowTopBound = 0.0f;
        auto windowBottomBound =  static_cast<float>(windowSize.y);

        if (leftX < windowLeftBound || rightX  > windowRightBound)  horizontalBounce();
        if (topY < windowTopBound  || bottomY > windowBottomBound)  verticalBounce();
    }
};
#endif //MOVINGSHAPES_SHAPE_HPP
