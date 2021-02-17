#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <string>
#include <chrono>
//#define DEBUG


using namespace std;

struct bucket {
    int count; // количество элементов в корзине
    int* data; // массив элементов корзины
};

void fillArr(int* arr, const int size) {
    for (int i = 0; i < size; ++i)
        arr[i] = rand() % 1000;
}

void showArr(const int* arr, const int size) {
    for (int i = 100000; i < size; i++)
        cout << arr[i] << '\t';
    cout << '\n';
}

int getMax(const int* arr, const int size) {
    int max = arr[0];
    for (size_t i = 1; i < size; ++i)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

void countSort(int* arr, const int size) {
    // 1 step: get a max number in an arr;
    // create an empty arr for a frequency; 
    // and tmp arr for sorting purposes
    int max = getMax(arr, size);
    int* tmp = new int[size];
    int* frequency = new int[max + 1];
    // and fill it with zeros 
    for (int i = 0; i < max + 1; ++i)
        frequency[i] = 0;

    // 2 step: count frequencies 0->X 1->X 2->X 3->X 4->X 5->X 6->X 
    for (int i = 0; i < size; ++i)
        frequency[arr[i]]++;

    // 3 step: fib frequencies 
    for (int i = 1; i <= max; ++i)
        frequency[i] += frequency[i - 1];

    // 4 step 
    for (int i = size - 1; i >= 0; --i) {
        tmp[frequency[arr[i]] - 1] = arr[i];
        frequency[arr[i]]--;
    }

    // copy data back and free our memory
    for (int i = 0; i < size; ++i)
        arr[i] = tmp[i];

    delete[] frequency;
    delete[] tmp;
}

void countSort(int* arr, int n, int exp) {
    int* output = new int[n];
    int count[10] = { 0 }; // выделяем память под массив частот размером 10, так как используем 10-чную систему счисления

    // вычисление разряда элемента arr[i] выполняет выражение 
    // (arr[i] / exp) % 10
    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;  // вычисляем значение разряда и считаем его частоту

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}

void radixSort(int* arr, int n) {
    int m = getMax(arr, n); // находим максимальное число
    // выполняем сортировку массива, пока не дойдем до старшего разряда наибольшего числа 

    int exp = 1;
    // ваш код
    for (int i = 1; i <= m; ++i)
        countSort(arr, n, exp * i);
}

int getExp(int value) {
    int exp = 1;

    while (value > 10)
    {
        value /= 10;
        exp *= 10;
    }

    return exp;
}

void bucketSort(int* arr, int n)
{
    struct bucket buckets[10];
    // вычисляем значение экспоненты
    int exp = getExp(getMax(arr, n));

    for (int i = 0; i < 10; i++)
    {
        buckets[i].count = 0;
        buckets[i].data = new int[n];
    }
    for (int i = 0; i < n; i++) {
        int bi = arr[i] / exp; // вычисляем индекс корзины
        buckets[bi].data[buckets[bi].count++] = arr[i]; // добавляем элемент в корзину
    }

    // здесь реализуйте шаг 4 описанный в алгоритме
    for (int i = 0; i < 10; ++i)
        countSort(buckets[i].data, buckets[i].count);

    int shift = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < buckets[i].count; ++j) {
            arr[shift + j] = buckets[i].data[j];
        }
        shift += buckets[i].count;
    }
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int* arr, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

void selectionSort(int* arr, int n) {
    for (int i = 0; i < n; ++i) {
        int tmp = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[tmp] > arr[j]) {
                tmp = j;
            }
        }
        if (tmp != i) {
            swap(&arr[tmp], &arr[i]);
        }
    }
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
        // get a size of arr; in our file it is a first line; and allocate a memory
        fs >> size; 
        arr = new int[size];
        // read our file 
        for (int i = 0; i < size; ++i)
            fs >> arr[i];
        fs.close();
    }
}

void addArr(int* arr, int* bigArr, const int start, const int end) {
    int counter = 0;
    for (int i = start; i < end; ++i) {
        bigArr[i] = arr[counter];
        counter++;
    }
}



