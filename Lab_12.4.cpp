#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <algorithm>  
using namespace std;

enum TrainType { REGULAR, COMFORT, EXPRESS };
string trainTypeStr[] = { "���������", "ϳ��������� ��������", "��������� �������" };

struct Train {
    char destination[50];
    TrainType type;
    char departureTime[6];
};

void AddTrain(const string& filename) {
    ofstream file(filename, ios::binary | ios::app);
    if (!file.is_open()) {
        cout << "�� ������� ������� ���� ��� ������!" << endl;
        return;
    }

    Train train;
    cin.ignore();
    cout << "����� ������ �����������: ";
    cin.getline(train.destination, 50);

    int type;
    cout << "��� ���������� (0 - ���������, 1 - ϳ��������� ��������, 2 - ��������� �������): ";
    cin >> type;
    while (type < 0 || type > 2) {
        cout << "������������ ���! �������� ������: ";
        cin >> type;
    }
    train.type = static_cast<TrainType>(type);

    cout << "��� ����������� (��:��): ";
    cin >> train.departureTime;

    file.write((char*)&train, sizeof(Train));
    file.close();
    cout << "����� ������ ������." << endl;
}

void PrintTrains(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "�� ������� ������� ���� ��� �������!" << endl;
        return;
    }

    Train train;
    cout << "===================================================================" << endl;
    cout << "| ����� �����������   | ���                     | ���          |" << endl;
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
    cout << "������ ��� (��:��): ";
    cin >> inputTime;

    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "�� ������� ������� ���� ��� �������!" << endl;
        return;
    }

    Train train;
    bool found = false;
    while (file.read((char*)&train, sizeof(Train))) {
        if (string(train.departureTime) > inputTime) {
            cout << "����� �����������: " << train.destination
                << ", ���: " << trainTypeStr[train.type]
                << ", ��� �����������: " << train.departureTime << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "���� �����, �� �������� ���� " << inputTime << "." << endl;
    }

    file.close();
}


void SortTrains(const string& filename) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file.is_open()) {
        cout << "�� ������� ������� ���� ��� �������/������!" << endl;
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
    cout << "��������� ��: " << endl;
    cout << "1. �������� �����������" << endl;
    cout << "2. ����� �����������" << endl;
    cout << "������� ������: ";
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
        cout << "������� ����!" << endl;
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
    cout << "����� ���������� ������." << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string filename;
    cout << "������ ����� �������� ����� (� �����������): ";
    cin >> filename;

    int menuItem;
    do {
        cout << endl << "����:" << endl;
        cout << " [1] - ������ �����" << endl;
        cout << " [2] - ������� �� ������" << endl;
        cout << " [3] - ����� ���� ��������� ����" << endl;
        cout << " [4] - ��������� ������" << endl;
        cout << " [0] - �����" << endl;
        cout << "������� ��: ";
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
            cout << "���������� ������ ��������." << endl;
            break;
        default:
            cout << "������������ ����! ��������� �� ���." << endl;
        }
    } while (menuItem != 0);

    return 0;
}