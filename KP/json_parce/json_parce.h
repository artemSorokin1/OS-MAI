#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <semaphore.h>
#include <unordered_set>

using namespace std;
using json = nlohmann::json;
using graph = vector<vector<int>>;

class Json_data {
public:
    int id;
    int prev_id;
    string cmd;
    string output;
    string sem_name;
    int num_parents = 0;
    int num_completed_parents = 0;
    bool semaphore;

};

class Parce_data {
public:
    graph g;
    graph g_undirected;
    std::vector<Json_data> json_data;
    unordered_map<string, int> sem_name_to_degree;

};

class Json_parser {
public:
    static Parce_data parce(const std::string &file_path);

};


