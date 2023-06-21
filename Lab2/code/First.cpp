#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <chrono>


using namespace std;

// funkcja sortowania szybkiego
void quick_sort(int* array, int poczatek, int koniec)
{
    if(poczatek >= koniec)
    return;

    int p = poczatek - 1, q = koniec + 1, pivot = array[(poczatek+koniec)/2];

    while(true)
    {
        while(pivot > array[++p]);
        while(pivot < array[--q]);
        if(p<=q)
            swap(array[p], array[q]);
        else
            break;
    }
    if(q > poczatek)
        quick_sort(array, poczatek, q);
    if(p < koniec)
        quick_sort(array, p, koniec);
    return;
}


void radixSort(int* array, long long elements) {
    // Znajdź maksymalną wartość w tablicy
    int max = abs(array[0]);
    for (long long i = 1; i < elements; i++) {
        if (abs(array[i]) > max) {
            max = abs(array[i]);
        }
    }

    // Wykonaj sortowanie pozycyjne dla każdej cyfry w kolejności od najmniej znaczącej do najbardziej znaczącej
    for (long long exp = 1; max / exp > 0; exp *= 10) {
        int* output = new int[elements];
        int* count = new int[10]();

        // Zlicz liczbę elementów z każdą cyfrą na i-tej pozycji
        for (long long i = 0; i < elements; i++) {
            int digit = (array[i] / exp) % 10;
            if (array[i] < 0) {
                digit = 9 - digit; // odwróć kolejność cyfr dla liczb ujemnych
            }
            count[digit]++;
        }

        // Zmień count[i] tak, aby zawierał rzeczywistą pozycję i-tego elementu w output[]
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // Skopiuj elementy do output[] w prawidłowej kolejności
        for (long long i = elements - 1; i >= 0; i--) {
            int digit = (array[i] / exp) % 10;
            if (array[i] < 0) {
                digit = 9 - digit; // odwróć kolejność cyfr dla liczb ujemnych
            }
            output[count[digit] - 1] = array[i];
            count[digit]--;
        }

        // Skopiuj posortowaną tablicę z powrotem do array[]
        for (long long i = 0; i < elements; i++) {
            array[i] = output[i];
        }

        delete[] output;
        delete[] count;
    }
}

/*
void countSort(int* arr, int n, int exp)
{
    int output[n];
    int i, count[10] = {0};

    for (i = 0; i < n; i++)
        count[(arr[i]/exp)%10]++;

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (i = n - 1; i >= 0; i--)
    {
        output[count[(arr[i]/exp)%10] - 1] = arr[i];
        count[(arr[i]/exp)%10]--;
    }

    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

void radixSort(int* arr, int n)
{
    int max_num = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > max_num)
            max_num = arr[i];
    }

    for (int exp = 1; max_num/exp > 0; exp *= 10)
    {
        countSort(arr, n, exp);
    }
    return;
}
*/
int main()
{
    int elements = 0;
    int choice = 100;
    int count = 0;
    string config[5];
    string bufor;
    double time_buf = 0;
    float task_percentage;

//odczytywanie pliku sterujacego

    ifstream configuration("config.txt");
    if (configuration.good())
    {
        cout << "Pobieranie danych z pliku config.txt" << endl;
        for (int i = 0; i < 5; i++)
        {
            getline(configuration, config[i]);
        }
        cout << "Proces zakonczony sukcesem" << endl;
    }
    configuration.close();

//odczytywanie elementow konfiguracji

    ifstream elementsFile(config[4]);
    if (!elementsFile)
    {
        cout << "Nie znaleziono elementow pliku" << endl;
    }
    else
    {
        elementsFile >> elements;
    }
    elementsFile.close();

    int* array = new int[elements];

// panel wyboru opcji

    while(choice != 0)
    {
        cout << endl;
        cout << "1. Wczytaj dane z pliku" << endl;
        cout << "2. Zapisz dane do pliku" << endl;
        cout << "3. Sortuj liczby algorytmem babelkowym (BubbleSort)" << endl;
        cout << "4. Sortuj liczby algorytmem szybkim (quickSort)" << endl;
        cout << "5. Sortuj liczby algorytmem pozycyjnym (radixSort)" << endl;

        cout << "0. Zamknij program" << endl;

        cin >> choice;


        switch(choice)
        {
            //wczytanie danych z pliku
            case 1:
            {

                ifstream input;
                input.open(config[0]);

                if (!input)
                {
                    cerr << "Nie mozna otworzyc pliku";
                    exit(1);
                }

                for (int i = 0; i < elements; i++)
                {
                    getline(input, bufor, ',');
                    //konwersja z string do int, do odczytu
                    array[i] = atoi(bufor.c_str());
                }

                input.close();

                cout << endl;

            }break;

            //zapisanie danych do pliku
            case 2:
            {
                ofstream output(config[1]);
                if (output.is_open()) {

                    for (int i = 0; i < elements; i++) {
                        output << array[i] << endl;
                    }
                    output.close();
                }
                else {
                    cout << "Nie mozna otworzyc pliku" << endl;
                }
            }break;

            case 3:
            {

                auto start = chrono::high_resolution_clock::now();

                if (elements != 0)
                {
                    //sortowanie babelkowe
                    for (int i = 0; i < elements - 1; i++)
                    {
                        task_percentage = (double)i / elements * 100;
                        cout << task_percentage << "%" << endl;
                        for (int j = 0; j < elements - 1; j++)
                        {
                            if (array[j] > array[j + 1])
                            {
                                //zamiana wartości dwoch stringow pierwszy jest drugim, a drugi jest pierwszym
                                swap(array[j], array[j + 1]);
                            }
                        }
                    }

                    auto end = chrono::high_resolution_clock::now();
                    auto time_of = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    cout<<endl;
                    cout << "Czas wykonywania: " << time_of.count() * 1e-9  << " sekund" << endl;

                    //czas trwania algorytmu
                    ofstream timeofalg(config[3]);
                    {
                        if (timeofalg.is_open())
                        {
                            timeofalg << time_of.count() * 1e-9 << " sekundy" << endl;
                        }
                    }
                }
            }break;

            case 4:
            {
                auto start = chrono::high_resolution_clock::now();

                if (elements != 0)
                {
                quick_sort(array, 0, elements - 1);
                //int n = sizeof(array) / sizeof(array[0]);

                }

                auto end = chrono::high_resolution_clock::now();
                auto time_of = chrono::duration_cast<chrono::nanoseconds>(end - start);
                cout<<endl;
                cout << "Czas wykonywania: " << time_of.count() * 1e-9  << " sekund" << endl;

                //czas trwania algorytmu
                ofstream timeofalg(config[3]);
                    {
                        if (timeofalg.is_open())
                        {
                            timeofalg << time_of.count() * 1e-9 << " sekundy" << endl;
                        }
                    }
            }break;

            case 5:
            {
                auto start = chrono::high_resolution_clock::now();
                if (elements != 0)
                {
                radixSort(array, elements);
                }
                auto end = chrono::high_resolution_clock::now();
                auto time_of = chrono::duration_cast<chrono::microseconds>(end - start);
                cout << "Czas sortowania pozycyjnego: " << time_of.count() * 1e-9 << " sekundy" << endl;
            }break;

            case 0:
            {
                cout << "Zamykanie programu" << endl;
            }break;

            default:
            {
                cout << "Nieprawidlowa opcja wyboru" << endl;
            }
        }
    }

    return 0;

}