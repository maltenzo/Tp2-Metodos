#include "./Eigen/Dense"
#include <vector>
#include <tuple>
#include <fstream>
using namespace std;
typedef Eigen::MatrixXd Matrix;
typedef Eigen::VectorXd MagicVector;

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

MagicVector getRandomVector(int dimension)
{
	MagicVector v = MagicVector::Ones(dimension);
	for(int i = 0; i < dimension ; i++){
		v[i] = rand() / (float)rand();
	}
	return v;
}

Matrix aplanar_matrices(vector<Matrix> vM){
	int n = vM.size();
	int cols = vM[n-1].cols();
	int rows = vM[n-1].rows();
	Matrix flatVer(n, cols*rows);

	for(int k = 0; k < n; k++){
		flatVer.row(k) = vM[k].reshaped(1, cols*rows).row(0);
	}
	return flatVer;
}
