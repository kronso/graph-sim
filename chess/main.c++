#include <iostream>
#include <cstring>
#include <string>
#include <conio.h>
using namespace std;

#define ENTER 13
#define ESC 27
#define clear() printf("\e[1;1H\e[2J");

#define GREEN_FG "\033[38;5;49m"
#define RED_FG "\033[38;5;197m"
#define WHITE_FG "\033[38;5;231m"
#define BLACK_FG "\033[38;5;239m"
#define RESET "\033[m"
#define HIDE "\033[8m"

class Board {
    const int HEIGHT = 8; 
    static const int WIDTH = 8;
    string royal = "RHBQKBHR", pawns = "PPPPPPPP";
    public:
        int cursor_y = HEIGHT / 2, cursor_x = WIDTH / 2;
        int temp_cursor_y = cursor_y, temp_cursor_x = cursor_x;

        int previous_y, previous_x;
        char previous_c;

        bool white_turn = true;
        bool black_turn = false;

        int wking_y, wking_x;
        int bking_y, bking_x;

        char (*grid)[WIDTH] = new char[HEIGHT][WIDTH];
        char (*valid_grid)[WIDTH] = new char[HEIGHT][WIDTH];
        char (*x_permutations)[WIDTH] = new char[HEIGHT][WIDTH];

        char white_kills[8];
        char black_kills[8];

        void playerTurn();

        void swapY();
        void swapX();
        void swapBoth();
        void toInitial(int initial_y, int initial_x);
        
        void initXPerm() ;
        void initValidBoard();
        void initBoard();

        void printKilledPieces();
        void whiteKilledPieces();
        void blackKilledPieces();
        void printXPerm();
        void printValid();
        void printBoard();

        bool checkmate();
        bool isCheck();
        void killPiece();
        void killBoard(char killed_piece);
        bool choosePiece(bool chosen = false);

        void moveHorse();
        void validBlackMove();
        void validWhiteMove();
        
        void bishopValid(bool perm = false); 
        void rookValid(bool perm = false);
        void kingValid(bool perm = false); 
        void horseValid(bool perm = false); 
        void pawnValid(bool perm = false);
        void allValid();

        // bool isRoyal(char piece); bool isPawn(char piece);
        bool isBlack(char piece);
        bool isWhite(char piece);
        bool isEnemy(char x);
        bool isKing(char piece);

        bool inRange(int i = 0, int j = 0);
        void position();
};

