#include "Rewrotter.h"
#include "Vec'n'Zn.h"

typedef struct pack {
	char addr[16];
	int sec;
	int usec;
	int size;
	int flag;
} pack;

typedef struct list {
	struct list* next;
	char * addr;
	int count;
} list;


list * nextDoor(list * root, char* addr)
{
	list * BNDoor = (list*)malloc(sizeof(list));
	char * nAddr = (char*)malloc(sizeof(char) * 16);
	strEq(addr,nAddr);
	BNDoor->count = 1;
	BNDoor->next = NULL;
	BNDoor->addr = nAddr;
	root->next = BNDoor;
}

void strEq(char * str1, char * str2)
{
	int i = 0;
	while (str1[i] != '\0')
	{
		str2[i] = str1[i];
		i++;
	}
	str2[i] = str1[i];
}

int sEq(char * str1, char * str2)
{
	int s = 0;
	if (str2[0] != str1[0])
	{
		s++;
	}
	int i = 1;
	while (str1[i - 1] != '\0' && str2[i - 1] != '\0' && s == 0)
	{
		if (str2[i] != str1[i])
		{
			s++;
		}
		i++;
	}
	return s;
}

int treatdump(EVENT* ev)
{
	int x = 0;
	if ((ev->ip).protocol == 0x06)
	{
		if ((ev->tcp).flags == 0x002)//Syn
			x = 1;
		if ((ev->tcp).flags == 0x012)//Syn-Ack
			x = 2;
		if ((ev->tcp).flags == 0x006)//Syn-Rst
			x = 3;
		if ((ev->tcp).flags == 0x003)//Syn-Fin
			x = 4;
		if ((ev->tcp).flags == 0x029)//Syn-Psh-Urg (X-Mas)
			x = 5;
		if ((ev->tcp).flags == 0x001)//Fin
			x = 6;
		if ((ev->tcp).flags == 0x011)//Fin-Ack
			x = 7;
		if ((ev->tcp).flags == 0x005)//Fin-Rst
			x = 8;
	}
	return x;
}

void reFile(char* infile, char* outfile)
{
	uint32_t  SIZE_LINK_HEADER;
	PCAPPKTHDR pkthdr;
	PCAPFILEHDR* pfilehdr;
	BYTE buffer[128];
	EVENT event;
	//Dump.pcap , reF
	FILE * dumpF = fopen(infile, "rb");
	FILE * outF = fopen(outfile, "w");


	fread(buffer, sizeof(PCAPFILEHDR), 1, dumpF);
	pfilehdr = (PCAPFILEHDR*)buffer;
	if (pfilehdr->magic != 0xa1b2c3d4)
	{
		fclose(dumpF);
		return;
	}
	switch (pfilehdr->linktype)
	{
	case 1:
		SIZE_LINK_HEADER = 14;
		break;
	default:
		fclose(dumpF);
		return;
	}

	int Sec = 0;
	int Usec = 0;
	int sSec,sUsec;
	int i = 0;
	while (1)
	{
		fread(&pkthdr, sizeof(PCAPPKTHDR), 1, dumpF);
		if (feof(dumpF))
		{
			break;
		}
		fread(buffer, SIZE_LINK_HEADER, 1, dumpF);
		fread(&event, pkthdr.caplen - SIZE_LINK_HEADER, 1, dumpF);
		i++;
		if (i == 2017)
		{
			i = 2020;
		}
		if (ntohs(*(uint16_t*)(buffer + SIZE_LINK_HEADER - 2)) == 0x0800)
		{
			int x = treatdump(&event);
			if (Sec == 0 && Usec == 0)
			{
				fprintf(outF, " sAddr: %s , sec: %d, usec: %d, size: %d, flag: %d \n", inet_ntoa(event.ip.saddr), 0, 0, pkthdr.len, x);
				Sec = pkthdr.ts.sec;
				Usec = pkthdr.ts.usec;
			}
			else
			{
				sSec = pkthdr.ts.sec - Sec;
				sUsec = pkthdr.ts.usec - Usec;
				if (sUsec < 0)
				{
					sSec -= 1;
					sUsec = 1000000 + sUsec;
				}
				fprintf(outF, " sAddr: %s , sec: %d, usec: %d, size: %d, flag: %d \n", inet_ntoa(event.ip.saddr), sSec, sUsec, pkthdr.len, x);
			}
	    }
	}
	fclose(dumpF);
	fclose(outF);
}



