#include "stdafx.h"
#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<chrono>
#include<Windows.h>
#include<fstream>
using namespace std;

////////// STRUCT //////////

typedef struct {
	int w, s; // wartoĹ›Ä‡ i ciÄ™ĹĽar
	float ws; // wartoĹ›Ä‡/ciÄ™ĹĽar
} paczka;

////////// SORT //////////

void sort_smin(paczka p[], int m, int n) { // poczÄ…tek i koniec sortowanego ciÄ…gu

	int i = m, j = n, piv;
	piv = p[i].s;					//obierz pierwszy element za pivot
	do {
		while (p[i].s < piv) i++;	//szukaj elementu >=pivot od prawej strony
		while (p[j].s > piv) j--;	//szukaj elementu <=pivot od lewej strony	
		if (i <= j) swap(p[i++], p[j--]);		//zamieĂ± nieuporzÄ…dkowane elementy	
	} while (i<j);				//powtarzaj pĂłki wskaĹşniki sie nie minÄ…
	if (m<j) sort_smin(p, m, j);    //sortuj lewÄ… czÄ™Ĺ›Ä‡	
	if (i<n) sort_smin(p, i, n);	//sortuj prawÄ… czÄ™Ĺ›Ä‡	
}

void sort_wmax(paczka p[], int m, int n) {

	int i = m, j = n, piv;
	piv = p[i].w;
	do {
		while (p[i].w > piv) i++;
		while (p[j].w < piv) j--;
		if (i <= j) swap(p[i++], p[j--]);
	} while (i<j);
	if (m<j) sort_wmax(p, m, j);
	if (i<n) sort_wmax(p, i, n);
}

void sort_wsmax(paczka p[], int m, int n) {

	int i = m, j = n;
	float piv = p[i].ws;
	do {
		while (p[i].ws > piv) i++;
		while (p[j].ws < piv) j--;
		if (i <= j) swap(p[i++], p[j--]);
	} while (i<j);
	if (m<j) sort_wsmax(p, m, j);
	if (i<n) sort_wsmax(p, i, n);
}

int maks = 0;
int bgen(int n, int ssum, int wsum, int b, paczka *p) {

	if (n<-1) return 0;

	if (ssum + p[n].s <= b) {
		//		printf(" wybieram %d %d %d\n", n, p[n].s, p[n].w);  
		bgen(n - 1, ssum + p[n].s, wsum + p[n].w, b, p);
		if (wsum>maks) { maks = wsum; }
	}	 	// wybierz, jeĹĽeli nie przekroczy pojemnoĹ›ci
	bgen(n - 1, ssum, wsum, b, p); 										// nie wybieraj, przejdĹş do nastÄ™pnego elementu;
	if (wsum>maks) { maks = wsum; }

}


