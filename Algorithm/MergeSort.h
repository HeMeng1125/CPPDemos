#ifndef MERGESORT_H
#define MERGESORT_H
#include <vector>

void input(std::vector<int> &v);

void merge(std::vector<int> &v, int p, int q, int r);

void merge_sort(std::vector<int> &v, int p, int r);

void output(std::vector<int> &v);

#endif // !MERGESORT_H
