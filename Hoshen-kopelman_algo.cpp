#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// ---------- Union–Find ----------
int findSet(vector<int> &parent, int x) {
    int y = x;

    // Find root
    while (parent[y] != y)
        y = parent[y];

    // Path compression
    while (parent[x] != x) {
        int z = parent[x];
        parent[x] = y;
        x = z;
    }
    return y;
}

void unionSet(vector<int> &parent, int x, int y) {
    parent[ findSet(parent, x) ] = findSet(parent, y);
}

// ---------- Connected Component Labeling with Continuous Labels ----------
vector<vector<int>> connectedComponentsContinuous(const vector<vector<int>>& occupied) {
    int n_columns = occupied.size();
    int n_rows = occupied[0].size();

    vector<vector<int>> label(n_columns, vector<int>(n_rows, 0));

    vector<int> parent(n_columns * n_rows + 1);
    for (int i = 0; i < parent.size(); i++) parent[i] = i;

    int largest_label = 0;

    // First pass
    for (int x = 0; x < n_columns; x++) {
        for (int y = 0; y < n_rows; y++) {

            if (!occupied[x][y]) continue;

            int left = (x > 0) ? label[x - 1][y] : 0;
            int above = (y > 0) ? label[x][y - 1] : 0;

            if (left == 0 && above == 0) {
                largest_label++;
                label[x][y] = largest_label;
            }
            else if (left != 0 && above == 0) {
                label[x][y] = findSet(parent, left);
            }
            else if (left == 0 && above != 0) {
                label[x][y] = findSet(parent, above);
            }
            else {
                unionSet(parent, left, above);
                label[x][y] = findSet(parent, left);
            }
        }
    }

    // Second pass (assign root labels)
    for (int x = 0; x < n_columns; x++)
        for (int y = 0; y < n_rows; y++)
            if (label[x][y] != 0)
                label[x][y] = findSet(parent, label[x][y]);


    // ---------- Third pass: Make labels continuous ----------
    unordered_map<int, int> remap;
    int new_label = 1;

    for (int x = 0; x < n_columns; x++) {
        for (int y = 0; y < n_rows; y++) {
            int old = label[x][y];
            if (old != 0) {
                if (remap.find(old) == remap.end()) {
                    remap[old] = new_label++;
                }
                label[x][y] = remap[old];
            }
        }
    }

    return label;
}

// ---------- Example ----------
int main() {
    vector<vector<int>> occupied = {
        {0,1,1,0,0},
        {1,1,0,1,1},
        {0,1,0,1,0},
        {0,0,0,1,1}
    };

    auto labels = connectedComponentsContinuous(occupied);

    for (auto &row : labels) {
        for (int v : row) cout << v << " ";
        cout << endl;
    }
}
/* Output
0 1 1 0 0 
1 1 0 2 2 
0 1 0 2 0 
0 0 0 2 2 
*/
