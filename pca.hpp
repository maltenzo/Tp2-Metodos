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
	Matrix imgsFlat;
	Matrix M;
	MagicVector autovalores;
	Matrix autovectores;
	int nitter;
	float epsilon;
	int alpha;

	Matrix matriz_covariancia_de_imagen(Matrix matImagen);
	void met_potencia(Matrix& M, int colnumber);

public:
	Matrix get_flat_imgs();
	Matrix get_matrix();
	MagicVector get_autoval();
	Matrix get_autovec();

	void met_potencia_y_defl();

	pca( vector<Matrix> A, int nitter, float epsilon, int alpha);
	~pca();
};

pca::pca(vector<Matrix> A, int nitter,  float epsilon, int alpha)
{
	srand(static_cast<unsigned>(time(0)));
	this->imgsFlat = aplanar_matrices(A);
	this->M = matriz_covariancia_de_imagen(this->imgsFlat);
	this->autovalores = MagicVector::Zero(alpha);
	this->autovectores = Matrix::Zero(this->imgsFlat.cols(),alpha);
	this->nitter = nitter;
	this->epsilon = epsilon;
	this->alpha = alpha;
}

pca::~pca()
{
}

Matrix pca::get_flat_imgs(){
	return this->imgsFlat;
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

	int n = matImagen.rows();
	Matrix mu = matImagen.colwise().mean();

	for(int i = 0; i<n; i++){
		matImagen.row(i) = (matImagen.row(i) - mu.row(0))/sqrt(n-1);
	}

	return matImagen.transpose()*matImagen;
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
	Matrix MDefl = this->M;
	for(int i = 0; i < this->alpha; i++){
		this->met_potencia(MDefl, i);
		float lambda = this->autovalores(i);
		MagicVector autovec = this->autovectores.col(i);
		MDefl = MDefl - lambda*(autovec * autovec.transpose());

	}
};


Matrix transformTrainImagesWithPCA(vector<Matrix>& imageList, int nitter=1000, float epsilon=1e-9, int alpha=15){
	
	pca PCAMethod(imageList, nitter, epsilon, alpha);
	PCAMethod.met_potencia_y_defl();
	Matrix transformedImages = PCAMethod.get_flat_imgs() * PCAMethod.get_autovec();;
	for(int i = 0; i < imageList.size(); i++){
		imageList[i] = transformedImages.row(i);
	}
	return PCAMethod.get_autovec();
}

void transformTestImagesWithPCA(vector<Matrix>& imageList, Matrix vTrans){

	Matrix transformedImages = aplanar_matrices(imageList) * vTrans;
	for(int i = 0; i < imageList.size(); i++){
		imageList[i] = transformedImages.row(i);
	}
}