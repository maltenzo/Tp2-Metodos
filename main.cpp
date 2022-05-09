#include <iostream>
#include "knn.hpp"

using namespace std;

int main(int, char **)
{
	Knn knn;
	knn.setDataset("../train.csv");
	auto it = knn.getDataset()->begin();
	while (it != knn.getDataset()->end())
	{
		cout << *it;
		break;
	}

	return 0;
}
