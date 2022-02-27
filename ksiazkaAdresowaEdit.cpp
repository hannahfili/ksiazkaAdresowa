#include <iostream>
#include <vector>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <filesystem>


using namespace std;

struct Kontakt {
	int id;
	int idUzytkownika;
	string imie, nazwisko, adres, numerTelefonu, email;
	bool przyjaciel;
	bool ok;
};
struct Uzytkownik {
	int id;
	string login;
	string haslo;
	bool ok;
};
struct kontaktPoEdycjiPlusPowrot {
	Kontakt osoba;
	int powrot;
};

bool kontaktySaTakieSame(Kontakt kontakt1, Kontakt kontakt2);
void wyswietlMenuZalogowany(Uzytkownik u);
int dodajOsobe(vector <Kontakt>& ksiazkaAdresowa, Uzytkownik uzytkownik, string sciezkaUzytkownikow);
void usunOsobe(vector <Kontakt>& ksiazkaAdresowa, string sciezkaKsiazki);
bool usunWybranaOsobeZwektora(vector <Kontakt>& ksiazkaAdresowa, int indeksUsuwanegoKontaktu);
bool sprawdzCzyKontaktJuzIstnieje(vector <Kontakt> ksiazkaAdresowa, Kontakt test);
void wyswietlDane(Kontakt osoba);
void wyswietlDaneOsobAbyWybracID(vector <Kontakt> ksiazkaAdresowa, string wiadomosc);
bool wczytajDaneDoKsiazki(vector <Kontakt>& ksiazkaAdresowa, string sciezkaDoPliku, Uzytkownik uzytkownik);
void zapiszOsobaDoPliku(Kontakt osoba, string sciezka);
void wyswietlKontakty(vector <Kontakt>& ksiazkaAdresowa);
void wyszukajPrzyjaciol(vector <Kontakt>& ksiazkaAdresowa);
void wyszukajPoImieniuLubNazwisku(vector <Kontakt>& ksiazkaAdresowa, string parametr);
Kontakt rozdzielDane(string dane, Uzytkownik uzytkownik, bool tylkoDaneKonkretnegoUzytkownika=false);
Kontakt edytujImieLubNazwisko(Kontakt osoba, string poleStrukturyOsoba);
Kontakt edytujNrTelEmailLubAdres(Kontakt osoba, string poleStrukturyOsoba);
Kontakt edytujStatusPrzyjaciela(Kontakt osoba);
void edytujOsobe(vector <Kontakt>& ksiazkaAdresowa, string sciezkaKsiazki);
kontaktPoEdycjiPlusPowrot wykonajWybranaOpcjeEdycji(kontaktPoEdycjiPlusPowrot osobaDoEdycji, int wybranaOpcja);
string menuZalogowany(vector <Kontakt>& ksiazkaAdresowa, Uzytkownik u, string sciezkaUzytk, string sciezkaKsiazki);
void menuWyswietlKontakty();
int pobierzIdOstatniegoUzytkownika(string sciezkaUzytkownikow);
bool loginJestZajety(string login, string sciezkaDoPlikuZuzytkownikami);
void menuStartowe(string sciezkaUzytkownikow, string sciezkaKsiazki);
void zapiszUzytkownikaDoPliku(string sciezkaDoPlikuZuzytkownikami, Uzytkownik uzytkownik);
bool zapiszKsiazkePoUsunieciuLubEdycjiKontaktu(string sciezkaDoPliku, Kontakt kontakt, string tryb);
string zmienHasloiZapiszDoPliku(string sciezkaDoPliku, Uzytkownik u);
string pobierzNoweHaslo(Uzytkownik u);

bool czyJestNumerem(string tekst)
{
	for (int i = 0; i < tekst.length(); i++) {
		if (isdigit(tekst[i]) == 0) return false;
	}
	return true;
}

void menuWyswietlKontakty() {
	system("cls");
	cout << "KSIAZKA ADRESOWA" << endl << endl;
	cout << "---wyswietl kontakty---" << endl << endl;
	cout << "1. Wyswietl wszystkie kontakty" << endl;
	cout << "2. Wyszukaj konktakty po imieniu" << endl;
	cout << "3. Wyszukaj kontakty po nazwisku" << endl;
	cout << "4. Wyswietl przyjaciol" << endl;
	cout << "5. Wroc do glownego Menu" << endl;
	cout << "0. Wyjscie" << endl << endl;

	cout << "Wybierz, co chcesz zrobic: ";
}

