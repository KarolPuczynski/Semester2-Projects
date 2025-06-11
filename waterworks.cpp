#include <iostream>
#include <ctime>

using namespace std;

void creating_city(int rows, int columns);
void menu();
int checking_number();
bool valid_city(char** city, int rows, int columns);
void BFS(bool* visited, int** adj_list, int** waterWorks_list, int number_of_cities);
void swapping(int* city_queue, int number_of_cities);
void generating_waterworks(int** waterWorks_list, char** city, int rows, int columns);
void drawing_city(char** city, int rows, int columns);
void drawing_city_with_waterworks(char** city, int rows, int columns);

int main()
{
	srand(time(0));
	cout << "             Program autorstwa Karola Puczynskiego i Piotra Rokity" << endl;
	menu();

	return 0;
}

void menu() {
	char input;
	int rows = 0, columns = 0;
	do {
		cout << endl << "Czy chcesz wygenerowac mape miasta?(T - Tak, N - Nie) ";
		cin >> input;
		if (input == 'T') {
			cout << endl << "Podaj wysokosc mapy miasta: ";
			rows = checking_number();
			cout << endl << "Podaj szerokosc mapy miasta: ";
			columns = checking_number();
			creating_city((2 * rows) - 1, (2 * columns) - 1);
		}
		else if (input == 'N') {
			break;
		}
	} while (input != 'T' && input != 'N');
}

int checking_number() { // sprawdzanie czy jest wpisywana liczba
	int input;
	while (!(cin >> input)) {
		cout << "Bledne dane. Wprowadz liczbe: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return input;
}

void creating_city(int rows, int columns) {
	int curr_letter = 65;
	int probability = 40;
	char** city = new char* [rows];
	for (int i = 0; i < rows; i++)
	{
		city[i] = new char[columns];
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			city[i][j] = ' ';
		}
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (curr_letter == 121) {
				curr_letter = 65;
			}

			int random_number = rand() % 100;
			if (j % 2 == 0 && i % 2 == 0) {
				city[i][j] = char(curr_letter);
				curr_letter++;
			}
			else if (j % 2 == 0 && i % 2 == 1) {
				if (probability > random_number) {
					city[i][j] = '|';
				}
				else {
					city[i][j] = ' ';
				}
			}
			else if (j % 2 == 1 && i % 2 == 0) {
				if (probability > random_number) {
					city[i][j] = '-';
				}
				else {
					city[i][j] = ' ';
				}
			}
		}
	}

	if (!valid_city(city, rows, columns)) {
		creating_city(rows, columns);
	}


	for (int i = 0; i < rows; i++)
	{
		delete[] city[i];
	}
	delete[] city;
	menu();
}

bool valid_city(char** city, int rows, int columns) {

	char start = city[0][0];
	int number_of_cities = ((rows + 1) / 2) * ((columns + 1) / 2);

	int** adj_list = new int* [number_of_cities];
	int** waterWorks_list = new int* [number_of_cities];
	bool* visited = new  bool[number_of_cities];

	for (int i = 0; i < number_of_cities; i++) {
		adj_list[i] = new int[number_of_cities];
		waterWorks_list[i] = new int[number_of_cities];
		visited[i] = false;
	}
	visited[0] = true;

	for (int i = 0; i < number_of_cities; i++) {
		for (int j = 0; j < number_of_cities; j++) {
			adj_list[i][j] = 0;
			waterWorks_list[i][j] = 0;
		}
	}

	int neighbor_index = 0;
	int city_index = 0;
	int acctual_city = 0;
	int acctual_columns = (columns + 1) / 2;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{


			if (i % 2 == 0 && j % 2 == 0)
			{
				acctual_city = j / 2 + (i / 2 * acctual_columns);

				if (i + 1 < rows && city[i + 1][j] == '|') // pod
				{
					neighbor_index = acctual_city + acctual_columns;
					adj_list[city_index][neighbor_index] = 1;
				}
				if (i - 1 > 0 && city[i - 1][j] == '|') // nad
				{
					neighbor_index = acctual_city - acctual_columns;
					adj_list[city_index][neighbor_index] = 1;
				}
				if (j + 1 < columns && city[i][j + 1] == '-') // prawo
				{
					neighbor_index = acctual_city + 1;
					adj_list[city_index][neighbor_index] = 1;
				}
				if (j - 1 > 0 && city[i][j - 1] == '-') // lewo
				{
					neighbor_index = acctual_city - 1;
					adj_list[city_index][neighbor_index] = 1;
				}
				city_index++;
			}
		}
	}


	BFS(visited, adj_list, waterWorks_list, number_of_cities);

	for (int i = 0; i < number_of_cities; i++) {
		if (!visited[i]) {
			for (int j = 0; j < rows; j++)
			{
				delete[] city[j];
			}
			delete[] city;
			return false;
		}
	}


	drawing_city(city, rows, columns);

	generating_waterworks(waterWorks_list, city, rows, columns);

	for (int i = 0; i < number_of_cities; i++)
	{
		delete[] waterWorks_list[i];
		delete[] adj_list[i];
	}
	delete[] waterWorks_list;
	delete[] adj_list;
	delete[] visited;

	return true;
}

