#include "../lib/timer.cpp"
#include <algorithm>
#include <vector>
#include <fstream>



using namespace std;


vector<int> result;
int32_t part = 0;


typedef struct Info {
    vector<vector<int>> vv;
    size_t thread_cnt;

    ~Info() = default;
    Info() = default;
    Info(vector<vector<int>> vv, size_t thread_cnt) : vv(vv), thread_cnt(thread_cnt) {}

} Info_about_input_data;


void* summ_of_k_arrays(void* args) {
    auto info = (Info_about_input_data*)args;
    size_t n = info->vv.size();
    size_t k = info->vv.size();
    size_t thread_cnt = info->thread_cnt;
    vector<vector<int>> & arrays = info->vv;
    size_t thread_id = part++;
    for (size_t j = thread_id; j < n; j += thread_cnt) {
        for (size_t i = 0; i < k; ++i) {
            result[j] += arrays[i][j];
        }
    }

    pthread_exit(nullptr);

}


vector<int> nul;


void compute_with_input_thread(int k, int n, Info_about_input_data* & info) {
    result = nul;
    Timer timer;
    pthread_t threads[info->thread_cnt];
    for (int i = 0 ; i < info->thread_cnt; ++i) {
        pthread_create(&threads[i], nullptr, summ_of_k_arrays, info);
    }

    for (int i = 0; i < info->thread_cnt; ++i) {
        pthread_join(threads[i], nullptr);
    }
    part = 0;
}


void compute(int k, int n, int trc, Info_about_input_data * & info) {
    const string path = "../lib/output_data";
    fstream output(path, fstream::out | fstream::app);
    result = nul;
    Timer timer;
    info->thread_cnt = trc;
    pthread_t threads[trc];
    for (int i = 0 ; i < trc; ++i) {
        pthread_create(&threads[i], nullptr, summ_of_k_arrays, info);
    }

    for (int i = 0; i < trc; ++i) {
        pthread_join(threads[i], nullptr);
    }
    part = 0;
    output << trc << ' ' << timer.get_duration() << endl;
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



    nul = result;

    srand(time(nullptr));

    vector<vector<int>> arrays(k, vector<int>(n));

    arrays.assign(k, vector<int>(n));
    for (auto & elem : arrays) {
        for (auto & el : elem) {
            el = rand() % 10;
        }
    }

    auto info = (Info_about_input_data*) 
                        ::new Info_about_input_data(arrays, thread_quantity);



//    compute_with_input_thread(k, n, info);
    compute(k, n, 1, info);
    compute(k, n, 2, info);
    compute(k, n, 3, info);
    compute(k, n, 4, info);
    compute(k, n, 5, info);
    compute(k, n, 6, info);
    compute(k, n, 7, info);
    compute(k, n, 8, info);
    compute(k, n, 9, info);
    compute(k, n, 10, info);
    compute(k, n, 200, info);


//    cout << vv << '\n';


//    cout << result << endl;

    delete info;


    return 0;
}

