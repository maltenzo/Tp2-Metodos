#include "./Eigen/Dense"
#include <vector>
#include <fstream>
using namespace std;
typedef Eigen::MatrixXd Matrix;
typedef tuple <float, vector<float> > AutoValAndVec;

class pca
{
private:
    /* data */
    Matrix M;
    vector<AutoValAndVec> autovalores_autovectores;

    Matrix matriz_covariancia_de_imagen (Matrix matImagen);

public:

    Matrix get_matrix(){
        return &(this->M);
    };

    vector<AutoValAndVec> get_autoval_y_autovec(){
        return &(this->autovalores_autovectores);
    };

    AutoValAndVec met_potencia (Matrix M, int nitter, float epsilon);

    vector<AutoValAndVec> met_potencia_y_defl (Matrix M, int nitter, float epsilon);



    pca(/Matrix A);
    ~pca();
};

pca::pca(A)
{
    this->M = matriz_covariancia_de_imagen(A);
    this->autovalores_autovectores = [];
}

pca::~pca()
{
}



