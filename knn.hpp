#include "./Eigen/Dense"
#include <vector>
#include <fstream>
using namespace std;
typedef Eigen::MatrixXd Matrix;
 
template<typename T> int findIndex(vector<T> &v, T elem ){
	int index = 0;
	for(int i = 0; i < v.size(); i++){
		if (v[i] ==  elem) 
		{
			index = i; 
			break;
		}
	}
	return index;
}

int mostCommonLabel(vector<int> labels){
	vector<int> buckets(10, 0);

	for(int i = 0; i<labels.size(); i++)
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

	void setDataset(string dataSetFileName)
	{

		ifstream dataSetFile;
		dataSetFile.open(dataSetFileName);
		if (dataSetFile.is_open())
		{
			string image = "";

			getline(dataSetFile, image); // Primer linea, no la queremos
			while (getline(dataSetFile, image))
			{				
				this->dataset.push_back(matrixize(image));
			}
			dataSetFile.close();
		}
		else
		{
			throw 502;
		}
	}


	void testImages(string testImagesFileName, int k){

		vector<int> responses;

		ifstream testImagesFile;
		testImagesFile.open(testImagesFileName);
		if (testImagesFile.is_open())
		{
			string image = "";

			getline(testImagesFile, image); // Primer linea, no la queremos
			while (getline(testImagesFile, image))
			{				
				responses.push_back(recognize(matrixize(image), k));
			}
			testImagesFile.close();
		}
		else
		{
			throw 503;
		}

	}

	
private:
	vector<Matrix> dataset;
	vector<int> labels;

	Matrix matrixize(string s, string del = ",")
	{

		Matrix matrix(28, 28);

		int start = 0;
		int end = s.find(del);
		this->labels.push_back(stoi(s.substr(start, end - start)));
		start = end + del.size();
		end = s.find(del, start);

		for (int i = 0; i < 28; i++)
		{
			for (int j = 0; j < 28; j++)
			{

				matrix(i, j) = stod(s.substr(start, end - start));
				start = end + del.size();
				end = s.find(del, start);

				if (end == -1)
				{
					matrix(i, j) = stod(s.substr(start, end - start));
					break;
				}
			}
			if (end == -1)
			{
				break;
			}
		}
		return matrix;
	}

	int recognize(Matrix imageMatrix, int k){
		
		vector<Matrix>* dataSet = this -> getDataset();
		int dataSetSize = dataSet -> size(); 

		vector<int> kVecinosMasCercanos(k, 0);
		vector<float> kMenoresDistancias(k, 0);
		float distanciaActual;
		float distanciaMaximaDeLasKMenores;
		int indiceDistanciaMaxima;
		for(int i = 0; i < dataSetSize; i++)
		{
			distanciaActual = (imageMatrix - (*dataSet)[i]).norm();
			distanciaMaximaDeLasKMenores = *max_element(kMenoresDistancias.begin(), kMenoresDistancias.end());

			if (i >= k && distanciaActual < distanciaMaximaDeLasKMenores)
			{
				indiceDistanciaMaxima = findIndex(kMenoresDistancias, distanciaMaximaDeLasKMenores);
				kMenoresDistancias[indiceDistanciaMaxima] = distanciaActual;
				kVecinosMasCercanos[indiceDistanciaMaxima] = i;
			}else if (i < k){
				kMenoresDistancias[i] = distanciaActual;
				kVecinosMasCercanos[i] = i;
			}	
		}

		vector<int> nearestLabels(k,0);

		for(int i = 0; i < k; i++)
		{
			nearestLabels[i] = this->labels[kVecinosMasCercanos[i]];	
		}

		int respuesta = mostCommonLabel(nearestLabels);		

		cout << "Es un " << respuesta << endl;

		return respuesta;

	}

};