int main() {
	
	int wybor = 0;
	string sciezkaDoKsiazkiAdresowej = "KsiazkaAdresowa.txt";
	string sciezkaUzytkownikow = "uzytkownicy.txt";
	menuStartowe("uzytkownicy.txt", "KsiazkaAdresowa.txt");

}
void zarejestrujSie(string sciezkaDoPlikuZuzytkownikami) {
	string login = "";
	string haslo = "";
	
	cout << endl << "Podaj login: ";
	cin.ignore();
	getline(cin, login);
	while (loginJestZajety(login, sciezkaDoPlikuZuzytkownikami) || login=="") {
		cout << "Podany login jest juz zajety / podano pusty ciag. Podaj nowy login: ";
		getline(cin, login);
	}
	
	while(haslo==""){
		cout << endl << "Podaj haslo: ";
		getline(cin, haslo);
	}

	int ostatnieId = pobierzIdOstatniegoUzytkownika(sciezkaDoPlikuZuzytkownikami);
	Uzytkownik nowyUzytkownik;
	if (ostatnieId == -1) return;
	nowyUzytkownik.id = ostatnieId + 1;
	nowyUzytkownik.login = login;
	nowyUzytkownik.haslo = haslo;
	zapiszUzytkownikaDoPliku(sciezkaDoPlikuZuzytkownikami, nowyUzytkownik);
}



void zapiszUzytkownikaDoPliku(string sciezkaDoPliku, Uzytkownik uzytkownik) {
	ofstream plik;
	plik.open(sciezkaDoPliku, ios_base::app);
	if (uzytkownik.id != 1) plik << "\n";
	plik << uzytkownik.id << "|" << uzytkownik.login << "|" << uzytkownik.haslo << "|";
	plik.close();
}
int pobierzIdOstatniegoUzytkownika(string sciezkaDoPliku) {
	ifstream ifs;
	char c;
	int id;
	string idStr = "";

	ifs.open(sciezkaDoPliku);
	if (!ifs) return 0;
	if (ifs.peek() == ifstream::traits_type::eof()) return 0;

	ifs.seekg(-1, ios::end);

	int pos = ifs.tellg();

	while (true) {
		ifs.seekg(pos);		
		ifs.get(c);
		if (pos == 0) {
			while (true) {
				ifs.seekg(pos);
				ifs.get(c);
				if (c == '|') {
					id = stoi(idStr); break;
				}
				idStr.push_back(c);
				pos++;
			}
			break;
		}
		if (char(c) == '\n') {
			ifs >> id;
			break;
		}
		pos--;		
	}	
	ifs.close();
	return id;

}
Uzytkownik pobierzDaneUzytkownika(string liniaPliku) {
	string id, login, haslo;
	string kawalek;
	Uzytkownik uzytkownik;

	int numerSeparatora = 0;

	for (int i = 0; i < liniaPliku.size(); i++) {
		
		if (liniaPliku[i] != '|') {
			kawalek += liniaPliku[i];
		}
		else {
			numerSeparatora++;
			switch (numerSeparatora) {
			case 1:
				uzytkownik.id = stoi(kawalek);
				kawalek = "";
				break;

			case 2:
				uzytkownik.login = kawalek;
				kawalek = "";
				break;
			case 3:
				uzytkownik.haslo = kawalek;
				kawalek = "";
				break;
			}			
		}		
	}
	return uzytkownik;
}


bool loginJestZajety(string login, string sciezkaDoPliku) {
	
	fstream plik;
	plik.open(sciezkaDoPliku, ios::in);
	string linia, kawalek;
	Uzytkownik u;
	if (plik.good()) {
		cout << "good";
		while (getline(plik, linia)) {
			u = pobierzDaneUzytkownika(linia);
			if (login == u.login) return true;
		}
		plik.close();
		return false;
	}
	else return false;
}
Uzytkownik zalogujSie(string sciezkaDoPlikuZuzytkownikami) {
	string login = "", haslo = "";
	Uzytkownik uzytkownik;
	uzytkownik.ok = false;

	cout << endl << "Podaj login: ";
	cin.ignore();
	getline(cin, login);
	while (login == "") {
		cout << "Podano pusty ciag. Podaj login: ";
		getline(cin, login);
	}
	cin.ignore();
	while (haslo == "") {
		cout << endl << "Podaj haslo: ";
		getline(cin, haslo);
	}
	
	fstream plik;
	string linia;
	plik.open(sciezkaDoPlikuZuzytkownikami, ios::in);
	if (plik.good()) {
		while (getline(plik, linia)) {
			uzytkownik = pobierzDaneUzytkownika(linia);
			uzytkownik.ok = false;
			if (uzytkownik.login == login && uzytkownik.haslo == haslo) {
				uzytkownik.ok = true;
				break;
			}
		}
		plik.close();
	}
	return uzytkownik;
}
void menuStartowe(string sciezkaUzytkownikow, string sciezkaKsiazki) {
	
	int wybor = 0;
	Uzytkownik uzytkownik;
	string poZalogowaniu = "";
	vector <Kontakt> ksiazkaAdresowa;
	system("cls");
	cout << "1. Logowanie" << endl;
	cout << "2. Rejestracja" << endl;
	cout << "0. Zamknij program" << endl;

	while (true) {
		cin >> wybor;
		switch (wybor) {
		case 1:
			uzytkownik=zalogujSie(sciezkaUzytkownikow);
			if (uzytkownik.ok) {
				cout << "---Zalogowano---";
				Sleep(1000);
				wczytajDaneDoKsiazki(ksiazkaAdresowa, sciezkaKsiazki, uzytkownik);
				poZalogowaniu = menuZalogowany(ksiazkaAdresowa, uzytkownik, sciezkaUzytkownikow, sciezkaKsiazki);
			}
			else {
				cout << "Brak uzytkownika w systemie. Zarejestruj sie";
				Sleep(1000);
				break;
			}
			if (poZalogowaniu == "wylogowano") menuStartowe(sciezkaUzytkownikow, sciezkaKsiazki);
			if (poZalogowaniu == "koniec programu") exit(0);
			break;
		case 2:
			zarejestrujSie(sciezkaUzytkownikow);
			cout << "---Pomyslnie zarejestrowano uzytkownika---";
			Sleep(1000);
			break;
		case 0:
			exit(0);
			break;
		default:
			cout << "Wybrano wartosc spoza zakresu, sprobuj jeszcze raz" << endl;
			Sleep(1000);
			break;
		}
		menuStartowe(sciezkaUzytkownikow, sciezkaKsiazki);
	}	
}

