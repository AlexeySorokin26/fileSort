#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <string>

using namespace std;

void generateData() {

}

void fillArr(int* arr, const int size) {
    for (int i = 0; i < size; ++i)\
        arr[i] = rand() % 1000;
}

void showArr(const int* arr, const int size) {
    for (size_t i = 0; i < size; i++)
        cout << arr[i] << '\t';
    cout << '\n';
}

void writeArrInFile(const string filePath, const int* arr, const int size) {
    fstream fs;
    fs.open(filePath, fstream::out);

    if (fs.is_open()) {
        fs << size << '\n';
        for (int i = 0; i < size; ++i)
            fs << arr[i] << ' ';
        fs << '\n';
        fs.close();
    }
}

void readArrFrFile(const string filePath, int*& arr, int& size) {
    fstream fs;
    fs.open(filePath, fstream::in);
    if (fs.is_open()) {
        // clean up a memory
        if (arr) {
            delete[] arr;
        }
        // get a size of arr; in our file it is a first line; and allocate a memory
        fs >> size; 
        arr = new int[size];
        // read our file 
        for (int i = 0; i < size; ++i)
            fs >> arr[i];
        fs.close();
    }
}


int main(int argc, char* argv[])
{
    string filePath = "D:\\Learning\\C++\\code\\SkillFactory\\sortFile\\sortFile\\tmp\\sortfile.txt";
    int arrSize = 100;
    switch (argc)
    {
    case 3:
        arrSize = stoi(argv[3]);
    case 2:
        filePath = argv[2];
    default:
        break;
    }

    srand(time(nullptr));
    int* arr = new int[arrSize];
    fillArr(arr, arrSize);
    writeArrInFile(filePath, arr, arrSize);
    readArrFrFile(filePath, arr, arrSize);
    showArr(arr, arrSize);

    arr = nullptr;



}

