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
	this->autovectores = Matrix::Zero(A.rows(),alpha);
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
	for (int i = 0; i < this->nitter ; i++)
	{
		MagicVector w = M * v;
		w = w / w.norm();

		double cos = w.dot(v);
		if(1-(this->epsilon) < cos && cos <= 1){
			v = w;
			break;
		}
		
	}
	double lambda_non_normal = v.dot(M * v);
	double lambda = lambda_non_normal / v.transpose().dot(v);

	this->autovalores(colnumber) = lambda;
	this->autovectores.col(colnumber) = v.transpose();
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

void transformImagesWithPCA(vector<Matrix>& imageList, int nitter=1000, float epsilon=1e-6, int alpha=28){
	for(int i = 0; i < imageList.size(); i++){
		Matrix image = imageList[i];
		pca PCAMethod(image, nitter, epsilon, alpha);
		PCAMethod.met_potencia_y_defl();
		imageList[i] = PCAMethod.get_autovec().transpose() * image;
	}
}