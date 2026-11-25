// Praktine uzduotis Nr. 3 "Dviraciu parduotuves uzsakymo skaiciuokle"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX = 101;
const string KAT[3] = {"Dviraciai", "Dalys", "Aksesuarai"};

int skaityti(string kat[], string pav[], double kaina[], int kiek[])
{
    ifstream fin("uzsakymas.txt");
    if (fin.fail()) {
        cout << "Klaida: nepavyko atidaryti failo uzsakymas.txt\n";
        return 0;
    }

    int n = 0;
    fin >> n;
    fin.ignore(1000, '\n');

    for (int i = 0; i < n; i++) {
        string eil;
        getline(fin, eil);
        stringstream ss(eil);
        char kabl = ' ';
        string likutis = "";
        kaina[i] = 0;
        kiek[i] = 0;
        getline(ss, kat[i], ';');
        getline(ss, pav[i], ';');
        ss >> kaina[i] >> kabl >> kiek[i] >> likutis;

        if (kat[i] != "Dviraciai" && kat[i] != "Dalys" && kat[i] != "Aksesuarai") {
            cout << "Klaida " << i + 2 << " eiluteje: netinkama kategorija " << kat[i] << "\n";
            return 0;
        }
        if (kabl != ';' || likutis != "" || kaina[i] <= 0 || kiek[i] <= 0) {
            cout << "Klaida " << i + 2 << " eiluteje: " << eil << "\n"
                 << "(kaina turi buti teigiama, su tasku, o kiekis - "
                 << "teigiamas sveikasis skaicius)\n";
            return 0;
        }
    }
    return n;
}

double suma(double kaina, int kiek)
{
    return kaina * kiek;
}

double nuolaida(double suma, int kiek)
{
    if (kiek >= 10)
        return suma * 0.10;
    return 0;
}

// Kategorijos k moketina suma po nuolaidu
double katSuma(const string kat[], const double kaina[], const int kiek[], int n, string k)
{
    double s = 0;
    for (int i = 0; i < n; i++)
        if (kat[i] == k)
            s += suma(kaina[i], kiek[i]) - nuolaida(suma(kaina[i], kiek[i]), kiek[i]);
    return s;
}

int lentele(ostream& out, const string kat[], const string pav[], const double kaina[],
            const int kiek[], int n, string k)
{
    out << left << setw(12) << "Kategorija" << setw(47) << "Pavadinimas" << right
        << setw(9) << "Kaina" << setw(8) << "Kiekis" << setw(10) << "Suma"
        << setw(10) << "Nuolaida" << setw(10) << "Moketi" << "\n";
    out << string(106, '-') << "\n";

    int kiekis = 0;
    for (int i = 0; i < n; i++) {
        if (k != "" && kat[i] != k)
            continue;
        double s = suma(kaina[i], kiek[i]);
        double nu = nuolaida(s, kiek[i]);
        out << left << setw(12) << kat[i] << setw(47) << pav[i] << right
            << setw(9) << kaina[i] << setw(8) << kiek[i] << setw(10) << s
            << setw(10) << nu << setw(10) << s - nu << "\n";
        kiekis++;
    }
    return kiekis;
}

void sumos(ostream& out, const double kaina[], const int kiek[], int n)
{
    double pries = 0, nuol = 0;
    for (int i = 0; i < n; i++) {
        pries += suma(kaina[i], kiek[i]);
        nuol += nuolaida(suma(kaina[i], kiek[i]), kiek[i]);
    }
    out << "Suma pries nuolaidas: " << setw(10) << pries << " Eur\n";
    out << "Nuolaidu suma:        " << setw(10) << nuol << " Eur\n";
    out << "Moketina suma:        " << setw(10) << pries - nuol << " Eur\n";
}

void katSumos(ostream& out, const string kat[], const double kaina[], const int kiek[], int n)
{
    for (int i = 0; i < 3; i++)
        out << left << setw(22) << KAT[i] << right << setw(10)
            << katSuma(kat, kaina, kiek, n, KAT[i]) << " Eur\n";
}


void ataskaita(ostream& out, const string kat[], const string pav[], const double kaina[],
               const int kiek[], int n)
{
    out << "========== UZSAKYMO ATASKAITA ==========\n\n";
    lentele(out, kat, pav, kaina, kiek, n, "");
    out << "\nVISO UZSAKYMO SUMOS\n";
    sumos(out, kaina, kiek, n);
    out << "\nSUMOS PAGAL KATEGORIJAS\n";
    katSumos(out, kat, kaina, kiek, n);
}

void saugoti(const string kat[], const string pav[], const double kaina[], const int kiek[], int n)
{
    ofstream rs("ataskaita.txt");
    if (rs.fail()) {
        cout << "Klaida: nepavyko atidaryti failo ataskaita.txt\n";
        return;
    }
    rs << fixed << setprecision(2);
    ataskaita(rs, kat, pav, kaina, kiek, n);
    rs.close();

    if (rs.fail())
        cout << "Klaida: nepavyko irasyti ataskaitos\n";
    else
        cout << "Ataskaita issaugota faile ataskaita.txt\n";
}

int ivesti(int nuo, int iki)
{
    int x = 0;
    cout << "Pasirinkite: ";
    cin >> x;
    while (cin.fail() || x < nuo || x > iki) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Netinkamas pasirinkimas. Iveskite skaiciu nuo " << nuo << " iki " << iki << ": ";
        cin >> x;
    }
    cin.ignore(1000, '\n');
    return x;
}

int main()
{
    string kat[MAX], pav[MAX];
    double kaina[MAX];
    int kiek[MAX];

    cout << fixed << setprecision(2);

    int n = skaityti(kat, pav, kaina, kiek);
    if (n == 0) {
        cout << "Programa baigiama.\n";
        return 1;
    }
    cout << "Nuskaityta prekiu: " << n << "\n";

    int p;
    do {
        cout << "\n----- MENIU -----\n"
             << "1. Perziureti visas uzsakymo prekes\n"
             << "2. Perziureti pasirinktos kategorijos prekes\n"
             << "3. Perziureti viso uzsakymo sumas\n"
             << "4. Perziureti sumas pagal kategorijas\n"
             << "5. Parodyti visa uzsakymo ataskaita\n"
             << "6. Issaugoti visa ataskaita i faila\n"
             << "7. Iseiti\n";
        p = ivesti(1, 7);
        cout << "\n";

        if (p == 1) {
            lentele(cout, kat, pav, kaina, kiek, n, "");
        }
        else if (p == 2) {
            cout << "1. Dviraciai\n2. Dalys\n3. Aksesuarai\n";
            string k = KAT[ivesti(1, 3) - 1];
            if (lentele(cout, kat, pav, kaina, kiek, n, k) == 0)
                cout << "Sios kategorijos prekiu uzsakyme nera.\n";
            cout << "Kategorijos " << k << " moketina suma: "
                 << katSuma(kat, kaina, kiek, n, k) << " Eur\n";
        }
        else if (p == 3) {
            sumos(cout, kaina, kiek, n);
        }
        else if (p == 4) {
            katSumos(cout, kat, kaina, kiek, n);
        }
        else if (p == 5) {
            ataskaita(cout, kat, pav, kaina, kiek, n);
        }
        else if (p == 6) {
            saugoti(kat, pav, kaina, kiek, n);
        }
    } while (p != 7);

    cout << "Viso gero!\n";
    return 0;
}