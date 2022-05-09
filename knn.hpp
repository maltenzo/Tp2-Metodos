#include "./Eigen/Dense"
#include <vector>
#include <fstream>
using namespace std;
typedef Eigen::MatrixXd Matrix;

class Knn
{
public:
	vector<Matrix> *getDataset()
	{
		return &(this->dataset);
	}

	void setDataset(string dataSetFileName)
	{

		ifstream dataSetFile;
		dataSetFile.open(dataSetFileName);
		if (dataSetFile.is_open())
		{
			string image = "";

			getline(dataSetFile, image); // Primer linea, no la queremos
			while (getline(dataSetFile, image))
			{
				matrixize(image);
			}
			dataSetFile.close();
		}
		else
		{
			throw 500;
		}
	}

private:
	vector<Matrix> dataset;
	vector<int> labels;

	void matrixize(string s, string del = ",")
	{

		Matrix matrix(28, 28);

		int start = 0;
		int end = s.find(del);
		this->labels.push_back(stoi(s.substr(start, end - start)));
		start = end + del.size();
		end = s.find(del, start);

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
		this->dataset.push_back(matrix);
	}
};