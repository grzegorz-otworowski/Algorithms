#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <chrono>
#include <Windows.h>
#include <fstream>

using namespace std;
int nr = 0;

struct lista
{ 
	int value;
	lista *next;
};

// SORTOWANIE TOPOLOGICZNE //

void topol(int* begin, int* end, bool* visited, int n, int v, lista **SL) {

	visited[v] = 1;
	begin[v] = ++nr; 
	lista *nastepnik = SL[v];
	while (nastepnik != NULL) {
		if (visited[nastepnik->value] == 0) topol(begin, end, visited, n, nastepnik->value, SL);
		nastepnik = nastepnik->next;
	}

	end[v] = ++nr; 

}

int main()
{
	fstream plik;
	plik.open("Wyniki.txt", ios::in | ios::out);
	for (double d = 0.2; d <= 0.4; d += 0.2)
	{
		cout << "Dane dla gestosci rownej " << d<<endl;
		if (d > 0.2)plik << endl;
		plik << "Dane dla gestosci rownej " << d << endl;
		int Luk[20], cz=0,l=0;
		chrono::duration<double> czas[200];
		for (int n = 200; n <= 2000; n += 200)
		{
			cout << "n = " << n << endl;
			int m = n*n*d, x, y; // n - liczba wierzcholkow, m - liczba krawedzi, x,y - zmienne pomocnicze
			srand(time(0));

			//MACIERZ SASIEDZTWA - TWORZENIE//

			int **MS = new int*[n];
			for (x = 0; x < n; x++)
			{
				MS[x] = new int[n];
			}
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					MS[i][j] = 0;
				}
			}

			int i = m;
			while (i) {
				x = rand() % n; 
				y = rand() % n;

				if (MS[x][y] != 1 && x != y && MS[y][x] != 1)
				{
					MS[x][y] = 1;
					i--;
				}
			}
			// MACIERZ SASIEDZTWA - WYSWIETLANIE //
			/*for (i = 0; i < n; i++)
			{
			for (int j = 0; j < n; j++)
			{
			cout << MS[i][j] << " ";
			}
			cout << endl;
			}*/

			// LISTA ŁUKÓW - TWORZENIE //

			int **L_lukow = new int*[2];
			L_lukow[0] = new int[m];
			L_lukow[1] = new int[m];

			for (i = 0, x = 0; x < n; x++) {
				for (y = 0; y < n; y++) {
					if (MS[x][y])
					{
						L_lukow[0][i] = x;
						L_lukow[1][i] = y;
						i++;
					}
				}
			}

			//LISTA ŁUKÓW - WYŚWIETLANIE //

			/*cout << endl;
			for (i = 0; i < 2; i++)
			{
			for (int j = 0; j < m; j++)
			cout << L_lukow[i][j]+1 << " ";
			cout << endl;
			}
			cout << endl;
			*/

			// LISTA NASTĘPNIKÓW - TWORZENIE //

			lista **lista_tab = new lista*[n];
			lista * prev;
			int p = 0;

			for (int i = 0; i < n; i++) {
				lista *x = new lista;
				prev = x;
				x->value = i;
				x->next = NULL;

				for (int j = 0; j < n; j++) {
					if (MS[i][j]) {
						lista *y = new lista;
						y->value = j;
						prev->next = y;
						prev = y;
						y->next = NULL;
					}
				}
				lista_tab[p] = x;
				p++;
			}

			// LISTA NASTĘPNIKÓW - WYŚWIETLANIE //

			/*for (int i = 0; i<n; i++) {
			lista *x;
			x = lista_tab[i];

			while (x) {
			printf("%2d -> ", 1 + x->value);
			x = x->next;
			}
			printf("\n");
			}
			printf("\n");*/

			// BADANIE ETYKIET CZASOWYCH - LISTA NASTĘPNIKÓW //

			bool *visited = new bool[n]; //stan odwiedzenia
			int *begin = new int[n]; //poczatek analizy
			int *end = new int[n]; //koniec analizy
			for (i = 0; i < n; i++)visited[i] = 0;
			auto czas_start = chrono::system_clock::now();
			for (x = 0; x < n; x++)
				if (visited[x] == 0)
				{
					//cout << "Wznowienie od " << x+1 << endl;
					topol(begin, end, visited, n, x, lista_tab);
				}
			auto czas_stop = chrono::system_clock::now();
			chrono::duration<double> elapsed_seconds = czas_stop - czas_start;
			cout << "Czas obliczania etykiet czasowych wyniosl: " << elapsed_seconds.count() << endl;
			czas[cz++] = elapsed_seconds;
			nr = 0;

			// ETYKIETY CZASOWE - WYSWIETLANIE // 

			//for (x = 0; x < n; x++) cout << "begin[" << x << "]=" << begin[x] << " end[" << x << "]=" << end[x]<<endl;

			// BADANIE LICZBY ŁUKÓW POWROTNYCH //

			czas_start = chrono::system_clock::now();
			int luki_powrotne = 0;
			for (x = 0; x < m; x++) 
			{
				if (begin[L_lukow[1][x]] < begin[L_lukow[0][x]] && end[L_lukow[0][x]] < end[L_lukow[1][x]] && begin[L_lukow[0][x]] < end[L_lukow[0][x]])
				{
					luki_powrotne++;
					//printf("Luk powrotny: %d, %d\n", L_lukow[0][x], L_lukow[1][x]);
				}
			}
			czas_stop = chrono::system_clock::now();
			elapsed_seconds = czas_stop - czas_start;
			cout << "Czas badania liczby lukow pow. dla listy lukow: " << elapsed_seconds.count() << endl;
			czas[cz++] = elapsed_seconds;
			czas_start = chrono::system_clock::now();
			//cout << "Liczba lukow powrotnych: " << luki_powrotne << endl;
			Luk[l++] = luki_powrotne;
			luki_powrotne = 0;
			for (x = 0; x<n; x++) {
				for (y = 0; y<n; y++) {
					if (MS[x][y]) {
						if (begin[y] < begin[x] && end[x] < end[y] && begin[x] < end[x])
						{
							luki_powrotne++;
							//printf("Luk powrotny: %d, %d\n", x, y);
						}
					}
				}
			}
			czas_stop = chrono::system_clock::now();
			elapsed_seconds = czas_stop - czas_start;
			cout << "Czas badania liczby lukow pow. dla macierzy: " << elapsed_seconds.count() << endl;
			czas[cz++] = elapsed_seconds;
			czas_start = chrono::system_clock::now();
			//cout << "Lukow powrotnych: " << luki_powrotne << endl;
			luki_powrotne = 0;
			for (int i = 0; i<n; i++)
			{
				lista *x;
				x = lista_tab[i];
				bool pom = 1;
				int X, Y;
				while (x) {
					if (pom) X = x->value;
					else Y = x->value;
					if (!pom&&begin[Y] < begin[X] && end[X] < end[Y] && begin[X] < end[X])
					{
						luki_powrotne++;
						//printf("Luk powrotny: %d, %d\n", x, y);
					}
					x = x->next;
					pom = 0;
				}
				pom = 1;
			}
			czas_stop = chrono::system_clock::now();
			elapsed_seconds = czas_stop - czas_start;
			cout << "Czas badania liczby lukow pow. dla listy nastepnikow: " << elapsed_seconds.count() << endl;
			czas[cz++] = elapsed_seconds;
			cout << "Liczba lukow powrotnych: " << luki_powrotne << endl;
			cout << endl;
		}
		
		plik << "Czas obliczania etykiet czasowych:\n";
		for (int i = 0; i < 40; i += 4)
		{
			plik << czas[i].count() << endl;
		}
		plik << "\nCzas badania liczby lukow powrotnych dla listy lukow:\n";
		for (int i = 1; i < 40; i += 4)
		{
			plik << czas[i].count() << endl;
		}
		plik << "\nCzas badania liczby lukow powrotnych dla macierzy sasiedztwa:\n";
		for (int i = 2; i < 40; i += 4)
		{
			plik << czas[i].count() << endl;
		}
		plik << "\nCzas badania liczby lukow powrotnych dla listy nastepnikow:\n";
		for (int i = 3; i < 40; i += 4)
		{
			plik << czas[i].count() << endl;
		}
		plik << "\nLiczba lukow powrotnych:\n";
		for (int i = 0; i < 10; i++)
		{
			plik << Luk[i] << endl;
		}
	}
	plik.close();
    return 0;
}

