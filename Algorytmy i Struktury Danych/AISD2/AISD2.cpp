#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <fstream>
#include <random>
#include <conio.h>
using namespace std;
fstream plik;
chrono::duration<double> czas[1000];
int W[10] = { 0 };
int cz = 0;

struct lista
{
	int key;   
	lista *nast;
};
struct node
{
	int klucz;
	node *lewy, *prawy;
};
node *stworz_BST(int tab[], int n) // tablica z liczbami, liczba elementow tablicy
{
	node *korzen = NULL, *dodaj, *obecny, *rodzic;

	for (unsigned int i = 0; i < n; i++) // dodawanie wierzcholka
	{
		dodaj = new node();
		dodaj->klucz = tab[i];
		dodaj->lewy = dodaj->prawy = NULL; // utworzenie nowego elementu

		if (korzen == NULL) // jesli korzen pusty
			korzen = dodaj; // dodajemy elemenet do korzenia;
		else
		{
			obecny = rodzic = korzen;
			while (obecny != NULL) // dopoki nie dotrzemy do konca drzewa
			{
				rodzic = obecny;
				if ((dodaj->klucz) < (obecny->klucz)) // idziemy do prawego poddrzewa
					obecny = obecny->lewy;
				else obecny = obecny->prawy; // idziemy do lewego poddrzewa
			}
			//rodzic - czyli ostatni wezel ktory jest niepusty
			if ((dodaj->klucz) < (rodzic->klucz))
				rodzic->lewy = dodaj;  //dodajemy po lewej

			else rodzic->prawy = dodaj;  //dodajemy po prawej 
		}
	}
	return korzen; //zwraca wskaznik do korzenia drzewa;
}
void szukaj_BST(node *korzen, int k) // wskaznik na korzen, szukany klucz
{
	while ((korzen->klucz != k)) // && (korzen != NULL)) {
	{
		if (k < (korzen->klucz))
			korzen = korzen->lewy; //idziemy w lewo
		else korzen = korzen->prawy; //idziemy w prawo
	}

}
int wysokosc(node *korzen)
{
	if (korzen == NULL) return 0;
	else return (1 + max(wysokosc(korzen->lewy), wysokosc(korzen->prawy)));
}
void liczby(int x)
{
	if (x == 1) plik.open("10.txt", ios::in | ios::out);
	if (x == 2) plik.open("20.txt", ios::in | ios::out);
	if (x == 3) plik.open("30.txt", ios::in | ios::out);
	if (x == 4) plik.open("40.txt", ios::in | ios::out);
	if (x == 5) plik.open("50.txt", ios::in | ios::out);
	if (x == 6) plik.open("60.txt", ios::in | ios::out);
	if (x == 7) plik.open("70.txt", ios::in | ios::out);
	if (x == 8) plik.open("80.txt", ios::in | ios::out);
	if (x == 9) plik.open("90.txt", ios::in | ios::out);
	if (x == 10) plik.open("100.txt", ios::in | ios::out);
}
void quick_sort_sr(int tab[], int i, int j)
{
	int lewy = i--, prawy = j++;
	int piwot = tab[(i + j) / 2], tym;
	while (j >= i)
	{
		while (tab[++i] < piwot);
		while (tab[--j] > piwot);
		if (j >= i)
		{
			tym = tab[i];
			tab[i] = tab[j];
			tab[j] = tym;
		}
	}
	if (lewy<j)quick_sort_sr(tab, lewy, j);
	if (prawy>i)quick_sort_sr(tab, i, prawy);
}
void generator()
{
	for (int x = 1; x <= 10; x++)
	{
		liczby(x);
		int n = 1000 * x;
		int *tab = new int[n];
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(1, n);
		for (int  i = 0; i < n; i++)
		{
			if(i==0)tab[i] = dis(gen);
			else while (1)
			{
				int pr = 1;
				tab[i] = dis(gen);
				for (int j = i-1; j >= 0; j--)
				{
					if (tab[i] == tab[j])
					{
						pr = 0;
						break;
					}
				}
				if (pr)break;
			}
		}
		for (int i = 0; i < n; i++)
		{
			plik << tab[i] << endl;
		}
		delete[] tab;
		plik.close();
	}
}
void cB(int tabA[], int tabB[], int n)
{
	auto start = chrono::system_clock::now();
	for (int j = 0; j < n; j++)
	{
		tabB[j] = tabA[j];
	}
	quick_sort_sr(tabB, 0, n - 1);
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	if(W[0])cout << "Dla tablicy o " << n << " roznych elementach czas cB wyniosl: " << elapsed_seconds.count() << "s\n";
	czas[cz++] = elapsed_seconds;
}
void SsB(int tabA[], int tabB[], int n)
{
	auto start = chrono::system_clock::now();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n&&tabA[i] != tabB[j]; j++);
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	if (W[1])cout << "Dla tablicy o " << n << " roznych elementach czas SsB wyniosl: " << elapsed_seconds.count() << "s\n";
	czas[cz++] = elapsed_seconds;
}
void SbB(int tabA[], int tabB[], int n)
{
	int sr, l, p;
	auto start = chrono::system_clock::now();
	for (int i = 0; i < n; i++)
	{
		l = 0, p = n - 1;
		while (l <= p)
		{
			sr = (l + p) / 2;
			if (tabB[sr] == tabA[i])break;
			if (tabB[sr] > tabA[i])p = sr - 1;
			else l = sr + 1;
		}
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	if (W[2])cout << "Dla tablicy o " << n << " roznych elementach czas SbB wyniosl: " << elapsed_seconds.count() << "s\n";
	czas[cz++] = elapsed_seconds;
}
void STR(int tabA[], int n)
{
	node *root = stworz_BST(tabA, n);
	auto start = chrono::system_clock::now();
	for (int i = 0; i < n; i++)
	{
		szukaj_BST(root, tabA[i]);
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	if (W[6])cout << "Dla tablicy o " << n << " roznych elementach czas STR wyniosl: " << elapsed_seconds.count() << "s\n";
	czas[cz++] = elapsed_seconds;
}
void STB(int tabA[], int tabC[], int n)
{
	node *root = stworz_BST(tabC, n);
	auto start = chrono::system_clock::now();
	for (int i = 0; i < n; i++)
	{
		szukaj_BST(root, tabA[i]);
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	if (W[7])cout << "Dla tablicy o " << n << " roznych elementach czas STB wyniosl: " << elapsed_seconds.count() << "s\n";
	czas[cz++] = elapsed_seconds;
}
void LC_SL(int tabA[], int n)
{
	auto start = chrono::system_clock::now();
	lista *first = NULL, *now = NULL, *last = NULL;
	for (int i = 0; i < n; i++)
	{

		now = new lista();
		now->key = tabA[i];
		if (first == NULL)
		{
			first = now;
			last = first;
		}
		else
		{
			last->nast = now;
			last = now;
		}
	}
	last = NULL;
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	if (W[3])cout << "Dla tablicy o " << n << " roznych elementach LC wynioslo: " << elapsed_seconds.count() << "s\n";
	czas[cz++] = elapsed_seconds;
	start = chrono::system_clock::now();
	for (int i = 0; i<n; i++)
	{
		while (last != NULL && last->key != tabA[i])
		{
			last = last->nast;
		}
		last = first;
	}
	end = chrono::system_clock::now();
	elapsed_seconds = end - start;
	if (W[4])cout << "Dla tablicy o " << n << " roznych elementach SL wynioslo: " << elapsed_seconds.count() << "s\n";
	czas[cz++] = elapsed_seconds;
}
void przepisz_polowkowo(int tab1[], int tab2[], int begin, int end, int i)
{ 
	int sr = (begin + end) / 2;
	tab2[i] = tab1[sr];
	if (sr>begin)
		przepisz_polowkowo(tab1, tab2, begin, sr - 1, i + 1);
	if (sr<end)
		przepisz_polowkowo(tab1, tab2, sr + 1, end, i + sr - begin + 1);
}

int main()
{
	//generator();//generowanie wartości losowych do plików .txt zakres od 1 tys do 10tys co 1 tys
	int H[20], h; //H-tablica zapisuje wartosci wysokosci do pozniejszego zapisania do pliku, h-ustala czy dokonany zostanie pomiar wysokosci
	cout << "WYBRANE ZLOZONE STRUKTURY DANYCH\n[1] Pomiar czasow\n[2] Pomiar wysokosci\n";
	char w = _getch();
	if (w == '1')//wyswietlanie pomiarow czasu w konsoli
	{
		h = 0;
		system("cls");
		while (1)
		{
			cout << "[1] Wyswietl cB\n[2] Wyswietl SsB\n[3] Wyswietl SbB\n[4] Wyswietl CL\n[5] Wyswietl SL\n[6] Wyswietl cTR\n[7] Wyswietl cTB\n[8] Wyswietl sTB\n[9] To wszystko\n";
			char w = _getch();
			if (w == '1')W[0] = 1;
			if (w == '2')W[1] = 1;
			if (w == '3')W[2] = 1;
			if (w == '4')W[3] = 1;
			if (w == '5')W[4] = 1;
			if (w == '6')W[5] = 1;
			if (w == '7')W[6] = 1;
			if (w == '8')W[7] = 1;
			if (w == '9')break;
			system("cls");
		}
		system("cls");
	} 
	else if (w == '2') //wyswietlanie wysokosci w konsoli
	{
		h = 1;
		system("cls");
		while (1)
		{
			cout << "[1] Wyswietl TR\n[2] Wyswietl TB\n[3] To wszystko\n";
			char w = _getch();
			if(w=='1')W[8] = 1;
			if(w=='2')W[9] = 1;
			if (w == '3')break;
			system("cls");
		}
		system("cls");
	} 
	else exit(0);
	for (int i = 1; i <= 10; i++)
	{
		int n = i * 1000; //wielkosc tablicy
		int *tabA = new int[n];//tablica oryginalna
		int *tabB = new int[n];//tablica skopiowana i posortowana
		int *tabC = new int[n];//tablica pomocnicza - przepisana połówkowo tablica B
		liczby(i);
		for (int j = 0; j < n; j++)
		{
			plik >> tabA[j];//wczytywanie danych z pliku
		}
		cB(tabA, tabB, n);//tworzenie kopii i QS
		SsB(tabA, tabB, n);//przeszukiwanie tab liniowe
		SbB(tabA, tabB, n);//przeszukiwanie tab polowkowe
		auto start = chrono::system_clock::now();
		stworz_BST(tabA, n);//tworzenie drzewa TR
		auto end = chrono::system_clock::now();
		chrono::duration<double> elapsed_seconds = end - start;
		if (W[5])cout << "Dla tablicy o " << n << " roznych elementach cTR wynioslo: " << elapsed_seconds.count() << "s\n";
		czas[cz++] = elapsed_seconds;
		STR(tabA, n);//przeszukiwanie drzewa TR wg tablicy A
		LC_SL(tabA, n);//tworzenie listy i przeszukiwanie listy wg tablicy A
		przepisz_polowkowo(tabB, tabC, 0, n - 1, 0);
		start = chrono::system_clock::now();
		stworz_BST(tabC, n);//tworzenie drzewa TB wg tablicy C
		end = chrono::system_clock::now();
		elapsed_seconds = end - start;
		if (W[7])cout << "Dla tablicy o " << n << " roznych elementach cTB wynioslo: " << elapsed_seconds.count() << "s\n";
		czas[cz++] = elapsed_seconds;
		STB(tabA, tabC, n);//przeszukiwanie drzewa TB wg tablicy A
		if (h)H[2 * (i - 1)] = wysokosc(stworz_BST(tabA, n));//mierzenie wysokości drzewa TR
		if (h)H[2 * (i - 1) + 1] = wysokosc(stworz_BST(tabC, n));//mierzenie wysokości drzewa TB
		plik.close();
		delete[] tabA;
		delete[] tabB;
		delete[] tabC;
	}
	plik.open("Czasy.txt", ios::in | ios::out);
	plik << "cB\n";
	for (int i = 0; i < 90; i+=9)plik << czas[i].count() << endl;//zapisywanie czasów do pliku Czasy.txt
	plik << "SsB\n";
	for (int i = 1; i < 90; i+=9)plik << czas[i].count() << endl;
	plik << "SbB\n";
	for (int i = 2; i < 90; i+=9)plik << czas[i].count() << endl;
	plik << "cTR\n";
	for (int i = 3; i < 90; i+=9)plik << czas[i].count() << endl;
	plik << "STR\n";
	for (int i = 4; i < 90; i += 9)plik << czas[i].count() << endl;
	plik << "LC\n";
	for (int i = 5; i < 90; i += 9)plik << czas[i].count() << endl;
	plik << "SL\n";
	for (int i = 6; i < 90; i += 9)plik << czas[i].count() << endl;
	plik << "cTB\n";
	for (int i = 7; i < 90; i += 9)plik << czas[i].count() << endl;
	plik << "STB\n";
	for (int i = 8; i < 90; i += 9)plik << czas[i].count() << endl;
	plik.close();
	if (h)
	{
		plik.open("Wysokosc.txt", ios::in | ios::out);//zapisywanie wysokości do pliku Wysokość.txt
		plik << "hTR\n";
		int p = 1;
		for (int i = 0; i < 20; i += 2)
		{
			plik << H[i] << endl;
			if (W[8])cout << "Wysokosc drzewa TR zbudowanego z " << (p++) * 1000 << " elementow wynosi: " << H[i]<<endl;
		}
		plik << "hTB\n";
		p = 1;
		for (int i = 1; i < 20; i += 2)
		{
			plik << H[i] << endl;
			if (W[9])cout << "Wysokosc drzewa zbudowanego TB z " << (p++) * 1000 << " elementow wynosi: " << H[i] << endl;
		}
		plik.close();
	}
	cout << "Program zakonczyl swoje dzialanie. Dane zapisano do odpowiednich plikow.";
    return 0;
}
