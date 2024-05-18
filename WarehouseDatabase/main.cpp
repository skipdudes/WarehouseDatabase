#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct record
{
	string rodzajPrzewodu = "";
	int numerRegalu = 0;
	int polkaRegalu = 0;
	int liczbaSztuk = 0;
};

bool isRecordEmpty(record databaseRecord)
{
	if (databaseRecord.rodzajPrzewodu == "" && databaseRecord.numerRegalu == 0
		&& databaseRecord.polkaRegalu == 0 && databaseRecord.liczbaSztuk == 0)
		return true;
	return false;
}

void addRecord(record* database, const int size, string rodzajPrzewodu,
	int numerRegalu, int polkaRegalu, int liczbaSztuk)
{
	int i;

	for (i = 0; i < size; i++)
	{
		if (isRecordEmpty(database[i]))
			break;
	}

	if (i == size)
	{
		cout << endl << "[Blad]: Proba przekroczenia maksymalnego rozmiaru bazy danych!" << endl;
		return;
	}

	// Istnieje wolne miejsce w bazie danych pod indeksem i
	database[i].rodzajPrzewodu = rodzajPrzewodu;
	database[i].numerRegalu = numerRegalu;
	database[i].polkaRegalu = polkaRegalu;
	database[i].liczbaSztuk = liczbaSztuk;

	cout << endl << "[Sukces]: Dodano '" << rodzajPrzewodu << "' do bazy danych!" << endl;
}

void moveEmptyRecords(record* database, const int size)
{
	for (int i = 1; i < size; i++)
	{
		for (int j = size - 1; j >= 1; j--)
		{
			if (isRecordEmpty(database[j - 1]))
			{
				record tempRecord = database[j - 1];
				database[j - 1] = database[j];
				database[j] = tempRecord;
			}
		}
	}
}

void removeRecord(record* database, const int size, int i)
{
	if (i < 0 || i >= size)
	{
		cout << endl << "[Blad]: Proba przekroczenia rozmiaru bazy danych!" << endl;
		return;
	}

	if (isRecordEmpty(database[i]))
	{
		cout << endl << "[Blad]: Ten rekord jest pusty!" << endl;
		return;
	}

	// Istnieje niepusty rekord w indeksie i
	string tmp = database[i].rodzajPrzewodu;
	database[i].rodzajPrzewodu = "";
	database[i].numerRegalu = 0;
	database[i].polkaRegalu = 0;
	database[i].liczbaSztuk = 0;

	// Przesuniecie pustego rekordu na sam dol bazy danych
	moveEmptyRecords(database, size);

	cout << endl << "[Sukces]: Usunieto '" << tmp << "' z bazy danych!" << endl;
}

void printSingleRecord(record databaseRecord, int i)
{
	cout << "Rekord #" << i + 1 << ": ";

	if (isRecordEmpty(databaseRecord))
		cout << "Pusty rekord" << endl;
	else
	{
		cout << databaseRecord.rodzajPrzewodu << ", " << databaseRecord.numerRegalu <<
			", " << databaseRecord.polkaRegalu << ", " << databaseRecord.liczbaSztuk << endl;
	}
}

void printDatabase(record* database, const int size)
{
	for (int i = 0; i < size; i++)
		printSingleRecord(database[i], i);
}

void printByKey(record* database, const int size, string rodzajPrzewodu)
{
	bool found = false;

	for (int i = 0; i < size; i++)
	{
		if (database[i].rodzajPrzewodu == rodzajPrzewodu)
		{
			if (!found)
			{
				cout << endl;
				found = !found;
			}

			printSingleRecord(database[i], i);
		}
	}

	if (!found)
		cout << endl << "[Blad]: Nie znaleziono zadnego rekordu pasujacego do podanego klucza!" << endl;
}