void winReW(char* infile, char* outfile, char* c1)
{
	//"reF.txt" "winF.txt"
	FILE * inF = fopen(infile, "rb");
	FILE * outF = fopen(outfile, "w");

	//c1 = "192.168.0.110";
	int winSize = 65, step = 10, k = 0, l = 0;
	char* addr = (char*)malloc(16*sizeof(char));
	int Sec = 0, Usec = 0, size = 0, flag = 0;
	pack* bBox = (pack*)malloc(sizeof(pack) * 35000);
	while (!feof(inF))
	{	
		while (Sec < winSize + k * step)
		{
			fscanf(inF, " sAddr: %s , sec: %d, usec: %d, size: %d, flag: %d \n", addr, &Sec, &Usec, &size, &flag);
			if (feof(inF))
			{
				break;
			}
			strEq(addr,bBox[l].addr);
			bBox[l].sec = Sec;
			bBox[l].usec = Usec;
			bBox[l].size = size;
			bBox[l].flag = flag;
			l++;
		}
		int Syn = 0,SynAck = 0,Fin = 0,Sus = 0;
		double mSize = 0;

		list * root = (list*)malloc(sizeof(list));
		list * pointer;

		for (int i = 0; i < l; i++)
		{
			if (i == 0)
			{
				char * nAddr = (char*)malloc(sizeof(char) * 16);
				strEq(bBox[i].addr, nAddr);
				root->count = 1;
				root->next = NULL;
				root->addr = nAddr;
			}
			else
			{
				pointer = root;
				int boo = 0;
				while (boo == 0 && pointer->next != NULL)
				{
					int r = sEq(pointer->addr,bBox[i].addr);
					if (r == 0)
					{
						boo = 1;
						pointer->count++;
					}
					pointer = pointer->next;
				}
				if (boo == 0)
				{
					int r = sEq(pointer->addr, bBox[i].addr);
					if (r == 0)
					{
						boo = 1;
						pointer->count++;
					}
					else
					{
						nextDoor(pointer , bBox[i].addr);
					}
				}
			}
			if (bBox[i].flag == 1)
			{
				Syn++;
			}
			if (bBox[i].flag == 2)
			{
				SynAck++;
			}
			if (bBox[i].flag == 6 || bBox[i].flag == 7)
			{
				Fin++;
			}
			if (bBox[i].flag == 3 || bBox[i].flag == 4 || bBox[i].flag == 5 || bBox[i].flag == 8)
			{
				Sus++;
			}
			mSize += bBox[i].size;
		}


		int MaxC = 0;

		char * addrActive;

		if (sEq(root->addr, c1) != 0)
		{
			pointer = root->next;
			MaxC = root->count;
			addrActive = root->addr;
		}
		else
		{
			pointer = root->next->next;
			MaxC = root->next->count;
			addrActive = root->next->addr;
		}
		while (pointer != NULL)
		{
			if (MaxC < pointer->count && sEq(pointer->addr, c1) != 0)
			{
				MaxC = pointer->count;
				addrActive = pointer->addr;
			}
			pointer = pointer->next;
		}


		double prevTime = 0, avTime = 0;
		for (int i = 0; i < l; i++)
		{
			if (sEq(bBox[i].addr, addrActive) == 0 && prevTime  < 0.00001)
			{
				prevTime = bBox[i].sec + ((double)bBox[i].usec) / 1000000;
	
			}
			if (sEq(bBox[i].addr, addrActive) == 0)
			{
				avTime += bBox[i].sec + ((double)bBox[i].usec) / 1000000 - prevTime;
				prevTime = bBox[i].sec + ((double)bBox[i].usec) / 1000000;

			}
		}

		avTime = avTime / MaxC;
		mSize = mSize / l;
		int SSA = Syn - SynAck;
		int FS = Fin - Syn;
		int winSt = k * step , winEnd = k * step + winSize;
		fprintf(outF, "winStart(sec): %d, winEnd(sec): %d, packCount: %d , mostActC: %d, S-SA: %d, avSize: %f, avTimeMostAct: %f, F-S: %d , susComb: %d \n", winSt, winEnd, l, MaxC, SSA, mSize, avTime, FS, Sus);
		k++;
		int m = 0;
		while (bBox[m].sec < k * step )
		{
			m++;
		}
		m++;
		for (int i = m; i < l; i++)
		{
			bBox[i - m] = bBox[i];
		}
		l = l - m;
	}
	fclose(inF);
	fclose(outF);
}


