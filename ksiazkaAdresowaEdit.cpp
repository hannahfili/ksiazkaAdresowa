#include <iostream>
#include <vector>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <algorithm>

using namespace std;

struct Osoba {
	int id;
	string imie, nazwisko, adres, numerTelefonu, email;
	bool przyjaciel;
};
struct powrotDoGlownegoMenu {
	Osoba osoba;
	bool powrot;
};

void dodajOsobe(vector <Osoba>& ksiazkaAdresowa);
void usunOsobe(vector <Osoba>& ksiazkaAdresowa);
void usunWybranaOsobe(vector <Osoba>& ksiazkaAdresowa, int indeksUsuwanegoKontaktu);
bool sprawdzCzyKontaktJuzIstnieje(vector <Osoba> ksiazkaAdresowa, Osoba test);
void wyswietlDane(Osoba osoba);
void wyswietlDaneOsobAbyWybracID(vector <Osoba> ksiazkaAdresowa, string wiadomosc);
bool wczytajDaneDoKsiazki(vector <Osoba>& ksiazkaAdresowa, string sciezkaDoPliku);
void zapiszKsiazkeDoPliku(vector <Osoba>& ksiazkaAdresowa);
void wyswietlKontakty(vector <Osoba>& ksiazkaAdresowa);
void wyszukajPrzyjaciol(vector <Osoba>& ksiazkaAdresowa);
void wyszukajPoImieniuLubNazwisku(vector <Osoba>& ksiazkaAdresowa, string parametr);
Osoba rozdzielDane(string);
Osoba edytujImieLubNazwisko(Osoba osoba, string poleStrukturyOsoba);
Osoba edytujNrTelEmailLubAdres(Osoba osoba, string poleStrukturyOsoba);
Osoba edytujStatusPrzyjaciela(Osoba osoba);
void edytujOsobe(vector <Osoba>& ksiazkaAdresowa);
powrotDoGlownegoMenu wykonajWybranaOpcjeEdycji(powrotDoGlownegoMenu osobaDoEdycji, int wybranaOpcja);
void menuGlowne();
void menuWyswietlKontakty();

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
	vector <Osoba> ksiazkaAdresowa;
	int wybor = 0;
	string sciezkaDoPliku = "KsiazkaAdresowa.txt";
	wczytajDaneDoKsiazki(ksiazkaAdresowa, sciezkaDoPliku);
	while (true) {
		menuGlowne();
		cin >> wybor;
		switch (wybor) {
		case 1:
			wyswietlKontakty(ksiazkaAdresowa);
			break;
		case 2:
			dodajOsobe(ksiazkaAdresowa);
			zapiszKsiazkeDoPliku(ksiazkaAdresowa);
			break;
		case 3:
			usunOsobe(ksiazkaAdresowa);
			zapiszKsiazkeDoPliku(ksiazkaAdresowa);
			break;
		case 4:
			edytujOsobe(ksiazkaAdresowa);
			zapiszKsiazkeDoPliku(ksiazkaAdresowa);
			break;
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

void menuGlowne() {
	system("cls");
	cout << "KSIAZKA ADRESOWA" << endl << endl << endl;
	cout << "MENU" << endl << endl;
	cout << "1. Wyswietl kontakty" << endl;
	cout << "2. Dodaj osobe do ksiazki" << endl;
	cout << "3. Usun osobe z ksiazki" << endl;
	cout << "4. Edytuj osobe z ksiazki" << endl;
	cout << "0. Wyjscie" << endl << endl;

	cout << "Wybierz, co chcesz zrobic: ";
}

void wyswietlMenuEdycji(vector <Osoba> ksiazkaAdresowa, int indeksKontaktu) {
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

int zwrocIndeksKontaktuJesliIstnieje(vector <Osoba> ksiazkaAdresowa, int wybraneID) {
	int indeksKontaktu = -1;

	for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
		if (wybraneID == ksiazkaAdresowa[i].id) {
			indeksKontaktu = i;
			break;
		}
	}
	return indeksKontaktu;
}
void wyswietlDaneOsobAbyWybracID(vector <Osoba> ksiazkaAdresowa, string wiadomosc = "") {


	for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
		wyswietlDane(ksiazkaAdresowa[i]);
		cout << "-------------------------" << endl << endl;
	}
	cout << wiadomosc << endl;
	if (wiadomosc!="") cout << "Twoj wybor: ";
}

powrotDoGlownegoMenu wykonajWybranaOpcjeEdycji(powrotDoGlownegoMenu osobaOrazPowrot, int wybranaOpcja) {
	Osoba osobaDoEdycji = osobaOrazPowrot.osoba;
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
		osobaOrazPowrot.powrot = true;
		break;
	case 0:
		exit(0);
		break;
	default:
		cout << "Wybrano wartosc spoza zakresu, sprobuj jeszcze raz" << endl;
		Sleep(2000);
		break;
	}

	osobaOrazPowrot.osoba = osobaDoEdycji;
	return osobaOrazPowrot;
}

