#pragma once

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>
using std::vector;

typedef std::vector<std::vector<double>> VECTOR2D;

template<int Size>
struct Solution
{
	double x[Size];

	double operator[] (int i) { return x[i]; }
};

class Matrix
{
	int row;
	int col;
	vector<vector<double>> matrix;

public:
	Matrix(int Row, int Col) : row(Row), col(Col)
	{
		do
		{
			matrix.push_back(vector<double>(col, 0.0));		//A row with COL columns
		} while (matrix.size() < row);						//For number of rows
	}

	Matrix(VECTOR2D vec) : row(0), col(0)
	{
		row = vec.size();
		if (row != 0)
			col = vec[0].size();

		matrix = vec;
	}

	void AddToMatrix(int rw, int cl, double val);

	double GetFromMatrix(int rw, int cl);
	int GetRow() { return row; }
	int GetCol() { return col; }

	void MakeNormal();
	Matrix* AddRow(int r1, int r2, double cr2);
	Matrix* MultRow(int r, double c);
	Matrix* SwapRow(int r1, int r2);
};

template<int Size>
Solution<Size> SolveEqn(Matrix matrix)
{
	Solution<Size> soln;
	matrix.MakeNormal();
	for (int i = 0; i < matrix.GetRow(); i++)
	{
		if (i < Size)
			soln.x[i] = matrix.GetFromMatrix(i, matrix.GetCol() - 1);
	}

	return soln;
}
#endif