void Board::playerTurn() {
    bool temp;
    temp = white_turn;
    white_turn = black_turn;
    black_turn = temp;
}
// changing positions of pieces or cursor
void Board::swapY() {
    char temp;
    temp = grid[previous_y][cursor_x];
    grid[previous_y][cursor_x] = grid[cursor_y][cursor_x];  
    grid[cursor_y][cursor_x] = temp; 
}
void Board::swapX() {
    char temp;
    temp = grid[cursor_y][previous_x];
    grid[cursor_y][previous_x] = grid[cursor_y][cursor_x]; 
    grid[cursor_y][cursor_x] = temp; 
}
void Board::swapBoth() {
    char temp;
    temp = grid[cursor_y][cursor_x];
    grid[cursor_y][cursor_x] = grid[previous_y][previous_x];
    grid[previous_y][previous_x] = temp;
}
void Board::toInitial(int initial_y, int initial_x)  {
    int temp;
    temp = grid[cursor_y][cursor_x];
    grid[cursor_y][cursor_x] = grid[initial_y][initial_x];
    grid[initial_y][initial_x] = temp;
}
// initialises boards
void Board::initXPerm() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) { 
            x_permutations[i][j] = '.';
        }   
    }
}
void Board::initValidBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            valid_grid[i][j] = '.';
        }   
    }
    allValid();
}   
void Board::initBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = '.';
            if (i == 0) { grid[i][j] = tolower(royal[j]); }
            if (i == (HEIGHT - 1)) { grid[i][j] = royal[j]; }
            if (i == 1) { grid[i][j] = tolower(pawns[j]); }
            if (i == 6) { grid[i][j] = pawns[j]; }

            if (isBlack(grid[i][j]) && isKing(grid[i][j])) {
                bking_y = i; bking_x = j;
            } else if (isWhite(grid[i][j]) && isKing(grid[i][j])) {
                wking_y = i; wking_x = j;
            }

        }   
    }
}
// this is so ugly to look at whatthe fuck.
void Board::printValid() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (valid_grid[i][j] == 'X' || valid_grid[i][j] == '*') {
                cout << RED_FG << valid_grid[i][j] << RESET << "   ";
            } else if (valid_grid[i][j] == 'K') {
                cout << BLACK_FG << valid_grid[i][j] << RESET << "   ";
            } else { cout << valid_grid[i][j] << "   ";}
        }
        cout << "\n"; 
    } 
}
void Board::printXPerm() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (x_permutations[i][j] == 'X' || x_permutations[i][j] == '*') {
                cout << RED_FG << x_permutations[i][j] << RESET << "   ";
            } else if (x_permutations[i][j] == 'K') {
                cout << BLACK_FG << x_permutations[i][j] << RESET << "   ";
            } else { cout << x_permutations[i][j] << "   ";}
        }
        cout << "\n"; 
    }  
}
void Board::printBoard() {
    clear();
    cout << "+       +       +       +       +       +       +       +       +\n";
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == cursor_y && j == cursor_x) {
                cout << "|";
                if (isBlack(grid[i][j])) { cout << "   " << GREEN_FG << (char) toupper(grid[i][j]) << RESET << "   "; }
                else { cout << "   " << GREEN_FG << grid[i][j] << RESET << "   "; }
            } else if (valid_grid[i][j] == 'X') {
                cout << "|";
                cout << "   " << RED_FG << valid_grid[i][j] << RESET << "   ";
            } else if (grid[i][j] == '.'){
                cout << "|   " << HIDE << grid[i][j] << RESET << "   ";
            } else if (isBlack(grid[i][j])) {
                cout << "|   " << BLACK_FG << (char) toupper(grid[i][j]) << RESET << "   ";
            } else if (isWhite(grid[i][j])) {
                cout << "|   " << WHITE_FG << grid[i][j] << RESET << "   ";
            }
        }
        cout << "\n+       +       +       +       +       +       +       +       +\n"; 
    }   
    // info on the game
    if (isCheck()) {cout << "CHECK" << endl;}
    if (white_turn) { 
        cout << WHITE_FG << "WHITE's TURN" << RESET << endl; 
        blackKilledPieces();
        cout << endl;

    } else if (black_turn) { 
        cout << BLACK_FG << "BLACK's TURN" << RESET << endl; 
        whiteKilledPieces();
        cout << endl;

    }
    position();
    cout << "\nPermutations of all possible movements and killings:\n";
    printXPerm();
    cout << "\nPermutations of cursor movements and killings:\n";
    printValid();
}
void Board::whiteKilledPieces() {
    for (int i = 0; i < 8; i++) {
        cout << WHITE_FG << white_kills[i] << RESET;
    }
}
void Board::blackKilledPieces() {
    for (int i = 0; i < 8; i++) {
        cout << BLACK_FG << (char) toupper(black_kills[i]) << RESET;
    }
}