string menuZalogowany(vector <Kontakt>& ksiazkaAdresowa, Uzytkownik u, string sciezkaUzytk, string sciezkaKsiazki) {
	int wybor = 0;
	string noweHaslo;
	while (true) {
		wyswietlMenuZalogowany(u);
		cin >> wybor;
		switch (wybor) {
		case 1:
			wyswietlKontakty(ksiazkaAdresowa);
			break;
		case 2:
			if (dodajOsobe(ksiazkaAdresowa, u, sciezkaKsiazki) == -1) {
				cout << "Problem z dodaniem kontaktu. Nie dodano kontaktu";
				getchar(); getchar();
			}
			break;
		case 3:
			usunOsobe(ksiazkaAdresowa, sciezkaKsiazki);
			break;
		case 4:
			edytujOsobe(ksiazkaAdresowa, sciezkaKsiazki);
			break;
		case 5:
			noweHaslo= zmienHasloiZapiszDoPliku(sciezkaUzytk, u);
			if(noweHaslo!="") u.haslo=noweHaslo;
			else {
				cout << "Problem ze zmiana hasla. Haslo nie zostalo zmienione";
				getchar(); getchar();
			}
			break;
		case 6:
			return "wylogowano"; //wylogujSie
		case 0:
			return "koniec programu";
		default:
			cout << "Wybrano wartosc spoza zakresu, sprobuj jeszcze raz" << endl;
			Sleep(2000);
			break;
		}
	}
}

void wyswietlMenuZalogowany(Uzytkownik u) {
	system("cls");
	cout << "KSIAZKA ADRESOWA" << endl << endl;
	cout << "zalogowany jako: " << u.login << endl;
	cout << "MENU" << endl << endl;
	cout << "1. Wyswietl kontakty" << endl;
	cout << "2. Dodaj osobe do ksiazki" << endl;
	cout << "3. Usun osobe z ksiazki" << endl;
	cout << "4. Edytuj osobe z ksiazki" << endl;
	cout << "5. Zmien haslo" << endl;
	cout << "6. Wyloguj sie" << endl;
	cout << "0. Wyjscie" << endl << endl;

	cout << "Wybierz, co chcesz zrobic: ";
}

void wyswietlMenuEdycji(vector <Kontakt> ksiazkaAdresowa, int indeksKontaktu) {
	system("cls");
	wyswietlDane(ksiazkaAdresowa[indeksKontaktu]);
	cout << endl << endl;
	cout << "KSIAZKA ADRESOWA" << endl;
	cout << "---edycja---" << endl;
	cout << "1. imie" << endl;
	cout << "2. nazwisko" << endl;
	cout << "3. numer telefonu" << endl;
	cout << "4. email" << endl;
	cout << "5. adres" << endl;
	cout << "6. etykieta przyjaciel" << endl;
	cout << "7. powrot do glownego menu" << endl;
	cout << "0. Wyjscie" << endl << endl;
	cout << "Wybierz, ktora dana chcesz edytowac: ";
}

void wyswietlInfoBrakDanychwKsiazce() {
	cout << "W ksiazce kontaktowej nie ma jeszcze zadnych danych!" << endl << endl;
	cout << "Wcisnij dowolny przycisk, aby powrocic do glownego Menu" << endl;
	cin.ignore();
	getchar();
}

void wyswietlInfoBrakKontaktuoPodanymID() {
	cout << "-------------------------" << endl;
	cout << "Brak kontaktu o podanym ID." << endl;
	cout << "-------------------------" << endl;
	cin.ignore();
	getchar();
}

int zwrocIndeksKontaktuJesliIstnieje(vector <Kontakt> ksiazkaAdresowa, int wybraneID) {
	int indeksKontaktu = -1;

	for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
		if (wybraneID == ksiazkaAdresowa[i].id) {
			indeksKontaktu = i;
			break;
		}
	}
	return indeksKontaktu;
}
void wyswietlDaneOsobAbyWybracID(vector <Kontakt> ksiazkaAdresowa, string wiadomosc = "") {
	for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
		wyswietlDane(ksiazkaAdresowa[i]);
		cout << "-------------------------" << endl << endl;
	}
	cout << wiadomosc << endl;
	if (wiadomosc != "") cout << "Twoj wybor: ";
}

