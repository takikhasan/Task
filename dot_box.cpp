#include<bits/stdc++.h>
#include<windows.h>
//#include<graphics.h>
using namespace std;


template<typename T1, typename T2>
inline ostream& operator<<(ostream& os, pair<T1, T2> p) { os << "{" << p.first << ", " << p.second << "}"; return os; }
template<typename T>
inline ostream& operator<<(ostream& os, vector<T>& a) { os << "["; for (int i = 0; i < (int)a.size(); i++) { if (i) os << ", "; os << a[i]; } os << "]"; return os; }


#define error(args...) { string _s = #args; replace(_s.begin(), _s.end(), ',', ' '); stringstream _ss(_s); istream_iterator<string> _it(_ss); err(_it, args); }
void err(istream_iterator<string> it) {}
template<typename T, typename... Args>
void err(istream_iterator<string> it, T a, Args... args) {
	cout << *it << " = " << a << endl;
	err(++it, args...);
}



int theme_1_h = 205;
int theme_1_v = 186;


HANDLE hConsole;
int iterations = 0;


class Board
{
    int n;
    vector<vector<vector<bool>>> vis;
    vector<vector<int>> point;
    int maxPlayer = 1;
    int minPlayer = 2;

public:
    bool lastMoveSecured = 0;
    Board() {}
    Board(int n) : n(n), point(n, vector<int>(n, 0))
    {
        vis = vector<vector<vector<bool>>> (n, vector<vector<bool>> (n, vector<bool>(4, 0)));
    }
    Board(int n, vector<vector<vector<bool>>> vis, vector<vector<int>> point) : n(n), vis(vis), point(point) {}