void edytujOsobe(vector <Osoba>& ksiazkaAdresowa) {

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
				powrotDoGlownegoMenu osobaOrazPowrot;
				osobaOrazPowrot.osoba = ksiazkaAdresowa[indeksWybranegoKontaktu];
				osobaOrazPowrot.powrot = false;

				osobaOrazPowrot = wykonajWybranaOpcjeEdycji(osobaOrazPowrot, stoi(wybranaOpcja));
				ksiazkaAdresowa[indeksWybranegoKontaktu] = osobaOrazPowrot.osoba;

				if (osobaOrazPowrot.powrot == true)
					return;
			}
			else cout << "Podano niepoprawne dane" << endl;
			Sleep(2000);
			system("cls");
			
		}

	}
}

Osoba edytujImieLubNazwisko(Osoba osoba, string poleStrukturyOsoba) {
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
	Sleep(2000);

	if (rodzajDanej == "imie") osoba.imie = wybor;
	else osoba.nazwisko = wybor;
	return osoba;
}

Osoba edytujNrTelEmailLubAdres(Osoba osoba, string poleStrukturyOsoba) {
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
	Sleep(2000);

	if (rodzajDanej == "numer telefonu") osoba.numerTelefonu = wybor;
	else if (rodzajDanej == "adres") osoba.adres = wybor;
	else osoba.email = wybor;
	return osoba;
}

Osoba edytujStatusPrzyjaciela(Osoba osoba) {
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

	Sleep(2000);

	return osoba;
}

