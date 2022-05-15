#include "./Eigen/Dense"
#include <vector>
#include <tuple>
#include <fstream>
using namespace std;
typedef Eigen::MatrixXd Matrix;
typedef Eigen::VectorXd MagicVector;

class pca
{
private:
	/* data */
	Matrix M;
	MagicVector autovalores;
	Matrix autovectores;
	int nitter;
	float epsilon;
	int alpha;

	Matrix matriz_covariancia_de_imagen(Matrix matImagen);

public:
	Matrix get_matrix();
	MagicVector get_autoval();
	Matrix get_autovec();

	void met_potencia(Matrix& M, int colnumber);

	void met_potencia_y_defl();

	pca( Matrix A, int nitter, float epsilon, int alpha);
	~pca();
};

pca::pca(Matrix A, int nitter,  float epsilon, int alpha)
{
	srand(static_cast<unsigned>(time(0)));
	this->M = matriz_covariancia_de_imagen(A);
	this->autovalores = MagicVector::Zero(alpha);
	this->autovectores = Matrix::Zero(alpha,alpha);
	this->nitter = nitter;
	this->epsilon = epsilon;
	this->alpha = alpha;
}

pca::~pca()
{
}

Matrix pca::get_matrix()
{
	return this->M;
};

MagicVector pca::get_autoval()
{
	return this->autovalores;
};

Matrix pca::get_autovec(){
	return this->autovectores;
}

Matrix pca::matriz_covariancia_de_imagen(Matrix matImagen){
	int n = matImagen.cols();
	for(int i = 0; i < n; i++){
		matImagen.col(i) = ( matImagen.col(i) - matImagen.col(i).mean()*MagicVector::Ones(n) )/sqrt(n-1);
	}

	return matImagen.transpose()*matImagen;
}

// revisar porque lo hizo enzo
void pca::met_potencia(Matrix& M, int colnumber)
{
	MagicVector v = getRandomVector(M.rows());
	float diff = 1;
	for (int i = 0; i < this->nitter && diff > this->epsilon; i++)
	{
		MagicVector w = M * v;
		w = w / w.norm();
		diff = abs(w.norm() - v.norm()); // esto esta bien?
		v = w;
	}
	double lambda_non_normal = (v.transpose() * (M * v));
	double lambda = lambda_non_normal / (v.transpose() * v);

	this->autovalores(colnumber) = lambda;
	this->autovectores.col(colnumber) = v;
}

void pca::met_potencia_y_defl(){
	Matrix MDefl = this->M;
	for(int i = 0; i < this->alpha; i++){
		this->met_potencia(MDefl, i);
		float lambda = this->autovalores(i);
		MagicVector autovec = this->autovectores.col(i);
		MDefl = MDefl - lambda*(autovec * autovec.transpose());

	}
};