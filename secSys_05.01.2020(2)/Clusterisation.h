#pragma once
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include "math.h"

double EPLSION = 0.1;
int MAX_EXECUTION_CYCLES = 150;
int pntCount = 0; //  число точек определяется при считывании
int clus_num0 = 3;
int clus_num2D0 = 2;
double FUZZ = 1.5;
const float RAND_MAX_F = RAND_MAX;

typedef struct Point
{
	double x, y;
}Point;

double ** fillUMatrix(int pointsCount, int clustersCount);
Point * calculateCenters(double ** MatrixU, double m, Point * points, int clus_num);
double calculateDecisionFunction(Point * MatrixPointX, Point * MatrixCentroids, double ** MatrixU, int clus_num);
double evklidDistance3D(Point pointA, Point pointB);
double * normalizeUMatrixRow(double * MatrixURow, int Num_cl);
double prepareU(double distance, double m);
Point * generateRandomPoints(int count);
Point * generateCenters(Point * arr, int count);

double random_float(double min, double max)
{
	return (min + rand() / RAND_MAX_F * (max - min));
}


//  Fuzzy C Means Algorithm
double ** distributeOverMatrixU(Point * arr, double m, Point * centers,int clus_num)
{
	srand(time(NULL));
	double ** MatrixU;
	centers = generateCenters(arr, clus_num);
	MatrixU = fillUMatrix(pntCount, clus_num);

	int previousDecisionValue = 0;
	int currentDecisionValue = 1;
	double distance = 0;
	for (int a = 0; a < MAX_EXECUTION_CYCLES && (abs(previousDecisionValue - currentDecisionValue) > EPLSION); a++)
	{
		previousDecisionValue = currentDecisionValue;
		centers = calculateCenters(MatrixU, m, arr, clus_num);

		for (int i = 0; i < pntCount; i++)
		{
			for (int j = 0; j < clus_num; j++) {
				distance = evklidDistance3D(arr[i], centers[j]);
				MatrixU[i][j] = prepareU(distance, m);
			}

			MatrixU[i] = normalizeUMatrixRow(MatrixU[i],clus_num);
			printf("%f %f /n", MatrixU[i][0], MatrixU[i][1]);
		}
		currentDecisionValue = calculateDecisionFunction(arr, centers, MatrixU, clus_num);
	}

	return MatrixU;
}


double ** fillUMatrix(int pointsCount, int clustersCount)
{
	double ** MatrixU = (double**)malloc(sizeof(double) * pntCount);
	for (int i = 0; i < pntCount; i++)
	{
		MatrixU[i] = (double*)malloc(sizeof(double) * clustersCount);
		for (int j = 0; j < clustersCount; j++) {
			MatrixU[i][j] = random_float(0, 1);
		}
		MatrixU[i] = normalizeUMatrixRow(MatrixU[i], clustersCount);
	}

	return MatrixU;
}

Point * calculateCenters(double ** MatrixU, double m, Point * points, int clus_num)
{
	Point * MatrixCentroids = (Point*)malloc(sizeof(Point) * clus_num);

	for (int clusterIndex = 0; clusterIndex < clus_num; clusterIndex++) {
		double tempAx = 0, tempBx = 0, tempAy = 0, tempBy = 0;


		for (int i = 0; i < pntCount; i++)
		{
			tempAx += pow(MatrixU[i][clusterIndex], m);
			tempBx += pow(MatrixU[i][clusterIndex], m) * points[i].x;

			tempAy += pow(MatrixU[i][clusterIndex], m);
			tempBy += pow(MatrixU[i][clusterIndex], m) * points[i].y;

		}

		MatrixCentroids[clusterIndex].x = tempBx / tempAx;
		MatrixCentroids[clusterIndex].y = tempBy / tempAy;
	}

	return MatrixCentroids;
}


double calculateDecisionFunction(Point * MatrixPointX, Point * MatrixCentroids, double ** MatrixU, int clus_num)
{
	double sum = 0;
	for (int i = 0; i < pntCount; i++)
	{
		for (int j = 0; j < clus_num; j++)
		{
			sum += MatrixU[i][j] * evklidDistance3D(MatrixCentroids[j], MatrixPointX[i]);
		}
	}

	return sum;
}

double evklidDistance3D(Point pointA, Point pointB)
{
	double distance1 = (pointA.x - pointB.x)*(pointA.x - pointB.x);
	double distance2 = (pointA.y - pointB.y)*(pointA.y - pointB.y);

	double distance = distance1 + distance2;
	return sqrt(distance);
}