    void color_box(int r, int c, int side, vector<vector<int>> &mark, vector<pair<int,int>> &curr) const
    {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        for (auto p : curr) {
            mark[p.first][p.second] = vis[r][c][side] ? 13 : 15;
        }
        curr.clear();
    }
    void color_rc(int r, int c, vector<vector<int>> &mark, vector<pair<int,int>> &curr) const
    {
        for (auto p : curr) {
            if (point[r][c]) mark[p.first][p.second] = point[r][c] == 1 ? 10 : 12;
        }
        curr.clear();
    }
    void print() const
    {
        #define pb push_back
        vector<vector<char>> grid(n * 4, vector<char>(n * 4, 0));
        vector<vector<int>> mark(n * 4, vector<int>(n * 4, 15));
        vector<pair<int,int>> curr;
        system("cls");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (!i && !j) grid[i][0] = 206; // top-left
                if (!i) { grid[i][j*3+2+1] = 206; curr.pb({i, j*3 + 1}); curr.pb({i, j*3 + 2}); color_box(i, j, 0, mark, curr); grid[i][j*3+1] = grid[i][j*3+1+1] = theme_1_h; } // top-right, top-mid, top-mid,
                if (!j) {
                    curr.pb({i*2 +1, 0});
                    color_box(i, j, 3, mark, curr);
                    grid[i*2+1][0] = theme_1_v; // middle-left
                }
                curr.pb({i*2+1, j*3+3});
                color_box(i, j, 1, mark, curr);
                curr.pb({i*2+1, j*3+1});
                curr.pb({i*2+1, j*3+2});
                color_rc(i, j, mark, curr);
                grid[i*2+1][j*3+2+1] = theme_1_v; grid[i*2+1][j*3+1] = '1' + i; grid[i*2+1][j*3+1+1] = '1' + j; // middle-right, digit, digit
                if (!j) grid[i*2+1+1][0] = 206; // bottom-left
                grid[i*2+1+1][j*3+2+1] = 206; curr.pb({i*2+2,j*3+1}); curr.pb({i*2+2, j*3+2}); color_box(i, j, 2, mark, curr); grid[i*2+1+1][j*3+1] = grid[i*2+1+1][j*3+1+1] = theme_1_h; // bottom-right, bottom-mid, bottom-mid;
            }
        }
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[0].size(); j++) {
                SetConsoleTextAttribute(hConsole, mark[i][j]);
                cout << grid[i][j];
            }
            cout << endl;
        }
    }
    int evaluate()
    {
        int score = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (point[i][j] == maxPlayer) score++;
                else if (point[i][j] == minPlayer) score--;
            }
        }
        return score;
    }
    bool game_over()
    {
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (point[i][j]) cnt++;
            }
        }
        return cnt == n * n;
    }
    bool playMove(int r, int c, int side, int curr_player)
    {
        if (r < 0 || c < 0 || r >= n || c >= n || side < 0 || side >= 4) return 0;
        if (vis[r][c][side]) return 0;
        lastMoveSecured = 0;
        bool before = vis[r][c][side];
        vis[r][c][side] = 1;
        bool b0;
        if (side == 0) if (r) b0 = vis[r-1][c][2];
        bool b1;
        if (side == 1) if (c != n - 1) b1 = vis[r][c+1][3];
        bool b2;
        if (side == 2) if (r != n - 1) b2 = vis[r+1][c][0];
        bool b3;
        if (side == 3) if (c) b3 = vis[r][c-1][1];
        if (side == 0) if (r) vis[r-1][c][2] = 1;
        if (side == 1) if (c != n - 1) vis[r][c+1][3] = 1;
        if (side == 2) if (r != n - 1) vis[r+1][c][0] = 1;
        if (side == 3) if (c) vis[r][c-1][1] = 1;
        for (int i = r - 1; i <= r + 1; i++) {
            for (int j = c - 1; j <= c + 1; j++) {
                if (i >= 0 && i < n && j >= 0 && j < n) {
                    vis[r][c][side] = before;
                    if (side == 0) if (r) vis[r-1][c][2] = b0;
                    if (side == 1) if (c != n - 1) vis[r][c+1][3] = b1;
                    if (side == 2) if (r != n - 1) vis[r+1][c][0] = b2;
                    if (side == 3) if (c) vis[r][c-1][1] = b3;
                    int prev = vis[i][j][0] + vis[i][j][1] + vis[i][j][2] + vis[i][j][3];
                    vis[r][c][side] = 1;
                    if (side == 0) if (r) vis[r-1][c][2] = 1;
                    if (side == 1) if (c != n - 1) vis[r][c+1][3] = 1;
                    if (side == 2) if (r != n - 1) vis[r+1][c][0] = 1;
                    if (side == 3) if (c) vis[r][c-1][1] = 1;
                    int curr = vis[i][j][0] + vis[i][j][1] + vis[i][j][2] + vis[i][j][3];
                    if (curr == 4 && prev == 3) {
                        point[i][j] = curr_player;
                        lastMoveSecured = 1;
                    }
                }
            }
        }
        return 1;
    }
    vector<Board> generateBoards(int currPlayer)
    {
        vector<Board> newBoards;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < 4; k++) {
                    if (!vis[i][j][k]) {
                        Board newBoard(n, vis, point);
                        newBoard.playMove(i, j, k, currPlayer);
                        newBoards.push_back(newBoard);
                    }
                }
            }
        }
        return newBoards;
    }
    int minimax(int depth, int alpha, int beta, int currPlayer)
    {
        iterations++;
//        if (iterations > 10000) return evaluate();
        if (game_over()) return evaluate();
        if (depth <= 0) return evaluate();
        vector<Board> newBoards = generateBoards(currPlayer);
        if (currPlayer == maxPlayer) {
            int maxEval = INT_MIN;
            for (child : newBoards) {
                int eval = child.minimax(depth - 1, alpha, beta, !child.lastMoveSecured ? (3 - currPlayer) : currPlayer);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) break;
            }
            return maxEval;
        }
        else {
            int minEval = INT_MAX;
            for (child : newBoards) {
                int eval = child.minimax(depth - 1, alpha, beta, !child.lastMoveSecured ? (3 - currPlayer) : currPlayer);
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) break;
            }
            return minEval;
        }
    }
    vector<int> bestMove(int player)
    {
        int limit = 100000;
        int curr = 1;
        int max_depth = 0;
        while (curr < limit) {
            max_depth++;
            curr *= 8 * (n + 2) * (n + 2);
        }
//        error(max_depth);
        int best_eval = minimax(max_depth, INT_MIN, INT_MAX, player);
//        error(best_eval);
        iterations = 1;
        int diff = INT_MAX, a, b, c;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < 4; k++) {
                    if (!vis[i][j][k]) {
                        Board newBoard(n, vis, point);
                        newBoard.playMove(i, j, k, player);
                        if (abs(newBoard.minimax(max_depth - 1, INT_MIN, INT_MAX, player) - best_eval) < diff) {
                            diff = abs(newBoard.minimax(max_depth - 1, INT_MIN, INT_MAX, player) - best_eval);
                            a = i; b = j; c = k;
                        }
                    }
                }
            }
        }
        return {a, b, c};
    }
};

class Agent
{
    string name;
public:
    Agent() {}
    Agent(string name) : name(name) {}
    string getName() {
        return name;
    }
};

