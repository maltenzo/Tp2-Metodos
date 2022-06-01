#include <iostream>
#include <chrono>
#include "knn.hpp"
#include "pca.hpp"

using namespace std;

void handleError(int error)
{

	if (error == 500)
	{
		clog << "La entrada debe tener 5 u 8 argumentos: " << endl;
		clog << "	Archivo de entrenamiento, archivo de prueba, archivo de salida, cantidad de vecinos (k), método (0 kNN, 1 PCA+kNN) "<< endl;
		clog << "	Ademas, si se eligió PCA: cantidad de iteraciones, punto de corte (epsilon), cantidad de componentes principales (alfa)" << endl;
	}
	else if (error == 501)
	{
		clog << "El método debe ser 0 o 1 (0 KNN, 1 PCA)" << endl;
	}
	else if (error == 502)
	{
		clog << "Error al abrir alguno de los archivos de entrada" << endl;
	}
}

int main(int argc, char **argv)
{
	try
	{
		if (argc != 6 && argc != 9)
			throw 500;

		string trainFileName = argv[1];
		string testFileName = argv[2];
		string outputFileName = argv[3];

		int k = stoi(argv[4]);
		int method = stoi(argv[5]);
		int nitter, epsilon, alfa;

		if (method > 1 || method < 0){
			throw 501;
		}
		else if(method == 1){
			nitter = stoi(argv[6]);
			epsilon = stoi(argv[7]);
			alfa = stoi(argv[8]);
		}

		vector<int> trainLabels;
		vector<Matrix> trainImages;
		csvImagesToMatrixVector(trainFileName, trainImages, trainLabels);

		//vector<int> testLabels;
		vector<Matrix> testImages;
		csvImagesToMatrixVector(testFileName, testImages);

		//trainImages.resize(20);
		//testImages.resize(5);


		vector<int> results;
		// inicio el reloj para medir la duración del algoritmo
		auto start = chrono::steady_clock::now();

		if (method == 0)
		{ // KNN
			Knn knnMethod;
			knnMethod.setDataset(trainImages, trainLabels);
			results = knnMethod.testImages(testImages, k);
		}else if (method == 1)
		{ //PCA+KNN

			pca PCAMethod = pca(trainImages, nitter, epsilon, alfa);
			PCAMethod.met_potencia_y_defl();
			PCAMethod.transform_train_images(trainImages);
			PCAMethod.transform_test_images(testImages);

			Knn knnMethod;
			knnMethod.setDataset(trainImages, trainLabels);


			results = knnMethod.testImages(testImages, k);
		}		

		// calculo cuanto tardó la ejecución
		auto end = chrono::steady_clock::now();
		double tiempoDeEjecucion = chrono::duration<double, milli>(end - start).count();

		cerr << tiempoDeEjecucion << endl;

		ofstream outputFile;
		outputFile.open(outputFileName);
		if (outputFile.is_open())
		{
			cout << "Escribiendo respuesta" << endl;

			string resultado;

			for(int i = 0 ; i < results.size() ; i++){
				resultado = to_string(results[i]) + "\n";
				outputFile.write(resultado.c_str(), resultado.size());
			}

			cout << "OK" << endl;
		}
		else
		{
			throw 504;
		}
	}
	catch (int e)
	{
		handleError(e);
	}

	return 0;
}
