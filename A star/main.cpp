#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <map>
#include <algorithm>

using namespace std;

int h1(const vector<int>& board);
int h2(const vector<int>& board);
int h3(const vector<int>& board);

void print_board(const vector<int>& board);
bool is_goal(const vector<int>& board); // checks if goal state reached
void goal(); // prints goal state
bool valid(const vector<int>& board); // check parity
void print_solution_path(const vector<vector<int>>& path);

vector<vector<int>> valid_neighbors(const vector<int>& board);
string board_to_string(const vector<int>& board);
void a_star_search(const vector<int>& start, int (*heuristic)(const vector<int>&));

int main() {
    // vector<int> start = {4, 5, 0, 6, 1, 8, 7, 3, 2}; 
    // this is the original initial state, but has odd number of inversions
    // goal state (as shown on the assginment, has even number of inversions).
    vector<int> inital_state_one = {1, 2, 3, 4, 5, 6, 0, 7, 8}; // even 
    vector<int> inital_state_two = {1, 2, 3, 4, 0, 5, 6, 7, 8}; // even 
    vector<int> inital_state_three = {1, 2, 3, 0, 4, 5, 6, 7, 8}; // even 
    vector<int> inital_state_four = {3, 1, 2, 6, 4, 5, 0, 7, 8}; // even
    vector<int> inital_state_five = {8, 6, 7, 2, 5, 4, 3, 0, 1}; // even

    cout << "Initial state is: ";
    if(valid(inital_state_five)){
        cout << "Valid!" << endl << endl;
        cout << "Initial Board:" << endl << endl;
        print_board(inital_state_five);
        cout << endl << endl << "Goal Board (state)" << endl << endl;
        goal();

        cout << "\n\nSearch with heuristic: h3\n\n";
        a_star_search(inital_state_five, h3);
    }
    else{
        cout << "not valid, entered an initial state with an appropriate number ";
        cout << "of inversions" << endl;
    }


    return 0;
}

bool valid(const vector<int>& board) {
    int inversions{int()};

    for (int i = 0; i < 9; ++i) {
        for (int j = i + 1; j < 9; ++j) {
            if (board[i] == 0 || board[j] == 0) 
                continue; 
            if (board[i] > board[j])
                inversions++;
        }
    }

    return inversions % 2 == 0; // true if even, false if odd
}

// misplaced tiles
int h1(const vector<int>& board) {
    int count{int()};
    for (int i = 0; i < 9; ++i) {
        if (board[i] != 0 && board[i] != i + 1)
            ++count;
    }
    return count;
}

// l1 norm (manhattan distance)
int h2(const vector<int>& board) {
    int distance{int()};
    int goal_index{int()};
    int curr_row{int()};
    int curr_col{int()};
    int goal_row{int()};
    int goal_col{int()};

    for (int i = 0; i < 9; ++i) {
        int value = board[i];
        if (value != 0) { 
            goal_index = value - 1;
            curr_row = i / 3;
            curr_col = i % 3;
            goal_row = goal_index / 3;
            goal_col = goal_index % 3;
            distance += abs(curr_row - goal_row) + abs(curr_col - goal_col);
        }
    }
    return distance;
}

// h1 and h2 are admissable, the max of two admissible heuristics is also admissible
int h3(const vector<int>& board) {
    return max(h1(board), h2(board));
}

void print_board(const vector<int>& board) {
    for (int i = 0; i < 9; ++i) {
        board[i] == 0 ? cout << "b " : cout << board[i] << " ";
        if ((i + 1) % 3 == 0)
            cout << endl;
    }
    return;
}

void goal() { // just prints the goal state 
    const vector<int> goal = {1,2,3,4,5,6,7,8,0}; // even inversions
    for (int i = 0; i < 9; ++i) {
        goal[i] == 0 ? cout << "b " : cout << goal[i] << " ";
        if ((i + 1) % 3 == 0)
            cout << endl;
    }
    return;
}

bool is_goal(const vector<int>& board) {
    const vector<int> goal = {1,2,3,4,5,6,7,8,0}; // goal state
    return board == goal;
}

string board_to_string(const vector<int>& board) {
    string s{string()};
    for (const int& n : board) 
        s += to_string(n);
    return s;
}

void print_solution_path(const vector<vector<int>>& path){
    for (const auto& step : path){
        print_board(step);
        cout << endl << endl;
    }
    return;
}

// generate valid neighbor states
vector<vector<int>> valid_neighbors(const vector<int>& board) {
    vector<vector<int>> valid_neighbors;
    int blank = -1;
    for (int i = 0; i < 9; ++i)
        if (board[i] == 0) {
            blank = i;
            break;
        }

    int row = blank / 3;
    int col = blank % 3;

    vector<pair<int, int>> valid_moves = {
        {-1, 0}, // up
        {1, 0},  // down
        {0, -1}, // left
        {0, 1}   // right
    };

    // pair<int, int>
    for (auto [first, second] : valid_moves) {
        int new_row = row + first;
        int new_col = col + second;

        if (new_row >= 0 && new_row < 3 && new_col >= 0 && new_col < 3) {
            int new_blank = new_row * 3 + new_col;
            vector<int> new_board = board;
            swap(new_board[blank], new_board[new_blank]);
            valid_neighbors.push_back(new_board);
        }
    }

    return valid_neighbors;
}

// best-first search using given heuristic 
// (*heuristic) allows best_first_search to work with both h1 and h2
// since they're both returning an int and have the same paramater
void a_star_search(const vector<int>& start, int (*heuristic)(const vector<int>&)) {
    struct Node {
        vector<int> board;
        int g; // cost so far
        int h; // heuristic
        // f(n) = g(n) + h(n)
        string parent; // (for reconstructing path)

        bool operator > (const Node& other) const {
            return (g + h) > (other.g + other.h);
        }
    };

    // Min-heap priority queue (lowest f(n) = g + h at top)
    priority_queue<Node, vector<Node>, greater<Node>> frontier;
    set<string> visited;
    map<string, string> came_from; // maps child -> parent

    string start_key = board_to_string(start);
    frontier.push({start, 0, heuristic(start), ""});
    visited.insert(start_key);

    while (!frontier.empty()) {
        Node current = frontier.top();
        frontier.pop();

        if (is_goal(current.board)) {
            // Reconstruct and print path
            vector<vector<int>> path;
            string key = board_to_string(current.board);
            while (came_from.count(key)) {
                vector<int> temp;
                for (const char& c : key) 
                    temp.push_back(c - '0');
                path.push_back(temp);
                key = came_from[key];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());

            cout << "Solution found! Moves: " << path.size() - 1 << "\n\n";
            print_solution_path(path);
            return;
        }

        for (const auto& neighbor : valid_neighbors(current.board)) {
            string neighbor_key = board_to_string(neighbor);
            if (!visited.count(neighbor_key)) {
                visited.insert(neighbor_key);
                came_from[neighbor_key] = board_to_string(current.board);
                int g_cost = current.g + 1;
                int h_cost = heuristic(neighbor);
                frontier.push({neighbor, g_cost, h_cost, board_to_string(current.board)});
            }
        }
    }

    cout << "No solution found.\n";
    return;
}