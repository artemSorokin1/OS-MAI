#include "../lib/functions.h"


int part = 0;

vector<int> v;



const int MAX_THREDS = 2;

void* foo(void* arg) {
    int thread = part++;
    int l = (int)v.size() / MAX_THREDS;

    int left = thread * l;
    int right = left + l - 1;

    OddEvenSort(v, left, right);

    pthread_exit(nullptr);

}



int main(int argc, char* argv[]) {
    int SIZE;
    if (argv[1] == nullptr) {
        cout << "Введите количество элементов в массиве: " << '\n';
        cin >> SIZE;
    } else {
        SIZE = stoi(argv[1]);
    }

    int tmp_size = (int)ceil(log2(SIZE));
//
//    // вырвнивание длины массива
    int new_size = bpow(2, tmp_size);

    // заполняю рандомными числами
    srand(time(nullptr));
    for (int i = 0; i < SIZE; ++i) {
        int x = rand() % 1000;
        v.push_back(x);
    }

    for (int i = 0; i < new_size - SIZE; ++i) {
        v.push_back((int)1e9);
    }


    pthread_t tid[MAX_THREDS];

    for (int i = 0; i < MAX_THREDS; ++i) {
        pthread_create(&tid[i], nullptr, foo, nullptr);
    }

    for (auto & elem : tid) {
        pthread_join(elem, nullptr);
    }


    MergeSort(v);

    cout << v << endl;

    return 0;
}