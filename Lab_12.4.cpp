#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <algorithm>  
using namespace std;

enum TrainType { REGULAR, COMFORT, EXPRESS };
string trainTypeStr[] = { "Звичайний", "Підвищеного комфорту", "Швидкісний експрес" };

struct Train {
    char destination[50];
    TrainType type;
    char departureTime[6];
};

void AddTrain(const string& filename) {
    ofstream file(filename, ios::binary | ios::app);
    if (!file.is_open()) {
        cout << "Не вдалося відкрити файл для запису!" << endl;
        return;
    }

    Train train;
    cin.ignore();
    cout << "Назва пункту призначення: ";
    cin.getline(train.destination, 50);

    int type;
    cout << "Тип електрички (0 - Звичайний, 1 - Підвищеного комфорту, 2 - Швидкісний експрес): ";
    cin >> type;
    while (type < 0 || type > 2) {
        cout << "Неправильний тип! Повторіть спробу: ";
        cin >> type;
    }
    train.type = static_cast<TrainType>(type);

    cout << "Час відправлення (ГГ:ХХ): ";
    cin >> train.departureTime;

    file.write((char*)&train, sizeof(Train));
    file.close();
    cout << "Запис успішно додано." << endl;
}

void PrintTrains(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Не вдалося відкрити файл для читання!" << endl;
        return;
    }

    Train train;
    cout << "===================================================================" << endl;
    cout << "| Пункт призначення   | Тип                     | Час          |" << endl;
    cout << "-------------------------------------------------------------------" << endl;
    while (file.read((char*)&train, sizeof(Train))) {
        cout << "| " << setw(20) << left << train.destination
            << "| " << setw(24) << left << trainTypeStr[train.type]
            << "| " << setw(10) << right << train.departureTime << " |" << endl;
    }
    cout << "===================================================================" << endl;
    file.close();
}

void TrainsAfterTime(const string& filename) {
    string inputTime;
    cout << "Введіть час (ГГ:ХХ): ";
    cin >> inputTime;

    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Не вдалося відкрити файл для читання!" << endl;
        return;
    }

    Train train;
    bool found = false;
    while (file.read((char*)&train, sizeof(Train))) {
        if (string(train.departureTime) > inputTime) {
            cout << "Пункт призначення: " << train.destination
                << ", Тип: " << trainTypeStr[train.type]
                << ", Час відправлення: " << train.departureTime << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "Немає поїздів, що відходять після " << inputTime << "." << endl;
    }

    file.close();
}


void SortTrains(const string& filename) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file.is_open()) {
        cout << "Не вдалося відкрити файл для читання/запису!" << endl;
        return;
    }


    file.seekg(0, ios::end);
    int numRecords = file.tellg() / sizeof(Train);
    file.seekg(0, ios::beg);

    Train* trains = new Train[numRecords];
    for (int i = 0; i < numRecords; i++) {
        file.read((char*)&trains[i], sizeof(Train));
    }


    int choice;
    cout << "Сортувати за: " << endl;
    cout << "1. Станцією призначення" << endl;
    cout << "2. Часом відправлення" << endl;
    cout << "Виберіть варіант: ";
    cin >> choice;

    if (choice == 1) {
        sort(trains, trains + numRecords, [](const Train& a, const Train& b) {
            return string(a.destination) < string(b.destination);
            });
    }
    else if (choice == 2) {
        sort(trains, trains + numRecords, [](const Train& a, const Train& b) {
            return string(a.departureTime) < string(b.departureTime);
            });
    }
    else {
        cout << "Невірний вибір!" << endl;
        delete[] trains;
        file.close();
        return;
    }

        file.seekp(0, ios::beg);
    for (int i = 0; i < numRecords; i++) {
        file.write((char*)&trains[i], sizeof(Train));
    }

    delete[] trains;
    file.close();
    cout << "Поїзди відсортовані успішно." << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string filename;
    cout << "Введіть назву бінарного файлу (з розширенням): ";
    cin >> filename;

    int menuItem;
    do {
        cout << endl << "Меню:" << endl;
        cout << " [1] - Додати запис" << endl;
        cout << " [2] - Вивести всі записи" << endl;
        cout << " [3] - Поїзди після вказаного часу" << endl;
        cout << " [4] - Сортувати записи" << endl;
        cout << " [0] - Вихід" << endl;
        cout << "Виберіть дію: ";
        cin >> menuItem;
        switch (menuItem) {
        case 1:
            AddTrain(filename);
            break;
        case 2:
            PrintTrains(filename);
            break;
        case 3:
            TrainsAfterTime(filename);
            break;
        case 4:
            SortTrains(filename);
            break;
        case 0:
            cout << "Завершення роботи програми." << endl;
            break;
        default:
            cout << "Неправильний вибір! Спробуйте ще раз." << endl;
        }
    } while (menuItem != 0);

    return 0;
}