void BFS(bool* visited, int** adj_list, int** waterWorks_list, int number_of_cities) {
	int* city_queue = new int[number_of_cities];
	for (int i = 0; i < number_of_cities; i++) {
		city_queue[i] = -1;
	}

	city_queue[0] = 0;
	int available_index = 0;

	while (city_queue[0] != -1) {
		int currVertex = city_queue[0];
		city_queue[0] = -1;
		if (available_index != 0) {
			available_index--;
			swapping(city_queue, number_of_cities);
		}

		for (int i = 0; i < number_of_cities; i++) {
			if (adj_list[currVertex][i] == 1 && !visited[i]) {
				visited[i] = true;
				waterWorks_list[currVertex][i] = 1;
				city_queue[available_index] = i;
				available_index++;
			}
		}
	}
	delete[] city_queue;
}

void swapping(int* city_queue, int number_of_cities) {
	for (int i = 0; i < number_of_cities - 1; i++) {
		city_queue[i] = city_queue[i + 1];
		city_queue[i + 1] = -1;
	}
}

void generating_waterworks(int** waterWorks_list, char** city, int rows, int columns) {
	int number_of_cities = ((rows + 1) / 2) * ((columns + 1) / 2);
	int city_index = 0;
	int neighbor_index = 0;
	int acctual_city = 0;
	int acctual_colums = (columns + 1) / 2;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (i % 2 == 0 && j % 2 == 0)
			{
				acctual_city = j / 2 + (i / 2 * acctual_colums);

				if (i + 1 < rows && city[i + 1][j] == '|')
				{
					neighbor_index = acctual_city + acctual_colums;
					if (waterWorks_list[city_index][neighbor_index] == 1)
						city[i + 1][j] = 'i';
				}
				if (i - 1 > 0 && city[i - 1][j] == '|')
				{
					neighbor_index = acctual_city - acctual_colums;
					if (waterWorks_list[city_index][neighbor_index] == 1)
						city[i - 1][j] = 'i';
				}
				if (j + 1 < columns && city[i][j + 1] == '-')
				{
					neighbor_index = acctual_city + 1;
					if (waterWorks_list[city_index][neighbor_index] == 1)
						city[i][j + 1] = '=';
				}
				if (j - 1 > 0 && city[i][j - 1] == '-')
				{
					neighbor_index = acctual_city - 1;
					if (waterWorks_list[city_index][neighbor_index] == 1)
						city[i][j - 1] = '=';
				}
				city_index++;
			}
		}
	}

	drawing_city_with_waterworks(city, rows, columns);
}

void drawing_city(char** city, int rows, int columns) {
	cout << endl << "City: " << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (city[i][j] == '-') {
				cout << "----";
			}
			else if (city[i][j] == '|') {
				cout << "| ";
			}
			else if (city[i][j] == ' ') {
				if (i % 2 == 0) {
					cout << "    ";
				}
				else if (j % 2 == 1) {
					cout << "   ";
				}
				else {
					cout << "  ";
				}
			}
			else {
				cout << city[i][j];
			}
		}
		cout << endl;
	}
}

void drawing_city_with_waterworks(char** city, int rows, int columns) {
	cout << endl << "Waterworks: " << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (city[i][j] == '=') {
				cout << "====";
			}
			else if (city[i][j] == '-') {
				cout << "----";
			}
			else if (city[i][j] == '|') {
				cout << "| ";
			}
			else if (city[i][j] == 'i' && i % 2 == 1) {
				cout << "||";
			}
			else if (city[i][j] == ' ') {
				if (i % 2 == 0) {
					cout << "    ";
				}
				else if (j % 2 == 1) {
					cout << "   ";
				}
				else {
					cout << "  ";
				}
			}
			else {
				cout << city[i][j];
			}
		}
		cout << endl;
	}
}