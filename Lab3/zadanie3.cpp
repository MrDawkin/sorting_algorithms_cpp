#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <vector>
using namespace std;
using namespace std::chrono;

struct config{
    string sort;
    string output;
    string input;
    string time_file;
    int iters;
};
#define data_amount 1000
int INT_MAX = 50;

//funkcja wczytujaca dane z pliku do wektora dwuwymiarowego
void dataIn(string input_file, vector<vector<int>> &data){
    ifstream infile(input_file);

    while (infile)
    {
        string t;
        if (!getline( infile, t )) break;

        istringstream sst( t );
        vector <int> record;

        while (sst)
        {
            string t;
            if (!getline( sst, t, ',' )) break;
            record.push_back(stoi(t));
        }

        data.push_back(record);
    }


}

//funkcja zapisujaca dane z wektora dwuwymiarowego do pliku
void dataOut(string output_file, vector<vector<int>> &data){
    fstream output;
    output.open(output_file, ios::out);
    for(int i = 0; i < data_amount; i++){
        for(int j = 0; j < data_amount; j++){
            output << data[i][j] << ", ";
        }
        output << endl;
    }
    output.close();
}
//funkcja analizuje plik konfiguracyjny i wczytuje konfiguracje do struktury config
void parseFile(ifstream &config_file, config *&data)
{
    string line;
    while(getline(config_file, line)){ //wczytywanie konfiguracji do struktury data
            istringstream config_line(line);
            string key;
            if(getline(config_line, key, '=')){
                string val;
                if(getline(config_line, val)){
                    if(key == "input_file") (*data).input += val;
                    if(key == "output_file") (*data).output += val;
                    if(key == "complete_time") (*data).time_file += val;
                    if(key == "sort_type") (*data).sort += val;
                    if(key == "iterations") (*data).iters = stoi(val);
                }
            }
        }
}
//funkcja zapisuje czasy wykonania pliku
void timeOutput(int times[], string output_file, int iteramount){
    fstream output;
    output.open(output_file, ios::out);

    for(int i = 0; i< iteramount; i++){
        output << times[i] << endl;
    }
    output.close();

}
//znajdowanie indeksu wierzcholka o najmniejszej wartosci sposrod wierzcholkow nie dodanych do drzewa rozpinajacego
int minKey(int key[], bool mstSet[])
{
    // Inicjalizacja wartosci minimalnej
    int min = INT_MAX, min_index;

    for (int v = 0; v < data_amount; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}
//funkcja wpisuje minimalne drzewo rozpinajace wyswietlajac krawedzie, wagi i sume wag
void printMST(int parent[], vector<vector<int>> &graph)
{
    int MST = 0;
    cout << "Krawedz \tWaga\n";
    for (int i = 1; i < data_amount; i++){
        cout << i << ". " << parent[i] << " - " << i << " \t" << graph[i][parent[i]] << " \n";
        MST = MST + graph[i][parent[i]];
    }

}
// funkcja implementujaca algorytm Jarnika-Prima. Drzewo rozpinajace wykorzystujace macierz sasiedztwa, przechowuje informacje o rodzicach wierzcholkow w tablicy
void primMST(vector<vector<int>> &graph)
{

    int parent[data_amount];

    int key[data_amount];

    bool mstSet[data_amount];

    for (int i = 0; i < data_amount; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    key[0] = 0;

    parent[0] = -1;

    for (int count = 0; count < data_amount - 1; count++) {
        int u = minKey(key, mstSet);

        mstSet[u] = true;

        for (int v = 0; v < data_amount; v++)
            if (graph[u][v] && mstSet[v] == false
                && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    printMST(parent, graph);
}


int main(){
    srand(time(NULL));

    int percentage = 1*data_amount;

    ifstream file("config.cfg"); //otwarcie pliku sterujacego
    config * cfg = new config;
    parseFile(file, cfg);
    vector<vector<int>> matrix;
    matrix.reserve(1000*1000*sizeof(int));
    int durations[10];
    dataIn((*cfg).input, matrix);

    for(int i = 0; i < percentage; i++){
        int row, column;
        row = (rand() % percentage);
        column = (rand() % percentage);
        matrix[row][column] = 0;
        matrix[column][row] = 0;
    }

    auto start = steady_clock::now(); // pomiar czasu rozpoczęcia sortowania
    primMST(matrix);
    auto end = steady_clock::now(); // pomiar czasu zakończenia sortowania
    auto diff = duration_cast<nanoseconds>(end - start).count(); // obliczenie czasu sortowania w nanosekundach
    cout << "Czas wykonywania: " << diff;


    dataOut((*cfg).output, matrix);
    timeOutput(durations, (*cfg).time_file, (*cfg).iters);
    delete cfg;
    return 0;
}