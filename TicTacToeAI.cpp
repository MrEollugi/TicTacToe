#include <iostream>
#include <vector>
#include <climits>

using namespace std;

const char PLAYER_X = 'X';
const char PLAYER_O = 'O';
const char EMPTY = ' ';

// 보드 출력 함수
void printBoard(const vector<vector<char>>& board) { // 2차원 벡터로 각 요소가 다른 vector<char>인 구조로 생성, 벡터 내에 벡터가 있는 형태로 행렬 또는 2D 배열처럼 사용 가능, & 참조자를 사용해 board의 복사본을 만들지 않고 원본 데이터를 참조해 성능을 최적화함
    cout << endl; // 빈 줄을 출력해 보드가 시작되는 부분과 이전 출력 사이에 간격을 생성
    for (int i = 0; i < 3; i++) { // 각행 출력
        for (int j = 0; j < 3; j++) { // 각 열 출력
            cout << " " << board[i][j] << " ";
            if (j < 2) cout << "|"; // 셀 구분선 추가
        }
        cout << endl;
        if (i < 2) {
            cout << "---|---|---" << endl; // 가로 구분선
        }
    }
    cout << endl; 
}

// 유효한 입력을 받기 위한 함수
bool isValidMove(const vector<vector<char>>& board, int row, int col) { // 현재 게임 보드의 상태를 참조 받으며 const를 이용해 보드의 내용은 변경하지 않도록 함.
    if (row < 0 || row >= 3 || col < 0 || col >= 3) { // 사용자가 입력한 row와 col이 유효한 범위 내에 있는지 확인하는 조건문
        cout << "Invalid input! Please enter row and column between 0 and 2." << endl;
        return false;
    }
    if (board[row][col] != EMPTY) { // 그리고 이건 사용자가 선택한 위치가 이미 채워져 있는지 확인하는 조건문
        cout << "Invalid move! This position is already occupied." << endl;
        return false;
    }
    return true; // 다 통과하면 true를 리턴
}

// 승리 체크 함수
bool checkWin(const vector<vector<char>>& board, char player) { // 위와 같이 현재 게임 보드 상태 참조하고 const로 변경하지 않도록, char player는 승리 여부를 확인하려는 플레이어
    // 가로, 세로, 대각선 체크
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) // 이게 가로 확인
            return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) // 이게 세로 확인
            return true;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) // 대각선 왼쪽 상단에서 오른쪽 하단
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) // 대각선 오른쪽 상단에서 왼쪽 하단
        return true;
    return false; // 충족 못하면 false 리턴
}

// 보드가 가득 찼는지 확인하는 함수
bool isBoardFull(const vector<vector<char>>& board) { // 위와 같음
    for (int i = 0; i < 3; i++) {  // i는 행 확인
        for (int j = 0; j < 3; j++) { // j는 열 확인
            if (board[i][j] == EMPTY) return false; // 비어있으니 false 리턴
        }
    }
    return true; // 꽉 찼으니 true 리턴
}

// Minimax 알고리즘으로 AI가 최적의 수를 찾는 함수(이 함수는 현재 게임보드의 상태에 대한 평가값을 리턴해야 하고 평가값은 10 - depth, depth - 10, 또는 0이 될 수 있으므로 int로 선언)
int minimax(vector<vector<char>>& board, int depth, bool isMaximizing) { //위와 같고, 현재 게임 트리의 깊이로 게임 진행 정도를 표시 보통은 0으로 시작함, 현재 차례가 누구 차례인지 true면 AI, false면 플레이어
    if (checkWin(board, PLAYER_X)) return 10 - depth; // ai가 승리할 경우
    if (checkWin(board, PLAYER_O)) return depth - 10; // 플레이어가 승리할 경우
    if (isBoardFull(board)) return 0; // 보드가 가득차 무승부

    int best = isMaximizing ? INT_MIN : INT_MAX; // 현재 상태에서의 최적의 평가값을 best에 저장

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) { // i, j는 보드의 각 칸을 확인하는 변수
            if (board[i][j] == EMPTY) { // 만약 빈칸이면 그 칸에 현재 차례인 대상의 기호를 놓고 다음 턴 계산을 위해 minimax를 재귀호출
                board[i][j] = isMaximizing ? PLAYER_X : PLAYER_O;
                int current = minimax(board, depth + 1, !isMaximizing); // depth를 1만큼 증가, 게임트리 다음 레벨로 내려가 다음 수를 탐색, 차례가 바꼈으니 isMaximizing을 !사용으로 반전
                best = isMaximizing ? max(best, current) : min(best, current); // AI차례일 경우 max(best, current)를 사용해 가장 큰 값 찾기, 플레이어 차례일 경우 min(best, current)를 사용해 가장 작은 값 찾기
                board[i][j] = EMPTY;
            }
        }
    }
    return best; // 모든 이동 탐색 후 최적의 평가값을 리턴, 이 값은 현재 상태에서 최선의 선택을 평가한 값, AI가 할 수 있는 최고의 수를 결정하는데 사용된다.
}

// AI가 최적의 수를 선택하는 함수
pair<int, int> findBestMove(vector<vector<char>>& board) { // 최적의 수에 해당하는 (행, 열)을 리턴할 수 있도록 pair<int, int> 자료형을 사용해 선언, 당연히 보드 상태를 참조하고 수정할 수 있게 함
    int bestVal = INT_MIN; // 모든 수에 대한 최고의 평가값 추적, 최소값에서 시작해 점차 더 높은 값을 찾기위해 이 값을 갱신해 갈 예정
    pair<int, int> bestMove = { -1, -1 }; //최적의 수 위치를 저장하는 변수 초기값은 -1로 설정

    for (int i = 0; i < 3; i++) { // 이중 for문으로 보드 각 칸을 순차적으로 확인, 위와 같이 i는 행 j는 열.
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) { // 현재 칸이 비었으면 AI가 그 칸에 둔 후 결과를 평가
                board[i][j] = PLAYER_X;
                int moveVal = minimax(board, 0, false); // minimax알고리즘 호출 AI가 해당 칸에 둔 후 다음 턴(플레이어)에 대한 평가 값을 계산, 0은 깊이, 플레이어 차례는 false로 설정해 플레이어가 최선의 선택을 하도록 함.
                board[i][j] = EMPTY; // 해당 칸의 평가가 끝나면 그 칸을 다시 빈칸으로 되돌려 다른 이동을 평가할 수 있도록 준비

                if (moveVal > bestVal) { // moveVal은 현재 이동에 대한 평가값 만약 현재 이동에 대한 평가값이 bestVal보다 크다면 이 수가 최선의 수라는 의미가 됨으로, bestMove를 이 위치로 갱신하고 bestVal을 새 평가값으로 갱신.
                    bestMove = { i, j }; 
                    bestVal = moveVal;
                }
            }
        }
    }

    return bestMove; // 최선의 수를 리턴 (AI가 선택할 수)
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
