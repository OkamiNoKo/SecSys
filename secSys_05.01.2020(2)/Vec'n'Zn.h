#pragma once
const double PI = 3.1415926536;
#include "Rewrotter.h"

int wrachenie(float **coefficients, int numberOfEquation, double **solution, double precision) {
	int result = 1;
	int i, j, k;
	int maxI, maxJ;
	double max, fi;
	double** matricaPoworota;
	matricaPoworota =  (double**)malloc(sizeof(double*) * numberOfEquation);
	for (i = 0; i < numberOfEquation; i++) {
		matricaPoworota[i] = (double*)malloc(sizeof(double) * numberOfEquation);
	}
	double** temp = (double**)malloc(sizeof(double*) * numberOfEquation);

	for (i = 0; i < numberOfEquation; i++) {
		temp[i] = (double*)malloc(sizeof(double) * numberOfEquation);
	}
	double fault = 0.0;
	for (i = 0; i < numberOfEquation; i++) {
		for (j = i + 1; j < numberOfEquation; j++) {
			fault = fault + coefficients[i][j] * coefficients[i][j];
		}
	}
	fault = sqrt(2 * fault);
	while (fault > precision) {
		max = 0.0;
		for (i = 0; i < numberOfEquation; i++) {
			for (j = i + 1; j < numberOfEquation; j++) {
				if (coefficients[i][j] > 0 && coefficients[i][j] > max) {
					max = coefficients[i][j];
					maxI = i;
					maxJ = j;
				}
				else if (coefficients[i][j] < 0 && -coefficients[i][j] > max) {
					max = -coefficients[i][j];
					maxI = i;
					maxJ = j;
				}
			}
		}
		for (i = 0; i < numberOfEquation; i++) {
			for (j = 0; j < numberOfEquation; j++) {
				matricaPoworota[i][j] = 0;
			}
			matricaPoworota[i][i] = 1;
		}
		if (coefficients[maxI][maxI] == coefficients[maxJ][maxJ]) {
			matricaPoworota[maxI][maxI] = matricaPoworota[maxJ][maxJ] =
				matricaPoworota[maxJ][maxI] = sqrt(2.0) / 2.0;
			matricaPoworota[maxI][maxJ] = -sqrt(2.0) / 2.0;
		}
		else {
			fi = 0.5 * atan((2.0 * coefficients[maxI][maxJ]) /
				(coefficients[maxI][maxI] - coefficients[maxJ][maxJ]));
			matricaPoworota[maxI][maxI] = matricaPoworota[maxJ][maxJ] = cos(fi);
			matricaPoworota[maxI][maxJ] = -sin(fi);
			matricaPoworota[maxJ][maxI] = sin(fi);
		}
		for (i = 0; i < numberOfEquation; i++) {
			for (j = 0; j < numberOfEquation; j++) {
				temp[i][j] = 0.0;
			}
		}
		for (i = 0; i < numberOfEquation; i++) {
			for (j = 0; j < numberOfEquation; j++) {
				for (k = 0; k < numberOfEquation; k++) {
					temp[i][j] = temp[i][j] + matricaPoworota[k][i] * coefficients[k][j];
				}
			}
		}
		for (i = 0; i < numberOfEquation; i++) {
			for (j = 0; j < numberOfEquation; j++) {
				coefficients[i][j] = 0.0;
			}
		}
		for (i = 0; i < numberOfEquation; i++) {
			for (j = 0; j < numberOfEquation; j++) {
				for (k = 0; k < numberOfEquation; k++) {
					coefficients[i][j] = coefficients[i][j] +
						temp[i][k] * matricaPoworota[k][j];
				}
			}
		}
		fault = 0.0;
		for (i = 0; i < numberOfEquation; i++) {
			for (j = i + 1; j < numberOfEquation; j++) {
				fault = fault + coefficients[i][j] * coefficients[i][j];
			}
		}
		fault = sqrt(2 * fault);
		for (i = 0; i < numberOfEquation; i++) {
			for (j = 0; j < numberOfEquation; j++) {
				temp[i][j] = 0.0;
			}
		}
		for (i = 0; i < numberOfEquation; i++) {
			for (j = 0; j < numberOfEquation; j++) {
				for (k = 0; k < numberOfEquation; k++) {
					temp[i][j] = temp[i][j] + solution[i][k] * matricaPoworota[k][j];
				}
			}
		}
		for (i = 0; i < numberOfEquation; i++) {
			for (j = 0; j < numberOfEquation; j++) {
				solution[i][j] = temp[i][j];
			}
		}
		result++;
	}
	return result;
}

void matr(float **coefficients, double precision,int size)
{

	int i, j;

	double** solution  = (double**)malloc(sizeof(double*) * size);

	for (i = 0; i < size; i++) {
		solution[i] = (double*)malloc(sizeof(double) * size);
	}
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			solution[i][j] = 0;
		}
		solution[i][i] = 1;
	}

	int steps = wrachenie(coefficients, size, solution, precision);
	int max1 = 0; double maxZ1 = coefficients[0][0];

	for (int i = 1; i < size; i++)
	{
		if (coefficients[i][i] > maxZ1)
		{
			maxZ1 = coefficients[i][i];
			max1 = i;
		}
	}

	int max2 = 0; double maxZ2 = coefficients[0][0];
	if (max1 == 0)
	{
		max2 = 1;
		maxZ2 = coefficients[1][1];
	}
	for (int i = max2 + 1; i < size; i++)
	{
		if (coefficients[i][i] > maxZ2 && max1 != i)
		{
			maxZ2 = coefficients[i][i];
			max2 = i;
		}
	}
	FILE * outF = fopen("M2Vec.txt", "w");

	fprintf(outF, "%f %f %f %f %f %f %f \n", solution[0][max1], solution[1][max1], solution[2][max1], solution[3][max1], solution[4][max1], solution[5][max1], solution[6][max1]);
	fprintf(outF, "%f %f %f %f %f %f %f \n", solution[0][max2], solution[1][max2], solution[2][max2], solution[3][max2], solution[4][max2], solution[5][max2], solution[6][max2]);

	fclose(outF);

}