double * normalizeUMatrixRow(double * MatrixURow,int Num_cl)
{
	double sum = 0;
	for (int i = 0; i < Num_cl; i++)
	{
		sum += MatrixURow[i];
	}
	for (int i = 0; i < Num_cl; i++)
	{
		MatrixURow[i] = MatrixURow[i] / sum;
	}

	return MatrixURow;
}

double prepareU(double distance, double m)
{
	return pow(1 / distance, 2 / (m - 1));
}

Point * generateRandomPoints(int count)
{
	Point *points = (Point*)malloc(sizeof(Point) * count);
	for (int i = 0; i < count; i++)
	{
		points[i].x = 20 + roundf(rand() / RAND_MAX_F * (255 - 20));
		points[i].y = 20 + roundf(rand() / RAND_MAX_F * (255 - 20));
	}

	return points;
}

Point * generateCenters(Point * arr, int count)
{
	Point *points = (Point*)malloc(sizeof(Point) * count);

	points[0].x = arr[0].x;
	points[0].y = arr[0].y;


	for (int j = 1; j < pntCount; j++)
	{
		if (arr[j].x < points[0].x)
		{
			points[0].x = arr[j].x;
			points[0].y = arr[j].y;
		}
	}

	for (int i = 1; i < count; i++)
	{
		points[i].x = arr[i].x;
		points[i].y = arr[i].y;

		for (int j = i; j < pntCount ; j++)
		{
			double distChamp = 0, distChel = 0;
			for (int k = 0; k < i; k++)
			{
				distChamp = distChamp + evklidDistance3D(points[i], points[k]);
				distChel = distChel + evklidDistance3D(arr[j], points[k]);
			}
			if (distChamp < distChel)
			{
				points[i].x = arr[j].x;
				points[i].y = arr[j].y;
			}
		}
	}

	for (int i = 0; i < count; i++)
	{
		printf("%f %f \n", points[i].x, points[i].y);
	}
	return points;
}

void main_func()
{
	//здесь мы вводим поинтсы points = generateRandomPoints(POINTS_COUNT);
	// добавить считывание точек их переход в 2D и заполнение матрицы ими

	float ** EigenVektor = (float**)malloc(sizeof(float*) * 2);

	float ** dumpData = (float**)malloc(sizeof(float) * 3500);
	for (int i = 0; i < 3500; i++)
	{
		dumpData[i] = (float*)malloc(sizeof(float) * 7);
	}
	EigenVektor[0] = (float*)malloc(sizeof(float) * 7);
	EigenVektor[1] = (float*)malloc(sizeof(float) * 7);

	FILE * in1 = fopen("M2Vec.txt", "rb");

	fscanf(in1, "%f %f %f %f %f %f %f \n", &EigenVektor[0][0], &EigenVektor[0][1], &EigenVektor[0][2], &EigenVektor[0][3], &EigenVektor[0][4], &EigenVektor[0][5], &EigenVektor[0][6]);
	fscanf(in1, "%f %f %f %f %f %f %f \n", &EigenVektor[1][0], &EigenVektor[1][1], &EigenVektor[1][2], &EigenVektor[1][3], &EigenVektor[1][4], &EigenVektor[1][5], &EigenVektor[1][6]);

	fclose(in1);

	//FILE * in2 = fopen("winF.txt", "rb");
	FILE * in2 = fopen("winF2.txt", "rb");
	int N = 0;

	while (!feof(in2))
	{
		/*int tr0, tr1;
		int d1, d2, d3, d4, d5;
		float f1, f2;*/
		float f1, f2, f3, f4, f5, f6, f7;
		//fscanf(in2, "winStart(sec): %d, winEnd(sec): %d, packCount: %d , mostActC: %d, S-SA: %d, avSize: %f, avTimeMostAct: %f, F-S: %d , susComb: %d \n", &tr0, &tr1, &d1, &d2, &d3, &f1, &f2, &d4, &d5);
		fscanf(in2, "%f %f %f %f %f %f %f \n", &f1, &f2, &f3, &f4, &f5, &f6, &f7);
		dumpData[N][0] = f1; dumpData[N][1] = f2; dumpData[N][2] = f3; dumpData[N][3] = f4; dumpData[N][4] = f5; dumpData[N][5] = f6; dumpData[N][6] = f7;
		N++;
	}
	fclose(in2);
	pntCount = N;


	Point * points = (Point*)malloc(sizeof(Point) * pntCount);

	FILE * fout = fopen("ClustF.txt", "w");
	FILE * fout2D = fopen("Clust2D.txt", "w");

	FILE * outF = fopen("Points2D.txt", "w");
	
	for (int i = 0; i < pntCount; i++)
	{
		float sumX = 0, sumY = 0;
		for (int j = 0; j < 7; j++)
		{
			sumX += dumpData[i][j] * EigenVektor[0][j];
			sumY += dumpData[i][j] * EigenVektor[1][j];
		}
		points[i].x = sumX;
		points[i].y = sumY;
		fprintf(outF, "%f %f \n", points[i].x, points[i].y);
	}

	Point * centers = (Point*)malloc(sizeof(Point) * clus_num0);
	Point * centers2D = (Point*)malloc(sizeof(Point) * clus_num2D0);

	double ** matrixU = distributeOverMatrixU(points, FUZZ, centers, clus_num0);
	double ** matrixU2D = distributeOverMatrixU(points, FUZZ, centers2D,clus_num2D0);

	for (int i = 0; i < pntCount; i++)
	{
		fprintf(fout, "%f %f %f \n", matrixU[i][0], matrixU[i][1], matrixU[i][2]);
		fprintf(fout2D, "%f %f \n", matrixU2D[i][0], matrixU2D[i][1]);
	}

	fclose(fout);
	fclose(fout2D);
	fclose(outF);
}


