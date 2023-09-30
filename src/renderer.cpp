#include <iostream>
#include "chess.h"

Renderer& Renderer::getInstance() {
    static Renderer instance;
    return instance;
}

void Renderer::mainloop() {

    while (win.isOpen()) {
        eventhandler();

        displayer();
        
    }
}

void Renderer::eventhandler() {
    while (win.pollEvent(eve)) {
        switch (eve.type) {
        case sf::Event::Closed: {
            win.close();
            break;
        }
        case sf::Event::MouseButtonPressed: {

            sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(win));
                for (std::pair<sf::Sprite*,Piece*> xxx :love)
                {
                	if (xxx.first->getGlobalBounds().contains(mousePosition) && board.turn % 2 == xxx.second->coly){
                		
                		drag = true;
                		offset = xxx.first->getPosition() - mousePosition;
                		thechosen = xxx.first;
                	}

                }

            break;
        }
        case sf::Event::MouseButtonReleased: {
            drag = false;
            setinplace();
            make_piece_sprites();
            break;

        }
        default:;
        }
    }
}

void Renderer::setinplace()
{
    // null check
    if (thechosen == nullptr) return;
	sf::Vector2i x = sf::Mouse::getPosition(win) / 80;
	thechosen->setPosition(sf::Vector2f((x) * 80) + sf::Vector2f(5, 5));
    // this gets called for somereason 
	board.set(love[thechosen], x.x, x.y);

}
void Renderer::drawChessboard() {
    bool is_white = true;
    int square_size = h / 8;
    sf::RectangleShape square(sf::Vector2f(square_size, square_size));
    for (int i = 0; i < 8; i++) {
        is_white = !is_white;
        for (int j = 0; j < 8; j++) {
            is_white = !is_white;
            square.setFillColor(is_white ? sf::Color(230, 230, 230) : sf::Color(60, 60, 60));
            square.setPosition(i * square_size, j * square_size);
            win.draw(square);
        }
    }
}

void Renderer::make_piece_sprites()
{
    love.clear();
    int square_size = h / 8;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.pieces[i][j] == nullptr) continue;
            sf::Texture *tx = assets[board.pieces[i][j]->getpng()];
            sf::Sprite* img = new sf::Sprite();
            img->setTexture(*tx);
            img->setScale(0.5, 0.5);
            img->setPosition(j * square_size + 5, i * square_size+5);
            love[img] = board.pieces[i][j];
        }
    }
}

void Renderer::draw_pieces()
{
	for (auto& xxx: love)
	{
        win.draw(*xxx.first);
	}
}

void Renderer::displayer() {
    win.clear();
    drawChessboard();
    draw_pieces();
    if (drag) {
        sf::Vector2f mousePosition = static_cast<sf::Vector2f> (sf::Mouse::getPosition(win));
        if (thechosen != nullptr) thechosen->setPosition(mousePosition + offset);
    }
    text_stuff();
    win.display();
}
void Renderer::text_stuff()
{
    std::string x = board.turn % 2 ? "Black's turn" : "White's turn";
    sf::Font font;
    font.loadFromFile(R"(C:\Windows\Fonts\arialbd.ttf)");
    sf::Text title("Chess", font, 50);
    title.setPosition(700, 20);
    win.draw(title);
    sf::Text turn(x, font, 25);
    turn.setPosition(700, 100);
    win.draw(turn);
}
sf::Vector2<int> Renderer::getpos(){
	sf::Vector2i mousePosition = (sf::Mouse::getPosition(win) / 80);
    std::cout << "Mouse Position: x = " << mousePosition.x << ", y = " << mousePosition.y << std::endl;
    return mousePosition ;
}

Renderer::Renderer() : win(sf::VideoMode(w, h), "Chess Game"), eve() ,board(),love(){
    namespace fs = std::filesystem;
    try {
        for (const auto& entry : fs::directory_iterator(path_to_assets)) {
            if (entry.is_regular_file() && entry.path().extension() == ".png") {
                sf::Texture* pp = new sf::Texture();
                pp->loadFromFile(entry.path().generic_string());
                assets[ entry.path().filename().generic_string()]= pp;
            }
        }
    }
    catch (const fs::filesystem_error& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    win.setFramerateLimit(60);
    make_piece_sprites();
}
