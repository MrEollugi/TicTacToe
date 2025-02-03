#include <iostream>
#include <vector>
#include <climits>

using namespace std;

const char PLAYER_X = 'X';
const char PLAYER_O = 'O';
const char EMPTY = ' ';

// 보드 출력 함수
void printBoard(const vector<vector<char>>& board) {
    cout << endl; // 빈 줄을 출력해 보드가 시작되는 부분과 이전 출력 사이에 간격을 생성
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << " " << board[i][j] << " ";
            if (j < 2) cout << "|"; // 셀 구분선
        }
        cout << endl;
        if (i < 2) {
            cout << "---|---|---" << endl; // 가로 구분선
        }
    }
    cout << endl; 
}

// 유효한 입력을 받기 위한 함수
bool isValidMove(const vector<vector<char>>& board, int row, int col) {
    if (row < 0 || row >= 3 || col < 0 || col >= 3) {
        cout << "Invalid input! Please enter row and column between 0 and 2." << endl;
        return false;
    }
    if (board[row][col] != EMPTY) {
        cout << "Invalid move! This position is already occupied." << endl;
        return false;
    }
    return true;
}

// 승리 체크 함수
bool checkWin(const vector<vector<char>>& board, char player) {
    // 가로, 세로, 대각선 체크
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return true;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;
    return false;
}

// 보드가 가득 찼는지 확인하는 함수
bool isBoardFull(const vector<vector<char>>& board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) return false;
        }
    }
    return true;
}

// Minimax 알고리즘으로 AI가 최적의 수를 찾는 함수
int minimax(vector<vector<char>>& board, int depth, bool isMaximizing) {
    if (checkWin(board, PLAYER_X)) return 10 - depth;
    if (checkWin(board, PLAYER_O)) return depth - 10;
    if (isBoardFull(board)) return 0;

    int best = isMaximizing ? INT_MIN : INT_MAX;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = isMaximizing ? PLAYER_X : PLAYER_O;
                int current = minimax(board, depth + 1, !isMaximizing);
                best = isMaximizing ? max(best, current) : min(best, current);
                board[i][j] = EMPTY;
            }
        }
    }
    return best;
}

// AI가 최적의 수를 선택하는 함수
pair<int, int> findBestMove(vector<vector<char>>& board) {
    int bestVal = INT_MIN;
    pair<int, int> bestMove = { -1, -1 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_X;
                int moveVal = minimax(board, 0, false);
                board[i][j] = EMPTY;

                if (moveVal > bestVal) {
                    bestMove = { i, j };
                    bestVal = moveVal;
                }
            }
        }
    }

    return bestMove;
}

// 게임 진행
bool playGame() {
    vector<vector<char>> board(3, vector<char>(3, EMPTY));
    bool playerTurn = false;  // False = Player O's turn, True = AI X's turn
    int row, col;

    while (true) {
        printBoard(board);

        if (playerTurn) {
            // AI 턴
            cout << "AI's turn (X)" << endl;
            pair<int, int> bestMove = findBestMove(board);
            row = bestMove.first;
            col = bestMove.second;
            board[row][col] = PLAYER_X;
        }
        else {
            // 플레이어 턴
            cout << "Player's turn (O). Enter row and column (0-2): ";
            cin >> row >> col;

            if (board[row][col] != EMPTY) {
                cout << "Invalid move! Try again." << endl;
                continue;
            }

            board[row][col] = PLAYER_O;
        }

        // 게임 종료 조건 체크
        if (checkWin(board, PLAYER_X)) {
            printBoard(board);
            cout << "AI (X) wins!" << endl;
            break;
        }
        if (checkWin(board, PLAYER_O)) {
            printBoard(board);
            cout << "YOU WIN !!" << endl;
            break;
        }
        if (isBoardFull(board)) {
            printBoard(board);
            cout << " DRAW ! " << endl;
            break;
        }

        // 차례 변경
        playerTurn = !playerTurn;
    }

    // 재시작 여부
    char playAgain;
    cout << "Try Again ? (Y/N): ";
    cin >> playAgain;
    return (playAgain == 'Y' || playAgain == 'y');
}

int main() {
    // 게임 반복 실행
    do {
        if (!playGame()) {
            cout << "Thanks for playing!" << endl;
            break;
        }
    } while (true);

    return 0;
}
