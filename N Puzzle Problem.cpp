#include<bits/stdc++.h>
using namespace std;

class Board
{
    int n, g, h;
    vector<vector<int>> B;
public:
    Board(int nn, int gg, vector<vector<int>> BB)
    {
        n = nn, g = gg;
        B = BB;
        h = heuristic();
    }
    vector<Board> generateStates()
    {
        int r, c;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (B[i][j] == 0) {
                    r = i;
                    c = j;
                    break;
                }
            }
        }
        vector<Board> ret;
        Board temp = Board(n, g + 1, B);
        if (r) {
            int t = temp.B[r][c];
            temp.B[r][c] = temp.B[r-1][c];
            temp.B[r-1][c] = t;
            ret.push_back(temp);
        }
        temp = Board(n, g + 1, B);
        if (r != (n - 1)) {
            int t = temp.B[r][c];
            temp.B[r][c] = temp.B[r+1][c];
            temp.B[r+1][c] = t;
            ret.push_back(temp);
        }
        temp = Board(n, g + 1, B);
        if (c) {
            int t = temp.B[r][c];
            temp.B[r][c] = temp.B[r][c-1];
            temp.B[r][c-1] = t;
            ret.push_back(temp);
        }
        temp = Board(n, g + 1, B);
        if (c != (n - 1)) {
            int t = temp.B[r][c];
            temp.B[r][c] = temp.B[r][c+1];
            temp.B[r][c+1] = t;
            ret.push_back(temp);
        }
        return ret;
    }
    int heuristic()
    {
        int ret = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int d = i * n + j + 1;
                if (d == (n * n)) d = 0;
                if (B[i][j] != d) {
                    ret++;
                }
            }
        }
        return ret;
    }
    bool operator < (const Board& newBoard) const
    {
        int h1 = g + h;
        int h2 = newBoard.g + newBoard.h;
        if (h1 < h2)
            return 1;
        else if (h1 > 2)
            return 0;
        else
            return B < newBoard.B;
    }
    bool operator == (const Board& newBoard) const
    {
        return (*this) < newBoard;
    }
};

void A_Star(Board start, Board goal)
{
    set<Board> openList, closedList;
    openList.insert(start);
    while (openList.size()) {
        Board current = *(openList.begin());
        openList.erase(openList.begin());
        closedList.insert(current);
        vector<Board> newStates = current.generateStates();
        for (Board newState: newStates) {
            if (closedList.find(newState) == closedList.end()) {
                auto it = openList.find(newState);
                if (it != openList.end()) {
                    if (newState < *it) {
                        openList.erase(it);
                        openList.insert(newState);

                    }
                }
                else {
                    openList.insert(newState);
//                    par.insert(make_pair(newState, current));
                }
            }
        }
    }
    vector<Board> ans;
    while (1) {
        ans.push_back(goal);
        if (goal == start)
            break;
        else {
            goal = par[goal];
        }
    }
}

int main()
{
    cout << "Enter dimension of the puzzle (N * N) : ";
    int n;
    cin >> n;
    vector<vector<int>> B(n);
    cout << "Enter the puzzle (0 represents starting pos): " << endl;
    for (int i = 0; i < n; i++) {
        B[i].resize(4);
        for (int j = 0; j < n; j++) {
            cin >> B[i][j];
        }
    }
    Board start(n, 0, B);
    A_Star(start, start);

    return 0;
}