kontaktPoEdycjiPlusPowrot wykonajWybranaOpcjeEdycji(kontaktPoEdycjiPlusPowrot osobaOrazPowrot, int wybranaOpcja) {
	Kontakt osobaDoEdycji = osobaOrazPowrot.osoba;
	switch (wybranaOpcja) {
	case 1:
		osobaDoEdycji = edytujImieLubNazwisko(osobaDoEdycji, osobaDoEdycji.imie);
		break;
	case 2:
		osobaDoEdycji = edytujImieLubNazwisko(osobaDoEdycji, osobaDoEdycji.nazwisko);
		break;
	case 3:
		osobaDoEdycji = edytujNrTelEmailLubAdres(osobaDoEdycji, osobaDoEdycji.numerTelefonu);
		break;
	case 4:
		osobaDoEdycji = edytujNrTelEmailLubAdres(osobaDoEdycji, osobaDoEdycji.email);
		break;
	case 5:
		osobaDoEdycji = edytujNrTelEmailLubAdres(osobaDoEdycji, osobaDoEdycji.adres);
		break;
	case 6:
		osobaDoEdycji = edytujStatusPrzyjaciela(osobaDoEdycji);
		break;
	case 7:
		osobaOrazPowrot.powrot = 1;
		break;
	case 0:
		osobaOrazPowrot.powrot = 2;
		break;
	default:
		cout << "Wybrano wartosc spoza zakresu, sprobuj jeszcze raz" << endl;
		Sleep(2000);
		break;
	}

	osobaOrazPowrot.osoba = osobaDoEdycji;
	return osobaOrazPowrot;
}

void edytujOsobe(vector <Kontakt>& ksiazkaAdresowa, string sciezkaKsiazki) {

	int indeksWybranegoKontaktu = 0;
	string wybranaOpcja = "", wybraneID = "";

	if (ksiazkaAdresowa.empty()) {
		wyswietlInfoBrakDanychwKsiazce();
		return;
	}
	cout << "EDYTUJ OSOBE Z KONTAKTOW - WYBIERZ ID OSOBY" << endl << endl;
	wyswietlDaneOsobAbyWybracID(ksiazkaAdresowa, "Wybierz ID osoby, ktorej dane chcesz edytowac");
	cin >> wybraneID; cout << endl;
	if (!czyJestNumerem(wybraneID)) {
		cout << "Wprowadzono niepoprawne dane" << endl; Sleep(2000); return;
	}
	indeksWybranegoKontaktu = zwrocIndeksKontaktuJesliIstnieje(ksiazkaAdresowa, stoi(wybraneID));

	if (indeksWybranegoKontaktu == -1) {
		wyswietlInfoBrakKontaktuoPodanymID();
		return;
	}
	else {
		while (true) {

			wyswietlMenuEdycji(ksiazkaAdresowa, indeksWybranegoKontaktu);
			cin >> wybranaOpcja;
			if (czyJestNumerem(wybranaOpcja)) {
				kontaktPoEdycjiPlusPowrot osobaOrazPowrot;
				osobaOrazPowrot.osoba = ksiazkaAdresowa[indeksWybranegoKontaktu];
				osobaOrazPowrot.powrot = 0;

				osobaOrazPowrot = wykonajWybranaOpcjeEdycji(osobaOrazPowrot, stoi(wybranaOpcja));
				ksiazkaAdresowa[indeksWybranegoKontaktu] = osobaOrazPowrot.osoba;
				zapiszKsiazkePoUsunieciuLubEdycjiKontaktu(sciezkaKsiazki, ksiazkaAdresowa[indeksWybranegoKontaktu], "edycja");
				if (osobaOrazPowrot.powrot == 1) return;
				if (osobaOrazPowrot.powrot == 2) exit(0);
				
			}
			else cout << "Podano niepoprawne dane" << endl;
			Sleep(1000);
			system("cls");
		}
	}
}

Kontakt edytujImieLubNazwisko(Kontakt osoba, string poleStrukturyOsoba) {
	string rodzajDanej = "";
	if (poleStrukturyOsoba == osoba.imie) rodzajDanej = "imie";
	else rodzajDanej = "nazwisko";

	string wybor = "";

	system("cls");
	cout << "--edycja--!!!" << endl;
	cout << "Rodzaj danej do zmiany: " + rodzajDanej << endl;
	cout << "Stare " + rodzajDanej + ": " + poleStrukturyOsoba;
	cout << endl << "Wprowadz nowe " + rodzajDanej + ": ";

	cin.ignore();
	getline(cin, wybor);
	cout << endl << endl << "Nowe " + rodzajDanej + ": " + wybor << endl;
	Sleep(1000);

	if (rodzajDanej == "imie") osoba.imie = wybor;
	else osoba.nazwisko = wybor;
	return osoba;
}