void ClustBusters(float ** Clusters, int *r, int *g, int *b,int N)
{
	int i1 = 0, i2 = 0, i3 = 0;
	for (int i = 0; i < N; i++)
	{
		if (Clusters[i][0] > 0.5)
		{
			i1++;
		}
		if (Clusters[i][1] > 0.5)
		{
			i2++;
		}
		if (Clusters[i][2] > 0.5)
		{
			i3++;
		}
	}
	int x = max(i1, i2, i3);
	if (x == i1)
	{
		*r = 0;
		*g = 1;
		*b = 2;
	}
	if (x == i2)
	{
		*r = 1;
		*g = 0;
		*b = 2;
	}
	if (x == i3)
	{
		*r = 2;
		*g = 1;
		*b = 0;
	}
}

void lim_fun(double SenWall)
{
	float ** Points = (float**)malloc(sizeof(float) * 3500);
	float ** Clusters = (float**)malloc(sizeof(float) * 3500);
	for (int i = 0; i < 3500; i++)
	{
		Points[i] = (float*)malloc(sizeof(float) * 2);
		Clusters[i] = (float*)malloc(sizeof(float) * 3);
	}

	FILE * inF = fopen("ClustF.txt", "rb");
	FILE * inF2 = fopen("Points2D.txt", "rb");

	int N = 0;

	while (!feof(inF2))
	{
		fscanf(inF2, "%f %f \n", &Points[N][0], &Points[N][1]);
		fscanf(inF, "%f %f %f \n", &Clusters[N][0], &Clusters[N][1], &Clusters[N][2]);
		N++;
	}

	int r = 0, g = 0, b = 0;

	ClustBusters(Clusters, &r, &g, &b,N);
	Point x1,x2,x3;
	for (int i = 0; i < N; i++)
	{
		if (Clusters[i][g] > SenWall)
		{
			x1.x = Points[i][0];
			x1.x = Points[i][1];
		}
		if (Clusters[i][b] > SenWall)
		{
			x2.x = Points[i][0];
			x2.x = Points[i][1];
		}
		if (Clusters[i][r] > SenWall)
		{
			x3.x = Points[i][0];
			x3.x = Points[i][1];
		}
	}
	if (evklidDistance3D(x1,x3) > evklidDistance3D(x1, x2))
	{
		int x = b;
		b = g;
		g = x;
	}
	float CenterX = 0, CenterY = 0;
	int k = 0;
	
	for (int i = 0; i < N; i++)
	{
		if (Clusters[i][r] > SenWall)
		{
			CenterX += Points[i][0];
			CenterY += Points[i][1];
			k++;
		}
	}
	CenterX = CenterX / k;
	CenterY = CenterY / k;
	
	double Edge = 0;
	Point A, B;
	A.x = CenterX;
	A.y = CenterY;
	int c = 0;
	double Wall = 50000;
	while (c == 0)
	{
		Edge = 0;
		for (int i = 0; i < N; i++)
		{
			if (Clusters[i][r] > SenWall)
			{
				B.x = Points[i][0];
				B.y = Points[i][1];
				if (Edge == 0 && evklidDistance3D(A, B) < Wall)
				{
					Edge = evklidDistance3D(A, B);
				}
				else
				{
					if (Edge < evklidDistance3D(A, B) && evklidDistance3D(A, B) < Wall)
					{
						Edge = evklidDistance3D(A, B);
					}
				}
			}
		}
		c++;
		for (int i = 0; i < N; i++)
		{
			if (Clusters[i][r] < 0.8 && c != 0)
			{
				B.x = Points[i][0];
				B.y = Points[i][1];
				if (Edge > evklidDistance3D(A, B))
				{
					c = 0;
					Wall = Edge;
				}
			}
		}
	}
	double EdgeG = 0;
	for (int i = 0; i < N; i++)
	{
		if (Clusters[i][g] > SenWall)
		{
			B.x = Points[i][0];
			B.y = Points[i][1];
			if (EdgeG == 0)
			{
				EdgeG = evklidDistance3D(A, B);
			}
			else
			{
				if (EdgeG > evklidDistance3D(A, B))
				{
					EdgeG = evklidDistance3D(A, B);
				}
			}
		}
	}
	
	double EdgeB = 0;
	for (int i = 0; i < N; i++)
	{
		if (Clusters[i][b] > SenWall)
		{
			B.x = Points[i][0];
			B.y = Points[i][1];
			if (EdgeB == 0)
			{
				EdgeB = evklidDistance3D(A, B);
			}
			else
			{
				if (EdgeB > evklidDistance3D(A, B))
				{
					EdgeB = evklidDistance3D(A, B);
				}
			}
		}
	}
	FILE * outF = fopen("EdgeF.txt", "w");
	fprintf(outF, "%f %f \n", CenterX, CenterY);
	fprintf(outF, "%f %f %f \n", Edge, EdgeG, EdgeB);
	fclose(outF);
}