void saveDatabaseToFile(record* database, const int size, const string filename)
{
	fstream output;
	output.open(filename, ios::out | ios::binary);

	if (!output.good())
	{
		cout << "[Blad]: Wystapil problem przy probie otwarcia pliku do zapisu!" << endl;
		return;
	}

	// Baza danych gotowa do zapisania
	for (int i = 0; i < size; i++)
	{
		output << i << " ";

		if (database[i].rodzajPrzewodu == "")
			output << "NaN";
		else
			output << database[i].rodzajPrzewodu;

		output << " " << database[i].numerRegalu << " " << database[i].polkaRegalu
			<< " " << database[i].liczbaSztuk << endl;
	}
	cout << "[Sukces]: Pomyslnie zapisano baze danych do pliku '" << filename << "'!" << endl;

	output.close();
}

void loadDatabaseFromFile(record* database, const int size, const string filename)
{
	fstream input;
	input.open(filename, ios::in | ios::binary);

	if (!input.good())
	{
		cout << "[Blad]: Wystapil problem przy probie otwarcia pliku do odczytu!" << endl;
		return;
	}

	while (!input.eof())
	{
		int index;

		if (!(input >> index))
			break;
		input >> database[index].rodzajPrzewodu;
		if (database[index].rodzajPrzewodu == "NaN")
			database[index].rodzajPrzewodu = "";
		input >> database[index].numerRegalu;
		input >> database[index].polkaRegalu;
		input >> database[index].liczbaSztuk;

		cout << "[Wczytano]: ";
		printSingleRecord(database[index], index);
	}

	cout << endl << "[Sukces]: Pomyslnie wczytano baze danych z pliku '" << filename << "'!" << endl;

	input.close();
}

void printMenu()
{
	cout << "  -------  MENU  -------  " << endl;
	cout << " 1. Wyswietl baze danych" << endl;
	cout << " 2. Dodaj nowy rekord" << endl;
	cout << " 3. Usun istniejacy rekord" << endl;
	cout << " 4. Wyszukaj po kluczu" << endl;
	cout << " 5. Zapisz baze danych do pliku" << endl;
	cout << " 6. Wyjdz z programu" << endl;
}

int main()
{
	const int size = 5;
	const string filename = "database.bin";
	record* database = new record[size];

	bool exit = false;
	int opcja = 0;

	loadDatabaseFromFile(database, size, filename);
	cout << endl << "Aby kontynuowac, nacisnij dowolny przycisk . . . ";
	(void)getchar();

	while (!exit)
	{
		system("cls");

		printMenu();
		cout << endl << "Wybierz opcje: ";

		// Jesli podana zostala litera
		if (!(cin >> opcja))
		{
			cin.clear();
			cin.ignore();
			continue;
		}

		cout << endl;

		switch (opcja)
		{
		case 1:
		{
			printDatabase(database, size);
			break;
		}
		case 2:
		{
			string rodzajPrzewodu;
			int numerRegalu;
			int polkaRegalu;
			int liczbaSztuk;

			cout << "Rodzaj przewodu: ";	cin >> rodzajPrzewodu;
			cout << "Numer regalu: ";	cin >> numerRegalu;
			cout << "Polka regalu: ";	cin >> polkaRegalu;
			cout << "Liczba sztuk: ";	cin >> liczbaSztuk;

			addRecord(database, size, rodzajPrzewodu, numerRegalu, polkaRegalu, liczbaSztuk);
			break;
		}
		case 3:
		{
			int index;
			cout << "Podaj indeks rekordu ktory chcesz usunac: ";	cin >> index;
			removeRecord(database, size, index - 1);
			break;
		}
		case 4:
		{
			string rodzajPrzewodu;
			cout << "Podaj klucz (rodzaj przewodu): ";	cin >> rodzajPrzewodu;
			printByKey(database, size, rodzajPrzewodu);
			break;
		}
		case 5:
		{
			saveDatabaseToFile(database, size, filename);
			break;
		}
		case 6:
			exit = true;
			cout << "Nastepuje wyjscie z programu" << endl;
			break;
		default:
			cout << "Podano nieprawidlowa opcje!" << endl;
			break;
		}

		cout << endl << "Aby kontynuowac, nacisnij dowolny przycisk . . . ";
		(void)getchar();
		(void)getchar();
	}

	delete[] database;
	return 0;
}