Kontakt edytujNrTelEmailLubAdres(Kontakt osoba, string poleStrukturyOsoba) {
	string rodzajDanej = "", wybor = "";

	if (poleStrukturyOsoba == osoba.numerTelefonu) rodzajDanej = "numer telefonu";
	else if (poleStrukturyOsoba == osoba.email) rodzajDanej = "email";
	else rodzajDanej = "adres";

	system("cls");
	cout << "--edycja--" << endl;
	cout << "Stary " + rodzajDanej + ": " + poleStrukturyOsoba << endl;
	cout << endl << "Wprowadz nowy " + rodzajDanej + ": ";

	cin.ignore();
	getline(cin, wybor);

	cout << endl << endl << "Nowy " + rodzajDanej + ": " + wybor << endl;
	Sleep(1000);

	if (rodzajDanej == "numer telefonu") osoba.numerTelefonu = wybor;
	else if (rodzajDanej == "adres") osoba.adres = wybor;
	else osoba.email = wybor;
	return osoba;
}

Kontakt edytujStatusPrzyjaciela(Kontakt osoba) {
	string wybor = "";
	system("cls");
	cout << "--edycja--" << endl;
	cout << "Stary status przyjaciela: ";
	if (osoba.przyjaciel == true) cout << "przyjaciel";
	else cout << "znajomy";
	cout << "\nCzy chcesz zmienic status przyjaciela? t/n" << endl;
	cout << "Twoj wybor: ";
	cin.ignore();
	getline(cin, wybor);

	if (wybor == "t") {
		osoba.przyjaciel = !osoba.przyjaciel;
		cout << endl << endl << "Nowy status przyjaciela: ";
		if (osoba.przyjaciel == true) cout << "przyjaciel";
		else cout << "znajomy";
	}
	else cout << endl << endl << "Nie zmieniono statusu przyjaciela ";

	Sleep(1000);

	return osoba;
}

