#include "wavelet_transform.h"












void IPL::PyramidExample(const std::vector<std::vector<double>> &src, std::vector<std::vector<double>> &approximate, std::vector<std::vector<double>> &residual)
{
	int rows = src.size();
	int cols = src[0].size();


	// ���ɽ���ͼ
	auto s = IPL::BlurArithmetic(src, 5);
	approximate = PryDown(s);

	// �ϲ��������в�ֵ�˲�
	// �ϲ�������һ���õ���ͼ��approximate�ߴ��������� ��������ؼ����0
	// ��ֵ�˲�������Ϊ0��λ�ø���ԭͼ��approximate���в�ֵ
	// �����������ڲ�ֵ����������������
	auto indicate = IPL::NearestNeighborInterpolation(approximate, rows, cols);
	

	residual = IPL::MakeVector2D(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			residual[i][j] = src[i][j] - indicate[i][j];
		}
	}
}

IPL::HaarBasis::HaarBasis(int k, int p, int q, int N)
	:_k(k), _p(p), _q(q), _N(N)
{
	if (_k != pow(2, _p) + _q - 1)
		throw std::invalid_argument("k != 2^p+1-1");
	_div_sqrt_N = 1.0 / sqrt(_N);
}

double IPL::HaarBasis::operator()(double z)
{
	if (_k == 0)
		return _div_sqrt_N;

	if ((z >= (_q - 1) / pow(2, _p)) && (z < (_q - 0.5) / pow(2, _p)))
	{
		return pow(2, _p / 2.0)*_div_sqrt_N;
	}
	else if ((z >= (_q - 0.5) / pow(2, _p)) && (z < _q  / pow(2, _p)))
	{
		return -pow(2, _p / 2.0)*_div_sqrt_N;
	}
	else
	{
		return 0;
	}
}