#include "colors.h"
const sf::Color darkGrey(26, 31, 40);
const sf::Color green(0, 255, 0);
const sf::Color red(255, 0, 0);
const sf::Color orange(255, 165, 0);
const sf::Color yellow(255, 255, 0);
const sf::Color purple(128, 0, 128);
const sf::Color cyan(0, 255, 255);
const sf::Color blue(0, 0, 255);
std::vector<sf::Color> GetCellColors(){
    return {
        darkGrey, 
        cyan,   
        blue,    
        green,   
        red,     
        yellow,   
        orange, 
        purple  
    };
}