#include "functions.h"


void OddEvenSort(vector<int> & vec, size_t l, size_t r) {
    bool check = false;
    while (!check) {
        check = true;
        for (size_t i = l; i < r - 1; i += 2) {
            if (vec[i] > vec[i + 1]) {
                swap(vec[i], vec[i + 1]);
                check = false;
            }
        }
        for (size_t i = l + 1; i < r - 1; i += 2) {
            if (vec[i] > vec[i + 1]) {
                swap(vec[i], vec[i + 1]);
                check = false;
            }
        }
    }
}

int bpow(int a, int b) {
    int z = 1;
    while (b > 0) {
        if (b & 1) {
            z *= a;
        }
        a *= a;
        b = b >> 1;
    }
    return z;
}

ostream& operator<< (ostream& out, const vector<int> & v) {
    out << "[ ";
    for (int i = 0 ; i < v.size() - 1;  ++i) {
        out << v[i] << ", ";
    }
    out << v[v.size() - 1] << " ]";
    return out;
}

int gcd(int a, int b) {
    while (a != 0) {
        a %= b;
        swap(a, b);
    }
    return b;
}

int nok(int a, int b) {
    return a / gcd(a, b) * b;
}


void comp(int & a, int & b) {
    if (a > b) {
        swap(a, b);
    }
}



vector<int> merge(vector<int> &a, vector<int> & b) {
    size_t n = a.size();
    size_t m = b.size();
    size_t i = 0, j = 0, k = 0;
    vector<int> c(n + m);
    while (i < n || j < m) {
        if (j == m || i < n && a[i] < b[j]) {
            c[k++] = a[i++];
        } else {
            c[k++] = b[j++];
        }
    }
    return c;
}


vector<int> MergeSort(vector<int> & v) {
    if (v.size() <= 1) {
        return v;
    }
    int half = (int)v.size() / 2;
    vector<int> l, r;
    for (int i = 0; i < half; ++i) {
        l.push_back(v[i]);
    }

    for (int i = half; i < v.size(); ++i) {
        r.push_back(v[i]);
    }

    l = MergeSort(l);
    r = MergeSort(r);

    return merge(l, r);
}

