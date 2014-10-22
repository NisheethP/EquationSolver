#include "Matrix.h"

template<typename T>
void Swap(T& a, T& b)
{
	T c = a;
	a = b;
	b = c;
}

void Matrix::AddToMatrix(int rw, int cl, double val)
{
	matrix.at(rw).at(cl) = val;
}

double Matrix::GetFromMatrix(int rw, int cl)
{
	return matrix.at(rw).at(cl);
}

void Matrix::MakeNormal()
{
	for (int i = 0; i < col; i++)
	{
		double x = 0, y = 0;
		for (int j = 0; j < row; j++)
		{
			if (i < row && j < col)
			{
				x = GetFromMatrix(i, i);
				y = GetFromMatrix(j, i);

				if (x != 0)
					MultRow(i, (double)1 / x);

				if (j != i)
					AddRow(j, i, -1 * y);

				y = GetFromMatrix(j, i);
			}
		}

	}
}

Matrix* Matrix::AddRow(int r1, int r2, double cr2 = 1)
{
	for (int i = 0; i<col; i++)
	{
		matrix.at(r1).at(i) += cr2*GetFromMatrix(r2, i);
	}

	return this;
}

Matrix* Matrix::MultRow(int r, double c)
{
	for (int i = 0; i<col; i++)
	{
		matrix.at(r).at(i) *= c;
	}

	return this;
}

Matrix* Matrix::SwapRow(int r1, int r2)
{
	for (int i = 0; i<col; i++)
	{
		Swap<double>(matrix.at(r1).at(i), matrix.at(r2).at(i));
	}

	return this;
}

