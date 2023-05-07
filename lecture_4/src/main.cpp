#include "MovingColoredShapeWithText.hpp"
#include "GameEngine.hpp"

int main()
{
    Engine engine("../config.txt");
    engine.createGameWindow();

    while (engine.gameWindow->isOpen())
    {

        engine.lookForEvents();
        engine.renderFrame();
        engine.updateFrame();
    }
    return 0;
}