void Board::position() {    
    char pos_x = cursor_x + 'a';

    int test[8] = {8, 7, 6, 5, 4, 3, 2, 1};
    int pos_y = test[cursor_y];
    char piece = toupper(grid[cursor_y][cursor_x]);
    piece = (piece == 'P' || piece == 'p' || piece == '.') ? '\b' : piece;
    piece = (piece == 'H' || piece == 'h') ? 'N' : piece;
    cout << piece << pos_x << pos_y << endl;
}
void Board::killBoard(char killed_piece) {
    static int white_kill_pos = 0;
    static int black_kill_pos = 0;
    if (isBlack(killed_piece)) {
        black_kills[black_kill_pos] = killed_piece;
        black_kill_pos++;
    } else if (isWhite(killed_piece)) {
        white_kills[white_kill_pos] = killed_piece;
        white_kill_pos++;
    }
}
void Board::killPiece() {
    switch(previous_c) {
        case '8': 
            killBoard(grid[previous_y][cursor_x]);
            grid[previous_y][cursor_x] = '.'; 
            break;
        case '4': 
            killBoard(grid[cursor_y][previous_x]);
            grid[cursor_y][previous_x] = '.'; 
            break;
        case '5': 
            killBoard(grid[previous_y][cursor_x]);
            grid[previous_y][cursor_x] = '.'; 
            break;
        case '6': 
            killBoard(grid[cursor_y][previous_x]);
            grid[cursor_y][previous_x] = '.'; 
            break;
        case '7': 
            killBoard(grid[previous_y][previous_x]);
            grid[previous_y][previous_x] = '.'; 
            break;
        case '1': 
            killBoard(grid[previous_y][previous_x]);
            grid[previous_y][previous_x] = '.'; 
            break;
        case '9': 
            killBoard(grid[previous_y][previous_x]);
            grid[previous_y][previous_x] = '.'; 
            break;
        case '3':
            killBoard(grid[previous_y][previous_x]);
            grid[previous_y][previous_x] = '.'; 
            break;
        default: break;
    }   
}
bool Board::choosePiece(bool chosen) {  
    static int initial_y, initial_x;
    if (!chosen) {initial_y = cursor_y; initial_x = cursor_x;}
    char c;
    _kbhit();
    switch(c = getch()) {
        // UP
        case '8':
            if (cursor_y > 0) { 
                if (chosen) { // when the user chooses a piece, they can move it.
                    if (valid_grid[cursor_y - 1][cursor_x] != '.') {
                        previous_c = c; previous_y = cursor_y; cursor_y -= 1; temp_cursor_y = cursor_y; swapY(); 
                    }
                    printBoard(); return choosePiece(true); 
                }
                cursor_y -= 1; 
                temp_cursor_y = cursor_y;
            }
            if (chosen) { return choosePiece(true); }
            break;
        // LEFT
        case '4':
            if (cursor_x > 0) { 
                if (chosen) {
                    if (valid_grid[cursor_y][cursor_x - 1] != '.') { 
                        previous_c = c; previous_x = cursor_x; cursor_x -= 1; temp_cursor_x = cursor_x; swapX(); 
                    }
                    printBoard(); return choosePiece(true);
                }
                cursor_x -= 1; 
                temp_cursor_x = cursor_x;
            }
            if (chosen) { return choosePiece(true); }
            break;
        // DOWN
        case '5':
            if (cursor_y < (HEIGHT - 1)) { 
                if (chosen) {
                    if (valid_grid[cursor_y + 1][cursor_x] != '.') { 
                        previous_c = c; previous_y = cursor_y; cursor_y += 1; temp_cursor_y = cursor_y; swapY(); 
                    }
                    printBoard(); return choosePiece(true);
                }
                cursor_y += 1; 
                temp_cursor_y = cursor_y;
            }
            if (chosen) { return choosePiece(true); }
            break;
        // RIGHT
        case '6':
            if (cursor_x < (WIDTH - 1)) {
                if (chosen) {
                    if (valid_grid[cursor_y][cursor_x + 1] != '.') { 
                        previous_c = c; previous_x = cursor_x; cursor_x += 1; temp_cursor_x = cursor_x; swapX(); 
                    }
                    printBoard(); return choosePiece(true);
                }
                cursor_x += 1; 
                temp_cursor_x = cursor_x;
            }
            if (chosen) { return choosePiece(true); }
            break;
        // TOP LEFT
        case '7':
            if (cursor_y > 0 && cursor_x > 0) { 
                if (chosen) {
                    if (valid_grid[cursor_y - 1][cursor_x - 1] != '.') {  
                        previous_x = cursor_x, previous_y = cursor_y;
                        cursor_y -= 1; cursor_x -= 1; 
                        swapBoth();
                    // horse movement
                    } else if (valid_grid[cursor_y - 2][cursor_x - 1] != '.') {
                        previous_x = cursor_x, previous_y = cursor_y;
                        cursor_y -= 2; cursor_x -= 1; 
                        swapBoth();
                    } else if (valid_grid[cursor_y - 1][cursor_x - 2] != '.') {
                        previous_x = cursor_x, previous_y = cursor_y;
                        cursor_y -= 1; cursor_x -= 2; 
                        swapBoth();
                    }
                    previous_c = c; 
                    temp_cursor_y = cursor_y; temp_cursor_x = cursor_x;
                    printBoard(); return choosePiece(true);
                }
                cursor_y -= 1; cursor_x -= 1; 
                temp_cursor_y = cursor_y; temp_cursor_x = cursor_x;
            }
            if (chosen) { return choosePiece(true); }
            break;
        // TOP RIGHT
        case '9':
            if (cursor_y > 0 && cursor_x < (WIDTH - 1)) { 
                if (chosen) {
                    if (valid_grid[cursor_y - 1][cursor_x + 1] != '.') { 
                        previous_x = cursor_x, previous_y = cursor_y;
                        cursor_y -= 1; cursor_x += 1; 
                        swapBoth();
                    } else if (valid_grid[cursor_y - 2][cursor_x + 1] != '.') {
                        previous_x = cursor_x, previous_y = cursor_y;
                        cursor_y -= 2; cursor_x += 1; 
                        swapBoth();
                    } else if (valid_grid[cursor_y - 1][cursor_x + 2] != '.') {
                        previous_x = cursor_x, previous_y = cursor_y;
                        cursor_y -= 1; cursor_x += 2; 
                        swapBoth();
                    }
                    previous_c = c; 
                    temp_cursor_y = cursor_y; temp_cursor_x = cursor_x;
                    printBoard(); return choosePiece(true);
                }
                cursor_y -= 1; cursor_x += 1; 
                temp_cursor_y = cursor_y; temp_cursor_x = cursor_x;
            }
            if (chosen) { return choosePiece(true); }
            break;
        // BOTTOM LEFT
        case '1':
            if (cursor_y < (HEIGHT - 1) && cursor_x > 0) { 
                if (chosen) {
                    if (valid_grid[cursor_y + 1][cursor_x - 1] != '.') { 
                        previous_x = cursor_x, previous_y = cursor_y; 
                        cursor_y += 1; cursor_x -= 1; 
                        swapBoth();
                    } else if (valid_grid[cursor_y + 2][cursor_x - 1] != '.') {
                        previous_x = cursor_x, previous_y = cursor_y; 
                        cursor_y += 2; cursor_x -= 1; 
                        swapBoth();
                    } else if (valid_grid[cursor_y + 1][cursor_x - 2] != '.') {
                        previous_x = cursor_x, previous_y = cursor_y; 
                        cursor_y += 1; cursor_x -= 2;   
                        swapBoth();
                    }
                    previous_c = c;
                    temp_cursor_y = cursor_y; temp_cursor_x = cursor_x;
                    printBoard(); return choosePiece(true);
                }    
                cursor_y += 1; cursor_x -= 1; 
                temp_cursor_y = cursor_y; temp_cursor_x = cursor_x;
            }
            if (chosen) { return choosePiece(true); }
            break;
        // BOTTOM RIGHT
        case '3':
            if (cursor_y < (HEIGHT - 1) && cursor_x < (WIDTH - 1)) { 
                if (chosen) {
                    if (valid_grid[cursor_y + 1][cursor_x + 1] != '.') {                         
                        previous_x = cursor_x, previous_y = cursor_y;
                        cursor_y += 1; cursor_x += 1;
                        swapBoth();
                    } else if (valid_grid[cursor_y + 2][cursor_x + 1] != '.') {
                        previous_x = cursor_x, previous_y = cursor_y;
                        cursor_y += 2; cursor_x += 1;                        
                        swapBoth();
                    } else if (valid_grid[cursor_y + 1][cursor_x + 2] != '.') {
                        previous_x = cursor_x, previous_y = cursor_y;
                        cursor_y += 1; cursor_x += 2;
                        swapBoth();
                    }
                    previous_c = c; 
                    temp_cursor_y = cursor_y; temp_cursor_x = cursor_x;
                    printBoard(); return choosePiece(true);
                }
                cursor_y += 1; cursor_x += 1; 
                temp_cursor_y = cursor_y; temp_cursor_x = cursor_x;
            }
            if (chosen) { return choosePiece(true); }
            break;
        // SELECT PIECE TO MOVE AND CONFIRM PLACEMENT AND OTHER SSHTIT  
        case ENTER:
            // to kill the piece the the player moves onto
            // swaps turns after placement
            if (valid_grid[cursor_y][cursor_x] == 'K') {playerTurn(); killPiece(); initValidBoard(); return false;}
            // move to a valid position
            // swaps turns after placement
            else if (valid_grid[cursor_y][cursor_x] == 'X') {playerTurn(); initXPerm(); initValidBoard(); return false;}
            
            if (isBlack(grid[cursor_y][cursor_x]) && black_turn) {validBlackMove(); return choosePiece(true);} 
            else if (isWhite(grid[cursor_y][cursor_x]) && white_turn) {validWhiteMove(); return choosePiece(true);}
            break;
        // RESET PIECE TO INITIAL POSITION 
        case ESC:
            toInitial(initial_y, initial_x);
            cursor_y = initial_y, cursor_x = initial_x;
            printBoard(); 
            break;
        default:
            if (chosen) { return choosePiece(true); }
            break;
    }
    return false;
}

