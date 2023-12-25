#include "graph_checker/graph_checker.h"
#include <unistd.h>
#include <csignal>
#include <pthread.h>

pthread_mutex_t mutex_;

typedef struct {
    int temp_id;
    Parce_data* parce_data;
} thread_data;

void* dfs_start_job(void* pthread_data) {
    auto data = (thread_data*)pthread_data;
    pthread_mutex_lock(&mutex_);
    bool return_flag = false;
    if (data->parce_data->json_data[data->temp_id - 1].num_completed_parents + 1 < data->parce_data->json_data[data->temp_id - 1].num_parents) {
        data->parce_data->json_data[data->temp_id - 1].num_completed_parents++;
        return_flag = true;
    }
    pthread_mutex_unlock(&mutex_);

//    if (data->parce_data.json_data[data->temp_id - 1].semaphore) {
//        // if (data->parce_data.semophore_compare[data->parce_data.semophore_name] == 0) {
//        // return flag = true;
//        // }
//    }

    if (return_flag) {
        return nullptr;
    }

    pid_t pid = fork();

    if (pid == -1) {
        throw std::runtime_error("fork problem");
    } else if (pid == 0) {
        int id = data->temp_id;
        const std::string path = data->parce_data->json_data[data->temp_id - 1].cmd;
        if (execl(path.c_str(), path.c_str(), to_string(id).c_str(), nullptr)) {
            cerr << "Exec problem" << endl;
            exit(1);
        }
    } else {
        int status;
        cout << "Job[" << data->temp_id << "] is started\n";
        wait(&status);

        if (WIFSIGNALED(status)) {
            // дочерний процесс закончился с ошибкой
            cerr << "Job ended with error\n";
            // передать ид дочернего процесса и убить его kill(pid, SIGTERM);
            exit(1);
        }
        if (WIFEXITED(status)) {
            // дочерний процесс закончился без ошибки
            // увеличить кол-во семофоров с таким именем, запустить всех детей в новых потоках
            // создаю потоки на всех детей и запускаюсь для детей
            vector<int> children = get_children(data->temp_id, *data->parce_data);
            // увеличеваем количество завершенных родителей у всех детей
//            pthread_mutex_lock(&mutex_);
//            for (const int & child : children) {
//                for (auto & job : data->parce_data->json_data) {
//                    if (job.id - 1 == child) {
//                        ++job.num_completed_parents;
//                    }
//                }
//            }
//            pthread_mutex_unlock(&mutex_);
            size_t cnt_children = count_children(data->temp_id, *data->parce_data);
            int cnt_threads = (int)cnt_children;
            void *memory_for_threads = ::operator new(cnt_children * sizeof(pthread_t));
            if (!memory_for_threads) {
                throw std::runtime_error("Not another memory for threads");
            }
            auto threads = static_cast<pthread_t*>(memory_for_threads);
            thread_data data_for_childes_thread[cnt_threads];
            pthread_mutex_lock(&mutex_);
            for (int i = 0; i < cnt_threads; ++i) {
                data_for_childes_thread[i].temp_id = children[i] + 1;
                data_for_childes_thread[i].parce_data = data->parce_data;
            }
            pthread_mutex_unlock(&mutex_);
            for (int i = 0; i < cnt_threads; ++i) {
                pthread_create(&threads[i], nullptr, dfs_start_job, &data_for_childes_thread[i]);
            }
            for (int i = 0; i < cnt_threads; ++i) {
                pthread_join(threads[i], nullptr);
            }
        }

    }
    return nullptr;
}

int main() {
    Parce_data pd;
    pd = Json_parser::parce("../data2.json");

    Graph_checker::check(pd);
    count_parents(pd);
    vector<Json_data> new_jd;
    for (auto & elem : pd.json_data) {
        bool is_here = false;
        for (auto & el : new_jd) {
            if (elem.id == el.id) {
                is_here = true;
            }
        }
        if (!is_here) {
            new_jd.push_back(elem);
        }
    }
    pd.json_data = new_jd;
    vector<int> start_jobs = Graph_checker::get_start_jobs(pd);
    int cnt_start_jobs = (int)start_jobs.size();

    thread_data data[cnt_start_jobs];
    pthread_t threads[cnt_start_jobs];
    for (int i = 0; i < cnt_start_jobs; ++i) {
        data[i].temp_id = start_jobs[i];
        data[i].parce_data = &pd;
    }
    pthread_mutex_init(&mutex_, nullptr);
    for (int i = 0; i < cnt_start_jobs; i++) {
        pthread_create(&threads[i], nullptr, dfs_start_job, &data[i]);
    }

    for (int i = 0; i < cnt_start_jobs; i++) {
        pthread_join(threads[i], nullptr);
    }

//    for (auto & job : pd.json_data) {
//        cout << job.id << ' ' << job.num_completed_parents << endl;
//    }

    cout << "End" << endl;

    return 0;
}