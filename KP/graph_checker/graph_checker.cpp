#include "graph_checker.h"

using namespace std;
using graph = vector<vector<int>>;

void Graph_checker::dfs_connectivity_component(int u, const graph &g, vector<int> &vis) {
    vis[u] = 1;

    for (auto & v : g[u]) {
        if (!vis[v]) {
            dfs_connectivity_component(v, g, vis);
        }
    }
}

int Graph_checker::connectivity_component(Parce_data & parce_data) {
    vector<int> vis(parce_data.g.size());
    int res = 0;
    for (int i = 0; i < vis.size(); ++i) {
        if (!vis[i]) {
            dfs_connectivity_component(i, parce_data.g_undirected, vis);
            res++;
        }
    }
    return res;
}

vector<int> get_all_edges(Parce_data & parce_data) {
    vector<int> edges(parce_data.json_data.size());
    int index = 0;
    for (auto & elem : parce_data.json_data) {
        edges[index++] = elem.id;
    }
    return edges;
}

size_t cnt_nodes(Parce_data & pd) {
    unordered_set<int> nodes;
    for (auto & elem : pd.json_data) {
        nodes.insert(elem.id);
    }
    return nodes.size();
}

bool Graph_checker::check_cycle_dfs(const graph &g, int u, vector<int> &color, unordered_set<int> & st) {
    color[u] = GREY;
    st.insert(u);

    for (int v : g[u]) {
        if (color[v] == WHITE) {
            if (check_cycle_dfs(g, v, color, st)) {
                return true;
            }
        } else if (st.count(v) > 0) {
            // Найден цикл
            return true;
        }
    }

    color[u] = BLACK;
    st.erase(u);
    return false;
}

vector<int> Graph_checker::get_start_jobs(Parce_data &parce_data) {
    vector<int> result;
    for (auto & job : parce_data.json_data) {
        if (job.prev_id == -1) {
            result.push_back(job.id);
        }
    }
    return result;
}

bool Graph_checker::check_cycle(Parce_data & parce_data) {
    vector<int> color(parce_data.g.size());
    unordered_set<int> st;
    vector<int> start_edges = get_start_jobs(parce_data);

    for (int i = 0; i < color.size(); ++i) {
        if (!color[i]) {
            if (check_cycle_dfs(parce_data.g, i, color, st)) {
                return true;
            }
        }
    }
    return false;
}

void Graph_checker::check(Parce_data & parce_data) {
    // проверяю на количество компонент связанности
    if (connectivity_component(parce_data) > 1) {
        throw std::runtime_error("graph have more than one connectivity component");
    }
    if (check_cycle(parce_data)) {
        throw std::runtime_error("graph has a cycle");
    }
}



