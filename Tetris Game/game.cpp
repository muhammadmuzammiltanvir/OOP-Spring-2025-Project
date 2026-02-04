#include "game.h"
#include <random>
#include <fstream>
#include <SFML/Graphics.hpp>
Game::Game(){
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    grid.ClearFullRows();
    gameOver = false;
    paused = false;
    score = 0;
    LoadHighScore();
    if (!moveBuffer.loadFromFile("assets/move.wav")) {
    }
    moveSound.setBuffer(moveBuffer);
    if (!rotateBuffer.loadFromFile("assets/rotate.wav")) {
    }
    rotateSound.setBuffer(rotateBuffer);
    if (!gameOverBuffer.loadFromFile("assets/gameover.wav")) {
    }
    gameOverSound.setBuffer(gameOverBuffer);
}
Block Game::GetRandomBlock(){
    if (blocks.empty()){
        blocks=GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks() {
    return { IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock() };
}

void Game::Draw(sf::RenderWindow& window) {
    grid.Draw(window);

    if (!gameOver) {
        currentBlock.Draw(window);
    }

    DrawSidePanel(window);

    if (paused && !gameOver) {
        currentBlock.Draw(window);
    }
}

void Game::HandleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (gameOver) {
            if (event.key.code == sf::Keyboard::Enter) {
                gameOver = false;
                Reset();
            }
            return;
        }

        switch (event.key.code) {
            case sf::Keyboard::Left:
                if (!paused) MoveBlockLeft();
                break;
            case sf::Keyboard::Right:
                if (!paused) MoveBlockRight();
                break;
            case sf::Keyboard::Down:
                if (!paused) MoveBlockDown();
                break;
            case sf::Keyboard::Up:
                if (!paused) RotateBlock();
                break;
            case sf::Keyboard::P:
                TogglePause();
                break;
            case sf::Keyboard::R:
                Reset();
                break;
            case sf::Keyboard::H:
                ResetHighScore();
                break;
            default:
                break;
        }
    }
}

void Game::MoveBlockLeft() {
    currentBlock.Move(0,-1);
    if(isBlockOutside() || !BlockFits()) {
        currentBlock.Move(0,1);
    } else {
        moveSound.play();
    }
}
void Game::MoveBlockRight() {
    currentBlock.Move(0, 1);
    if (isBlockOutside() || !BlockFits()) {
        currentBlock.Move(0,-1);
    } else {
        moveSound.play();
    }
}
void Game::MoveBlockDown() {
    if (gameOver || paused) return;

    currentBlock.Move(1, 0);
    if (isBlockOutside() || !BlockFits()) {
        currentBlock.Move(-1, 0);
        LockBlock();
    }
}
void Game::RotateBlock() {
    currentBlock.Rotate();
    if (isBlockOutside() || !BlockFits()) {
        currentBlock.UndoRotation();
    } else {
        rotateSound.play();
    }
}
bool Game::isBlockOutside(){
    std::vector<Position> tiles=currentBlock.GetCellPositions();
    for(Position item:tiles){
        if(grid.IsCellOutside(item.row, item.column)) {
            return true;
        }
    }
    return false;
}
bool Game::BlockFits(){
    std::vector<Position> tiles=currentBlock.GetCellPositions();
    for (Position item : tiles){
        if(item.row<0) continue;
        if(!grid.IsCellEmpty(item.row, item.column)){
            return false;
        }
    }
    return true;
}
void Game::LockBlock(){
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles) {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    int clearedRows = grid.ClearFullRows();
    score += clearedRows * 100;
    if (score > highScore) {
        highScore = score;
        SaveHighScore();
    }
    currentBlock = nextBlock;
    nextBlock = GetRandomBlock();
    if (!BlockFits()) {
        for (Position p : currentBlock.GetCellPositions()) {
            if (p.row < 1){ 
                gameOver = true;
                gameOverSound.play();
                break;
            }
        }
    }    
}

void Game::Reset() {
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
    paused = false;
    gameOver = false;
}

void Game::TogglePause() {
    if (!gameOver) paused = !paused;
}

bool Game::IsGameOver() {
    return gameOver;
}

