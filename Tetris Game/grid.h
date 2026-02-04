#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
class Grid{
public:
    Grid();
    void Initialize();
    void Print();
    void Draw(sf::RenderWindow& window);
    bool IsCellOutside(int row, int column);
    bool IsCellEmpty(int row, int column);
    int ClearFullRows();
    int grid[20][10];
private:
    bool IsRowFull(int row);
    void ClearRow(int row);
    void MoveRowDown(int row, int numRows);
    int numRows;
    int numCols;
    int cellSize;
    std::vector<sf::Color> colors;
};