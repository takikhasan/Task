#include<bits/stdc++.h>
using namespace std;

struct Board
{
    int n;
    vector<vector<char> > grid;
    int h() const
    {
        vector<pair<int, int> > queens;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 'Q') {
                    queens.push_back({i, j});
                }
            }
        }
        int ret = 0;
        for (int i = 0; i < queens.size(); i++) {
            for (int j = i + 1; j < queens.size(); j++) {
                int rowDiff = abs(queens[i].first - queens[j].first);
                int colDiff = abs(queens[i].second - queens[j].second);
                if (!rowDiff || !colDiff || rowDiff == colDiff) {
                    ret++;
                }
            }
        }
        return ret;
    }
    vector<Board> generateStates()
    {
        vector<Board> ret;
        for (int i = 0; i < n; i++) {
            Board z;
            z.n = n;
            z.grid = grid;
            for (int j = 0; j < n; j++) {
                z.grid[j][i] = '.';
            }
            for (int j = 0; j < n; j++) {
                if (grid[j][i] != 'Q') {
                    z.grid[j][i] = 'Q';
                    ret.push_back(z);
                    z.grid[j][i] = '.';
                }
            }
        }
        return ret;
    }
    void print()
    {
        cout << "Board Grid: " << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << grid[i][j] << ' ';
            }
            cout << endl;
        }
    }
};

inline bool operator<(const Board& lhs, const Board& rhs)
{
    return lhs.h() < rhs.h();
}

void hill_climbing(Board curr)
{
    int i = 1;
    while (1) {
        cout << "Iteration: " << i++ << "\t" << "h(Board): " << curr.h() << endl;
        curr.print();
        cout << endl;
        vector<Board> states = curr.generateStates();
        int hmin = INT_MAX;
        Board next;
        for (Board b : states) {
            if (hmin > b.h()) {
                hmin = b.h();
                next = b;
            }
        }
        if (hmin < curr.h()) {
            curr = next;
        }
        else {
            break;
        }
    }
}

int main()
{
    Board b;
    b.n = 4;
    b.grid = {
        { '.', '.', '.', '.' },
        { '.', 'Q', '.', 'Q' },
        { 'Q', '.', 'Q', '.' },
        { '.', '.', '.', '.' }
    };
    hill_climbing(b);


    return 0;
}
