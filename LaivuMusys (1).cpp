#include <iostream>
#include <ctime>
#include <set>

using namespace std;

const int eil = 10;
const int stulp = 10;
const int maxlaivai = 15;
const int laivai[] = { 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5 };

class Lenta;

class Player {
public:
    virtual void MakeMove(Lenta& board) = 0; 
    virtual bool Surrender() const = 0;
    virtual ~Player() {}
};

class Lenta {
private:
    int lenta[eil][stulp];
    set<pair<int, int>> attackedCells;

public:
    Lenta() { Clear(); }
    ~Lenta() { attackedCells.clear(); }
    void Clear()
    { 
        for (int i = 0; i < eil; i++)
        { 
            for (int j = 0; j < stulp; j++) lenta[i][j] = 0; 
        } 
    }

    void ShowBoard() const
    {
        cout << "  ";
        for (int j = 0; j < stulp; j++) { cout << j << " "; }
        cout << endl;

        for (int i = 0; i < eil; i++)
        {
            cout << i << " ";
            for (int j = 0; j < stulp; j++)
            {
                if (lenta[i][j] == 0) cout << "~ ";
                else if (lenta[i][j] == 1) cout << "~ ";
                else if (lenta[i][j] == 2) cout << "X ";
                else if (lenta[i][j] == 3) cout << "- ";
            }
            cout << endl;
        }
    }

    bool Attack(int x, int y)
    {
        if (lenta[x][y] == 1)
        {
            lenta[x][y] = 2;
            return true;
        }
        else if (lenta[x][y] == 0) lenta[x][y] = 3;
        return false;
    }

    bool IsCellAttacked(int x, int y) const { return attackedCells.count(make_pair(x, y)) > 0; }
    void MarkCellAsAttacked(int x, int y) { attackedCells.insert(make_pair(x, y)); }

    int KiekLaivu() const
    {
        int count = 0;
        for (int i = 0; i < eil; i++)
        { 
            for (int j = 0; j < stulp; j++) if (lenta[i][j] == 1) count++;
        }
        return count;
    }

    void Laivai()
    {
        set<pair<int, int>> occupiedCells;

        for (int shipIndex = 0; shipIndex < maxlaivai; shipIndex++)
        {
            int shipLength = laivai[shipIndex % (sizeof(laivai) / sizeof(int))];
            bool placed = false;
            while (!placed)
            {
                int x = rand() % eil;
                int y = rand() % stulp;
                if (x + shipLength <= eil)
                {
                    bool galima = true;
                    for (int i = 0; i < shipLength; i++)
                    {
                        if (occupiedCells.count(make_pair(x + i, y)) ||
                        (y > 0 && occupiedCells.count(make_pair(x + i, y - 1))) ||
                        (y < stulp - 1 && occupiedCells.count(make_pair(x + i, y + 1))))
                        {
                            galima = false;
                            break;
                        }
                    }

                    if (galima)
                    {
                        for (int i = 0; i < shipLength; i++)
                        {
                            lenta[x + i][y] = 1;
                            occupiedCells.insert(make_pair(x + i, y));
                        }
                        placed = true;
                        continue;
                    }
                }

                if (y + shipLength <= stulp)
                {
                    bool galima = true;
                    for (int i = 0; i < shipLength; i++) {
                        if (occupiedCells.count(make_pair(x, y + i)) ||
                        (x > 0 && occupiedCells.count(make_pair(x - 1, y + i))) ||
                        (x < eil - 1 && occupiedCells.count(make_pair(x + 1, y + i))))
                        {
                            galima = false;
                            break;
                        }
                    }

                    if (galima)
                    {
                        for (int i = 0; i < shipLength; i++)
                        {
                            lenta[x][y + i] = 1;
                            occupiedCells.insert(make_pair(x, y + i));
                        }
                        placed = true;
                    }
                }
            }
        }
    }

    friend ostream& operator<<(ostream& os, const Lenta& board) {
        for (int i = 0; i < eil; i++) {
            for (int j = 0; j < stulp; j++) {
                if (board.lenta[i][j] == 0) os << "- ";
                else if (board.lenta[i][j] == 1) os << "X ";
                else if (board.lenta[i][j] == 2) os << "X ";
                else if (board.lenta[i][j] == 3) os << "- ";
            }
            os << endl;
        }
        return os;
    }