int partition(int* arr, int low, int high) {
    int pivot = arr[high];    // опорный элемент
    int i = (low - 1);  // индекс наименьшего элемента
    for (int j = low; j <= high - 1; j++)
    {
        // если текущий элемент меньше или равен опорнуму
        if (arr[j] <= pivot)
        {
            i++;    // увеличиваем индекс минимального элемента
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int* arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high); 
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(int* arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;
    
    try
    {
        int* left = new int[nl];
        int* right = new int[nr];

        for (int i = 0; i < nl; i++)
            left[i] = arr[l + i];
        for (int j = 0; j < nr; j++)
            right[j] = arr[m + 1 + j];

        int i = 0, j = 0;
        int k = l;

        while (i < nl && j < nr) {
            if (left[i] <= right[j]) {
                arr[k] = left[i];
                i++;
            }
            else {
                arr[k] = right[j];
                j++;
            }
            k++;
        }

        while (i < nl) {
            arr[k] = left[i];
            i++;
            k++;
        }

        while (j < nr) {
            arr[k] = right[j];
            j++;
            k++;
        }

        delete[] left;
        delete[] right;
    }
    catch (const std::exception& ex)
    {
        cout << ex.what() << endl;
    }
       
}

void insertionSort(int* arr, int l, int r)
{
    for (int i = l + 1; i <= r; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= l && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        arr[j + 1] = key;
    }
}

void timSort(int* arr, int n)
{
    const int RUN = 16;
    for (int i = 0; i < n; i += RUN)
        insertionSort(arr, i, min((i + 31), (n - 1)));

    for (int size = RUN; size < n; size = 2 * size)
    {
        for (int left = 0; left < n; left += 2 * size)
        {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (n - 1));
            if (mid > right)
                mid = left;
            merge(arr, left, mid, right);
        }
    }
}

void mergeSort(int* arr, int l, int r) {
    if (l >= r)
        return;
    int m = (l + r - 1) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int main(int argc, char* argv[])
{
    string mainFolder= "D:\\Learning\\C++\\code\\SkillFactory\\sortFile\\sortFile\\tmp\\";
    string fileName = "sortfile.txt";


    int arrSize = 100;
    switch (argc)
    {
    case 3:
        arrSize = stol(argv[2]);
    case 2:
        mainFolder = argv[1];
        break;
    default:
        break;
    }

    //srand(time(nullptr));
    //fillArr(arr, arrSize);
    
    const int numberOfFiles = 307;
    int* arr = new int[arrSize];
    int* bigArr = new int[arrSize * numberOfFiles];
    for (int i = 0; i < numberOfFiles; ++i) {
        // we first get our data; then sort it file; after sort it; then adding to the result file with sorting again
#ifdef DEBUG
        auto start = chrono::high_resolution_clock::now();
#endif // DEBUG
        // if we don't have a data we can generate it
        //fillArr(arr, arrSize);
        //writeArrInFile(mainFolder + fileName, arr, arrSize);

        // or just read it 
        fileName = "sortfile" + to_string(i) + ".txt";
        readArrFrFile(mainFolder + fileName, arr, arrSize);
       
#ifdef DEBUG  
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "read file: " << i << " done with time: " << elapsed.count() << endl;
#endif // DEBUG

#ifdef DEBUG
        start = chrono::high_resolution_clock::now();
#endif // DEBUG
        timSort(arr, arrSize);
        //quickSort(arr, 0, arrSize - 1);
#ifdef DEBUG
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
        cout << "first timSort file: " << i << " done with time: " << elapsed.count() << endl;
#endif // DEBUG

#ifdef DEBUG
        start = chrono::high_resolution_clock::now();
#endif // DEBUG
        addArr(arr, bigArr, arrSize * i, arrSize * (i + 1));
#ifdef DEBUG
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
        cout << "add array to big one file: " << i << " done with time: " << elapsed.count() << endl;
#endif // DEBUG

#ifdef DEBUG
        start = chrono::high_resolution_clock::now();
#endif // DEBUG
        timSort(bigArr, arrSize * (i+1));
        //quickSort(bigArr, 0, arrSize * (i + 1) - 1);
#ifdef DEBUG
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
        cout << "second timSort: " << i << " done with time: " << elapsed.count() << endl;
        cout << "file: " << i << " done with time: " << elapsed.count() << endl;
#endif // DEBUG
    }

    //(mainFolder + "/result.txt", bigArr, arrSize*5);

    delete[] arr;
    delete[] bigArr;
    arr = nullptr;
    bigArr = nullptr;
    return 0;
}

