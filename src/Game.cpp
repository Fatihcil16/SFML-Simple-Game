#include "Game.hpp"

//private functions
void Game::initVariables()
{
    this->window = nullptr;

    //game logic
    this->endgame = false;
    this->points = 0;
    this->healt = 10;
    this->enemyspawn_timer_max = 10.0f;
    this->enemyspawn_timer = this->enemyspawn_timer_max;
    this->max_enemies = 5;
    this->mouse_held = false;
}

void Game::initWindow()
{
    this->video_mode.height = 600;
    this->video_mode.width = 800;

    this->window = new sf::RenderWindow(this->video_mode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);
}

void Game::initFonts() 
{
   if(this->font.loadFromFile("../fonts/PixelGameFont.ttf"));
   {
       std::cout << "ERROR : FAILED TO LOAD FONT" << std::endl;
   }
}

void Game::initEnemies() 
{
    this->enemy.setPosition(10.0f, 10.0f);
    this->enemy.setSize(sf::Vector2f(100.0f, 100.0f));
    this->enemy.setFillColor(sf::Color::Red);
    //this->enemy.setOutlineColor(sf::Color::White);
    //this->enemy.setOutlineThickness(1.0f);
}

void Game::initText() 
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}

//cons dest
Game::Game()
{
    this->initVariables();
    this->initWindow(); 
    this->initFonts();
    this->initText();
    this->initEnemies(); 
}


Game::~Game() 
{
    delete this->window;
}

//Accessors

const bool Game::getWindowIsOpen() const
{
    return this->window->isOpen();
}

const bool Game::getEndGame() const
{
    return this->endgame;
}

//public functions

void Game::spawnEnemy()
{
    /*
        @return void
        Spawns enemies and sets their colors and positions
    */
    this->enemy.setPosition(static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)), 0);
    int type = rand() % 5;
    switch (type)
    {
    case 0:
        this->enemy.setSize(sf::Vector2f(10.0f, 10.0f));
        this->enemy.setFillColor(sf::Color::Red);
        break;
    case 1:
        this->enemy.setSize(sf::Vector2f(30.0f, 30.0f));
        this->enemy.setFillColor(sf::Color::Blue);
        break;
    case 2:
        this->enemy.setSize(sf::Vector2f(50.0f, 50.0f));
        this->enemy.setFillColor(sf::Color::Yellow);
        break;
    case 3:
        this->enemy.setSize(sf::Vector2f(70.0f, 70.0f));
        this->enemy.setFillColor(sf::Color::Green);
        break;
    case 4:
        this->enemy.setSize(sf::Vector2f(100.0f, 100.0f));
        this->enemy.setFillColor(sf::Color::Cyan);
        break;

    default:
            this->enemy.setSize(sf::Vector2f(100.0f, 100.0f));
        this->enemy.setFillColor(sf::Color::Cyan);
        break;
    }
    this->enemies.push_back(this->enemy);
}
    

void Game::pollEvents()
{
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        
        default:
            break;
        }
    }
    
}

void Game::updateEnemies() 
{
    /*
        @return void 
        Updates the enemy spawn timer and spawns enemies
    */
    if (this->enemies.size() < this->max_enemies) {
        if (this->enemyspawn_timer >= this->enemyspawn_timer_max)
        {
            this->spawnEnemy();
            this->enemyspawn_timer = 0.0f;
        }
        else
        {
            this->enemyspawn_timer += 1.f;
        }
    }

    //Move and update the enemies 
    for (int i = 0; i < this->enemies.size(); i++)
    {
        this->enemies[i].move(0.f, 5.f);

        if (this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->healt -= 1;
        }
  }
    //check if click upon
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {    
        if (this->mouse_held == false)
        {
            this->mouse_held = true;
            bool deleted = false;
            for (size_t i = 0; i < this->enemies.size() && !deleted; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mouse_pos_view))
                {
                    //Gain Points and delete enemy
                    deleted = true;
                    if (this->enemies[i].getFillColor() == sf::Color::Red)
                        this->points += 10;
                    else if (this->enemies[i].getFillColor() == sf::Color::Blue)
                        this->points += 7;
                    else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
                        this->points += 5;
                    else if (this->enemies[i].getFillColor() == sf::Color::Green)
                        this->points += 3;
                    else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
                        this->points += 1;
                    this->enemies.erase(this->enemies.begin() + i);
                }
            }
        }
        else 
        {
            this->mouse_held = false;
        }
        
    }

}

void Game::updateMousePositions() 
{
    /*
        @return void
        Updates mouse positions:
        - Mouse positions relative to window (Vector2i)
    */
    this->mouse_pos = sf::Mouse::getPosition(*this->window);
    this->mouse_pos_view = this->window->mapPixelToCoords(this->mouse_pos);
}

void Game::updateText() 
{
    std::stringstream ss;
    ss << "POINTS : " << this->points << "\n" << "HEALTH : " << this->healt << "\n";
    this->uiText.setString(ss.str());
}

void Game::update() 
{
    this->pollEvents();
    if (!this->endgame)
    {
        this->updateMousePositions();
        this->updateText();
        this->updateEnemies();
    }
    if (this->healt <= 0)
    {
        this->endgame = true;
    }
    
}

void Game::renderText(sf::RenderTarget& target) 
{
   target.draw(this->uiText);    
}

void Game::renderEnemies(sf::RenderTarget& target) 
{
    for(auto &e : this->enemies)
    {
        target.draw(e);
    }
}

void Game::render() 
{
    /*
        @return void
        - clear old frame
        - render objects
        - display frame in window 
        REnders the Game Objects
    */
    this->window->clear();

    this->renderEnemies(*this->window);
    this->renderText(*this->window);
    this->window->display();
}
