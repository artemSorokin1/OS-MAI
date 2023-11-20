#include <iostream>
#include "../../../Desktop/Programming/labs/lab5/lib/vector.h"


//using namespace lab5;
using graph = lab5::vector<lab5::vector<int>>;


enum {
    WHITE = 0,
    GREY = 1,
    BLACK = 2
};


bool dfs(const graph & g, int u, lab5::vector<int> & color) {
    color[u] = GREY;
    if (g[u].size() > 1) {
        return false;
    }
    if (color[g[u][0]] == WHITE)
        return dfs(g, g[u][0], color);
    if (color[g[u][0]] == GREY)
        return false;
    color[u] = BLACK;
}


