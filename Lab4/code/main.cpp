#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int INF = 1e9;

void printPath(const vector<int>& path, int destination)
{
    if (path[destination] == -1)
    {
        cout << destination << " ";
        return;
    }

    printPath(path, path[destination]);
    cout << destination << " ";
}

int main()
{
    cout << "1. Generuj nowa macierz i zapisz ja do pliku" << endl;
    cout << "2. Odczytaj macierz z pliku i wykonaj algorytm Dijkstry" << endl;

    int x;
    cin >> x;
    switch (x)
    {
        case 1:
        {
            int SIZE = 1000;

            int** matrix = new int* [SIZE];

            for (int i = 0; i < SIZE; i++)
            {
                matrix[i] = new int[SIZE];
            }

            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    matrix[i][j] = rand() % 20 + 1;
                    matrix[j][i] = matrix[i][j];
                }
            }

            ofstream output("matrix.csv");
            if (output.is_open())
            {
                for (int i = 0; i < SIZE; i++)
                {
                    for (int j = 0; j < SIZE; j++)
                    {
                        output << matrix[i][j] << ";";
                    }
                    output << endl;
                }
                output.close();
                cout << "Dane zapisane." << endl;
            }
            else
            {
                cout << "Blad otwarcia pliku" << endl;
            }
        } break;

        case 2:
        {
            cout << "Podaj wielkosc macierzy, z ktorej chcesz odczytac dane: ";
            int SIZE;
            cin >> SIZE;

            int** matrix = new int* [SIZE];

            for (int i = 0; i < SIZE; i++)
            {
                matrix[i] = new int[SIZE];
            }

            ifstream input("matrix.csv");
            string buf;

            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    getline(input, buf, ';');
                    matrix[i][j] = atoi(buf.c_str());
                }
                getline(input, buf, '\n');
            }

            input.close();

            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    cout << matrix[i][j] << "\t";
                }
                cout << endl;
            }

            cout << "Podaj procent usuwanych krawedzi (0-100): ";
            int percent;
            cin >> percent;

            int edgesToRemove = (SIZE * (SIZE - 1) / 2) * percent / 100;

            srand(time(nullptr));
            for (int i = 0; i < edgesToRemove; i++)
            {
                int row, col;
                do
                {
                    row = rand() % SIZE;
                    col = rand() % SIZE;
                } while (row == col || matrix[row][col] == 0);

                matrix[row][col] = 0;
                matrix[col][row] = 0;
            }

            int* dist = new int[SIZE];
            int* check = new int[SIZE];
            vector<int> path(SIZE, -1);  // Nowa definicja tablicy path

            for (int i = 0; i < SIZE; i++)
            {
                dist[i] = INF;
                check[i] = 0;
            }

            dist[0] = 0;

            auto start = chrono::high_resolution_clock::now();

            for (int i = 0; i < SIZE - 1; i++)
            {
                int min_dist = INF, min_edge;
                for (int j = 0; j < SIZE; j++)
                {
                    if (!check[j] && dist[j] < min_dist)
                    {
                        min_edge = j;
                        min_dist = dist[j];
                    }
                }

                check[min_edge] = 1;

                for (int k = 0; k < SIZE; k++)
                {
                    if (matrix[min_edge][k] != 0)
                    {
                        if (dist[min_edge] != INF)
                        {
                            if (dist[min_edge] + matrix[min_edge][k] < dist[k])
                            {
                                dist[k] = dist[min_edge] + matrix[min_edge][k];
                                path[k] = min_edge;  // Aktualizacja ścieżki
                            }
                        }
                    }
                }
            }

            auto end = chrono::high_resolution_clock::now();
            auto time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            //cout << endl << endl << "Krawedz: Odleglosc:" << endl;

            //for (int d = 0; d < SIZE; d++)
            //{
            //    cout << d + 1 << "\t" << dist[d] << endl;
            //}

            auto time_of = chrono::duration_cast<chrono::nanoseconds>(end - start);

            cout << endl << endl << "Krawedz: Odleglosc: Droga:" << endl;

            for (int del = 0; del < SIZE; del++)
            {
                cout << del + 1 << "\t" << dist[del] << "\t\t";
                printPath(path, del);
                cout << endl;
            }

            cout << endl << endl << "Czas wykonywania algorytmu w sekundach: " << time_of.count() * 1e-9 << endl;
            cout << "Czas wykonania algorytmu: " << time_taken * 1e-6 << " ms" << endl;

            for (int del = 0; del < SIZE; del++)
            {
                delete[] matrix[del];
            }

            delete[] matrix;
            delete[] dist;
            delete[] check;
        } break;

        default:
        {
            cout << "Zly numer" << endl;
        } break;
    }

    return 0;
}
