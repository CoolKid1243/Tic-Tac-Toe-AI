#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
using namespace std;

char checkWin(char board[3][3]) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != '_') {
            return board[i][0];
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != '_') {
            return board[0][i];
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '_') {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '_') {
        return board[0][2];
    }

    // Check for draw
    bool draw = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '_') {
                draw = false;
            }
        }
    }
    if (draw) return 'D';

    // Game is still going
    return '_';
}

int evaluate(char board[3][3]) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == 'O') return +10;
            else if (board[i][0] == 'X') return -10;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == 'O') return +10;
            else if (board[0][i] == 'X') return -10;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'O') return +10;
        else if (board[0][0] == 'X') return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'O') return +10;
        else if (board[0][2] == 'X') return -10;
    }

    // No winner
    return 0;
}

int minimax(char board[3][3], int depth, bool isMax) {
    int score = evaluate(board);

    // If AI has won
    if (score == 10)
        return score - depth; // Prefer faster wins
    // If player has won
    if (score == -10)
        return score + depth; // Prefer slower losses

    // If no moves left, it's a draw
    bool movesLeft = false;
    for (int i = 0; i < 3 && !movesLeft; i++) {
        for (int j = 0; j < 3 && !movesLeft; j++) {
            if (board[i][j] == '_') {
                movesLeft = true;
            }
        }
    } 

    if (!movesLeft) {
        return 0;
    }

    if (isMax) {
        int best = -1000;
        // Try all moves for AI
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '_') {
                    board[i][j] = 'O';
                    best = max(best, minimax(board, depth + 1, false));
                    board[i][j] = '_';
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        // Try all moves for player
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '_') {
                    board[i][j] = 'X';
                    best = min(best, minimax(board, depth + 1, true));
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }
}

struct Move {
    int row, col;
};

Move findBestMove(char board[3][3]) {
    int bestVal = -1000;
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Check if cell is empty
            if (board[i][j] == '_') {
                board[i][j] = 'O'; // AI makes move
                int moveVal = minimax(board, 0, false);
                board[i][j] = '_'; // Undo move

                if (moveVal > bestVal) {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

void printBoard(char board[3][3]) {
    cout << "\n    0   1   2\n";
    for (int i = 0; i < 3; i++) {
        cout << "  +---+---+---+\n";
        cout << i << " |";
        for (int j = 0; j < 3; j++) {
            char cell = (board[i][j] == '_') ? ' ' : board[i][j];
            cout << " " << cell << " |";
        }
        cout << endl;
    }
    cout << "  +---+---+---+\n" << endl;
}

int main() {
    int difficulty = 0;
    cout << "Choose AI difficulty (1 - 5): ";
    cin >> difficulty;
    difficulty = max(1, min(5, difficulty)); // Clamp between 1 and 5

    // Create the board
    char board[3][3] = {
        {'_', '_', '_'},
        {'_', '_', '_'},
        {'_', '_', '_'}
    };

    while (true) {
        // Print the board
        system("clear");
        printBoard(board);

        // Player move
        int row, col;
        cout << "Enter your move (horizontal and vertical): ";
        cin >> row >> col;
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == '_') {
            board[row][col] = 'X';
        } else {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        // Check for win/draw after player move
        char result = checkWin(board);
        if (result == 'X') {
            printBoard(board);
            cout << "X wins!" << endl;
            break;
        }
        if (result == 'D') {
            printBoard(board);
            cout << "It's a draw!" << endl;
            break;
        }

        // AI move
        bool aiPlayed = false;
        int chance = rand() % 5 + 1; // 1 to 5
        if (chance > difficulty) {
            // Play random move (easy)
            vector<Move> possibleMoves;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (board[i][j] == '_') {
                        possibleMoves.push_back({i, j});
                    }
                }
            }

            if (!possibleMoves.empty()) {
                Move randomMove = possibleMoves[rand() % possibleMoves.size()];
                board[randomMove.row][randomMove.col] = 'O';
                cout << "AI played (random): " << randomMove.row << " " << randomMove.col << endl;
                aiPlayed = true;
            }
        } else if (!aiPlayed) {
            // Play best move (hard)
            Move aiMove = findBestMove(board);
            if (aiMove.row != -1 && aiMove.col != -1) {
                board[aiMove.row][aiMove.col] = 'O';
                cout << "AI played (smart): " << aiMove.row << " " << aiMove.col << endl;
            }
        }

        // Check for win/draw after AI move
        result = checkWin(board);
        if (result == 'O') {
            printBoard(board);
            cout << "AI wins!" << endl;
            break;
        }
        if (result == 'D') {
            printBoard(board);
            cout << "It's a draw!" << endl;
            break;
        }
    }

    return 0;
}
