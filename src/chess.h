#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>
#include <map>

#include <string>
#include <vector>
#include <string>
#include <vector>
#include <map>



class Piece {
public:
    enum Color { White, Black };
    enum Type { Rook, Knight, King, Queen, Bishop, Pawn };
    Piece(Color c, Type t);
    std::string getpng();

    Color coly;
    Type topy;
};



class Board {
public:
    Board();

    ~Board();

    void initializeBoard();

    std::vector<int> getpos(Piece* p);
    Piece* getPieceAt(const std::vector<int>& position) const;
    bool possiblemove(Piece& x, int j, int i);
    void set(Piece* x, int j, int i);
    bool specialmove(Piece& x, int j, int i);
    std::vector<std::vector<Piece*>> pieces;
    std::map<Piece*, std::vector<int>> loo;
    int turn = 0;

};

class Renderer {
public:
    static Renderer& getInstance();

    void mainloop();

private:
    int w = 1000, h = 640;
    std::string path_to_assets = R"(C:\Users\heii\Desktop\pjs\ches-sfml\assets\)";
    std::map<std::string,sf::Texture*> assets;
    std::map<sf::Sprite*,Piece*> love;
    sf::RenderWindow win;
    sf::Event eve;
    
    Board board;
    sf::Vector2<float> offset;
	sf::Sprite* thechosen = nullptr;
    bool drag = false;

    void eventhandler();
    void setinplace();
    void drawChessboard();
    void make_piece_sprites();
    void make_piece_sprites_less_expansive();

    void draw_pieces() ;
    void displayer();
    void text_stuff();
    sf::Vector2<int> getpos();
    Renderer();

    // Declare copy constructor and assignment operator as deleted
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};

