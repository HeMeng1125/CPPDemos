#include "MergeSort.h"

using namespace std;

int main(int argc, char *argv [])
{
	vector<int> v;
	input(v);
	merge_sort(v, 0, v.size() - 1);
	output(v);
	system("PAUSE");
	return EXIT_SUCCESS;
}