int dodajOsobe(vector <Kontakt>& ksiazkaAdresowa, Uzytkownik u, string sciezkaKsiazki) {
	Kontakt osoba;
	string imie, nazwisko, adres, przyjacielStr, email, telefon;
	int id = 0;
	bool przyjaciel = false, nowyKontakt = false;
	cin.ignore();
	cout << "Podaj imie: ";	getline(cin, imie); cin.ignore();
	cout << "\nPodaj nazwisko: ";	getline(cin, nazwisko); cin.ignore();
	cout << "\nPodaj numer telefonu: ";	getline(cin, telefon); cin.ignore();
	cout << "\nPodaj adres e-mail: ";	getline(cin, email); cin.ignore();
	cout << "\nPodaj adres: ";	getline(cin, adres); cin.ignore();
	cout << "\nDodac etykiete \"przyjaciel\"? t/n: ";	getline(cin, przyjacielStr);

	if (przyjacielStr == "t" || przyjacielStr == "T") przyjaciel = true;
	else przyjaciel = false;

	osoba.imie = imie;
	osoba.nazwisko = nazwisko;
	osoba.numerTelefonu = telefon;
	osoba.adres = adres;
	osoba.przyjaciel = przyjaciel;
	osoba.email = email;
	osoba.idUzytkownika = u.id;

	int ostatnieId=pobierzIdOstatniegoUzytkownika(sciezkaKsiazki);
	if (ostatnieId != -1) osoba.id = ostatnieId + 1;
	else return -1;

	if (ksiazkaAdresowa.size() > 0) {
		if (!sprawdzCzyKontaktJuzIstnieje(ksiazkaAdresowa, osoba)) nowyKontakt = true;
		else {
			cout << "!!! Nie dodano kontaktu, kontakt juz istnieje !!!\n\n";
			Sleep(2000);
		}
	}
	else nowyKontakt = true;

	if (nowyKontakt) {
		ksiazkaAdresowa.push_back(osoba);
		cout << "Pomyslnie dodano kontakt. Szczegoly kontaktu:\n\n";
		wyswietlDane(osoba);
		cin.ignore();
		zapiszOsobaDoPliku(osoba, sciezkaKsiazki);
		
		getchar();
		return 0;
	}
	return -1;
}
bool zapiszKsiazkePoUsunieciuLubEdycjiKontaktu(string sciezkaDoPliku, Kontakt kontakt, string tryb) {
	
	fstream plikOryginalny;
	ofstream plikTymczasowy;
	string sciezkaPlikuTymczasowego = "KsiazkaAdresowa_tymczas.txt", linia;
	Uzytkownik uzytkownik;
	uzytkownik.id = -1;
	Kontakt osoba;
	char* char_arr_sciezkaOryginalna= &sciezkaDoPliku[0];
	char* char_arr_sciezkaTymczas = &sciezkaPlikuTymczasowego[0];

	int nrOdczytanejLinii = 0;
	plikOryginalny.open(sciezkaDoPliku, ios::in);
	plikTymczasowy.open(sciezkaPlikuTymczasowego);

	if (plikOryginalny.good()) {

		while (getline(plikOryginalny, linia)) {
			nrOdczytanejLinii++;
			osoba = rozdzielDane(linia, uzytkownik);
			if (tryb == "usuniecie") {
				if (!kontaktySaTakieSame(osoba, kontakt)) {
					if (nrOdczytanejLinii != 1) plikTymczasowy << endl;
					plikTymczasowy << linia;
				}
			}
			else if (tryb == "edycja") {
				if (nrOdczytanejLinii != 1) plikTymczasowy << endl;
				if (osoba.id == kontakt.id) {
					plikTymczasowy << kontakt.id << "|";
					plikTymczasowy << kontakt.idUzytkownika << "|";
					plikTymczasowy << kontakt.imie << "|";
					plikTymczasowy << kontakt.nazwisko << "|";
					plikTymczasowy << kontakt.numerTelefonu << "|";
					plikTymczasowy << kontakt.email << "|";
					plikTymczasowy << kontakt.adres << "|";
					if (kontakt.przyjaciel)
						plikTymczasowy << "tak" << "|";
					else
						plikTymczasowy << "nie" << "|";
				}
				else plikTymczasowy << linia;
			}
			
		}
		plikTymczasowy.close();
		plikOryginalny.close();
		remove(char_arr_sciezkaOryginalna);
		rename(char_arr_sciezkaTymczas, char_arr_sciezkaOryginalna);
		return true;
	}
	else return false;
}
string zmienHasloiZapiszDoPliku(string sciezkaDoPlikuzUzytkownikami, Uzytkownik uzytkownikDoZmiany) {
	string noweHaslo = pobierzNoweHaslo(uzytkownikDoZmiany);
	getchar();getchar();
	Uzytkownik uzytkownik;
	fstream plikOryginalny;
	ofstream plikTymczasowy;
	string linia, sciezkaPlikuTymczasowego = "uzytkownicy_tymczas.txt";
	char* char_arr_sciezkaOryginalna = &sciezkaDoPlikuzUzytkownikami[0];
	char* char_arr_sciezkaTymczas = &sciezkaPlikuTymczasowego[0];

	int nrOdczytanejLinii = 0;
	plikOryginalny.open(sciezkaDoPlikuzUzytkownikami, ios::in);
	plikTymczasowy.open(sciezkaPlikuTymczasowego);

	if (plikOryginalny.good()) {
		while (getline(plikOryginalny, linia)) {
			nrOdczytanejLinii++;
			uzytkownik = pobierzDaneUzytkownika(linia);
			if (nrOdczytanejLinii != 1) plikTymczasowy << endl;
			plikTymczasowy << uzytkownik.id << "|" << uzytkownik.login << "|";
			if (uzytkownik.id == uzytkownikDoZmiany.id) plikTymczasowy << noweHaslo << "|";
			else plikTymczasowy << uzytkownik.haslo << "|";
		}
		plikTymczasowy.close();
		plikOryginalny.close();
		remove(char_arr_sciezkaOryginalna);
		rename(char_arr_sciezkaTymczas, char_arr_sciezkaOryginalna);
		return noweHaslo;
	}

	else return "";
}
string pobierzNoweHaslo(Uzytkownik uzytkownik) {
	string haslo = "";
	string stareHaslo = "";
	cout << "--ZMIANA HASLA--" << endl;
	cout << "Aby ustawic nowe haslo, najpierw prosze podac stare haslo" << endl;
	while (stareHaslo != uzytkownik.haslo) {
		cout << endl << "Podaj stare haslo: ";
		cin.ignore(); getline(cin, stareHaslo);
		if (stareHaslo != uzytkownik.haslo) cout << "Wprowadzono niepoprawne stare haslo. Sprobuj jeszcze raz" << endl;
	}
	cout << "Poprawnie wprowadzono stare haslo" << endl;
	while (haslo == "") {
		cout << endl << "Podaj nowe haslo: ";
		getline(cin, haslo);
	}
	return haslo;
}
bool kontaktySaTakieSame(Kontakt kontakt1, Kontakt kontakt2) {
	if (kontakt1.id == kontakt2.id
		&& kontakt1.idUzytkownika == kontakt2.idUzytkownika
		&& kontakt1.imie == kontakt2.imie
		&& kontakt1.nazwisko == kontakt2.nazwisko
		&& kontakt1.adres == kontakt2.adres
		&& kontakt1.numerTelefonu == kontakt2.numerTelefonu
		&& kontakt1.email == kontakt2.email
		&& kontakt1.przyjaciel == kontakt2.przyjaciel) return true;
	return false;
}
void usunOsobe(vector <Kontakt>& ksiazkaAdresowa, string sciezkaKsiazki) {

	int indeksUsuwanegoKontaktu = -1;
	string wybor = "";
	Kontakt kontaktDoUsuniecia;
	cout << "USUN OSOBE Z KONTAKTOW - WYBIERZ ID OSOBY" << endl << endl;
	if (ksiazkaAdresowa.size() == 0) {
		cout << "Ksiazka adresowa jest pusta - wcisnij dowolny przycisk, aby powrocic do glownego Menu";
		cin.ignore();
		getchar();
		return;
	}
	wyswietlDaneOsobAbyWybracID(ksiazkaAdresowa, "Wybierz ID osoby, ktora chcesz usunac z ksiazki");

	cin >> wybor; cout << endl;
	if (!czyJestNumerem(wybor)) {
		cout << "Podano niepoprawne dane" << endl;
		Sleep(2000); system("cls"); return;
	}
	if (stoi(wybor) == ksiazkaAdresowa[ksiazkaAdresowa.size() - 1].id)
	{
		indeksUsuwanegoKontaktu = ksiazkaAdresowa.size() - 1;
		kontaktDoUsuniecia = ksiazkaAdresowa[indeksUsuwanegoKontaktu];
		cout << "kontakt do usuniecia: " << kontaktDoUsuniecia.imie;
	}
	else {
		for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
			if (ksiazkaAdresowa[i].id == stoi(wybor)) {
				indeksUsuwanegoKontaktu = i;
				kontaktDoUsuniecia = ksiazkaAdresowa[i];
				break;
			}
		}
	}
	if (indeksUsuwanegoKontaktu != -1) {
		if(usunWybranaOsobeZwektora(ksiazkaAdresowa, indeksUsuwanegoKontaktu)) zapiszKsiazkePoUsunieciuLubEdycjiKontaktu(sciezkaKsiazki, kontaktDoUsuniecia, "usuniecie");
	}
	else {
		cout << "-------------------------" << endl;
		cout << "Brak kontaktu o podanym ID." << endl;
		cout << "Kontakt nie zostal usuniety." << endl;
		cout << "Nacisnij dowolny klawisz, aby wrocic do glownego Menu" << endl;
		cout << "-------------------------" << endl;
	}

	cin.ignore();
	getchar();
	return;
}

