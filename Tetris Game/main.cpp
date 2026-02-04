#include <SFML/Graphics.hpp>
#include "game.h"
int main() {
    sf::RenderWindow window(sf::VideoMode(500, 620), "SFML Tetris");
    window.setFramerateLimit(60);
    Game game;
    sf::Clock dropClock;
    float dropInterval = 0.4f;
    sf::Color darkBlue(44, 44, 127, 255);

    sf::RectangleShape sidePanel(sf::Vector2f(150, 600));
    sidePanel.setFillColor(sf::Color(30, 30, 30));
    sidePanel.setPosition(340, 10);

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            game.HandleInput(event);
        }

        if (dropClock.getElapsedTime().asSeconds() >= dropInterval){
            game.MoveBlockDown();
            dropClock.restart();
        }
        window.clear(darkBlue);
        window.draw(sidePanel);
        game.Draw(window);
        window.display();
    }
    return 0;
}