#include <iostream>
#include <windows.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h> 

using namespace std;

// Bêdzie nam potrzebne do zmienienia kolorów w konsoli
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Deklaracja klasy enum - pomocne przy wyborze typu gry
enum class Typ { standardowa = 1, renju, caro, omok, connect6 };

// Funkcja sprawdzajaca czy znak w stringu jest cyfr¹
bool isNumber(string s) {
    return !s.empty() && find_if(s.begin(),
        s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

// Prosta struktura gracza
struct Player
{
    string name = "";
};

// Struktura ca³ej rozgrywki
struct Game
{
    // Podstawowe informacje
    Typ typ = Typ::standardowa;
    int tableSize = 15;
    int firstPlayer = 0;
    int currentPlayer = 0;

    // Funkcja inicjowana po wygraniu jednego z graczy
    bool playAgain() {
        char in;
        wcout << L"Chcesz zagraæ ponownie?" << endl;
        wcout << L"Napisz 'T' jeœli tak" << endl;
        wcout << L"Napisz 'N' jeœli nie" << endl;
        wcout << L"Napisz 'Z' jeœli tak, ale chcesz zmieniæ tryb" << endl;
        wcout << L"Twój wybór: ";
        cin >> in;
        // Gramy ponownie + zwracamy true
        if (in == 'T') {
            return true;
        }
        // Nie gramy ponownie + zwracamy true
        else if (in == 'N') {
            return false;
        }
        // Zmieniamy tryb gry + Gramy ponownie + zwracamy true
        else if (in == 'Z') {
            this->setGameType();
            return true;
        }
        // Ponownie inicjujemy funkcje
        else {
            system("CLS");
            wcout << L"Wybierz w³aœciwie!" << endl;
            this->playAgain();
        }
    }

    // Funkcja ktora obs³uguje zmianê gracza
    void changeCurrectPlayer() {
        if (currentPlayer == 0) {
            this->currentPlayer = 1;
        }
        else {
            this->currentPlayer = 0;
        }
    }

    // Funkcja ktory obs³uguje zmianê trybu
    void setGameType() {
        string in;
        system("CLS");
        cout << "Wybierz typ gry!" << endl;
        wcout << L"Napisz 1 aby wybraæ standard" << endl;
        wcout << L"Napisz 2 aby wybraæ renju" << endl;
        wcout << L"Napisz 3 aby wybraæ caro" << endl;
        wcout << L"Napisz 4 aby wybraæ omok" << endl;
        wcout << L"Napisz 5 aby wybraæ connect6" << endl;
        wcout << L"Twój wybór: ";
        cin >> in;
        while (!isNumber(in) || atoi(in.c_str()) < 1 || atoi(in.c_str()) > 5)
        {
            system("CLS");
            cout << "Wybierz poprawny typ gry!" << endl;
            wcout << L"Napisz 1 aby wybraæ standard" << endl;
            wcout << L"Napisz 2 aby wybraæ renju" << endl;
            wcout << L"Napisz 3 aby wybraæ caro" << endl;
            wcout << L"Napisz 4 aby wybraæ omok" << endl;
            wcout << L"Napisz 5 aby wybraæ connect6" << endl;
            wcout << L"Twój wybór: ";
            cin >> in;
        }
        this->typ = static_cast<Typ>(atoi(in.c_str())); // Zmieniamy tryb w naszej strukturze
        tableSizeChange();
    }

    // W zale¿noœci od trybu mamy ró¿n¹ wielkoœæ planszy
    void tableSizeChange() {
        switch (typ)
        {
        case Typ::standardowa:
            this->tableSize = 15;
            break;
        case Typ::renju:
            this->tableSize = 15;
            break;
        case Typ::caro:
            this->tableSize = 15;
            break;
        case Typ::omok:
            this->tableSize = 19;
            break;
        case Typ::connect6:
            this->tableSize = 19;
            break;
        }
    }

    // Funkcja odpowiadaj¹ca za wyœwietlanie naszej planszy
    void displayTable(int** table) {
        for (int i = -1; i < tableSize; i++) {
            for (int j = -1; j < tableSize; j++) {
                if (i == -1 && j == -1) {
                    cout << "   ";
                }
                // Linia X-ów
                else if (i < 0 && j >= 0) {
                    cout << static_cast<char>(j + 'A') << " ";
                }
                // Linia Y-ów
                else if (j < 0 && i >= 0) {
                    cout << i + 1 << " ";
                    if (i + 1 < 10) {
                        cout << ' ';
                    }
                }
                // Plansza
                else if (table[i][j] == 1) {
                    SetConsoleTextAttribute(hConsole, 12); // Na pocz¹tku pliku wspomniane kolory
                    cout << 'X';
                    cout << ' ';
                    SetConsoleTextAttribute(hConsole, 15);
                }
                else if (table[i][j] == 2) {
                    SetConsoleTextAttribute(hConsole, 10);
                    cout << 'O';
                    cout << ' ';
                    SetConsoleTextAttribute(hConsole, 15);
                }
                else {
                    SetConsoleTextAttribute(hConsole, 15);
                    cout << '+';
                    cout << ' ';
                }

            }
            cout << endl;
        }
    }

    // Funckja odpowiadajaca za przechwytywanie koordynatów
    void getInputs(int& currentX, int& currentY) {
        string in;
        bool firstRun = true;
        bool bigLetterExist = false;
        currentX = 0;
        currentY = 0;
        int i = 0;
        bool tooManyLeters = false;
        do {
            if (!firstRun) {
                cout << "Podaj poprawne koordynaty!" << endl;
                i = 0;
                tooManyLeters = false;
                bigLetterExist = false;
            }
            firstRun = false;
            cout << "Wpisz koordynaty: ";
            cin >> in;
            while ((int)in[i] > 47 && (int)in[i] < 58) { // ASCII cyfry
                i++;
            }
            if ((int)in[i] > 64 && (int)in[i] < 91) { // ASCII du¿e litery
                bigLetterExist = true;
                if (i != in.length() - 1) {
                    tooManyLeters = true;
                }
            }
        } while (in.length() < 2 || in.length() - i < 1 || !bigLetterExist || i < 1 || tooManyLeters); // Dopóki u¿ytkownik poda prawid³owe koordynaty
        i = 0;
        while ((int)in[i] > 47 && (int)in[i] < 58) {
            currentY += ((int)in[i] - 48) * pow(10, (in.length() - (i + 2))); // Pow potrzebne nam jest przy braniu np 15A = 1 * 10 + 5 * 1
            i++;
        }
        if ((int)in[i] > 64 && (int)in[i] < 91) {
            currentX = (int)in[i] - 64;
        }
        // Jeœli u¿ytkownik poda np. 20Z, program przyjmie to jako 15O (lub 19U jeœli mamy plansze 19x19)
        if (currentY > tableSize) {
            currentY = tableSize;
        }
        if (currentX > tableSize) {
            currentX = tableSize;
        }
    }

    // Funkcja sprawdzajaca czy dany gracz posiada odpowiednia ilosc kamieni na ukos aby wygrac
    bool checkCross(int** table, int player, bool right, int i, int j) {
        int points = 0;
        while (table[i][j] == player) {
            points++;
            i++;
            if (right) {
                j++;
            }
            else {
                j--;
            }
            if (j < 0 || i > tableSize - 1 || j > tableSize) {
                break;
            }
        }
        if (points >= 6 && typ != Typ::connect6 && typ != Typ::standardowa || (typ == Typ::caro && points >= 5)) {
            return true;
        }
        else if (typ == Typ::connect6 && points == 6 || typ == Typ::standardowa && points == 5) {
            return true;
        }
        else {
            return false;
        }
    }

    // Funkcja odpowiadaj¹ca za ca³¹ grê
    void play(vector <Player> players) {
        int** table = new int* [tableSize];
        for (int i = 0; i < tableSize; ++i) {
            table[i] = new int[tableSize] {};
        } // Tworzymy dwuwymiarow¹ tablicê
        int currentX = 0;
        int currentY = 0;
        int additionalMove = 1;
        bool firstRun = true;
        do {
            // Przy trybie connect6 potrzebujemy dodatkowego ruchu
            if (typ == Typ::connect6 && additionalMove > 0) {
                additionalMove--;
            }
            else if (typ == Typ::connect6) {
                changeCurrectPlayer();
                additionalMove = 1;
            }
            else {
                changeCurrectPlayer();
            }
            system("CLS");
            // Wyœwietlamy nasz¹ planszê
            cout << "Teraz ruch: " << players[currentPlayer].name << endl;
            displayTable(table);
            firstRun = true;
            // Pobieramy koordynaty od u¿ytkownika
            do {
                if (!firstRun) {
                    cout << "Wybierz puste miejsce!" << endl;
                }
                cout << "Pozycja X ruchu przeciwnika: " << currentX << endl;
                cout << "Pozycja Y ruchu przeciwnika: " << currentY << endl;
                getInputs(currentX, currentY);
                firstRun = false;
            } while (table[currentY - 1][currentX - 1] != 0);
            table[currentY - 1][currentX - 1] = currentPlayer + 1;
            cout << endl;
        } while (!ifWin(table, currentPlayer + 1));

        // Je¿eli jeden z graczy wygra³ inicjujemy poni¿szy kod
        system("CLS");
        displayTable(table);
        if (typ == Typ::connect6 && additionalMove > 0) {
            if (currentPlayer == 1) {
                currentPlayer = 0;
            }
            else {
                currentPlayer = 1;
            }
        }
        cout << "Wygral: " << players[currentPlayer].name << endl;
    }

    // Funkcja sprawdzaj¹ca czy u¿ytkownik wygra³
    bool ifWin(int** table, int player) {
        int columnPointsCount = 0;
        int rowPointsCount = 0;
        for (int i = 0; i < tableSize; i++) {
            for (int j = 0; j < tableSize; j++) {

                // Sprawdzamy czy jest odpowiednia iloœæ kamieni na skosy ( nie musimy sprawdzaæ do góry skosów gdy¿ sprawdzamy tablice od góry )
                if (table[i][j] == player && i < tableSize - 1 && j < tableSize) {
                    if (i == 0 || j == 0) {
                        // Skos w prawo
                        if (table[i + 1][j + 1] == player) {
                            if (checkCross(table, player, true, i, j)) {
                                return true;
                            }
                        }
                        // Skos w lewo
                        else if (table[i + 1][j - 1] == player) {
                            if (checkCross(table, player, false, i, j)) {
                                return true;
                            }
                        }
                    }
                    else {
                        // Skos w prawo
                        if (table[i - 1][j - 1] != player) {
                            if (table[i + 1][j + 1] == player) {
                                if (checkCross(table, player, true, i, j)) {
                                    return true;
                                }
                            }
                        }
                        // Skos w lewo
                        if (table[i - 1][j + 1] != player) {
                            if (table[i + 1][j - 1] == player) {
                                if (checkCross(table, player, false, i, j)) {
                                    return true;
                                }
                            }
                        }
                    }
                }

                // Sprawdzamy czy jest odpowiednia iloœæ kamieni w danym rzêdzie
                if (table[i][j] == player) {
                    if (rowPointsCount == 0) {
                        rowPointsCount++;
                    }
                    else {
                        rowPointsCount++;
                    }
                }
                else {
                    if (rowPointsCount >= 6 && typ != Typ::connect6 && typ != Typ::standardowa || (typ == Typ::caro && rowPointsCount >= 5)) {
                        return true;
                    }
                    else if (typ == Typ::connect6 && rowPointsCount == 6 || typ == Typ::standardowa && rowPointsCount == 5) {
                        return true;
                    }
                    rowPointsCount = 0;
                }

            }
            cout << endl;
        }

        // Sprawdzamy czy jest odpowiednia iloœæ kamieni w danej kolumnie
        for (int j = 0; j < tableSize; j++) {
            for (int i = 0; i < tableSize; i++) {
                if (table[i][j] == player) {
                    if (columnPointsCount == 0) {
                        columnPointsCount++;
                    }
                    else {
                        columnPointsCount++;
                    }
                }
                else {
                    if (columnPointsCount >= 6 && typ != Typ::connect6 && typ != Typ::standardowa || (typ == Typ::caro && columnPointsCount >= 5)) {
                        return true;
                    }
                    else if (typ == Typ::connect6 && columnPointsCount == 6 || typ == Typ::standardowa && columnPointsCount == 5) {
                        return true;
                    }
                    columnPointsCount = 0;
                }

            }
            cout << endl;
        }
        return false;
    }
};

// Funkcja wyswietlajaca menu
int menu()
{
    string in;
    wcout << L"Witaj graczu! Wybierz co chcesz zrobiæ:" << endl;
    wcout << L"Napisz 1 aby zacz¹æ grê" << endl;
    wcout << L"Napisz 2 aby przeczytaæ instrukcjê gry" << endl;
    wcout << L"Napisz 3 aby wyjœæ z gry" << endl;
    wcout << L"Twój wybór: ";
    cin >> in;
    while (!isNumber(in) || atoi(in.c_str()) < 1 || atoi(in.c_str()) > 3)
    {
        system("CLS");
        wcout << L"Wybierz w³aœciwie!" << endl;
        wcout << L"Napisz 1 aby zacz¹æ grê" << endl;
        wcout << L"Napisz 2 aby przeczytaæ instrukcjê gry" << endl;
        wcout << L"Napisz 3 aby wyjœæ z gry" << endl;
        wcout << L"Twój wybór: ";
        cin >> in;
    }
    return atoi(in.c_str()); // String -> Ci¹g znaków -> Int
}

// Funkcja wyswietlajaca instrukcje
void instruction() {
    string in;
    system("CLS");
    cout << "--- Instrukcja rozgrywki ---" << endl;
    wcout << "1. Standard: " << endl << "- Gra przebiega na dwuwymiarowej planszy 15 na 15" << endl << L"- ¯eby wygraæ, gracz musi mieæ ustawione 5 kamieni poziomo, pionowo lub na ukos" << endl
        << L"- Gracz nie wygrywa, gdy ma ustawione 6 lub wiêcej kamieni w jednym rzêdzie" << endl << L"- Gracze na zmianê (Hotseat) postawiaj¹ jeden kamieñ na planszy za pomoc¹ wpisywania " <<
        L"wspó³rzêdnych np. 15H" << endl << L"POWY¯SZE REGU£Y ZASTOSOWANE S¥ RÓWNIE¯ DO POZOSTA£YCH TRYBÓW (ORAZ MOG¥ BYÆ NADPISANE)" << endl;
    cout << "----------" << endl;
    wcout << "2. Renju: " << endl << L"- Zasada trzy na trzy (nie mo¿na zrobiæ ruchu, który uformuje jednoczeœnie dwa otwarte rzêdy sk³adaj¹ce siê z trzech kamieni " << endl <<
        L"(otwarty - rz¹d kamieni nie jest zablokowany przez przeciwnika z ¿adnej strony))" << endl << L"- Zasada cztery i cztery (nie mo¿na zrobiæ ruchu, który uformuje jednoczeœnie dwa rzêdy" <<
        L" sk³adaj¹ce siê z czterech kamieni)" << endl << L"- Drugi gracz mo¿e wygraæ za pomoc¹ ustawienia 6 kamieni lub wiêcej w jednym rzêdzie" << endl;
    cout << "----------" << endl;
    wcout << "3. Caro: " << endl << L"- Gracz wygrywa, kiedy ma ustawione 6 kamieni lub wiêcej w jednym rzêdzie" << endl << L"- Gracz równie¿ wygrywa, kiedy ma ustawione 5 kamieni, " <<
        L"ale tylko wtedy, kiedy jest to otwarty rz¹d" << endl;
    cout << "----------" << endl;
    wcout << "4. Omok: " << endl << "- Gra przebiega na dwuwymiarowej planszy 19 na 19" << endl << "- Zasada trzy i trzy (patrz Renju)" << endl <<
        L"- Gracz wygrywa, kiedy ma ustawione 6 kamieni lub wiêcej w jednym rzêdzie" << endl;
    cout << "----------" << endl;
    wcout << "5. Connect6: " << endl << "- Gra przebiega na dwuwymiarowej planszy 19 na 19" << endl << L"- ¯eby wygraæ, gracz musi mieæ ustawione 6 kamieni poziomo, pionowo lub na ukos" << endl <<
        L"- Gracz nie wygrywa, gdy ma ustawione 7 lub wiêcej kamieni w jednym rzêdzie" << endl << L"- Pierwszy gracz postawia na pocz¹tku jeden kamieñ, po czym obaj gracze postawiaj¹" <<
        " naprzemiennie 2 kamienie" << endl;
    cout << "--- Koniec instrukcji ---" << endl;
    wcout << L"Naciœnij dowolny klawisz, a potem enter, aby wróciæ do menu" << endl;
    cin >> in;
}

int main()
{
    vector <Player> players; // Struktura danych w ktorej przechowujemy graczy
    static int action = 0;
    srand(time(NULL)); // Potrzebne zeby wylosowac pierwszego gracza i zeby sie to nie powtarzalo (pierwszy gracz ma przewage w trybie standard)
    setlocale(LC_CTYPE, ""); // Polskie znaki w konsoli
    do {
        system("CLS"); // Sluzy do czyszczenia konsoli
        action = menu(); // Wywolujemy funkcje menu i w zaleznosci co ona zwraca, wykonujemy dana czynnosc

        // Rozpoczecie gry
        if (action == 1) {
            players.clear(); // Na wszelki wypadek usuwa wszystko z vectora 'players', jak to mowia, lepiej dmuchac na zimne
            bool firstRun = true;
            int firstPlayer = rand() % 2;

            // Ustalamy tryb gry
            Game game{ Typ::standardowa,15,firstPlayer,firstPlayer };
            game.setGameType();

            // Oboje graczy podaj¹ swoje imiona
            string imie;
            system("CLS");
            wcout << L"Graczu 1 podaj swoje imiê:" << endl;
            cin >> imie;
            players.push_back(Player({ imie }));
            system("CLS");
            wcout << L"Graczu 2 podaj swoje imiê:" << endl;
            cin >> imie;
            players.push_back(Player({ imie }));

            // LET THE GAMES BEGIN!
            while (firstRun || game.playAgain()) {
                system("CLS");
                game.play(players);
                firstRun = false;
            }

        }
        // Instrukcja
        else if (action == 2) {
            instruction();
        }
        // Zakonczenie programu
    } while (action != 3);
    return 0;

}
