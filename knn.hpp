#include "./Eigen/Dense"
#include <vector>
#include <fstream>
#include "utils.hpp"
using namespace std;
typedef Eigen::MatrixXd Matrix;

template <typename T>
int findIndex(vector<T> &v, T elem)
{
	int index = 0;
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] == elem)
		{
			index = i;
			break;
		}
	}
	return index;
}

int mostCommonLabel(vector<int> labels)
{
	vector<int> buckets(10, 0);

	for (int i = 0; i < labels.size(); i++)
	{
		buckets[labels[i]]++;
	}

	return findIndex(buckets, *max_element(buckets.begin(), buckets.end()));
}

class Knn
{
public:
	vector<Matrix> *getDataset()
	{
		return &(this->dataset);
	}

	void setDataset(vector<Matrix> dataset, vector<int> labels)
	{
		this -> dataset = dataset;
		this -> labels = labels;
	}

	vector<int> testImages(vector<Matrix> &testImages, int k)
	{

		vector<int> responses;

		for (int i = 0; i < testImages.size(); i++)
		{
			responses.push_back(recognize(testImages[i], k));
		}

		return responses;
	}

private:
	vector<Matrix> dataset;
	vector<int> labels;

	int recognize(Matrix imageMatrix, int k)
	{

		vector<Matrix> *dataSet = this->getDataset();
		int dataSetSize = dataSet->size();

		vector<int> kVecinosMasCercanos(k, 0);
		vector<float> kMenoresDistancias(k, 0);
		float distanciaActual;
		float distanciaMaximaDeLasKMenores;
		int indiceDistanciaMaxima;
		for (int i = 0; i < dataSetSize; i++)
		{
			distanciaActual = (imageMatrix - (*dataSet)[i]).norm();
			distanciaMaximaDeLasKMenores = *max_element(kMenoresDistancias.begin(), kMenoresDistancias.end());

			if (i >= k && distanciaActual < distanciaMaximaDeLasKMenores)
			{
				indiceDistanciaMaxima = findIndex(kMenoresDistancias, distanciaMaximaDeLasKMenores);
				kMenoresDistancias[indiceDistanciaMaxima] = distanciaActual;
				kVecinosMasCercanos[indiceDistanciaMaxima] = i;
			}
			else if (i < k)
			{
				kMenoresDistancias[i] = distanciaActual;
				kVecinosMasCercanos[i] = i;
			}
		}

		vector<int> nearestLabels(k, 0);

		for (int i = 0; i < k; i++)
		{
			nearestLabels[i] = this->labels[kVecinosMasCercanos[i]];
		}

		int respuesta = mostCommonLabel(nearestLabels);

		cout << "Es un " << respuesta << endl;

		return respuesta;
	}
};