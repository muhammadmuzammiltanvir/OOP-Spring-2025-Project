#pragma once
#include "grid.h"
#include "blocks.cpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>  
#include <vector>
class Game{
public:
    Game();
    void Draw(sf::RenderWindow& window);
    void HandleInput(const sf::Event& event);
    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockDown();
    bool IsGameOver();
private:
    Grid grid;
    bool gameOver;
    bool paused;
    int score;
    int highScore;

    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;

    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    bool isBlockOutside();
    void Reset();
    void TogglePause();
    void DrawSidePanel(sf::RenderWindow& window);

    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();

    void LoadHighScore();
    void SaveHighScore();
    void ResetHighScore();

    sf::SoundBuffer moveBuffer;
    sf::SoundBuffer rotateBuffer;
    sf::SoundBuffer gameOverBuffer;

    sf::Sound moveSound;
    sf::Sound rotateSound;
    sf::Sound gameOverSound;
};