bool usunWybranaOsobeZwektora(vector <Kontakt>& ksiazkaAdresowa, int indeksUsuwanegoKontaktu) {
	string wyborStr = "";
	cout << "Czy na pewno chcesz usunac ponizszy kontakt? t/n" << endl << endl;

	cout << "-------------------------" << endl;
	wyswietlDane(ksiazkaAdresowa[indeksUsuwanegoKontaktu]);
	cout << "-------------------------" << endl << endl;
	cout << "czlowiek do usuniecia: " << ksiazkaAdresowa[indeksUsuwanegoKontaktu].imie;
	cin >> wyborStr;
	if (wyborStr == "t") {
		ksiazkaAdresowa.erase(ksiazkaAdresowa.begin() + indeksUsuwanegoKontaktu);
		system("cls");
		cout << "Pomyslnie usunieto kontakt. Nacisnij dowolny klawisz, aby wrocic do glownego menu.";
		return true;
	}
	else {
		cout << "-------------------------" << endl;
		cout << "Kontakt nie zostal usuniety" << endl;
		cout << "Nacisnij dowolny klawisz, aby wrocic do glownego Menu" << endl;
		cout << "-------------------------" << endl;
		return false;
	}
}

bool sprawdzCzyKontaktJuzIstnieje(vector <Kontakt> ksiazkaAdresowa, Kontakt test) {
	Kontakt kontakt;

	for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
		kontakt = ksiazkaAdresowa[i];

		if (kontakt.imie == test.imie && kontakt.nazwisko == test.nazwisko && kontakt.adres == test.adres && kontakt.email == test.email
			&& kontakt.numerTelefonu == test.numerTelefonu)
			return true;
	}
	return false;
}

void wyswietlDane(Kontakt osoba) {
	cout << "Id: " << osoba.id << endl;
	cout << "Imie: " << osoba.imie << endl;
	cout << "Nazwisko: " << osoba.nazwisko << endl;
	cout << "Adres: " << osoba.adres << endl;
	cout << "Numer telefonu: " << osoba.numerTelefonu << endl;
	cout << "E-mail: " << osoba.email << endl;
	if (osoba.przyjaciel) {
		cout << "Kategoria: przyjaciel" << endl;
	}
	else {
		cout << "Kategoria: brak" << endl;
	}
}
bool wczytajDaneDoKsiazki(vector <Kontakt>& ksiazkaAdresowa, string sciezkaDoPliku, Uzytkownik uzytkownik) {

	fstream plik;
	string linia, imie, nazwisko, email, adres, telefon, dane;
	int nrLinii = 1;
	int reszta, id;
	bool przyjaciel, ostatni;
	Kontakt osoba;
	ostatni = false;
	plik.open(sciezkaDoPliku, ios::in);
	if (plik.good()) {

		while (getline(plik, linia)) {

			dane = linia;

			osoba = rozdzielDane(dane, uzytkownik, true);
			if (osoba.ok) ksiazkaAdresowa.push_back(osoba);


		}
		plik.close();
		return true;

	}
	else {

		return false;
	}

}

