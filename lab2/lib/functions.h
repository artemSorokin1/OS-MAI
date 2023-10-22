#pragma once
#include <iostream>
#include <pthread.h>
#include <ctime>
#include <cstdlib>
#include <cmath>


using namespace std;


int bpow(int a, int b);

void OddEvenSort(vector<int> & vec, size_t l, size_t r);

void OddEvenMergeSort(vector<int> & vec, size_t l, size_t r);

void shuffle(vector<int> & vec, size_t l, size_t r);

void unsuffle(vector<int> & vec, size_t l, size_t r);

ostream& operator<< (ostream& out, const vector<int> & v);

int gcd(int a, int b);

int nok(int a, int b);


vector<int> merge(vector<int> &a, vector<int> & b);

vector<int> MergeSort(vector<int> & v);