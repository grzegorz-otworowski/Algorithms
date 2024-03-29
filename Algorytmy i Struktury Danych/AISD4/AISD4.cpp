#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <set>
#include <fstream>
#include <vector>

struct AdjListNode
{
	int value;
	AdjListNode* next;
};

struct AdjList
{
	AdjListNode *head;
};

std::chrono::time_point<std::chrono::high_resolution_clock> start_h;
std::chrono::time_point<std::chrono::high_resolution_clock> end_h;
std::chrono::duration<double, std::milli> elapsed_h;

void generate_graph(bool **T, int n, int m);
void adjacency_list(AdjList * & adj, bool **T, int n);
void addEdge(AdjList * & adj, int src, int dest);
void print_T(bool **T, int n);
void print_adj(AdjList *adj, int n);
void selection_sort(int T[], int n);
bool deg(bool **T, int n);
void dfs_euler(bool **T, int n, int v, int *S, int & it);
void dfs_hamilton(AdjList *adj, int n, int *S, int & it, int v, bool *visited, int & count);
void dfs_hamilton_1(AdjList *adj, int n, int *S, int & it, int v, bool *visited, int & count, bool & stop);


int main()
{
	srand(time(NULL));

	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
	std::chrono::duration<double, std::milli> elapsed;
	std::fstream plik;
	plik.open("wyniki.txt", std::ios::in | std::ios::out);
	double d = 0.2;
	int n, m;
	for (d; d <= 0.7; d += 0.4)
	{
		for (int n = 11; n <= 20; n++)
		{
			if (n >= 20)for (n; n <= 70; n += 5)
			{

				std::cout << std::endl;
				m = (d*n*(n - 1)) / 2;

				//std::cout << "n = " << n << std::endl;
				//std::cout << "d = " << d << std::endl;
				//std::cout << "m = " << m << std::endl;

				// generowanie grafu z cyklem Eulera
				bool **T = new bool *[n];
				for (int i = 0; i < n; i++)
					T[i] = new bool[n];
				generate_graph(T, n, m);

				// tworzenie listy następników na podstawie macierzy sąsiedztwa
				AdjList *adj = new AdjList[n];
				for (int i = 0; i < n; i++)
					adj[i].head = NULL;
				adjacency_list(adj, T, n);

				/*
				std::cout << std::endl;
				print_T(T, n);
				std::cout << std::endl;
				print_adj(adj, n);
				std::cout << std::endl;
				*/
				plik << n<<"|"<< d << std::endl;
				// -----------> CYKL EULERA <----------- //
				int *S_euler = new int[m + 4];
				int it = 0;
				start = std::chrono::high_resolution_clock::now();
				dfs_euler(T, n, 0, S_euler, it);
				end = std::chrono::high_resolution_clock::now();
				elapsed = end - start;
				std::cout << "Czas wyszukiwania cyklu Eulera: " << elapsed.count() << " milisekund" << std::endl;
				plik << "E: " << elapsed.count() << "\n";
				/*
				std::cout << "Eulerian Cycle: ";
				for (int i = 0; i < it; i++) {
				std::cout << S_euler[i];
				if(i != it-1)
				std::cout << ", ";
				}
				*/

				std::cout << std::endl;

				int *S_hamilton = new int[n];
				bool *visited = new bool[n];
				for (int i = 0; i < n; i++) {
					visited[i] = false;
				}
				int it2 = 0;
				int count = 0;
				bool stop = false;

				// -----------> JEDEN CYKL HAMILTONA <----------- //
				start_h = std::chrono::high_resolution_clock::now();
				dfs_hamilton_1(adj, n, S_hamilton, it2, 0, visited, count, stop);
				plik << "H1: " << elapsed_h.count() << "\n";
				std::cout << std::endl;

				// -----------> WSZYSTKIE CYKLE HAMILTONA <----------- //
				if (n <= 15)
				{
					start = std::chrono::high_resolution_clock::now();
					dfs_hamilton(adj, n, S_hamilton, it2, 0, visited, count);
					end = std::chrono::high_resolution_clock::now();
					elapsed = end - start;
					plik << "HA: " << elapsed.count() << "\n";
					std::cout << "Czas wyszukiwania wszystkich cykli Hamiltona: " << elapsed.count() << " milisekund" << std::endl;
					std::cout << std::endl;
					std::cout << "Liczba wszystkich cykli Hamiltona: " << count / 2 << std::endl;
					plik << "CH: " << count / 2 << "\n";
					std::cout << std::endl;

				}
			}
			else
			{
				std::cout << std::endl;
				m = (d*n*(n - 1)) / 2;

				//std::cout << "n = " << n << std::endl;
				//std::cout << "d = " << d << std::endl;
				//std::cout << "m = " << m << std::endl;

				// generowanie grafu z cyklem Eulera
				bool **T = new bool *[n];
				for (int i = 0; i < n; i++)
					T[i] = new bool[n];
				generate_graph(T, n, m);

				// tworzenie listy następników na podstawie macierzy sąsiedztwa
				AdjList *adj = new AdjList[n];
				for (int i = 0; i < n; i++)
					adj[i].head = NULL;
				adjacency_list(adj, T, n);

				/*
				std::cout << std::endl;
				print_T(T, n);
				std::cout << std::endl;
				print_adj(adj, n);
				std::cout << std::endl;
				*/
				plik << n << "|" << d << std::endl;
				// -----------> CYKL EULERA <----------- //
				int *S_euler = new int[m + 4];
				int it = 0;
				start = std::chrono::high_resolution_clock::now();
				dfs_euler(T, n, 0, S_euler, it);
				end = std::chrono::high_resolution_clock::now();
				elapsed = end - start;
				std::cout << "Czas wyszukiwania cyklu Eulera: " << elapsed.count() << " milisekund" << std::endl;
				plik << "E: " << elapsed.count() << "\n";
				/*
				std::cout << "Eulerian Cycle: ";
				for (int i = 0; i < it; i++) {
				std::cout << S_euler[i];
				if(i != it-1)
				std::cout << ", ";
				}
				*/

				std::cout << std::endl;

				int *S_hamilton = new int[n];
				bool *visited = new bool[n];
				for (int i = 0; i < n; i++) {
					visited[i] = false;
				}
				int it2 = 0;
				int count = 0;
				bool stop = false;

				// -----------> JEDEN CYKL HAMILTONA <----------- //
				start_h = std::chrono::high_resolution_clock::now();
				dfs_hamilton_1(adj, n, S_hamilton, it2, 0, visited, count, stop);
				plik << "H1: " << elapsed_h.count() << "\n";
				std::cout << std::endl;

				// -----------> WSZYSTKIE CYKLE HAMILTONA <----------- //
				if (n <= 15)
				{
					start = std::chrono::high_resolution_clock::now();
					dfs_hamilton(adj, n, S_hamilton, it2, 0, visited, count);
					end = std::chrono::high_resolution_clock::now();
					elapsed = end - start;
					plik << "HA: " << elapsed.count() << "\n";
					std::cout << "Czas wyszukiwania wszystkich cykli Hamiltona: " << elapsed.count() << " milisekund" << std::endl;
					std::cout << std::endl;
					std::cout << "Liczba wszystkich cykli Hamiltona: " << count / 2 << std::endl;
					plik << "CH: " << count / 2 << "\n";
					std::cout << std::endl;

				}
			}
		}
	}
	

	plik.close();
	return 0;
}

