#include<iostream>
#include<math.h>
#include<vector>
#include <signal.h>
#include <cstring>
using namespace std;

//bool *V;//tablica odwiedzenia danego miasta
int *S, *Sh;//stos miast optymalny/dla danego cyklu
float d; //odległość optymalna/dla danego cyklu
float **D; //macierz odległości
int n; //licznik dla stosów, liczba miast
int x0 = 0; //wierzchołek startow
bool flag;
//volatile sig_atomic_t tle = 0;

/*void term(int signum)
{
	tle = 1;
}*/

float distance(float x1, float x2, float y1, float y2)
{
	float x = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	x = sqrt(x);
	return x;
}

float route()
{
	float dh = 0;
	for (int i = 0; i < n; i++)dh += D[Sh[i]][Sh[i + 1]];
	return dh;
}

void exroute()
{
	for (int i = 0; i < n; i++)
	{
		Sh[i] = S[i] = i;
	}
	S[n] = Sh[n] = 0;
	d = route();
}

void OptSwap(int a, int b)
{
	int t = a;
	for (int i = 0; i < a; i++) Sh[i] = S[i];
	for (int i = b; i >= a; i--) Sh[t++] = S[i];
	for (int i = b + 1; i < n; i++) Sh[i] = S[i];
}

void LS()
{
	float td;
	flag = true;
	for (int i = 1; i < n - 1 && flag ; i++)
	{
		for (int k = i + 1; k < n && flag; k++)
		{
			OptSwap(i, k);
			td = route();
			if (td < d)
			{
				d = td;
				for (int j = 0; j < n; j++)S[j] = Sh[j];
				flag = false;
				LS();
			}
		}
	}
}

struct miasto
{
	int id;
	float x;
	float y;
};
int main()
{
	/*struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = term;
	sigaction(SIGTERM, &action, NULL);

	ios_base::sync_with_stdio(false);*/
	miasto pom;
	vector <miasto> M;
	while (cin >> pom.id >> pom.x >> pom.y)
	{
		M.push_back(pom);
	}
	n = M.size();
	S = new int[n + 1];
	Sh = new int[n + 1];
	//V = new bool[n];
	//for (int i = 0; i < n; i++)V[i] = false;
	D = new float *[n];
	for (int i = 0; i < n; i++)
	{
		D[i] = new float[n];
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			D[i][j] = distance(M[i].x, M[j].x, M[i].y, M[j].y);
		}
	}
	exroute();
	LS();
	for (int i = 0; i < n; i++)
	{
		cout << M[S[i]].id << endl;
	}
	/*for (int i = 0; i < n; i++)
	{
		delete[] D[i];
	}
	delete[] D, S, Sh; //V */
	return 0;
}