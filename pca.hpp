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
	Matrix mu;
	Matrix X;
	Matrix Mx;
	MagicVector autovalores;
	Matrix autovectores;
	int n;
	int nitter;
	float epsilon;
	int alpha;

	void matriz_varianza_0(Matrix matImagen);
	void met_potencia(Matrix& M, int colnumber);

public:
	Matrix get_mu();
	Matrix get_X();
	Matrix get_Mx();
	MagicVector get_autoval();
	Matrix get_autovec();

	void met_potencia_y_defl();
	void transform_train_images(vector<Matrix>& ImageList);
	void transform_test_images(vector<Matrix>& ImageList);

	pca( vector<Matrix> A, int nitter, float epsilon, int alpha);
	~pca();
};

pca::pca(vector<Matrix> A, int nitter,  float epsilon, int alpha)
{
	srand(static_cast<unsigned>(time(0)));
	this->matriz_varianza_0(aplanar_matrices(A));
	this->autovalores = MagicVector::Zero(alpha);
	this->autovectores = Matrix::Zero(this->X.cols(),alpha);
	this->nitter = nitter;
	this->epsilon = epsilon;
	this->alpha = alpha;
}

pca::~pca()
{
}

Matrix pca::get_mu(){
	return this->mu;
}

Matrix pca::get_X()
{
	return this->X;
};

Matrix pca::get_Mx()
{
	return this->Mx;
};

MagicVector pca::get_autoval()
{
	return this->autovalores;
};

Matrix pca::get_autovec(){
	return this->autovectores;
}

void pca::matriz_varianza_0(Matrix matImagen){

	this->n = matImagen.rows();
	this->mu = matImagen.colwise().mean();

	for(int i = 0; i<this->n; i++){
		matImagen.row(i) = (matImagen.row(i) - this->mu.row(0))/sqrt(n-1);
	}

	this->X = matImagen;
	this->Mx = matImagen.transpose()*matImagen;
}

// revisar porque lo hizo enzo
void pca::met_potencia(Matrix& M, int colnumber)
{
	MagicVector v = getRandomVector(M.rows());
	v = v.cwiseAbs();
	v = v/v.norm();
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
	Matrix MDefl = this->Mx;
	for(int i = 0; i < this->alpha; i++){
		this->met_potencia(MDefl, i);
		float lambda = this->autovalores(i);
		MagicVector autovec = this->autovectores.col(i);
		MDefl = MDefl - lambda*(autovec * autovec.transpose());

	}
};

void pca::transform_train_images(vector<Matrix>& ImageList){
	Matrix transformedImages = this->X * this->autovectores;
	for(int i = 0; i < ImageList.size(); i++){
		ImageList[i] = transformedImages.row(i);
	}
};

void pca::transform_test_images(vector<Matrix>& ImageList){
	Matrix flatTest = aplanar_matrices(ImageList);
	for(int i = 0; i<ImageList.size(); i++){
		flatTest.row(i) = (flatTest.row(i) - this->mu)/sqrt(this->n - 1);
		ImageList[i] = flatTest.row(i) * this->autovectores;
	}
};