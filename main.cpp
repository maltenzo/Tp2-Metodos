#include <iostream>
#include <chrono>
#include "knn.hpp"

using namespace std;

void handleError(int error)
{

	if (error == 500)
	{
		clog << "La entrada debe tener 4 argumentos: archivo de entrenamiento, archivo de prueba, cantidad de vecinos (k) y método (0 KNN, 1 PCA)" << endl;
	}
	else if (error == 501)
	{
		clog << "El método debe ser 0 o 1 (0 KNN, 1 PCA)" << endl;
	}
	else if (error == 502)
	{
		clog << "Error al abrir el archivo de entrenamiento" << endl;
	}
	else if (error == 503)
	{
		clog << "Error al abrir el archivo de prueba" << endl;
	}
}

Matrix matrixize(string s, string del = ",")
{

	Matrix matrix(28, 28);

	int start = 0;
	int end = s.find(del);

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

void csvImagesToMatrixVector(string csvImagesFileName, vector<Matrix> &matrixVector, string delimitador = ",")
{

	ifstream csvImagesFile;
	csvImagesFile.open(csvImagesFileName);
	if (csvImagesFile.is_open())
	{
		string image = "";

		getline(csvImagesFile, image); // Primer linea, no la queremos
		while (getline(csvImagesFile, image))
		{
			matrixVector.push_back(matrixize(image));
		}
		csvImagesFile.close();
	}
	else
	{
		throw 502;
	}
}

// Version que procesa labels, es un overload de la otra
void csvImagesToMatrixVector(string csvImagesFileName, vector<Matrix> &matrixVector, vector<int> &labels, string delimitador = ",")
{

	ifstream csvImagesFile;
	csvImagesFile.open(csvImagesFileName);
	if (csvImagesFile.is_open())
	{
		string image = "";

		getline(csvImagesFile, image); // Primer linea, no la queremos
		while (getline(csvImagesFile, image))
		{
			int posicionDelPrimerDelimitador = image.find(delimitador);
			labels.push_back(stoi(image.substr(0, posicionDelPrimerDelimitador)));
			matrixVector.push_back(matrixize(image.substr(posicionDelPrimerDelimitador + delimitador.size())));
		}
		csvImagesFile.close();
	}
	else
	{
		throw 502;
	}
}

int main(int argc, char **argv)
{
	try
	{
		if (argc != 5)
			throw 500;

		string trainFileName = argv[1];
		string testFileName = argv[2];

		int k = stoi(argv[3]);
		int method = stoi(argv[4]);

		if (method > 1 || method < 0)
			throw 501;

		vector<int> testLabels;
		vector<Matrix> testImages;
		csvImagesToMatrixVector(testFileName, testImages);

		// inicio el reloj para medir la duración del algoritmo
		auto start = chrono::steady_clock::now();

		if (method == 0)
		{ // KNN
			Knn knnMethod;
			knnMethod.setDataset(trainFileName);
			knnMethod.testImages(testImages, k);
		}

		// calculo cuanto tardó la ejecución
		auto end = chrono::steady_clock::now();
		double tiempoDeEjecucion = chrono::duration<double, milli>(end - start).count();

		cerr << tiempoDeEjecucion << endl;

		// ofstream outputFile;
		// outputFile.open(outputFileName);
		// if (outputFile.is_open())
		// {
		// 	cout << "Escribiendo respuesta" << endl;

		// 	cout << "OK" << endl;
		// }
		// else
		// {
		// 	throw 504;
		// }
	}
	catch (int e)
	{
		handleError(e);
	}

	return 0;
}