    friend istream& operator>>(istream& is, Lenta& board)
    { 
        return is; 
    }

    bool operator==(const Lenta& other) const
    {
        for (int i = 0; i < eil; i++)
        { 
            for (int j = 0; j < stulp; j++)
            { 
                if (lenta[i][j] != other.lenta[i][j]) return false; 
            } 
        }
        return true;
    }

    bool operator!=(const Lenta& other) const { return !(*this == other); }
};

class HumanPlayer : public Player {
private:
    string ZaidejoVardas;
    int ejimai;
    bool pasiduoti;

public:
    HumanPlayer(const string& name) : ZaidejoVardas(name), ejimai(0), pasiduoti(false) {}

    void MakeMove(Lenta& board) override
    {
        cout << ZaidejoVardas << ", iveskite koordinates: ";
        int row, column;
        cin >> row >> column;

        while (row < 0 || row >= eil || column < 0 || column >= stulp)
        {
            cout << "Netinkamos koordinates! Bandykite dar karta." << endl;
            cout << ZaidejoVardas << ", iveskite koordinates: ";
            cin >> row >> column;
        }

        if (board.IsCellAttacked(row, column))
        {
            cout << "Jau sovet i ta vieta anksčiau!" << endl;
            MakeMove(board);
            return;
        }

        if (board.Attack(row, column))
        {
            cout << "Auc, " << ZaidejoVardas << ", jus pataikete! :)" << endl;
            board.MarkCellAsAttacked(row, column);
        }
        else
        {
            cout << "Atsiprasome, " << ZaidejoVardas << ", toje vietoje nera laivo!" << endl;
            board.MarkCellAsAttacked(row, column);
        }

        cout << "Liko langeliu su laivais: " << board.KiekLaivu() << endl;
        cout << "Atlikti ejimai: " << ++ejimai << endl;

        if (board.KiekLaivu() == 0)
        {
            cout << "Visi prieso laivai sunaikinti! Jus laimejote!" << endl;
            return;
        }

        if (ejimai % 10 == 0)
        {
            string answer;
            cout << "Ar norite pasiduoti? (taip/ne): ";
            cin >> answer;
            if (answer == "taip")
            {
                pasiduoti = true;
                cout << "Jus pasiduodate! Kompiuteris laimejo!" << endl;
                return;
            }
        }
    }

    bool Surrender() const override { return pasiduoti; }
};

class ComputerPlayer : public Player {
public:
    void MakeMove(Lenta& board) override {
        int row = rand() % eil;
        int column = rand() % stulp;

        while (board.IsCellAttacked(row, column))
        {
            row = rand() % eil;
            column = rand() % stulp;
        }

        cout << "Kompiuteris puola koordinate (" << row << ", " << column << ")" << endl;

        if (board.Attack(row, column))
        {
            cout << "Kompiuteris pataike!" << endl;
            board.MarkCellAsAttacked(row, column);
        }
        else
        {
            cout << "Kompiuteris nepataike." << endl;
            board.MarkCellAsAttacked(row, column);
        }

        cout << "Liko laivu: " << board.KiekLaivu() << endl;
    }

    bool Surrender() const override { return false; }
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "LAIVŲ MŪŠIO SIMULIACINIS ŽAIDIMAS" << endl;
    Lenta zaidimas;
    zaidimas.Laivai();
    zaidimas.ShowBoard();

    string playerName;
    cout << "Iveskite savo varda: ";
    cin >> playerName;

    Player* player;

    int choice;
    cout << "Pasirinkite zaidimo tipą: 1 - Šaudymas į kompiuterio lentą, 2 - Kompiuterio parodomasis žaidimas: ";
    cin >> choice;

    if (choice == 1) player = new HumanPlayer(playerName);
    else if (choice == 2) player = new ComputerPlayer();
    else
    {
        cout << "Neteisingas pasirinkimas. Baigiu programa." << endl;
        return 1;
    }

    while (zaidimas.KiekLaivu() > 0 && !player->Surrender())
    {
        player->MakeMove(zaidimas);
        zaidimas.ShowBoard();
    }

    delete player;
    return 0;
}

