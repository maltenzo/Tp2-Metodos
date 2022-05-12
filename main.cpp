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




int main(int argc, char ** argv)
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

		// inicio el reloj para medir la duración del algoritmo
		auto start = chrono::steady_clock::now();

		if(method == 0){ //KNN
			Knn knnMethod;
			knnMethod.setDataset(trainFileName);
			knnMethod.testImages(testFileName, k);
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

