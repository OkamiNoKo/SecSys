#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Rewrotter.h"
#include "Clusterisation.h"

// Sens - Sencitive from 0 to 1 , re_F = 1 if that Dump is new
void Analisys(char* Dump, double Sens,int re_F)
{
	if (re_F == 1)
	{
		reFile(Dump, "reFile1.txt");
	}
	winReW("reFile1.txt", "winFile.txt","192.168.1.149");
	ChnCreator("winFile.txt", "winFile2.txt");
	FILE * inF = fopen("winFile2.txt", "rb");
	float ** dumpData = (float**)malloc(sizeof(float*) * 3500);
	for (int i = 0; i < 3500; i++)
	{
		dumpData[i] = (float*)malloc(sizeof(float) * 7);
	}
	int *  tr0 = (int*)malloc(sizeof(int) * 3500) ,* tr1 = (int*)malloc(sizeof(int) * 3500);
	int N = 0;
	while (!feof(inF))
	{
		int d1, d2, d3, d4, d5;
		float f1, f2;
		fscanf(inF, "%f %f %f %f %f %f %f \n", &dumpData[N][0], &dumpData[N][1], &dumpData[N][2], &dumpData[N][3], &dumpData[N][4], &dumpData[N][5], &dumpData[N][6]);
		N++;
	}
	fclose(inF);
	

	float ** dumpData2 = (float**)malloc(sizeof(float*) * 3500);
	for (int i = 0; i < 3500; i++)
	{
		dumpData2[i] = (float*)malloc(sizeof(float) * 7);
	}

	FILE * inF1 = fopen("winFile.txt", "rb");
	N = 0;
	while (!feof(inF1))
	{
		int d1, d2, d3, d4, d5;
		float f1, f2;
		fscanf(inF1, "winStart(sec): %d, winEnd(sec): %d, packCount: %d , mostActC: %d, S-SA: %d, avSize: %f, avTimeMostAct: %f, F-S: %d , susComb: %d \n", &tr0[N], &tr1[N], &d1, &d2, &d3, &f1, &f2, &d4, &d5);
		dumpData2[N][0] = d1; dumpData2[N][1] = d2; dumpData2[N][2] = d3; dumpData2[N][3] = f1; dumpData2[N][4] = f2; dumpData2[N][5] = d4; dumpData2[N][6] = d5;
		N++;
	}
	fclose(inF1);

	float CenterX = 0, CenterY = 0;
	float EdgeR = 0, EdgeG = 0, EdgeB = 0;
	FILE * inF2 = fopen("EdgeF.txt", "rb");

	fscanf(inF2, "%f %f \n", &CenterX, &CenterY);
	fscanf(inF2, "%f %f %f \n", &EdgeR, &EdgeB, &EdgeG );

	fclose(inF2);

	float ** Vec = (float**)malloc(sizeof(float*) * 2);
	Vec[0] = (float*)malloc(sizeof(float) * 7);
    Vec[1] = (float*)malloc(sizeof(float) * 7);

	FILE * inF3 = fopen("M2Vec.txt", "rb");

	fscanf(inF3, "%f %f %f %f %f %f %f \n", &Vec[0][0], &Vec[0][1], &Vec[0][2], &Vec[0][3], &Vec[0][4], &Vec[0][5], &Vec[0][6]);
	fscanf(inF3, "%f %f %f %f %f %f %f \n", &Vec[1][0], &Vec[1][1], &Vec[1][2], &Vec[1][3], &Vec[1][4], &Vec[1][5], &Vec[1][6]);

	fclose(inF3);

	Point * Points = (Point*)malloc(sizeof(Point) * N);

	float sumX, sumY;
	for (int i = 0; i < N ; i++)
	{
		sumX = 0;
		sumY = 0;
		for (int j = 0; j < 7; j++)
		{
			sumX += Vec[0][j] * dumpData[i][j];
			sumY += Vec[1][j] * dumpData[i][j];
		}
		Points[i].x = sumX;
		Points[i].y = sumY;
	}
	Point Cent;
	Cent.x = CenterX;
	Cent.y = CenterY;

	double Dis;
	double prob;
	FILE * outF = fopen("Log.txt", "w");
	for (int i = 0; i < N; i++)
	{
		Dis = evklidDistance3D(Cent, Points[i]);
		if (Dis < EdgeR)
		{
			prob = 0.25 - 0.25 * ((EdgeR - Dis) / EdgeR);
		}
		else
		{
			if (Dis < EdgeG)
			{
				prob = 0.52 +  0.51 - 0.51 * ((EdgeG - Dis) / EdgeG);
			}
			else
			{
				if (Dis < EdgeB)
				{
					prob = 0.8 + 0.21 - 0.21 * ((EdgeB - Dis) / EdgeB);
				}
				else
				{
					prob = 0.92 + 0.1 * ((EdgeB - Dis) / EdgeB);
				}
			}
		}
		if (prob >= 1.00)
		{
			prob = 0.9854;
		}
		if (prob > Sens)
		{
			fprintf(outF, "                   *** \n startSec: %d, endSec: %d, PackNum: %d, Probability: %f \n packCount: %f , mostActC: %f, S-SA: %f, avSize: %f, avTimeMostAct: %f, F-S: %f , susComb: %f \n ", tr0[i], tr1[i], N, prob, dumpData2[i][0], dumpData2[i][1], dumpData2[i][2], dumpData2[i][3], dumpData2[i][4], dumpData2[i][5], dumpData2[i][6]);
		}
	}
	fclose(outF);
}
