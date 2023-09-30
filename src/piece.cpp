#include "chess.h"

Piece::Piece(Color c, Type t  ) {

    coly = c;
    topy = t;
}

std::string Piece::getpng()
{
    std::map<Type, std::string> ts = {
    {Type::Rook, "rook"},
    {Type::Knight, "knight"},
    {Type::King, "king"},
    {Type::Queen, "queen"},
    {Type::Bishop, "bishop"},
    {Type::Pawn, "pawn"}
    };
    std::map<Color, std::string> cs = {
    {Color::White, "white"},
    {Color::Black, "black"}
    };

    return cs[coly] + "-" + ts[topy]+".png";
}

