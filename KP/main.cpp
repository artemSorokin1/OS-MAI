#include "graph_checker/graph_checker.h"
#include <unistd.h>
#include <csignal>
#include <pthread.h>
#include <chrono>
#include <thread>

pthread_mutex_t mutex_;

typedef struct {
    int temp_id;
    Parce_data* parce_data;
} thread_data;

void* dfs_scheduler(void* pthread_data) {
    auto data = (thread_data*)pthread_data;
    pthread_mutex_lock(&mutex_);
    bool return_flag = false;
    if (data->parce_data->json_data[data->temp_id - 1].num_completed_parents + 1 < data->parce_data->json_data[data->temp_id - 1].num_parents) {
        data->parce_data->json_data[data->temp_id - 1].num_completed_parents++;
        return_flag = true;
    }
    pthread_mutex_unlock(&mutex_);

    if (data->parce_data->json_data[data->temp_id - 1].semaphore) {
        while (true) {
            pthread_mutex_lock(&mutex_);
            if (data->parce_data->sem_name_to_degree[data->parce_data->json_data[data->temp_id - 1].sem_name]) {
                data->parce_data->sem_name_to_degree[data->parce_data->json_data[data->temp_id - 1].sem_name]--;
                pthread_mutex_unlock(&mutex_);
                break;
            }
            pthread_mutex_unlock(&mutex_);
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }

    if (return_flag) {
        return nullptr;
    }

    int fd[2];
    if (pipe(fd) == -1) {
        throw std::runtime_error("some problem with pipe");
    }

    pid_t pid = fork();

    if (pid == -1) {
        throw std::runtime_error("fork problem");
    } else if (pid == 0) {
        int id = data->temp_id;
        const std::string path = data->parce_data->json_data[data->temp_id - 1].cmd;
        if (execl(path.c_str(), path.c_str(), to_string(id).c_str(), nullptr)) {
            close(fd[0]);
            pid_t child_pid = getpid();
            write(fd[1], &child_pid, sizeof(pid_t));
            cerr << "Exec problem" << endl;
            exit(1);
        }
    } else {
        int status;
        cout << "Job[" << data->temp_id << "] is started\n";
        wait(&status);

        if (WIFSIGNALED(status)) {
            // дочерний процесс закончился с ошибкой
            close(fd[1]);
            pid_t child_pid;
            read(fd[0], &child_pid, sizeof(pid_t));
            kill(child_pid, SIGTERM);
            cerr << "Job ended with error\n";
            exit(1);
        }
        if (WIFEXITED(status)) {
            // дочерний процесс закончился без ошибки
            pthread_mutex_lock(&mutex_);
            data->parce_data->sem_name_to_degree[data->parce_data->json_data[data->temp_id - 1].sem_name]++;
            pthread_mutex_unlock(&mutex_);

            vector<int> children = get_children(data->temp_id, *data->parce_data);
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
                pthread_create(&threads[i], nullptr, dfs_scheduler, &data_for_childes_thread[i]);
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
    pd = Json_parser::parce("../examples/semaphore.json");

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
        pthread_create(&threads[i], nullptr, dfs_scheduler, &data[i]);
    }

    for (int i = 0; i < cnt_start_jobs; i++) {
        pthread_join(threads[i], nullptr);
    }

    cout << "End" << endl;

    return 0;
}
