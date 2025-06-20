#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <stdexcept>

using namespace std;

// Максимальное количество дат, которое может быть в строке
const int MAX_DATES = 100;

// Функция проверки, является ли строка корректной датой в формате dd/mm/yyyy
bool is_valid_date(string date) {
    // Удаляем знаки пунктуации с конца
        while (!date.empty() && ispunct(static_cast<unsigned char>(date.back()))) {
            date.pop_back();
        }

    // Проверка длины строки
    if (date.length() != 10) return false;

    // Проверка разделителей
    if (date[2] != '/' || date[5] != '/') return false;

    try {
        // Проверяем, что все символы кроме '/' являются цифрами
        for (int i = 0; i < 10; ++i) {
            if (i == 2 || i == 5) continue; // Пропускаем разделители
            if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
        }

        // Извлечение дня, месяца и года
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));

        // Проверка года
        if (year < 1 || year > 9999) return false;

        // Проверка месяца
        if (month < 1 || month > 12) return false;

        // Проверка дня
        int max_days;
        switch (month) {
        case 2:
            if ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0)) {
                max_days = 29;
            }
            else {
                max_days = 28;
            }
            break;
        case 4: case 6: case 9: case 11:
            max_days = 30;
            break;
        default:
            max_days = 31;
        }

        if (day < 1 || day > max_days) return false;

        return true;
    }
    catch (const invalid_argument&) {
        return false;
    }
    catch (const out_of_range&) {
        return false;
    }
}

// Функция сравнения дат для сортировки
bool compare_dates(const string& a, const string& b) {
    int day_a = stoi(a.substr(0, 2));
    int month_a = stoi(a.substr(3, 2));
    int year_a = stoi(a.substr(6, 4));

    int day_b = stoi(b.substr(0, 2));
    int month_b = stoi(b.substr(3, 2));
    int year_b = stoi(b.substr(6, 4));

    if (year_a != year_b) return year_a < year_b;
    if (month_a != month_b) return month_a < month_b;
    return day_a < day_b;
}

int main() {
    try {
        string input;
        string dates[MAX_DATES];
        int date_count = 0;

        cout << "Enter the string with the words (separated by spaces):" << endl;
        getline(cin, input);

        // Разбиение строки на слова
        istringstream iss(input);
        string word;

        while (iss >> word && date_count < MAX_DATES) {
            if (is_valid_date(word)) {
                dates[date_count++] = word;
            }
        }

        if (date_count == 0) {
            cout << "No valid dates were found in the row in the dd/mm/yyyy format." << endl;
            return 0;
        }

        // Сортировка дат (пузырьковая сортировка)
        for (int i = 0; i < date_count - 1; ++i) {
            for (int j = 0; j < date_count - i - 1; ++j) {
                if (!compare_dates(dates[j], dates[j + 1])) {
                    swap(dates[j], dates[j + 1]);
                }
            }
        }

        cout << "Correct dates found, ordered in ascending order:" << endl;
        for (int i = 0; i < date_count; ++i) {
            cout << dates[i] << endl;
        }

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}