void ChnCreator(char * infile, char * outfile)
{
	//"winF.txt"
	FILE * inF = fopen(infile, "rb");
	float ** dumpData = (float**)malloc(sizeof(float*) * 3500);
	for (int i = 0; i < 3500; i++)
	{
		dumpData[i] = (float*)malloc(sizeof(float) * 7);
	}
	int tr0, tr1;
	int N = 0;
	while (!feof(inF))
	{
		int d1, d2, d3, d4, d5;
		float f1, f2;
		fscanf(inF, "winStart(sec): %d, winEnd(sec): %d, packCount: %d , mostActC: %d, S-SA: %d, avSize: %f, avTimeMostAct: %f, F-S: %d , susComb: %d \n", &tr0, &tr1,&d1, &d2, &d3, &f1, &f2, &d4, &d5);
		dumpData[N][0] = d1; dumpData[N][1] = d2; dumpData[N][2] = d3; dumpData[N][3] = f1; dumpData[N][4] = f2; dumpData[N][5] = d4; dumpData[N][6] = d5;
		N++;
	}
	///////////////////////////

	float ** Sig = (float**)malloc(sizeof(float) * 7);
	for (int i = 0; i < 7; i++)
	{
		Sig[i] = (float*)malloc(sizeof(float) * 7);
		Sig[i][i] = 1;
	}
	float * norVe = (float*)malloc(sizeof(float) * 7);
	for (int i = 0; i < 7; i++)
	{
		float sum = 0;
		for (int j = 0; j < N; j++)
		{
			sum += dumpData[j][i];
		}
		norVe[i] = sum / N;
	}

	float * dist = (float*)malloc(sizeof(float) * 7);
	for (int i = 0; i < 7; i++)
	{
		float sum = 0;
		for (int j = 0; j < N; j++)
		{
			sum += (dumpData[j][i] - norVe[i]) * (dumpData[j][i] - norVe[i]);
		}
		if (sum == 0)
		{
			sum = 0.0001;
		}
		dist[i] = sum / N;
	}

	double ** NewDum = (double**)malloc(sizeof(double) * N);
	for (int i = 0; i < N; i++)
	{
		NewDum[i] = (double*)malloc(sizeof(double) * 7);
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			NewDum[i][j] = (dumpData[i][j] - norVe[j]) / sqrt(dist[j]);
		}
	}

	FILE * outF = fopen(outfile, "w");
	for (int i = 0; i < N; i++)
	{
		fprintf(outF, "%f %f %f %f %f %f %f \n", NewDum[i][0], NewDum[i][1], NewDum[i][2], NewDum[i][3], NewDum[i][4], NewDum[i][5], NewDum[i][6]);
	}
	fclose(outF);

	for (int i = 0; i < 7; i++)
	{
		for (int j = i + 1; j < 7; j++)
		{
			float Sum = 0;
			for (int k = 0; k < N; k++)
			{
				Sum += NewDum[k][i] * NewDum[k][j];
			}
			Sig[i][j] = Sum / N;
			Sig[j][i] = Sum / N;
		}
	}
	matr(Sig,0.0000001,7);

	fclose(inF);
}