void Game::DrawSidePanel(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) return;

    float x = 350;
    float spacing = 40;  
    sf::RectangleShape scoreBox(sf::Vector2f(130, 60));
    scoreBox.setFillColor(sf::Color(50, 50, 150));
    scoreBox.setPosition(x, 30);
    window.draw(scoreBox);

    sf::Text scoreLabel("Score", font, 18);
    scoreLabel.setFillColor(sf::Color::White);
    sf::FloatRect sl = scoreLabel.getLocalBounds();
    scoreLabel.setPosition(x + (130 - sl.width) / 2, 35);
    window.draw(scoreLabel);

    sf::Text scoreValue(std::to_string(score), font, 24);
    scoreValue.setFillColor(sf::Color(0, 255, 0));
    scoreValue.setStyle(sf::Text::Bold);
    sf::FloatRect sv = scoreValue.getLocalBounds();
    scoreValue.setPosition(x + (130 - sv.width) / 2, 35 + sl.height + 10);
    window.draw(scoreValue);

    float highScoreBoxY = 30 + 60 + spacing;
    sf::RectangleShape highScoreBox(sf::Vector2f(130, 60));
    highScoreBox.setFillColor(sf::Color(50,50,150));
    highScoreBox.setPosition(x, highScoreBoxY);
    window.draw(highScoreBox);

    sf::Text highScoreLabel("High Score", font, 18);
    highScoreLabel.setFillColor(sf::Color::White);
    sf::FloatRect hsl = highScoreLabel.getLocalBounds();
    highScoreLabel.setPosition(x + (130 - hsl.width) / 2, highScoreBoxY + 5);
    window.draw(highScoreLabel);

    sf::Text highScoreValue(std::to_string(highScore), font, 22);
    highScoreValue.setFillColor(sf::Color(255, 215, 0));
    highScoreValue.setStyle(sf::Text::Bold);
    sf::FloatRect hsv = highScoreValue.getLocalBounds();
    highScoreValue.setPosition(x + (130 - hsv.width) / 2, highScoreBoxY + 5 + hsl.height + 10);
    window.draw(highScoreValue);

    float commandBoxY = highScoreBoxY + 60 + spacing;
    sf::RectangleShape commandBox(sf::Vector2f(130, 110));
    commandBox.setFillColor(sf::Color(50, 50, 150));
    commandBox.setPosition(x, commandBoxY);
    window.draw(commandBox);

    sf::Text commandsTitle("Commands", font, 16);
    commandsTitle.setFillColor(sf::Color::White);
    commandsTitle.setStyle(sf::Text::Bold);
    sf::FloatRect commandsTitleBounds = commandsTitle.getLocalBounds();
    commandsTitle.setPosition(x + (130 - commandsTitleBounds.width) / 2, commandBoxY + 5);
    window.draw(commandsTitle);

    sf::Text commandsList("P: Pause\nR: Restart\nH: Reset HS", font, 14);
    commandsList.setFillColor(sf::Color(220, 220, 220));
    commandsList.setPosition(x + 10, commandBoxY + 30);
    window.draw(commandsList);

    if (paused) {
        float pausedBoxY = commandBoxY + 110 + spacing;
        sf::RectangleShape pausedBox(sf::Vector2f(120, 40));
        pausedBox.setFillColor(sf::Color(255, 255, 0));
        pausedBox.setPosition(x + 5, pausedBoxY);
        window.draw(pausedBox);

        sf::Text pausedText("PAUSED", font, 18);
        pausedText.setFillColor(sf::Color::Black);
        pausedText.setStyle(sf::Text::Bold);
        sf::FloatRect pb = pausedText.getLocalBounds();
        pausedText.setOrigin(pb.left + pb.width / 2, pb.top + pb.height / 2);
        pausedText.setPosition(pausedBox.getPosition().x + pausedBox.getSize().x / 2,
                               pausedBox.getPosition().y + pausedBox.getSize().y / 2);
        window.draw(pausedText);
    }

    if (gameOver) {
        float gameOverBoxY = commandBoxY + 110 + spacing;
        sf::RectangleShape gameOverBox(sf::Vector2f(120, 50));
        gameOverBox.setFillColor(sf::Color(180, 0, 0));
        gameOverBox.setPosition(x + 5, gameOverBoxY);
        window.draw(gameOverBox);

        sf::Text gameOverText("GAME OVER", font, 16);
        gameOverText.setFillColor(sf::Color::White);
        gameOverText.setStyle(sf::Text::Bold);
        sf::FloatRect gb = gameOverText.getLocalBounds();
        gameOverText.setOrigin(gb.left + gb.width / 2, gb.top + gb.height / 2);
        gameOverText.setPosition(gameOverBox.getPosition().x + gameOverBox.getSize().x / 2,
                                 gameOverBox.getPosition().y + gameOverBox.getSize().y / 2);
        window.draw(gameOverText);

        sf::RectangleShape enterBox(sf::Vector2f(120, 40));
        enterBox.setFillColor(sf::Color(144, 238, 144));
        enterBox.setPosition(x + 5, gameOverBoxY + 60);
        window.draw(enterBox);

        sf::Text restartText("Press Enter key", font, 16);
        restartText.setFillColor(sf::Color::Black);
        sf::FloatRect rb = restartText.getLocalBounds();
        restartText.setOrigin(rb.left + rb.width / 2, rb.top + rb.height / 2);
        restartText.setPosition(enterBox.getPosition().x + enterBox.getSize().x / 2,
                                enterBox.getPosition().y + enterBox.getSize().y / 2);
        window.draw(restartText);
    }
}

void Game::LoadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        highScore = 0;
    }
}

void Game::SaveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

void Game::ResetHighScore() {
    highScore = 0;
    SaveHighScore();
}