#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void karpa_rabina(string tekst, string wzorzec)
{
    int d = 256; //zmienna ktora odpowiada wielkoscia liczba znakow w ASCI
    int q = 101; //liczba pierwsza

    int wzorzecLength = wzorzec.length();
    int tekstLength = tekst.length();
    int i, j;
    int wzorzecHash = 0;
    int tekstHash = 0;
    int h = 1;

    //petla oblicza wartosc do aktualizacji hasza, wykonywanie dla indeksow i
    for (i = 0; i < wzorzecLength - 1; i++)
        h = (h * d) % q;

    //petla oblicza poczatkowy hasz dla wzorca i pierwszego fragmentu tekstu
    for (i = 0; i < wzorzecLength; i++)
    {
        wzorzecHash = (d * wzorzecHash + wzorzec[i]) % q;
        tekstHash = (d * tekstHash + tekst[i]) % q;
    }
    //glowna petla przeszukiwania tekstu w ktorej przeszukiwane sa hasze i podciagi wzorca
    for (i = 0; i <= tekstLength - wzorzecLength; i++)
    {
        if (wzorzecHash == tekstHash)
        {
            //petla sprawdza kolejne znaki wzorca i tekstu
            for (j = 0; j < wzorzecLength; j++)
            {
                if (tekst[i + j] != wzorzec[j])
                    break;
            }

            if (j == wzorzecLength)
            {
                // Znaleziono wzorzec - wypisanie wzorca, indeksu i numeru linii
                cout << "Znaleziono wzorzec - " << wzorzec << " - na indeksie: " << i;
                int lineNum = 1;
                for (int k = 0; k < i; k++)
                {
                    if (tekst[k] == '\n')
                        lineNum++;
                }
                cout << " w linii: " << lineNum << endl;
            }
        }

        if (i < tekstLength - wzorzecLength)
        {
            tekstHash = (d * (tekstHash - tekst[i] * h) + tekst[i + wzorzecLength]) % q;

            if (tekstHash < 0)
                tekstHash = tekstHash + q;
        }
    }
}


int main()
{
    // Wczytanie tekstu z pliku
    ifstream plik("utwor_literacki.txt");
    if (!plik)
    {
        cout << "Blad otwarcia pliku." << endl;
        return 1;
    }

    // Zapisanie tekstu do zmiennej tekst
    string tekst;
    string linia;

    while (getline(plik, linia))
    {
        tekst += linia + "\n";
    }

    int x = 0;
    string wzorzec;

    // Wyszukiwanie wzorców dopóki użytkownik nie wprowadzi wartości 2
    while (x != 2)
    {
        // Podawanie wzorca przez użytkownika
        cout << "Podaj wzorzec: ";
        cin >> wzorzec;

        // Wywołanie funkcji przekazując argumenty tekst i wzorzec, wykorzystywane jest haszowanie do porównywania wzorca z podciągami tekstu
        karpa_rabina(tekst, wzorzec);

        cout << "1. Wyszukac kolejna fraze" << endl;
        cout << "2. Koniec wyszukiwania" << endl;
        cin >> x;
    }
    plik.close();

    return 0;
}
