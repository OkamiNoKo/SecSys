#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include "math.h"

#include "Ev.h"

int treatdump(EVENT* ev);

void reFile(char* infile, char* outfile);

void winReW(char* infile, char* outfile, char * c1);

void strEq(char * str1, char * str2);

int sEq(char * str1, char * str2);

void ChnCreator(char* infile, char * outfile);