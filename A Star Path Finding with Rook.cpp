#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

int ROW, COL;
int heuristicOption;
int simulationMode;

typedef pair<int, int> Pair;
typedef pair<int, pair<int, int>> pPair;

struct cell
{
	int parent_i, parent_j;
	int f, g, h;
};

bool isValid(int row, int col)
{
	return (row >= 0) && (row < ROW) &&
		(col >= 0) && (col < COL);
}

bool isUnBlocked(vector<vector<int>>& grid, int row, int col)
{
	if (grid[row][col] == 1)
		return (true);
	else
		return (false);
}

bool isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

int calculateHValue(int row, int col, Pair dest)
{
	if (heuristicOption == 1) {
        // Manhattan Distance
        return abs(dest.first - row) + abs(dest.second - col);
	}
	else if (heuristicOption == 2) {
        // Diagonal Distance
        return max(abs(dest.first - row), abs(dest.second - col));
	}
	else if (heuristicOption == 3) {
        // Euclidean Distance
        return (dest.first - row) * (dest.first - row) + (dest.second - col) * (dest.second - col);
	}
}

void tracePath(vector<vector<cell>>& cellDetails, Pair dest)
{
	printf ("The Path is ");
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col))
	{
		Path.push (make_pair (row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push (make_pair (row, col));
	while (!Path.empty())
	{
		pair<int,int> p = Path.top();
		Path.pop();
		printf("-> (%d,%d) ",p.first,p.second);
	}
	cout << endl;

	return;
}

void aStarSearch(vector<vector<int>> grid, Pair src, Pair dest)
{
	if (isValid(src.first, src.second) == false) {
		printf("Source is invalid\n");
		return;
	}

	if (isValid(dest.first, dest.second) == false) {
		printf("Destination is invalid\n");
		return;
	}

	if (isUnBlocked(grid, src.first, src.second) == false || isUnBlocked(grid, dest.first, dest.second) == false) {
		printf ("Source or the destination is blocked\n");
		return;
	}

	if (isDestination(src.first, src.second, dest) == true) {
		printf ("We are already at the destination\n");
		return;
	}

	vector<vector<bool>> closedList;
	vector<vector<cell>> cellDetails;

	closedList.resize(ROW);
    cellDetails.resize(ROW);

	int i, j;

	for (i = 0; i < ROW; i++) {
        closedList[i].resize(COL);
        cellDetails[i].resize(COL);
		for (j = 0; j < COL; j++) {
			cellDetails[i][j].f = INT_MAX;
			cellDetails[i][j].g = INT_MAX;
			cellDetails[i][j].h = INT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
			closedList[i][j] = false;
		}
	}

	// Initializing the parameters of the starting node
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0;
	cellDetails[i][j].g = 0;
	cellDetails[i][j].h = 0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	set<pPair> openList;

	openList.insert(make_pair(0, make_pair (i, j)));

	bool foundDest = false;
	int stepNumber = 1;

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	while (!openList.empty()) {
		pPair p = *openList.begin();
		openList.erase(openList.begin());

		i = p.second.first;
		j = p.second.second;
        if (closedList[i][j] == true)
            continue;
		closedList[i][j] = true;

        if (simulationMode == 1)
            getchar();

        system("cls");
        cout << "Step Number: " << stepNumber++ << endl;

        for (int k = 0; k < ROW; k++) {
            for (int l = 0; l < COL; l++) {
                if (i == k && j == l) {
                    SetConsoleTextAttribute(hConsole, 13);
                    if (src.first == k && src.second == l) {
                        cout << "S ";
                    }
                    else {
                        cout << "* ";
                    }
                }
                else if (src.first == k && src.second == l) {
                    SetConsoleTextAttribute(hConsole, 11);
                    cout << "S ";
                }
                else if (isDestination(k, l, dest)) {
                    SetConsoleTextAttribute(hConsole, 9);
                    cout << "D ";
                }
                else if (closedList[k][l]) {
                    SetConsoleTextAttribute(hConsole, 11);
                    cout << "* ";
                }
                else if (isUnBlocked(grid, k, l)) {
                    SetConsoleTextAttribute(hConsole, 10);
                    cout << "1 ";
                }
                else {
                    SetConsoleTextAttribute(hConsole, 12);
                    cout << "0 ";
                }
            }
            cout << endl;
        }

        Sleep(500);
        SetConsoleTextAttribute(hConsole, 15);

		int gNew, hNew, fNew;

        int step = 0;

		while (isValid(i - ++step, j) == true) {
			if (isDestination(i - step, j, dest) == true) {
				cellDetails[i-step][j].parent_i = i;
				cellDetails[i-step][j].parent_j = j;
				printf ("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i-step][j] == false && isUnBlocked(grid, i - step, j) == true) {
				gNew = cellDetails[i][j].g + 1;
				hNew = calculateHValue(i - step, j, dest);
				fNew = gNew + hNew;

				if (cellDetails[i-step][j].f == INT_MAX || cellDetails[i-step][j].f > fNew) {
					openList.insert(make_pair(fNew, make_pair(i - step, j)));
					cellDetails[i-step][j].f = fNew;
					cellDetails[i-step][j].g = gNew;
					cellDetails[i-step][j].h = hNew;
					cellDetails[i-step][j].parent_i = i;
					cellDetails[i-step][j].parent_j = j;
				}
			}
			else if (!isUnBlocked(grid, i - step, j)) {
                break;
			}
		}

		step = 0;

		while (isValid(i + ++step, j) == true) {
			if (isDestination(i + step, j, dest) == true) {
				cellDetails[i+step][j].parent_i = i;
				cellDetails[i+step][j].parent_j = j;
				printf ("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i+step][j] == false && isUnBlocked(grid, i + step, j) == true) {
				gNew = cellDetails[i][j].g + 1;
				hNew = calculateHValue(i + step, j, dest);
				fNew = gNew + hNew;

				if (cellDetails[i+step][j].f == INT_MAX || cellDetails[i+step][j].f > fNew) {
					openList.insert(make_pair(fNew, make_pair(i + step, j)));
					cellDetails[i+step][j].f = fNew;
					cellDetails[i+step][j].g = gNew;
					cellDetails[i+step][j].h = hNew;
					cellDetails[i+step][j].parent_i = i;
					cellDetails[i+step][j].parent_j = j;
				}
			}
			else if (!isUnBlocked(grid, i + step, j)) {
                break;
			}
		}

		step = 0;

		while (isValid(i, j + ++step) == true) {
			if (isDestination(i, j + step, dest) == true) {
				cellDetails[i][j+step].parent_i = i;
				cellDetails[i][j+step].parent_j = j;
				printf ("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i][j+step] == false && isUnBlocked(grid, i, j + step) == true) {
				gNew = cellDetails[i][j].g + 1;
				hNew = calculateHValue(i, j + step, dest);
				fNew = gNew + hNew;

				if (cellDetails[i][j+step].f == INT_MAX || cellDetails[i][j+step].f > fNew) {
					openList.insert(make_pair(fNew, make_pair(i, j + step)));
					cellDetails[i][j+step].f = fNew;
					cellDetails[i][j+step].g = gNew;
					cellDetails[i][j+step].h = hNew;
					cellDetails[i][j+step].parent_i = i;
					cellDetails[i][j+step].parent_j = j;
				}
			}
			else if (!isUnBlocked(grid, i, j + step)) {
                break;
			}
		}

		step = 0;

		while (isValid(i, j - ++step) == true) {
			if (isDestination(i, j - step, dest) == true) {
				cellDetails[i][j-step].parent_i = i;
				cellDetails[i][j-step].parent_j = j;
				printf ("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i][j-step] == false && isUnBlocked(grid, i, j - step) == true) {
				gNew = cellDetails[i][j].g + 1;
				hNew = calculateHValue(i, j - step, dest);
				fNew = gNew + hNew;

				if (cellDetails[i][j-step].f == INT_MAX || cellDetails[i][j-step].f > fNew) {
					openList.insert(make_pair(fNew, make_pair(i, j - step)));
					cellDetails[i][j-step].f = fNew;
					cellDetails[i][j-step].g = gNew;
					cellDetails[i][j-step].h = hNew;
					cellDetails[i][j-step].parent_i = i;
					cellDetails[i][j-step].parent_j = j;
				}
			}
			else if (!isUnBlocked(grid, i, j - step)) {
                break;
			}
		}
	}

	if (foundDest == false)
		printf("Failed to find the Destination Cell\n");

	return;
}

struct TemplateGrid
{
    int r, c;
    vector<vector<int>> grid;
    Pair src, dest;
};

int main()
{
    cout << "Use template grid OR create new grid? (T / C): ";
    char c;
    cin >> c;
    if (c == 'T') {
        int gridCnt = 4;
        TemplateGrid t[gridCnt];
        t[0].r = 5;
        t[0].c = 5;
        t[0].grid = {
            { 1, 1, 1, 0, 1 },
            { 1, 1, 1, 0, 0 },
            { 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1 }
        };
        t[1].r = 9;
        t[1].c = 10;
        t[1].grid = {
            { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
            { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
            { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
            { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
            { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
            { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
            { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 }
        };
        t[2].r = 30;
        t[2].c = 30;
        t[2].grid = {
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 }
        };
	t[3].r = 8;
        t[3].c = 8;
        t[3].grid = {
            { 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, 0, 0, 0, 0, 1, 1, 1 },
            { 1, 0, 1, 1, 0, 1, 0, 1 },
            { 1, 0, 1, 1, 0, 1, 0, 1 },
            { 1, 0, 1, 1, 0, 1, 0, 1 },
            { 1, 0, 1, 1, 1, 1, 0, 1 },
            { 1, 0, 0, 0, 0, 0, 0, 1 },
            { 1, 1, 1, 1, 1, 1, 1, 1 }
        };
        for (int i = 0; i < gridCnt; i++) {
            cout << endl;
            cout << "Template number: " << i + 1 << endl;
            cout << "Number of rows: " << t[i].r << endl;
            cout << "Number of columns: " << t[i].c << endl;
            cout << "Grid (1 means unblocked, 0 means blocked):" << endl;
            for (int j = 0; j < t[i].r; j++) {
                for (int k = 0; k < t[i].c; k++) {
                    cout << t[i].grid[j][k] << ' ';
                }
                cout << endl;
            }
        }
        int option;
        cout << "\nSelect Template: ";
        cin >> option;
        if (option < 1 || option > gridCnt) {
            cout << "Template Does Not Exist!" << endl;
        }
        else {
            ROW = t[option-1].r;
            COL = t[option-1].c;
            vector<vector<int>> grid = t[option-1].grid;
            cout << "Enter starting cell: ";
            Pair src, dest;
            int x, y;
            cin >> x >> y;
            src = {x, y};
            cout << "Enter destination cell: ";
            cin >> x >> y;
            dest = {x, y};
            cout << "Enter heuristic option from {Manhattan / Diagonal / Euclidean} (1 / 2 / 3): ";
            cin >> heuristicOption;
            cout << "Enter simulation mode from {Step-By-Step / Auto} (1 / 2): ";
            cin >> simulationMode;
            aStarSearch(grid, src, dest);
        }
    }
    else {
        cout << "Number of rows: ";
        cin >> ROW;
        cout << "Number of columns: ";
        cin >> COL;
        cout << "Enter the grid (1 means unblocked, 0 means blocked):" << endl;
        vector<vector<int>> grid;
        grid.resize(ROW);
        for (int i = 0; i < ROW; i++) {
            grid[i].resize(COL);
            for (int j = 0; j < COL; j++) {
                cin >> grid[i][j];
            }
        }
        cout << "Enter starting cell: ";
        Pair src, dest;
        int x, y;
        cin >> x >> y;
        src = {x, y};
        cout << "Enter destination cell: ";
        cin >> x >> y;
        dest = {x, y};
        cout << "Enter heuristic option from {Manhattan / Diagonal / Euclidean} (1 / 2 / 3): ";
        cin >> heuristicOption;
        cout << "Enter simulation mode from {Step-By-Step / Auto} (1 / 2): ";
        cin >> simulationMode;
        aStarSearch(grid, src, dest);
    }


	return(0);
}