bool Board::isBlack(char piece) {
    for (int i = 0; i < WIDTH; i++) {
        if (piece == tolower(royal[i]) || piece == tolower(pawns[i])) {return true;}
    }
    return false;
}
bool Board::isWhite(char piece) {
    for (int i = 0; i < WIDTH; i++) {
        if (piece == royal[i] || piece == pawns[i]) {return true;}
    }
    return false;
}
bool Board::isEnemy(char x) {
    char piece = grid[cursor_y][cursor_x];
    if ((isWhite(piece) && isBlack(x)) || (isBlack(piece) && isWhite(x))) {return true;}
    return false;
}
bool Board::isKing(char piece) {
    if (piece == 'K' || piece == 'k') {return true;}
    return false;
}

// all possible movements and kills from all pieces on the board
void Board::allValid() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cursor_y = i; cursor_x = j;
            switch(grid[i][j]) {
                case 'K': kingValid(true); break;
                case 'k': kingValid(true); break;
                case 'Q': bishopValid(true); rookValid(true); break;
                case 'q': bishopValid(true); rookValid(true); break;
                case 'R': rookValid(true); break;
                case 'r': rookValid(true); break;
                case 'B': bishopValid(true); break;
                case 'b': bishopValid(true); break;
                case 'H': horseValid(true); break;
                case 'h': horseValid(true); break;
                case 'P': pawnValid(true); break;
                case 'p': pawnValid(true); break;
            }
        }
    }
    cursor_y = temp_cursor_y; cursor_x = temp_cursor_x;
}

