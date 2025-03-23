#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;

const int N = 3;

struct State {
    vector<vector<int>> board;
    int x, y; // Position of the empty tile (0)
    int cost; // Heuristic cost (Manhattan Distance)
    int level; // Depth in the search tree
    State* parent; // Pointer to parent state for backtracking

    bool operator<(const State& other) const {
        return (cost + level) > (other.cost + other.level); // Lower cost + level has higher priority
    }
};

// Calculate Manhattan Distance heuristic
int calculateCost(const vector<vector<int>>& board, const vector<vector<int>>& goal) {
    int cost = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] != 0) { // Ignore the empty tile
                // Find the position of the tile in the goal state
                for (int k = 0; k < N; k++) {
                    for (int l = 0; l < N; l++) {
                        if (board[i][j] == goal[k][l]) {
                            cost += abs(i - k) + abs(j - l); // Manhattan Distance
                            break;
                        }
                    }
                }
            }
        }
    }
    return cost;
}

bool isSafe(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N);
}

void printBoard(const vector<vector<int>>& board, int heuristic) {
    cout << "Heuristic: " << heuristic << "\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "-----\n";
}

void printSolutionPath(State* goalState) {
    vector<State*> path;
    State* current = goalState;

    // Backtrack from goal to initial state
    while (current) {
        path.push_back(current);
        current = current->parent;
    }

    // Print the solution path in reverse order
    reverse(path.begin(), path.end());

    // Explanation message
    cout << "\nWhy this path was chosen:\n";
    cout << "The algorithm selects the path with the lowest total cost (heuristic + level) at each step.\n";
    cout << "This ensures that the goal state is reached efficiently.\n\n";

    cout << "Solution Path (Initial to Goal):\n";
    int step = 0;
    for (State* state : path) {
        cout << "Step " << step++ << " (Heuristic: " << state->cost << ", Level: " << state->level << "):\n";
        printBoard(state->board, state->cost);
    }
    cout << "Solution completed in " << path.size() - 1 << " steps.\n";
}

bool solvePuzzle(const vector<vector<int>>& initial, const vector<vector<int>>& goal, int x, int y) {
    priority_queue<State> pq;
    set<vector<vector<int>>> visited;

    // Root node (initial state)
    State* root = new State{ initial, x, y, calculateCost(initial, goal), 0, nullptr };
    pq.push(*root);
    visited.insert(initial);

    int row[] = { 1, 0, -1, 0 }; // Directions: Down, Left, Up, Right
    int col[] = { 0, -1, 0, 1 };

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        // Print the current state and its heuristic value
        cout << "Exploring state at level " << current.level << ":\n";
        printBoard(current.board, current.cost);

        // Check if the current state is the goal state
        if (current.cost == 0) {
            cout << "\nGoal State Reached at Level " << current.level << ":\n";
            printBoard(current.board, current.cost);
            printSolutionPath(&current);
            return true;
        }

        // Generate child states
        for (int i = 0; i < 4; i++) {
            int newX = current.x + row[i];
            int newY = current.y + col[i];

            if (isSafe(newX, newY)) {
                vector<vector<int>> newBoard = current.board;
                swap(newBoard[current.x][current.y], newBoard[newX][newY]);

                // Check if the new state has already been visited
                if (visited.find(newBoard) == visited.end()) {
                    int newCost = calculateCost(newBoard, goal);
                    State* child = new State{ newBoard, newX, newY, newCost, current.level + 1, new State(current) };
                    pq.push(*child);
                    visited.insert(newBoard);
                }
            }
        }
    }

    cout << "No solution exists." << endl;
    return false;
}

int main() {
    // Initial state
    vector<vector<int>> initial = { {1, 2, 3}, {8, 5, 6}, {4, 7, 0} };

    // Goal state
    vector<vector<int>> goal = { {1, 2, 3}, {4, 5, 6}, {7, 8, 0} };

    // Initial position of the empty tile (0)
    int x = 2, y = 2;

    cout << "Initial State:\n";
    printBoard(initial, calculateCost(initial, goal));
    cout << "\nGoal State:\n";
    printBoard(goal, 0);
    cout << "\nSolving the puzzle...\n";

    solvePuzzle(initial, goal, x, y);

    return 0;
}