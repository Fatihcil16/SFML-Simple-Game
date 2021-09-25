#include <Game.h>


int main(int argc, char const *argv[])
{
    //Init srand
    srand(static_cast<unsigned>(time(NULL)));

    //init Game Engine
    Game game;

    while (game.getWindowIsOpen() && !game.getEndGame())
    {
        //Update 
        game.update();
        //Render
        game.render();
    }
    

    return 0;
}
