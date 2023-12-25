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
    string semaphore_name;
    int num_parents = 0;
    int num_completed_parents = 0;
    int num_semaphores;
    int num_completed_semaphores;
    bool semaphore;
    unordered_map<string, int> sem_name_to_degree;

};

class Parce_data {
public:
    graph g;
    graph g_undirected;
    std::vector<Json_data> json_data;

};

class Json_parser {
public:
    static Parce_data parce(const std::string &file_path);

};