bool Board::inRange(int i, int j) {
    if (cursor_y + i > -1 && cursor_y + i < HEIGHT && cursor_x + j > -1 && cursor_x + j < WIDTH) { return true; }
    return false; 
}

bool Board::checkmate() {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) {continue;}
            if (isBlack(grid[bking_y + i][bking_x + j]) == false) {
                if (x_permutations[bking_y + i][bking_x + j] != 'X') {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Board::isCheck() { 
    if (x_permutations[wking_y][wking_x] == 'K') {return true;}
    else if (x_permutations[bking_y][bking_x] == 'K') {return true;}
    return false;
}
// checks where the piece can move
// if there are obstructions
// and if it can kill a piece
void Board::bishopValid(bool perm) {
    char piece = grid[cursor_y][cursor_x];
    bool tr_obstruct, tl_obstruct, br_obstruct, bl_obstruct;
    tr_obstruct = tl_obstruct = br_obstruct = bl_obstruct = false;
    for (int i = 0; i <= HEIGHT; i++) {
        // doesn't check the initial position of piece
        if (i > 0) { 
            // the obstructions is the killable piece denoted as 'K';
            // checks if there is a piece blocking the bottom right relative position
            if ((isBlack(grid[cursor_y + i][cursor_x + i]) || isWhite(grid[cursor_y + i][cursor_x + i])) && inRange(i, i)) { 
                if (!br_obstruct && isEnemy(grid[cursor_y + i][cursor_x + i])) {
                    if (perm) {x_permutations[cursor_y + i][cursor_x + i] = 'K';}
                    else {valid_grid[cursor_y + i][cursor_x + i] = 'K';}
                } 
                br_obstruct = true;
            } 
            // bottom left
            if ((isBlack(grid[cursor_y + i][cursor_x - i]) || isWhite(grid[cursor_y + i][cursor_x - i])) && inRange(i, -i)) { 
                if (!bl_obstruct && isEnemy(grid[cursor_y + i][cursor_x - i])) {
                    if (perm) {x_permutations[cursor_y + i][cursor_x - i] = 'K';}
                    else {valid_grid[cursor_y + i][cursor_x - i] = 'K';}
                } 
                bl_obstruct = true;
            }
            // top right
            if ((isBlack(grid[cursor_y - i][cursor_x + i]) || isWhite(grid[cursor_y - i][cursor_x + i])) && inRange(-i, i)) { 
                if (!tr_obstruct && isEnemy(grid[cursor_y - i][cursor_x + i])) {
                    if (perm) {x_permutations[cursor_y - i][cursor_x + i] = 'K';}
                    else {valid_grid[cursor_y - i][cursor_x + i] = 'K';}
                } 
                tr_obstruct = true;
            }
            // top left
            if ((isBlack(grid[cursor_y - i][cursor_x - i]) || isWhite(grid[cursor_y - i][cursor_x - i])) && inRange(-i, -i)) {
                if (!tl_obstruct && isEnemy(grid[cursor_y - i][cursor_x - i])) {
                    if (perm) {x_permutations[cursor_y - i][cursor_x - i] = 'K';}
                    else {valid_grid[cursor_y - i][cursor_x - i] = 'K';}
                } 
                tl_obstruct = true;
            } 

        }      
        // The range of the chess board
        if (inRange(0, i)) {
            // sets values as possible positions to move as 'X' if there are no obstructions.
            if (!br_obstruct) {
                if (perm) {
                    if (isBlack(piece)) {x_permutations[cursor_y + i][cursor_x + i] = '*';} 
                    if (isWhite(piece)) {x_permutations[cursor_y + i][cursor_x + i] = 'X';} 
                } else {valid_grid[cursor_y + i][cursor_x + i] = 'X'; }
            }
            if (!tr_obstruct) {
                if (perm) {
                    if (isBlack(piece)) {x_permutations[cursor_y - i][cursor_x + i] = '*';}
                    if (isWhite(piece)) {x_permutations[cursor_y - i][cursor_x + i] = 'X';}
                } else {valid_grid[cursor_y - i][cursor_x + i] = 'X'; }
            }
        }   

        if (inRange(0, -i)) {
            if (!bl_obstruct) {
                if (perm) {
                    if (isBlack(piece)) {x_permutations[cursor_y + i][cursor_x - i] = '*';}
                    if (isWhite(piece)) {x_permutations[cursor_y + i][cursor_x - i] = 'X';}
                } else {valid_grid[cursor_y + i][cursor_x - i] = 'X';} 
            }
            if (!tl_obstruct) {
                if (perm) {
                    if (isBlack(piece)) {x_permutations[cursor_y - i][cursor_x - i] = '*';}
                    if (isWhite(piece)) {x_permutations[cursor_y - i][cursor_x - i] = 'X';}
                } else {valid_grid[cursor_y - i][cursor_x - i] = 'X';}
            }
        }
    } 
}
void Board::rookValid(bool perm) {
    // the same things as bishopValid(), just h and v positions...
    char piece = grid[cursor_y][cursor_x];
    bool rrow_obstruct, bcol_obstruct, lrow_obstruct, tcol_obstruct;
    rrow_obstruct = bcol_obstruct = lrow_obstruct = tcol_obstruct = false;
    for (int i = 0; i <= HEIGHT; i++) {
        if (i > 0) {
            // right row
            if ((isBlack(grid[cursor_y][cursor_x + i]) || isWhite(grid[cursor_y][cursor_x + i])) && inRange(0, i)) { 
                if (!rrow_obstruct && isEnemy(grid[cursor_y][cursor_x + i])) { 
                    if (perm) {x_permutations[cursor_y][cursor_x + i] = 'K';}
                    else {valid_grid[cursor_y][cursor_x + i] = 'K';} 
                }
                rrow_obstruct = true;
            }
            // bottom col
            if ((isBlack(grid[cursor_y + i][cursor_x]) || isWhite(grid[cursor_y + i][cursor_x])) && inRange(i)) { 
                if (!bcol_obstruct && isEnemy(grid[cursor_y + i][cursor_x])) { 
                    if (perm) {x_permutations[cursor_y + i][cursor_x] = 'K';}
                    else {valid_grid[cursor_y + i][cursor_x] = 'K'; }
                } 
                bcol_obstruct = true;
            }
            // left row
            if ((isBlack(grid[cursor_y][cursor_x - i]) || isWhite(grid[cursor_y][cursor_x - i])) && inRange(0, -i)) { 
                if (!lrow_obstruct && isEnemy(grid[cursor_y][cursor_x - i])) { 
                    if (perm) {x_permutations[cursor_y][cursor_x - i] = 'K'; }
                    else {valid_grid[cursor_y][cursor_x - i] = 'K'; }
                } 
                lrow_obstruct = true;
            }
            // top col
            if ((isBlack(grid[cursor_y - i][cursor_x]) || isWhite(grid[cursor_y - i][cursor_x])) && inRange(-i) ) { 
                if (!tcol_obstruct && isEnemy(grid[cursor_y - i][cursor_x])) { 
                    if (perm) {x_permutations[cursor_y - i][cursor_x] = 'K';}
                    else {valid_grid[cursor_y - i][cursor_x] = 'K';}
                } 
                tcol_obstruct = true;
            }   
        }
        if (inRange(0, i)) {
            if (!rrow_obstruct) {
                if (perm) {
                    if (isBlack(piece)) {x_permutations[cursor_y][cursor_x + i] = '*';}
                    if (isWhite(piece)) {x_permutations[cursor_y][cursor_x + i] = 'X';}
                } else {valid_grid[cursor_y][cursor_x + i] = 'X';}
            }
        }
        if (inRange(i)) {
            if (!bcol_obstruct) {
                if (perm) {
                    if (isBlack(piece)) {x_permutations[cursor_y + i][cursor_x] = '*';}
                    if (isWhite(piece)) {x_permutations[cursor_y + i][cursor_x] = 'X';}
                } else {valid_grid[cursor_y + i][cursor_x] = 'X';}
            }
        }
        if (inRange(0, -i)) {
            if (!lrow_obstruct) {
                if (perm) {
                    if (isBlack(piece)) {x_permutations[cursor_y][cursor_x - i] = '*';}
                    if (isWhite(piece)) {x_permutations[cursor_y][cursor_x - i] = 'X';}
                } else {valid_grid[cursor_y][cursor_x - i] = 'X';}
            }
        }
        if (inRange(-i)) {
            if (!tcol_obstruct) {
                if (perm) {
                    if (isBlack(piece)) {x_permutations[cursor_y - i][cursor_x] = '*';}
                    if (isWhite(piece)) {x_permutations[cursor_y - i][cursor_x] = 'X';}
                } else {valid_grid[cursor_y - i][cursor_x] = 'X';}
            }
        } 
 
    } 
}
void Board::kingValid(bool perm) {

    char piece = grid[cursor_y][cursor_x];
    // if (isBlack(piece)) {
    //     bking_y = cursor_y, bking_x = cursor_x;
    //     if (perm) {x_permutations[cursor_y][cursor_x] = '*';}
    //     else {valid_grid[cursor_y][cursor_x] = 'X';}
    // } else if (isWhite(piece)) {
    //     wking_y = cursor_y, wking_x = cursor_x;
    //     if (perm) {x_permutations[cursor_y][cursor_x] = 'X';}
    //     else {valid_grid[cursor_y][cursor_x] = 'X';}
    // }
    
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if ((isWhite(grid[cursor_y + i][cursor_x + j]) || isBlack(grid[cursor_y + i][cursor_x + j])) && !isKing(grid[cursor_y + i][cursor_x + j])) {
                if (isEnemy(grid[cursor_y + i][cursor_x + j])) {
                    if (perm) {x_permutations[cursor_y + i][cursor_x + j] = 'K';}
                    else {valid_grid[cursor_y + i][cursor_x + j] = 'K';}
                } 
            } else if (isBlack(piece) && (x_permutations[cursor_y + i][cursor_x + j] == 'X' || x_permutations[cursor_y + i][cursor_x + j] == 'K')) {
                valid_grid[cursor_y + i][cursor_x + j] = '.';
            } else if (isWhite(piece) && (x_permutations[cursor_y + i][cursor_x + j] == '*' || x_permutations[cursor_y + i][cursor_x + j] == 'K')) {
                valid_grid[cursor_y + i][cursor_x + j] = '.';
            } else {
                if (perm) {
                    if (isBlack(piece)) {x_permutations[cursor_y + i][cursor_x + j] = '*'; bking_y = cursor_y, bking_x = cursor_x;}
                    else if (isWhite(piece)) {x_permutations[cursor_y + i][cursor_x + j] = 'X'; wking_y = cursor_y, wking_x = cursor_x;}
                } else {valid_grid[cursor_y + i][cursor_x + j] = 'X';}        
            }

        }
    }
}
void Board::horseValid(bool perm) {
    char piece = grid[cursor_y][cursor_x];
    if (isBlack(piece)) {
        if (perm) {x_permutations[cursor_y][cursor_x] = '*';}
        else {valid_grid[cursor_y][cursor_x] = 'X';}
    } else if (isWhite(piece)) {
        if (perm) {x_permutations[cursor_y][cursor_x] = 'X';}
        else {valid_grid[cursor_y][cursor_x] = 'X';}
    }

    for (int i = -1; i < 2; i += 2) {
        for (int j = -2; j < 4; j += 4) {
            if (inRange(i, j)) {
                if (isBlack(grid[cursor_y + i][cursor_x + j]) || isWhite(grid[cursor_y + i][cursor_x + j])) {
                    if (isEnemy(grid[cursor_y + i][cursor_x + j])) {
                        if (perm) {
                            x_permutations[cursor_y + i][cursor_x + j] = 'K';  
                        } else {valid_grid[cursor_y + i][cursor_x + j] = 'K';}
                    }
                } else { 
                    if (perm) {
                        if (isBlack(piece)) { x_permutations[cursor_y + i][cursor_x + j] = '*'; }
                        if (isWhite(piece)) { x_permutations[cursor_y + i][cursor_x + j] = 'X'; }
                    } else { valid_grid[cursor_y + i][cursor_x + j] = 'X'; }
                } 
            }
            if (inRange(j, i)) {
                if (isBlack(grid[cursor_y + j][cursor_x + i]) || isWhite(grid[cursor_y + j][cursor_x + i])) {
                    if (isEnemy(grid[cursor_y + j][cursor_x + i])) {
                        if (perm) {
                            x_permutations[cursor_y + j][cursor_x + i] = 'K';
                        } else {valid_grid[cursor_y + j][cursor_x + i] = 'K';}
                    }
                } else { 
                    if (perm) {
                        if (isBlack(piece)) { x_permutations[cursor_y + j][cursor_x + i] = '*'; }
                        if (isWhite(piece)) { x_permutations[cursor_y + j][cursor_x + i] = 'X'; } 
                    } else { valid_grid[cursor_y + j][cursor_x + i] = 'X'; }
                }
            }
        }
    }

}
void Board::pawnValid(bool perm) {
    char piece = grid[cursor_y][cursor_x];
     // diagonal kill piece
    for (int i = 0; i < 3; i++) {
        if (isBlack(piece) && inRange(i) && (!isBlack(grid[cursor_y + i][cursor_x]) && !isWhite(grid[cursor_y + i][cursor_x]))) {
            if (cursor_y == 1) {
                if (perm) {x_permutations[cursor_y + i][cursor_x] = '*';}
                else {valid_grid[cursor_y + i][cursor_x] = 'X';}
            }
            if (i < 2) {
                if (perm) {x_permutations[cursor_y + i][cursor_x] = '*';}
                else {valid_grid[cursor_y + i][cursor_x] = 'X';}
            }

        }
        if (isWhite(piece) && inRange(-i) && (!isBlack(grid[cursor_y - i][cursor_x]) && !isWhite(grid[cursor_y - i][cursor_x]))) {

            if (cursor_y == 6) {
                if (perm) {x_permutations[cursor_y - i][cursor_x] = 'X';}
                else {valid_grid[cursor_y - i][cursor_x] = 'X';}
            }
            if (i < 2) {
                if (perm) {x_permutations[cursor_y - i][cursor_x] = 'X';}
                else {valid_grid[cursor_y - i][cursor_x] = 'X';}
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        // bottom right
        if (isBlack(piece)) {

            if (isEnemy(grid[cursor_y + i][cursor_x + i]) && inRange(i, i)) {
                if (perm) {x_permutations[cursor_y + i][cursor_x + i] = 'K';}
                else {valid_grid[cursor_y + i][cursor_x + i] = 'K';}
            }
            // bottom left
            if (isEnemy(grid[cursor_y + i][cursor_x - i]) && inRange(i, -i)) {
                if (perm) {x_permutations[cursor_y + i][cursor_x - i] = 'K';} 
                else {valid_grid[cursor_y + i][cursor_x - i] = 'K';}
            }
        } 
        if (isWhite(piece)) {
            // top right
            if (isEnemy(grid[cursor_y - i][cursor_x + i]) && inRange(-i, i)) {
                if (perm) {x_permutations[cursor_y - i][cursor_x + i] = 'K';}
                else {valid_grid[cursor_y - i][cursor_x + i] = 'K';}
            }
            // top left
            if (isEnemy(grid[cursor_y - i][cursor_x - i]) && inRange(-i, -i)) {
                if (perm) {x_permutations[cursor_y - i][cursor_x - i] = 'K';}
                else {valid_grid[cursor_y - i][cursor_x - i] = 'K';}
            }
        }
    }
}

// could do it in one method but this just is more readable
void Board::validBlackMove() {
    char piece = grid[cursor_y][cursor_x];
    switch(piece) {
        case 'q': bishopValid(); rookValid();break;

        case 'k': kingValid(); break;

        case 'r': rookValid(); break;

        case 'b': bishopValid(); break;

        case 'h': horseValid(); break;

        case 'p': pawnValid(); break;
    }
}
void Board::validWhiteMove() {
    char piece = grid[cursor_y][cursor_x];
    switch(piece) {
        case 'Q': bishopValid(); rookValid(); break;

        case 'K': kingValid(); break;

        case 'R': rookValid(); break;

        case 'B': bishopValid(); break;

        case 'H': horseValid(); break;
        // case 'P':
        case 'P': pawnValid(); break;
    }
}

int main() {
    Board board;
    // needed to initialise the game
    board.initBoard();
    board.initValidBoard();
    board.initXPerm();

    board.allValid();
    while (!board.checkmate()) {
        board.printBoard();
        board.initValidBoard();
        board.choosePiece();
        if (board.white_turn) { board.validWhiteMove(); } 
        else if (board.black_turn) { board.validBlackMove(); }
    }
}