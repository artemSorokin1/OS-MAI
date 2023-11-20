#include "../lib/timer.cpp"
#include <algorithm>
#include <vector>
#include <fstream>


using namespace std;


vector<int> result;
vector<int> clear;
int32_t part = 0;
pthread_mutex_t mutex_;


typedef struct Info {
    vector<vector<int>> vv;
    size_t thread_cnt;

    ~Info() = default;
    Info() = default;
    Info(vector<vector<int>> vv, size_t thread_cnt) : vv(std::move(vv)), thread_cnt(thread_cnt) {}

} Info_about_input_data;


void* summ_of_k_arrays(void* args) {
    auto info = (Info_about_input_data*)args;
    size_t n = info->vv[0].size();
    size_t k = info->vv.size();
    size_t thread_cnt = info->thread_cnt;
    vector<vector<int>> & arrays = info->vv;
    pthread_mutex_lock(&mutex_);
    size_t thread_id = part++;
    pthread_mutex_unlock(&mutex_);
    if (k < n) {
        for (size_t j = thread_id; j < n; j += thread_cnt) {
            for (size_t i = 0; i < k; ++i) {
                result[j] += arrays[i][j];
            }
        }
    } else {
        for (size_t j = thread_id; j < k; j += thread_cnt) {
            for (size_t i = 0; i < n; ++i) {
                result[i] += arrays[j][i];
            }
        }
    }
    pthread_exit(nullptr);
}


void compute_with_input_thread(Info_about_input_data* & info) {
    result = clear;
    Timer timer;
    pthread_mutex_init(&mutex_, nullptr);
    pthread_t threads[info->thread_cnt];
    for (int i = 0 ; i < info->thread_cnt; ++i) {
        pthread_create(&threads[i], nullptr, summ_of_k_arrays, info);
    }
    for (int i = 0; i < info->thread_cnt; ++i) {
        pthread_join(threads[i], nullptr);
    }
    part = 0;
}


void compute(int trc, Info_about_input_data * & info) {
    const string path = "../lib/output_data";
    fstream output(path, fstream::out | fstream::app);
    result = clear;
    {
        Timer timer;
        info->thread_cnt = trc;
        pthread_t threads[trc];
        for (int i = 0; i < trc; ++i) {
            pthread_create(&threads[i], nullptr, summ_of_k_arrays, info);
        }

        for (int i = 0; i < trc; ++i) {
            pthread_join(threads[i], nullptr);
        }
        output << trc << ' ' << timer.get_duration() << endl;
    }
    part = 0;
}


int main(int argc, char* argv[]) {
    int thread_quantity;
    if (argv[1] != nullptr) {
        string s = argv[1];
        thread_quantity = stoi(s);
    } else {
        cout << "Введите количество потоков: ";
        cin >> thread_quantity;
    }
    int k;
    cout << "Введите количесво массивов: ";
    cin >> k;
    int n;
    cout << "Введите длинну массивов: ";
    cin >> n;

    result.resize(n);
    clear = result;

    vector<vector<int>> arrays(k, vector<int>(n));

    srand(time(nullptr));
    arrays.assign(k, vector<int>(n));
    for (auto & elem : arrays) {
        for (auto & el : elem) {
            el = rand() % 10;
        }
    }

    auto info = (Info_about_input_data*)
                        ::new Info(arrays, thread_quantity);

//    compute_with_input_thread(info);
    compute(1, info);
    compute(2, info);
    compute(3, info);
    compute(4, info);
    compute(5, info);
    compute(6, info);
    compute(7, info);
    compute(8, info);
    compute(9, info);
    compute(10, info);
    compute(200, info);


    for (auto & elem : arrays) {
        for (auto & el : elem) {
            cout << el << ' ';
        }
        cout << endl;
    }


    for (auto & elem : result) {
        cout << elem << ' ';
    }
    cout << endl;

    delete info;

    return 0;
}