double **vPoint(int *N0,int *M0)
{
	float ** PointClu = (float**)malloc(sizeof(float) * 7000);
	for (int i = 0; i < 7000; i++)
	{
		PointClu[i] = (float*)malloc(sizeof(float) * 5);
	}

	FILE * inF = fopen("ClustF.txt", "rb");
	FILE * inF2 = fopen("Points2D.txt", "rb");

	int N = 0;

	while (!feof(inF2))
	{
		fscanf(inF2, "%f %f \n", &PointClu[N][0], &PointClu[N][1]);
		fscanf(inF, "%f %f %f \n", &PointClu[N][2], &PointClu[N][3], &PointClu[N][4]);
		N++;
	}
	*N0 = N;
	FILE * inF0 = fopen("winFile2.txt", "rb");
	float ** dumpData = (float**)malloc(sizeof(float*) * 3500);
	for (int i = 0; i < 3500; i++)
	{
		dumpData[i] = (float*)malloc(sizeof(float) * 7);
	}
	int M = 0;
	while (!feof(inF0))
	{
		int d1, d2, d3, d4, d5;
		float f1, f2;
		fscanf(inF0, "%f %f %f %f %f %f %f \n", &dumpData[M][0], &dumpData[M][1], &dumpData[M][2], &dumpData[M][3], &dumpData[M][4], &dumpData[M][5], &dumpData[M][6]);
		M++;
	}
	*M0 = M;
	fclose(inF0);
	float ** Vec = (float**)malloc(sizeof(float*) * 2);
	Vec[0] = (float*)malloc(sizeof(float) * 7);
	Vec[1] = (float*)malloc(sizeof(float) * 7);

	FILE * inF3 = fopen("M2Vec.txt", "rb");

	fscanf(inF3, "%f %f %f %f %f %f %f \n", &Vec[0][0], &Vec[0][1], &Vec[0][2], &Vec[0][3], &Vec[0][4], &Vec[0][5], &Vec[0][6]);
	fscanf(inF3, "%f %f %f %f %f %f %f \n", &Vec[1][0], &Vec[1][1], &Vec[1][2], &Vec[1][3], &Vec[1][4], &Vec[1][5], &Vec[1][6]);

	fclose(inF3);
	double sumX, sumY;
	for (int i = N; i < N + M; i++)
	{
		sumX = 0;
		sumY = 0;
		for (int j = 0; j < 7; j++)
		{
			sumX += Vec[0][j] * dumpData[i-N][j];
			sumY += Vec[1][j] * dumpData[i-N][j];
		}
		PointClu[i][0] = sumX;
		PointClu[i][1] = sumY;
	}
	
	return PointClu;
}