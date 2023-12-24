#include "graph_checker/graph_checker.h"

int main() {
    Parce_data pd;
    pd = Json_parser::parce("../data.json");
    const graph & g = pd.g;
    for (int i = 0; i < g.size(); ++i) {
        std::cout << i  << ": ";
        for (auto & elem : g[i]) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }

//    vector<int> start = Graph_checker::get_start_jobs(pd);
//    for (auto & elem : start) {
//        cout << elem << '\n';
//    }
//    std::cout << Graph_checker::get_start_jobs(pd)[0] << endl;
    Graph_checker::check(pd);

    return 0;
}