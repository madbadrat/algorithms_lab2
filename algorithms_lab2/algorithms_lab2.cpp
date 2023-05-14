#include <iostream>
#include <time.h>

#define stringLength 4

using namespace std;

class HashTable
{
private:
	int size, count; // параметры таблицы
	int comparsions = 0; // счетчик сравнений
	int r, s;
	char** table; // таблица

	char** CreateTable()
	{
		char** new_table = new char* [size];
		for (int i = 0; i < size; ++i)
			new_table[i] = nullptr;

		return new_table;
	}
	void RecreateTable(int q)
	{
		int old_size = size;
		size = q * 2;
		char** temp_table = new char* [size];
		for (int i = 0; i < size; ++i)
			temp_table = nullptr;

		swap(table, temp_table);

		for (int i = 0; i < old_size; i++)
			if (temp_table[i])
				AddString(temp_table[i]);

		for (int i = 0; i < size; ++i)
			if (temp_table[i])
				delete temp_table[i];
		delete[] temp_table;
	}

	int GetHash1(const char* string)
	{
		int hash = 0;
		for (int i = 0; string[i] != '\0'; i++)
			hash = (hash * r + string[i]) % size;

		return hash;
	}
	int GetHash2(const char* string)
	{
		int hash = 0;
		for (int i = 0; string[i] != '\0'; i++)
			hash = (r * GetHash1(string) + s) % size;

		return hash + 1;
	}
public:
	HashTable(int q)
	{
		size = q * 2;
		count = 0;
		table = CreateTable();
		r = 31;
		s = 17;
	}
	~HashTable()
	{
		for (int i = 0; i < size; i++)
			if (table[i])
				delete table[i];

		delete[] table;
	}

	void AddString(const char* string)
	{
		if (count / size > 0.75) RecreateTable(size); // если занято больше 75% таблицы, пересоздаем ее

		int hash1 = GetHash1(string),
			hash2 = GetHash2(string),
			i = 0;

		while (table[hash1] != nullptr && i < size) // пока на месте hash1 есть элемент
		{
			if (strcmp(table[hash1], string) == 0) // если пытаемся вставить элемент, который уже есть в таблице, то выход из функции
			{
				comparsions++;
				return;
			}
			hash1 = (hash1 + hash2) % size;
			i++;
		}
		table[hash1] = (char*)string; // вставляем элемент в таблицу

		count++; // увеличиваем счетчик элементов
	}
	bool FindString(const char* string)
	{
		int hash1 = GetHash1(string),
			hash2 = GetHash2(string),
			i = 0;

		while (table[hash1] != nullptr && i < size)
		{
			if (strcmp(string, table[hash1]) == 0)
			{
				comparsions++;
				return true;
			}

			hash1 = (hash1 + hash2) % size;
			i++;
		}
		return false;
	}
	int GetUniqueString()
	{
		return count;
	}
	int GetComparsions()
	{
		return comparsions;
	}
};

int GetRandomNumber(int min, int max)
{
	int num = min + rand() % (max - min + 1);
	return num;
}

int main()
{
	srand(time(NULL));

	int n; // (n <= 100000)

	cout << "Введите n: " << endl;
	cin >> n;

	char** arrayOfString = new char* [n]; // массив строк

	for (int i = 0; i < n; i++)
	{
		char* string = new char[stringLength];
		for (int j = 0; j < stringLength - 1; j++)
		{
			string[j] = char(GetRandomNumber(65, 66));
		}
		string[stringLength - 1] = '\0'; // последний нуль-символ
		arrayOfString[i] = string;
	}

	cout << "Сгенерированные строки: " << endl;
	for (int i = 0; i < n; i++)
		cout << arrayOfString[i] << endl;

	HashTable table(n);

	for (int i = 0; i < n; i++)
		table.AddString(arrayOfString[i]);

	if (table.FindString("AAA") == 0) cout << "Искомая строка не найдена" << endl;
	else cout << "Искомая строка найдена" << endl;

	cout << "Уникальных строк в таблице: " << table.GetUniqueString() << endl;
	cout << "Среднее число сравнений строк: " << (float)table.GetComparsions() / n << endl;
	return 0;
}