Kontakt rozdzielDane(string dane, Uzytkownik uzytkownik, bool tylkoDaneKonkretnegoUzytkownika) {
	string kawalek = "";
	int numer_separatora = 1, intKawalek;
	Kontakt kontakt;

	for (int i = 0; i < dane.size(); i++) {
		if (dane[i] != '|') {
			kawalek += dane[i];
		}
		else {
			switch (numer_separatora) {
			case 1:
				kontakt.id = atoi(kawalek.c_str());
				break;
			case 2:
				intKawalek = atoi(kawalek.c_str());
				if (intKawalek !=uzytkownik.id && tylkoDaneKonkretnegoUzytkownika==true) {
					kontakt.ok = false;
					return kontakt;
				}
				else {
					kontakt.ok = true;
					kontakt.idUzytkownika = intKawalek;
				}
				break;
			case 3:
				kontakt.imie = kawalek;
				break;
			case 4:
				kontakt.nazwisko = kawalek;
				break;
			case 5:
				kontakt.numerTelefonu = kawalek;
				break;
			case 6:
				kontakt.email = kawalek;
				break;
			case 7:
				kontakt.adres = kawalek;
				break;
			case 8:
				if (kawalek == "tak") {
					kontakt.przyjaciel = true;
				}
				else
					kontakt.przyjaciel = false;
				break;
			}
			numer_separatora++;
			kawalek = "";
		}


	}

	return kontakt;
}

void zapiszOsobaDoPliku(Kontakt osoba, string sciezka) {
	fstream plik;
	
	plik.open(sciezka, ios_base::app);
	if (osoba.id != 1) plik << "\n";
	plik<< osoba.id << "|";
	plik << osoba.idUzytkownika << "|";
	plik << osoba.imie << "|";
	plik << osoba.nazwisko << "|";
	plik << osoba.numerTelefonu << "|";
	plik << osoba.email << "|";
	plik << osoba.adres << "|";
	if (osoba.przyjaciel)
		plik << "tak" << "|";
	else
		plik << "nie" << "|";

	plik.close();
}
void wyswietlKontakty(vector <Kontakt>& ksiazkaAdresowa) {
	int wybor = 0;
	if (ksiazkaAdresowa.size() > 0) {
		while (true) {
			menuWyswietlKontakty();
			cin >> wybor;

			switch (wybor) {
			case 1:
				wyswietlDaneOsobAbyWybracID(ksiazkaAdresowa);
				cin.ignore();
				getchar();
				break;
			case 2:
				wyszukajPoImieniuLubNazwisku(ksiazkaAdresowa, "imie");
				break;
			case 3:
				wyszukajPoImieniuLubNazwisku(ksiazkaAdresowa, "nazwisko");
				break;
			case 4:
				wyszukajPrzyjaciol(ksiazkaAdresowa);
				break;
			case 5:
				return;
			case 0:
				exit(0);
				break;
			default:
				cout << "Wybrano wartosc spoza zakresu, sprobuj jeszcze raz" << endl;
				Sleep(2000);
				break;
			}
		}

	}
	else {
		cout << "W ksiazce kontaktowej nie ma jeszcze zadnych danych!" << endl << endl;
		cout << "Wcisnij dowolny przycisk, aby powrocic do glownego Menu" << endl;
		cin.ignore();
		getchar();

	}

}

void wyszukajPoImieniuLubNazwisku(vector <Kontakt>& ksiazkaAdresowa, string parametr) {
	int count = 0;
	string wprowadzonaDanaDoSprawdzenia = "", danaZksiazkiAdresowej = "", odpowiedz = "n";
	while (odpowiedz != "t") {
		cout << "Wpisz " << parametr << ": ";
		cin >> wprowadzonaDanaDoSprawdzenia;

		cout << "\nCzy na pewno wyswietlic wszystkie konktakty ";
		if (parametr == "imie") cout << "o imieniu " << wprowadzonaDanaDoSprawdzenia << " ? t / n ";
		else cout << "o nazwisku " << wprowadzonaDanaDoSprawdzenia << " ? t / n ";

		cin >> odpowiedz;
		cout << endl;
	}

	transform(wprowadzonaDanaDoSprawdzenia.begin(), wprowadzonaDanaDoSprawdzenia.end(), wprowadzonaDanaDoSprawdzenia.begin(), ::toupper);

	for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
		if (parametr == "imie") danaZksiazkiAdresowej = ksiazkaAdresowa[i].imie;
		else danaZksiazkiAdresowej = ksiazkaAdresowa[i].nazwisko;

		transform(danaZksiazkiAdresowej.begin(), danaZksiazkiAdresowej.end(), danaZksiazkiAdresowej.begin(), ::toupper);
		if (wprowadzonaDanaDoSprawdzenia == danaZksiazkiAdresowej) {
			cout << "------------------" << endl << endl;
			wyswietlDane(ksiazkaAdresowa[i]);
			cout << endl;

			count++;
		}
	}
	if (count == 0) {
		if (parametr == "imie") cout << "Brak w ksiazce osoby o imieniu " << wprowadzonaDanaDoSprawdzenia;
		else cout << "Brak w ksiazce osoby o nazwisku " << wprowadzonaDanaDoSprawdzenia;
	}

	cin.ignore();
	getchar();
}
void wyszukajPrzyjaciol(vector <Kontakt>& ksiazkaAdresowa) {
	int count = 0;

	for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
		if (ksiazkaAdresowa[i].przyjaciel == true) {
			cout << "------------------" << endl << endl;
			wyswietlDane(ksiazkaAdresowa[i]);
			cout << endl;
			count++;
		}
	}
	if (count == 0) {
		cout << "Brak w ksiazce osob o etykiecie \"przyjaciel\"";
	}
	cin.ignore();
	getchar();
}