void dodajOsobe(vector <Osoba>& ksiazkaAdresowa) {
	Osoba osoba;
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

	if (ksiazkaAdresowa.empty()) osoba.id = 1;
	else osoba.id = ksiazkaAdresowa[ksiazkaAdresowa.size() - 1].id + 1;

	if (ksiazkaAdresowa.size() > 0) {
		if (!sprawdzCzyKontaktJuzIstnieje(ksiazkaAdresowa, osoba)) {
			nowyKontakt = true;
		}
		else {
			cout << "!!! Nie dodano kontaktu, kontakt juz istnieje !!!\n\n";
			Sleep(2000);
		}
	}
	if (ksiazkaAdresowa.empty() || nowyKontakt) {
		ksiazkaAdresowa.push_back(osoba);
		cout << "Pomyslnie dodano kontakt. Szczegoly kontaktu:\n\n";
		wyswietlDane(osoba);
		cin.ignore();
		getchar();
	}
}
void usunOsobe(vector <Osoba>& ksiazkaAdresowa) {

	int indeksUsuwanegoKontaktu = -1;
	string wybor = "";
	cout << "USUN OSOBE Z KONTAKTOW - WYBIERZ ID OSOBY" << endl << endl;
	if (ksiazkaAdresowa.size() == 0) {
		cout << "Ksiazka adresowa jest pusta - wcisnij dowolny przycisk, aby powrocic do glownego Menu";
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
		indeksUsuwanegoKontaktu = ksiazkaAdresowa.size() - 1;
	else {
		for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
			if (ksiazkaAdresowa[i].id == stoi(wybor)) indeksUsuwanegoKontaktu = i;
		}
	}

	if (indeksUsuwanegoKontaktu != -1) {
		usunWybranaOsobe(ksiazkaAdresowa, indeksUsuwanegoKontaktu);
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

void usunWybranaOsobe(vector <Osoba>& ksiazkaAdresowa, int indeksUsuwanegoKontaktu) {
	string wyborStr = "";
	cout << "Czy na pewno chcesz usunac ponizszy kontakt? t/n" << endl << endl;

	cout << "-------------------------" << endl;
	wyswietlDane(ksiazkaAdresowa[indeksUsuwanegoKontaktu]);
	cout << "-------------------------" << endl << endl;

	cin >> wyborStr;
	if (wyborStr == "t") {
		ksiazkaAdresowa.erase(ksiazkaAdresowa.begin() + indeksUsuwanegoKontaktu);
		system("cls");
		cout << "Pomyslnie usunieto kontakt. Nacisnij dowolny klawisz, aby wrocic do glownego menu.";
	}
	else {
		cout << "-------------------------" << endl;
		cout << "Kontakt nie zostal usuniety" << endl;
		cout << "Nacisnij dowolny klawisz, aby wrocic do glownego Menu" << endl;
		cout << "-------------------------" << endl;
	}
}

bool sprawdzCzyKontaktJuzIstnieje(vector <Osoba> ksiazkaAdresowa, Osoba test) {
	Osoba kontakt;

	for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
		kontakt = ksiazkaAdresowa[i];

		if (kontakt.imie == test.imie && kontakt.nazwisko == test.nazwisko && kontakt.adres == test.adres && kontakt.email == test.email
			&& kontakt.numerTelefonu == test.numerTelefonu)
			return true;
	}
	return false;
}

void wyswietlDane(Osoba osoba) {
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
bool wczytajDaneDoKsiazki(vector <Osoba>& ksiazkaAdresowa, string sciezkaDoPliku) {

	fstream plik;
	string linia, imie, nazwisko, email, adres, telefon, dane;
	int nrLinii = 1;
	int reszta, id;
	bool przyjaciel, ostatni;
	Osoba osoba;
	ostatni = false;
	plik.open(sciezkaDoPliku, ios::in);
	if (plik.good()) {

		while (getline(plik, linia)) {

			dane = linia;

			osoba = rozdzielDane(dane);
			ksiazkaAdresowa.push_back(osoba);

		}
		plik.close();
		return true;

	}
	else {

		return false;
	}

}

Osoba rozdzielDane(string dane) {
	string kawalek = "";
	int numer_separatora = 1;
	Osoba kontakt;
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
				kontakt.imie = kawalek;
				break;
			case 3:
				kontakt.nazwisko = kawalek;
				break;
			case 4:
				kontakt.numerTelefonu = kawalek;
				break;
			case 5:
				kontakt.email = kawalek;
				break;
			case 6:
				kontakt.adres = kawalek;
				break;
			case 7:
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

void zapiszKsiazkeDoPliku(vector <Osoba>& ksiazkaAdresowa) {
	fstream plik;
	getchar();
	plik.open("KsiazkaAdresowa.txt", ios::trunc);
	plik.close();

	plik.open("KsiazkaAdresowa.txt", ios::out);

	for (int i = 0; i < ksiazkaAdresowa.size(); i++) {
		plik << ksiazkaAdresowa[i].id << "|";
		plik << ksiazkaAdresowa[i].imie << "|";
		plik << ksiazkaAdresowa[i].nazwisko << "|";
		plik << ksiazkaAdresowa[i].numerTelefonu << "|";
		plik << ksiazkaAdresowa[i].email << "|";
		plik << ksiazkaAdresowa[i].adres << "|";
		if (ksiazkaAdresowa[i].przyjaciel)
			plik << "tak" << "|";
		else
			plik << "nie" << "|";
		plik << endl;
	}
	plik.close();
}
void wyswietlKontakty(vector <Osoba>& ksiazkaAdresowa) {
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

void wyszukajPoImieniuLubNazwisku(vector <Osoba>& ksiazkaAdresowa, string parametr) {
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
void wyszukajPrzyjaciol(vector <Osoba>& ksiazkaAdresowa) {
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







