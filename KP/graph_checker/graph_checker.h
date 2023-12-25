#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include "../json_parce/json_parce.h"

using namespace std;
using graph = vector<vector<int>>;

enum {
    WHITE = 0, GREY = 1, BLACK = 2
};

class Graph_checker {
public:
    static void dfs_connectivity_component(int u, const graph & g, vector<int> & vis);
    static bool check_cycle(Parce_data & parce_data);
    static int connectivity_component(Parce_data & parce_data);
    static bool check_cycle_dfs(const graph & g, int u, vector<int> & vis, unordered_set<int> & st);
public:
    static vector<int> get_start_jobs(Parce_data & parce_data);
    static void check(Parce_data & parce_data);
};

size_t cnt_nodes(Parce_data & pd);
vector<int> get_all_edges(Parce_data & parce_data);
void count_parents(Parce_data & parce_data);
size_t count_children(int u, Parce_data & pd);
vector<int> get_children(int u, Parce_data & parce_data);


