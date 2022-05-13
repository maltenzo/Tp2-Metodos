#include "./Eigen/Dense"
#include <vector>
#include <fstream>
using namespace std;
typedef Eigen::MatrixXd Matrix;
typedef Eigen::VectorXd MagicVector;
typedef tuple<float, vector<float>> AutoValAndVec;

class pca
{
private:
	/* data */
	Matrix M;
	vector<AutoValAndVec> autovalores_autovectores;

	Matrix matriz_covariancia_de_imagen(Matrix matImagen);
	// utils
	MagicVector getRandomVector(int dimension)
	{
		MagicVector v;
		for (int i = 0; i < dimension : i++)
		{
			v[i] = ran() / (float)rand()
		}
		return v;
	};

public:
	Matrix get_matrix()
	{
		return &(this->M);
	};

	vector<AutoValAndVec> get_autoval_y_autovec()
	{
		return &(this->autovalores_autovectores);
	};

	// revisar porque lo hizo enzo
	AutoValAndVec met_potencia(Matrix M, int nitter, float epsilon)
	{
		MagicVector v = this.getRandomVector();
		float diff = 1;
		for (int i = 0; i < nitter && diff > epsilon; i++)
		{
			MagicVector w = M * v;
			w = w / w.norm();
			diff = abs(w.norm() - v.norm()); // esto esta bien?
			v = w;
		}
		float lambda = (v.transpose * M * v) / (v.transpose * v);
		return lambda;
	}

	vector<AutoValAndVec> met_potencia_y_defl(Matrix M, int nitter, float epsilon);

	pca(/ Matrix A);
	~pca();
};

pca::pca(A)
{
	srand(static_cast<unsigned>(time(0)));
	this->M = matriz_covariancia_de_imagen(A);
	this->autovalores_autovectores = [];
}

pca::~pca()
{
}