void generate_graph(bool **T, int n, int m)  // generuje macierz sąsiedztwa dla grafu niekierowanego z cyklem Eulera
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			T[i][j] = false;
	}

	if (m < n)
		std::cout << "Nie istnieje taki graf eulerowski" << std::endl;

	else if (m == n) {
		for (int i = 0; i < n - 1; i++) {
			T[i][i + 1] = true;
			T[i + 1][i] = true;
		}
		T[n - 1][0] = true;
		T[0][n - 1] = true;
	}

	else {
		//łączenie krawędziami po kolei wszystkich wierzchołków
		for (int i = 0; i < n - 1; i++) {
			T[i][i + 1] = true;
			T[i + 1][i] = true;
			m--;
		}
		T[n - 1][0] = true;
		T[0][n - 1] = true;
		m--;

		int mod = m % 4;
		if (mod != 0)
			m += (4 - mod);

		while (m > 0) {
			int counter = 0;
			int vertices[4];
			do {
				counter = 0;
				std::set<int> uniq_vertices;
				for (int i = 0; i < 4; i++) {  // losowanie 4 wierzchołków
					do {
						vertices[i] = rand() % n;
						//std::cout << "vertices[" << i << "] = " << vertices[i] << std::endl;
						uniq_vertices.insert(vertices[i]);
						//std::cout << "size = " << uniq_vertices.size() << std::endl;
					} while (uniq_vertices.size() != i + 1);  // sprawdza czy wylosowany wierzchołek się nie powtórzył
				}
				selection_sort(vertices, 4);  // posortowanie wierzchołków rosnąco

				for (int i = 0; i < 3; i++) {  // jeżeli między wylosowanymi wierzchołkami nie ma krawędzi lub są tylko między parą najmniejszą lub największą to wychodzi z pętli, w przeciwnym wypadku losuje jeszcze raz
					for (int j = i + 1; j < 4; j++) {
						if (T[vertices[i]][vertices[j]] == true) {
							counter++;
							if ((i == 0 && j == 1) || (i == 2 && j == 3))
								counter--;
						}
					}
				}


			} while (counter != 0);

			// jak już są wylosowane odpowiednie wierzchołki to tworzy między nimi 4 krawędzie

			T[vertices[0]][vertices[2]] = true;
			T[vertices[2]][vertices[0]] = true;
			T[vertices[0]][vertices[3]] = true;
			T[vertices[3]][vertices[0]] = true;
			T[vertices[1]][vertices[2]] = true;
			T[vertices[2]][vertices[1]] = true;
			T[vertices[1]][vertices[3]] = true;
			T[vertices[3]][vertices[1]] = true;

			m -= 4;
		}
	}
}

