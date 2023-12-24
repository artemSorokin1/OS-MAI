#include "json_parce.h"

// фун-ия чтобы распарсить json в графф и не только
Parce_data Json_parser::parce(const std::string &file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("problem with file open");
    }
    json json_input_data;
    file >> json_input_data;
    file.close();
    Parce_data parce_data;
    json array_of_jobs = json_input_data["jobs"];
    parce_data.json_data.resize(array_of_jobs.size());
    int i = 0;

    // считываем данные из json в массив
    for (const auto & job : array_of_jobs) {
        parce_data.json_data[i].id = job["id"];
        parce_data.json_data[i].prev_id = job["prev_id"];
        parce_data.json_data[i].cmd = job["cmd"];
        parce_data.json_data[i].output = job["output"];
        if (job.count("sem")) {
            parce_data.json_data[i].semaphore = true;
            parce_data.json_data[i].semaphore_name = job["sem"];
        }
        i++;
    }
    // количество уникальных элементов
    unordered_set<int> st;
    for (const auto & job : parce_data.json_data) {
        st.insert(job.id);
        if (job.prev_id != -1) {
            st.insert(job.prev_id);
        }
    }
    // возможно это лучше
    int n = 0;
    for (auto & job : parce_data.json_data) {
        if (n < job.id) {
            n = job.id;
        }
        if (n < job.prev_id) {
            n = job.prev_id;
        }
    }
    // строим графф
    graph g(st.size());
    graph g_undirected(st.size());
    for (const auto & job : parce_data.json_data) {
        int cur_id = job.id;
        int prev_id = job.prev_id;
        if (prev_id == -1) {
            continue;
        }
        --cur_id; --prev_id;
        g[prev_id].push_back(cur_id);
        g_undirected[prev_id].push_back(cur_id);
        g_undirected[cur_id].push_back(prev_id);
    }

    parce_data.g = g;
    parce_data.g_undirected = g_undirected;

    return parce_data;
}

