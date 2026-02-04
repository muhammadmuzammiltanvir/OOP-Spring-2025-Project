#include "grid.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "colors.h"
Grid::Grid()
{
    numRows=20;
    numCols=10;
    cellSize=30;
    colors = {
        sf::Color::Black,             
        sf::Color::Cyan,           
        sf::Color::Blue,         
        sf::Color::Green,         
        sf::Color::Red,             
        sf::Color::Yellow,            
        sf::Color(255, 165, 0),       
        sf::Color(128, 0, 128)    
    };
    Initialize();
}
void Grid::Initialize()
{
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numCols; column++)
        {
            grid[row][column] = 0;
        }
    }
}
void Grid::Print()
{
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numCols; column++)
        {
            std::cout << grid[row][column] << " ";
        }
        std::cout << std::endl;
    }
}
void Grid::Draw(sf::RenderWindow& window)
{
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numCols; column++)
        {
            int cellValue = grid[row][column];

            sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1)); 
            cell.setFillColor(colors[cellValue]);
            cell.setPosition(column * cellSize +11, row * cellSize+11);

            window.draw(cell);
        }
    }
}
bool Grid::IsCellOutside(int row, int column)
{
    if(row>=0 && row<numRows && column>=0 && column<numCols)
    {
        return false;
    }
    return true;
}
bool Grid::IsCellEmpty(int row, int column)
{
    if(grid[row][column]==0)
    {
        return true;
    }
    return false;
}
int Grid::ClearFullRows()
{
    int completed = 0;
    for (int row = numRows - 1; row >= 0; row--)
    {
        if (IsRowFull(row))
        {
            ClearRow(row);
            completed++;
        }
        else if (completed > 0)
        {
            MoveRowDown(row, completed);
            ClearRow(row);
        }
    }
    return completed;
}
bool Grid::IsRowFull(int row)
{
    for(int column=0; column<numCols;column++)
    {
        if(grid[row][column]==0)
        {
            return false;
        }
    }
    return true;
}
void Grid::ClearRow(int row)
{
    for(int column=0;column<numCols;column++)
    {
        grid[row][column]=0;
    }
}
void Grid::MoveRowDown(int row, int numRows)
{
    for(int column=0;column<numCols;column++)
    {
        grid[row+numRows][column]=grid[row][column];
        grid[row][column]=0;
    }
}