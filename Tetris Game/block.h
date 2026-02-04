#pragma once
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "position.h"
class Block{
public:
    Block();
    void Draw(sf::RenderWindow &window);
    void Move(int rows,int columns);
    std::vector<Position>GetCellPositions();
    void Rotate();
    void UndoRotation();
    void Draw();
    int id;
    std::map<int,std::vector<Position>> cells;
private:
    int cellSize;
    int rotationState;
    std::vector<sf::Color>colors;
    int rowOffset;
    int columnOffset;
};