class HumanAgent : public Agent
{
public:
    HumanAgent() : Agent("Human") {}
    HumanAgent(string name) : Agent(name) {}
    bool playMove(Board &board, int playerOrder) {
        int r, c, side;
        cin >> r >> c >> side;
        return board.playMove(r - 1, c - 1, side, playerOrder);
    }
};

class AlphaBetaAgent : public Agent
{
public:
    AlphaBetaAgent() : Agent("Computer") {}
    AlphaBetaAgent(string name) : Agent(name) {}
    void playMove(Board &board, int playerOrder) {
        vector<int> bestMove = board.bestMove(2);
        board.playMove(bestMove[0], bestMove[1], bestMove[2], playerOrder);
    }
};

class GameHumanVsComputer
{
    HumanAgent H;
    AlphaBetaAgent C;
    Board B;
    int currPlayer = 1; /** 1 is human (maxPlayer), 2 is computer (minPlayer) */
public:
    GameHumanVsComputer() {}
    GameHumanVsComputer(int n) : H("Tony"), C("Jarvis"), B(n) {}
    GameHumanVsComputer(int n, string hname, string cname) : H(hname), C(cname), B(n) {}
    bool gameOver() {
        return B.game_over();
    }
    void playNextMove() {
        B.print();
        iterations = 0;
        if (currPlayer == 1) {
            bool valid = H.playMove(B, 1);
            if (valid && !B.lastMoveSecured) currPlayer = 2;
        }
        else {
            C.playMove(B, 2);
            if (!B.lastMoveSecured) currPlayer = 1;
        }
//        error("Played");
//        Sleep(2000);
        B.print();
    }
    void victoryMessage() {
        int res = B.evaluate();
        if (res == 0) cout << "Amazing, it's a draw!!!" << endl;
        else if (res > 0) cout << H.getName() << " (Human, green) won the match by " << res << " points!" << endl;
        else cout << C.getName() << " (Computer, red) won the match by " << -res << " points!" << endl;
        cout << endl;
    }
};

class GameComputerVsComputer
{
    AlphaBetaAgent H;
    AlphaBetaAgent C;
    Board B;
    int currPlayer = 1; /** 1 is computer (H: maxPlayer), 2 is computer (C: minPlayer) */
public:
    GameComputerVsComputer() {}
    GameComputerVsComputer(int n) : H("Sherlock"), C("Skynet"), B(n) {}
    GameComputerVsComputer(int n, string hname, string cname) : H(hname), C(cname), B(n) {}
    bool gameOver() {
        return B.game_over();
    }
    void playNextMove() {
        B.print();
        if (currPlayer == 1) {
            H.playMove(B, 1);
            if (!B.lastMoveSecured) currPlayer = 2;
        }
        else {
            C.playMove(B, 2);
            if (!B.lastMoveSecured) currPlayer = 1;
        }
        B.print();
        Sleep(200);
    }
    void victoryMessage() {
        int res = B.evaluate();
        if (res == 0) cout << "Amazing, it's a draw!!!" << endl;
        else if (res > 0) cout << H.getName() << " (AI, green) won the match by " << res << " points!" << endl;
        else cout << C.getName() << " (AI, red) won the match by " << -res << " points!" << endl;
        cout << endl;
    }
};

class DriverType1
{
    GameHumanVsComputer G;
public:
    DriverType1(int n) : G(n) {}
    DriverType1(int n, string hname, string cname) : G(n, hname, cname) {}
    void Play() {
        while (!G.gameOver()) G.playNextMove();
        G.victoryMessage();
    }
};

class DriverType2
{
    GameComputerVsComputer G;
public:
    DriverType2(int n) : G(n) {}
    DriverType2(int n, string hname, string cname) : G(n, hname, cname) {}
    void Play() {
        while (!G.gameOver()) G.playNextMove();
        G.victoryMessage();
    }
};

int main()
{
    cout << "(Play against computer / Watch AI vs AI game) ? (1 / 2) : ";
    int option;
    cin >> option;
    if (option == 1) {
        int sz;
        cout << "Grid square side length? : ";
        cin >> sz;
        string n1, n2;
        cout << "Player 1 name : ";
        cin >> n1;
        cout << "Player 2 name : ";
        cin >> n2;
        DriverType1 playstation(sz, n1, n2);
        playstation.Play();
    }
    else {
        int sz;
        cout << "Grid square side length? : ";
        cin >> sz;
        string n1, n2;
        cout << "Player 1 name : ";
        cin >> n1;
        cout << "Player 2 name : ";
        cin >> n2;
        DriverType2 playstation(sz, n1, n2);
        playstation.Play();
    }

    return 0;
}
