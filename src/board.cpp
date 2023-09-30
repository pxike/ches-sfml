#include "chess.h"

Board::Board() : pieces(8, std::vector<Piece*>(8, nullptr)) {
    pieces[0][0] = new Piece(Piece::Color::White, Piece::Type::Rook);
    pieces[0][1] = new Piece(Piece::Color::White, Piece::Type::Knight);
    pieces[0][2] = new Piece(Piece::Color::White, Piece::Type::Bishop);
    pieces[0][3] = new Piece(Piece::Color::White, Piece::Type::Queen);
    pieces[0][4] = new Piece(Piece::Color::White, Piece::Type::King);
    pieces[0][5] = new Piece(Piece::Color::White, Piece::Type::Bishop);
    pieces[0][6] = new Piece(Piece::Color::White, Piece::Type::Knight);
    pieces[0][7] = new Piece(Piece::Color::White, Piece::Type::Rook);

    for (int i = 0; i < 8; i++) {
        pieces[1][i] = new Piece(Piece::Color::White, Piece::Type::Pawn);
    }

    pieces[7][0] = new Piece(Piece::Color::Black, Piece::Type::Rook);
    pieces[7][1] = new Piece(Piece::Color::Black, Piece::Type::Knight);
    pieces[7][2] = new Piece(Piece::Color::Black, Piece::Type::Bishop);
    pieces[7][3] = new Piece(Piece::Color::Black, Piece::Type::Queen);
    pieces[7][4] = new Piece(Piece::Color::Black, Piece::Type::King);
    pieces[7][5] = new Piece(Piece::Color::Black, Piece::Type::Bishop);
    pieces[7][6] = new Piece(Piece::Color::Black, Piece::Type::Knight);
    pieces[7][7] = new Piece(Piece::Color::Black, Piece::Type::Rook);

    for (int i = 0; i < 8; i++) {
        pieces[6][i] = new Piece(Piece::Color::Black, Piece::Type::Pawn);
    }
    loo[pieces[0][0]] = { 0, 0 };
    loo[pieces[0][1]] = { 0, 1 };
    loo[pieces[0][2]] = { 0, 2 };
    loo[pieces[0][3]] = { 0, 3 };
    loo[pieces[0][4]] = { 0, 4 };
    loo[pieces[0][5]] = { 0, 5 };
    loo[pieces[0][6]] = { 0, 6 };
    loo[pieces[0][7]] = { 0, 7 };
    for (int i = 0; i < 8; i++) {
        loo[pieces[1][i]] = { 1, i };
    }

    loo[pieces[7][0]] = { 7, 0 };
    loo[pieces[7][1]] = { 7, 1 };
    loo[pieces[7][2]] = { 7, 2 };
    loo[pieces[7][3]] = { 7, 3 };
    loo[pieces[7][4]] = { 7, 4 };
    loo[pieces[7][5]] = { 7, 5 };
    loo[pieces[7][6]] = { 7, 6 };
    loo[pieces[7][7]] = { 7, 7 };
    for (int i = 0; i < 8; i++) {
        loo[pieces[6][i]] = { 6, i };
    }
}

Board::~Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete pieces[i][j];
        }
    }
}

std::vector<int> Board::getpos(Piece* p)
{
    return loo[p];
}

Piece* Board::getPieceAt(const std::vector<int>& position) const {
    int i = position[0];
    int j = position[1];
    return pieces[j][i];
}
void Board::set(Piece* x, int j, int i)
{
    if (x == nullptr) return;
    Piece* pt = pieces[loo[x][0]][loo[x][1]];
    if (!possi(*x, j, i)) return;
	pieces[loo[x][0]][loo[x][1]] = nullptr;
    loo[x] = { i , j };
    pieces[i][j] = pt;
}



bool Board::possi(Piece& x, int j, int i) {

    int current_y = loo[&x][1]; int current_x = loo[&x][0];
    if (pieces[i][j] != nullptr &&(current_y == j && current_x == i || x.coly == pieces[i][j]->coly)) return false;

    int stepin_x  = (i != current_x) ? ((i > current_x) ? 1 : -1) : 0;
    int stepin_y  = (j != current_y) ? ((j > current_y) ? 1 : -1) : 0;
    auto very_smart_thing = (j - current_y) == 0 ? 0 : (i - current_x) / (j - current_y);


    switch (x.topy) {
    case Piece::King: {
        if (abs(current_x - i) < 2 && std::abs(current_y - j) < 2) return true;
        return false;
    }
    case Piece::Bishop: {
        if (std::abs(very_smart_thing) != 1) return false;
        current_x += stepin_x;
        current_y += stepin_y;
        while (current_y != j || current_x != i) {
            if (pieces[current_x][current_y] != nullptr) return false;
            current_x += stepin_x;
            current_y += stepin_y;
        }
        return true;
    }
    case Piece::Rook: {
        if (very_smart_thing != 0) return false;
        current_x += stepin_x;
        current_y += stepin_y;
        while (current_y != j || current_x != i) {
            if (pieces[current_x][current_y] != nullptr) return false;
            current_x += stepin_x;
            current_y += stepin_y;
        }
        return true;
    }
    case Piece::Queen: {
        if (very_smart_thing != 0 && std::abs(very_smart_thing) != 1) return false;
        current_x += stepin_x;
        current_y += stepin_y;
        while (current_y != j || current_x != i) {
            if (pieces[current_x][current_y] != nullptr) return false;
            current_x += stepin_x;
            current_y += stepin_y;
        }
        return true;
    }
    case Piece::Knight: {
        int kin = std::abs(current_x - i) + std::abs(current_y - j );
        if (kin == 3 && (current_x - i) * (current_y - j) != 0) return true;
        return false;
    }
    case Piece::Pawn: {
        if (x.coly == Piece::White) {
            int check1 = current_x - i, check2 = current_y - j;
            if (!check2 && check1 == -1)    return      pieces[i][j] == nullptr;
            if ( check2 && check1 == -1)    return      pieces[i][j] != nullptr;
            if (!check2 && check1 == -2)    return      current_x == 1;
            return false;
        }
        if (x.coly == Piece::Black) {
            int check1 = current_x - i, check2 = current_y - j;
            if (!check2 && check1 == 1)    return      pieces[i][j] == nullptr;
            if ( check2 && check1 == 1)    return      pieces[i][j] != nullptr;
            if (!check2 && check1 == 2)    return      current_x == 6;
            return false;
        }
    }
    }
    return false;

}