void adjacency_list(AdjList * & adj, bool **T, int n)  // generuje listę następników na podstawie macierzy sąsiedztwa
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (T[i][j])
				addEdge(adj, i, j);
}

void addEdge(AdjList * & adj, int src, int dest)
{
	AdjListNode* newNode = new AdjListNode;
	newNode->value = dest;
	newNode->next = NULL;

	newNode->next = adj[src].head;
	adj[src].head = newNode;
}

void print_T(bool **T, int n)  // wypisywanie macierzy sąsiedztwa
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			std::cout << T[i][j] << " ";
		std::cout << std::endl;
	}
}

void print_adj(AdjList *adj, int n)  // wypisywanie listy następników
{
	for (int i = 0; i < n; i++) {
		AdjListNode* tmp = adj[i].head;
		std::cout << i;
		while (tmp) {
			std::cout << " -> " << tmp->value;
			tmp = tmp->next;
		}
		std::cout << std::endl;
	}
}

void selection_sort(int T[], int n)
{
	int minimum, pom;
	for (int k = 0; k<n - 1; k++) {
		minimum = k;

		for (int i = k + 1; i < n; i++) {
			if (T[i]<T[minimum])
				minimum = i;
		}

		pom = T[minimum];
		T[minimum] = T[k];
		T[k] = pom;
	}
}

bool deg(bool **T, int n)  // sprawdza czy wszystkie wierzchołki są parzystego stopnia
{
	int tmp = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (T[i][j] == true) tmp++;
		}
		if (tmp % 2 == 1) return false;
	}
	return true;
}

void dfs_euler(bool **T, int n, int v, int *S, int & it)  // zmodyfikowany DFS do poszukiwania cyklu Eulera
{
	for (int i = 0; i < n; i++)
		while (T[v][i])
		{
			T[v][i] = false;
			T[i][v] = false;
			dfs_euler(T, n, i, S, it);
		}
	S[it++] = v;
}

void dfs_hamilton(AdjList *adj, int n, int *S, int & it, int v, bool *visited, int & count)
{
	int i;
	bool test;
	AdjListNode *p;

	S[it++] = v;  // Wierzchołek v na stos
	if (it < n) {  // Jest ścieżka Hamiltona?
		visited[v] = true;  // Nie ma, odwiedzamy v
		for (p = adj[v].head; p; p = p->next)  // Przeglądamy sąsiadów v
			if (!visited[p->value]) dfs_hamilton(adj, n, S, it, p->value, visited, count);  // Wywołanie rekurencyjne
		visited[v] = false;  // Wycofujemy się z v
	}
	else {  // Jest ścieżka Hamiltona
		test = false;   // Zakładamy brak cyklu
		for (p = adj[v].head; p; p = p->next)  // Przeglądamy sąsiadów
			if (!(p->value)) {  // Jeśli sąsiadem jest wierzchołek 0,
				test = true;  // to mamy cykl
				count++;
				break;
			}

		/*
		if (test) {
		std::cout << "Hamiltonian Cycle: ";
		for (i = 0; i < it; i++)  // Wypisujemy ścieżkę Hamiltona
		std::cout << S[i] << ", ";
		std::cout << 0;  // Dla cyklu dopisujemy wierzchołek startowy
		std::cout << std::endl;
		}
		*/

	}
	it--;  // Wierzchołek v usuwamy ze stosu
}

void dfs_hamilton_1(AdjList *adj, int n, int *S, int & it, int v, bool *visited, int & count, bool & stop)
{
	if (!stop) {
		int i;
		bool test;
		AdjListNode *p;

		S[it++] = v;
		if (it < n) {
			visited[v] = true;
			for (p = adj[v].head; p; p = p->next)
				if (!visited[p->value]) dfs_hamilton_1(adj, n, S, it, p->value, visited, count, stop);
			visited[v] = false;
		}
		else {
			test = false;
			for (p = adj[v].head; p; p = p->next)
				if (!(p->value)) {
					test = true;
					count++;
					end_h = std::chrono::high_resolution_clock::now();
					if (count == 1) {
						elapsed_h = end_h - start_h;
						std::cout << "Czas wyszukiwania jednego cyklu Hamiltona: " << elapsed_h.count() << " milisekund" << std::endl;
						stop = true;
					}
					break;
				}

			/*
			if (test) {
			std::cout << "Hamiltonian Cycle: ";
			for (i = 0; i < it; i++)  // Wypisujemy ścieżkę Hamiltona
			std::cout << S[i] << ", ";
			std::cout << 0;  // Dla cyklu dopisujemy wierzchołek startowy
			std::cout << std::endl;
			}
			*/
		}
		it--;
	}
}