int main()
{
	chrono::time_point<std::chrono::high_resolution_clock> start;
	chrono::time_point<std::chrono::high_resolution_clock> end;
	chrono::duration<double, std::milli> sec;
	fstream plik;
	plik.open("Wyniki.txt", ios::out| ios::trunc);
	for (int xsr = 0; xsr <= 1; xsr++)
	{
		int s, k ,p;
		if (!xsr)
		{
			s = 2;
			k = 24;
			p = 2;
		}
		else
		{
			s = 50;
			k = 500;
			p = 50;
		}
		for (int r = 0; r < 3; r++)
		{
			double d;
			if (!r) d = 0.5;//	procent sumy wag
			if (r == 1) d = 0.25;
			if (r == 2) d = 0.75;
			plik << "d = " << d << endl;
			if (!xsr)plik << "n\tPD\tBF_1\tBF_2\tGH_4\n";
			else plik << "n\tGH_1\tGH_2\tGH_3\tGH_4\n";
			for (int n = s; n <= k; n+=p)
			{
				plik << n << "\t";
				paczka *p = new paczka[n]; // tworzenie tablicy paczek
				srand(time(0));
				for (int i = 0; i < n; i++) {
					p[i].s = (rand() % 991) + 10;// s(ai)=<10,1000>
					p[i].w = (rand() % 9901) + 100;// w(ai)=[100,10 000>
					//		printf("%d: s = %d, w = %d\n",i,p[i].s,p[i].w);		
				}
				// b = suma(s(ai))*d
				int b = 0;
				for (int x = 0; x < n; x++) b += p[x].s; // liczenie sumy wag

				b = b*d;			// liczenie pojemnoĹ›ci plecaka
				printf("pojemnosc = %d\n", b);
				cout << "d = " << d << " ; n = " << n;

				// Progrmowanie dynamiczne (PD)

				start = std::chrono::high_resolution_clock::now();

				// tworzenie tabeli (b+1)*(n+1)	
				int **tab = new int*[b + 1];
				for (int i = 0; i <= b; i++)
				{
					tab[i] = new int[n + 1];
				}
				for (int i = 0; i <= b; i++)
				{
					for (int j = 0; j <= n; j++)
					{
						tab[i][j] = 0;
					}
				}

				// wypeĹ‚nianie jej
				for (int x = 1; x <= b; x++) {
					for (int y = 1; y <= n; y++) {
						if (p[y - 1].s > x)
						{
							tab[x][y] = tab[x][y - 1];
							continue;
						} 	// przedmiot siÄ™ nie mieĹ›ci? przepisz wartoĹ›Ä‡ z gĂłry
						tab[x][y] = max(tab[x][y - 1], tab[x - p[y - 1].s][y - 1] + p[y - 1].w);// wĹ‚ĂłĹĽ przedmiot, jeĹ›li jest to opĹ‚acalne

					}
				}

				// znalezienie wybranych elementĂłw
				int x = b, y = n;
				while (y) {
					if (tab[x][y] == tab[x][y - 1]) {} // printf("%d not chosen\n", y-1); 
					else { x -= p[y - 1].s; }
					y--;
				}
				end = std::chrono::high_resolution_clock::now();
				sec = end - start;
				printf("PD: %d \n", tab[b][n]);
				cout << sec.count() << endl;	// wypisujemy sumÄ™ wartoĹ›ci wybranych przedmiotĂłw
				if(!xsr)plik << sec.count() << "\t";
				// PeĹ‚en przeglÄ…d (BF1)
				int ssum, wsum, wmax = 0, smax = 0;
				if (!xsr)
				{
					start = std::chrono::high_resolution_clock::now();

					
					for (x = 1; x < (1 << n); x++) { // dla wszystkich moĹĽliwych ukĹ‚adĂłw n zer i jedynek != od 0
						ssum = wsum = 0;
						for (int t = 0; t < n; t++) { // jeĹ›li t-ety bit od koĹ„ca x == 1 to dodajemy t-etÄ… paczkÄ™ do rozwiÄ…zania
							if ((x >> t) & 1) { ssum += p[t].s; wsum += p[t].w; }
						}
						if ((wsum > wmax) && (ssum <= b)) { wmax = wsum; smax = x; }
					}

					end = std::chrono::high_resolution_clock::now();
					sec = end - start;
					printf("BF1: %d \n", wmax);
					cout << sec.count() << endl;	// wypisujemy sumÄ™ wartoĹ›ci wybranych przedmiotĂłw
					if (!xsr)plik << sec.count() << "\t";

					// PeĹ‚en przeglÄ…d z eliminacjÄ… rozwiÄ…zaĹ„ niedopuszczalnych (BF2)	

					start = std::chrono::high_resolution_clock::now();
					maks = 0;
					bgen(n - 1, 0, 0, b, p);


					end = std::chrono::high_resolution_clock::now();
					sec = end - start;
					printf("BF2: %d \n", maks);
					cout << sec.count() << endl;
					if (!xsr)plik << sec.count() << "\t";
				}
				// Heurystyka - losowo (GH1)
				int sleft = b; // indeks
				if (xsr)
				{
					wsum = 0;

					start = std::chrono::high_resolution_clock::now();

					
					wsum = 0;

					for (int i = 0; i < n; i++) {
						if (p[i].s <= sleft) {
							sleft -= p[i].s;
							wsum += p[i].w;
							//			printf("chosen %d\n", i);	
						}
					}

					end = std::chrono::high_resolution_clock::now();
					sec = end - start;
					printf("GH1: %d ", wsum);
					float xgh1 = ((static_cast<float>(tab[b][n]) - static_cast<float>(wsum)) / static_cast<float>(tab[b][n])) * 100;
					printf("(blad: %.2f%%) ", xgh1);
					cout << sec.count() << endl;
					plik << xgh1 << "\t";

					// Heurystyka - minimalny ciÄ™ĹĽar (GH2)

					start = std::chrono::high_resolution_clock::now();

					sort_smin(p, 0, n - 1);
					sleft = b;
					wsum = 0;
					//	for (int i=0; i<n; i++) printf("%d: s=%d w=%d\n", i, p[i].s, p[i].w);	

					for (int i = 0; i < n; i++) {
						if (p[i].s > sleft) break; // jeĹ›li najmniejszy element siÄ™ nie mieĹ›ci
						sleft -= p[i].s;
						wsum += p[i].w;
						//		printf("chosen %d\n", i);
					}

					end = std::chrono::high_resolution_clock::now();
					sec = end - start;
					printf("GH2: %d ", wsum);
					float xgh2 = ((static_cast<float>(tab[b][n]) - static_cast<float>(wsum)) / static_cast<float>(tab[b][n])) * 100;
					printf("(blad: %.2f%%) ", xgh2);
					cout << sec.count() << endl;
					plik << xgh2 << "\t";

					// Heurystyka - maksymalna wartoĹ›Ä‡ (GH3)

					start = std::chrono::high_resolution_clock::now();

					sort_wmax(p, 0, n - 1);
					sleft = b;
					wsum = 0;
					//	for (int i=0; i<n; i++) printf("%d: s=%d w=%d\n", i, p[i].s, p[i].w);

					for (int i = 0; i < n; i++) {
						if (p[i].s <= sleft) {
							sleft -= p[i].s;
							wsum += p[i].w;
							//			printf("chosen %d\n", i);
						}
					}

					end = std::chrono::high_resolution_clock::now();
					printf("GH3: %d ", wsum);
					float xgh3 = ((static_cast<float>(tab[b][n]) - static_cast<float>(wsum)) / static_cast<float>(tab[b][n])) * 100;
					printf("(blad: %.2f%%) ", xgh3);
					sec = end - start;
					cout << sec.count() << endl;
					plik << xgh3 << "\t";

					// Heurystyka - maksymalna wartoĹ›Ä‡/ciÄ™ĹĽar (GH4)
				}
				start = std::chrono::high_resolution_clock::now();

				for (int i = 0; i < n; i++) p[i].ws = static_cast<float>(p[i].w) / static_cast<float>(p[i].s);	// obliczanie w/s
				sort_wsmax(p, 0, n - 1);
				//	for (int i=0; i<n; i++) printf("%d: s=%d w=%d ws=%.6f\n", i, p[i].s, p[i].w, p[i].ws);
				sleft = b;
				wsum = 0;

				for (int i = 0; i < n; i++) {
					if (p[i].s <= sleft) {
						sleft -= p[i].s;
						wsum += p[i].w;
						//			printf("chosen %d\n", i);
					}
				}

				end = std::chrono::high_resolution_clock::now();
				printf("GH4: %d ", wsum);
				float xgh4 = ((static_cast<float>(tab[b][n]) - wsum) / static_cast<float>(tab[b][n])) * 100;
				if (xsr)plik << xgh4 << "\t\n";
				printf("(blad: %.2f%%) ", xgh4);
				sec = end - start;
				cout << sec.count() << endl;
				if (!xsr)plik << sec.count() << "\t\n";
				for (int i = 0; i <= b; i++)
					delete[] tab[i];

				delete[] tab;
			}
		}
	}
	plik.close();
	return 0;
}

