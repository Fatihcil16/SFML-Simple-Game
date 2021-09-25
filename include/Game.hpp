#pragma once

#include <iostream>
#include <vector> 
#include <ctime>

#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>  
/*
    CLass that acts as the game engine
    wrapper class.

*/

class Game
{
private:
    //Variables
    //window
    sf::RenderWindow* window; 
    sf::VideoMode video_mode; 
    sf::Event ev;

    //Mouse positions
    sf::Vector2i mouse_pos;
    sf::Vector2f mouse_pos_view;
    //Resources
    sf::Font font;
    //text
    sf::Text uiText;
    //Game Logic
    bool endgame;
    unsigned points;
    int healt;
    float enemyspawn_timer;
    float enemyspawn_timer_max;
    int max_enemies;
    
    bool mouse_held;


    //GameObjects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;

    //private func
    void initVariables();
    void initWindow();
    void initFonts();
    void initEnemies();
    void initText();
public:
    ~Game();
    Game();

    const bool getWindowIsOpen() const;
    const bool getEndGame() const;
    
    void spawnEnemy();
    void pollEvents();
    void updateEnemies(); 
    void updateMousePositions();
    void updateText();
    void update();

    void renderText(sf::RenderTarget& target);
    void renderEnemies(sf::RenderTarget& target);
    void render();
};