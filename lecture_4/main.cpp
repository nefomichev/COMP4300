#include "SFML/Graphics.hpp"
#include "fstream"
#include "map"
#include "vector"
#include "iostream"

class Engine
{
private:
    unsigned int m_windowWidth;
    unsigned int m_windowHeight;
    std::vector<std::shared_ptr<sf::Shape>> m_shapes;
    // TODO understand that

    void setWindowSettings(std::ifstream& fin)
    {
        unsigned int windowWidth;
        unsigned int windowHeight;
        fin >> windowWidth >> windowHeight;

        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;
    };

    void addCircle(float initX, float initY, int rColor, int gColor, int bColor, float radius)
    {
        // TODO understand this code
        std::shared_ptr<sf::Shape> circle = std::make_shared<sf::CircleShape>(radius);
        // TODO Need to add name
        circle->setPosition(initX, initY);
        circle->setFillColor(sf::Color(rColor, gColor, bColor));
        // TODO add speed property
        m_shapes.push_back(circle);
    };

    void parseCircle(std::ifstream& fin)
    {
        std::string shapeName;
        float initX;
        float initY;
        float initSX;
        float initSY;
        int rColor;
        int gColor;
        int bColor;
        float radius;
        fin >> shapeName >> initX >> initY >> initSX >> initSY >> rColor >> gColor >> bColor >> radius;

        // TODO start to parse speed somehow
        addCircle(initX, initY, rColor, gColor, bColor, radius);
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

    std::vector<std::shared_ptr<sf::Shape>>& getShapes()
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
        {
            // TODO call different parse function depending on the option name in MAP ?
            if (option_name == "Window")
            {
                std::cout << "Windows settings were found" << std::endl;
                setWindowSettings(fin);
            }
            // TODO add font parsing
            if (option_name == "Circle")
            {
                std::cout << "Circle was found" << std::endl;
                parseCircle(fin);
            }

            if (option_name == "Rectangle")
                std::cout << "Rectangle was found" << std::endl;
                // TODO do something
        }
    };

};



int main()
{
    Engine e;
    e.loadFromFile("/Users/nefomichev/repos/COMP4300/lecture_4/config.txt");
    sf::RenderWindow window(sf::VideoMode(e.getWindowWidth(), e.getWindowHeight()), "My Window");

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
            window.draw(*shape);
            // dereference
        }
        window.display();

        // TODO for all figures update positions and check for bounce
    }
    return 0;
}

/*
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Application");
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
     */
