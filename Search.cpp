#include <iostream>
#include <windows.h>
#include <conio.h>
#include <set>
#include <vector>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {

	HANDLE hWritePipe = (HANDLE)atoi(argv[1]);
	HANDLE hReadPipe = (HANDLE)atoi(argv[2]);
	int size1, size2;
	DWORD bytesWritten;
	if (!ReadFile(hReadPipe, &size1, sizeof(size1), &bytesWritten, NULL))
	{
		cout << "Array1 size read is failed " << endl;
		return GetLastError();
	}
	double* arr1 = new double[size1];
	if (!ReadFile(hReadPipe, arr1, sizeof(double) * size1, &bytesWritten, NULL))
	{
		cout << "Array1 elements read is failed " << endl;
		return GetLastError();
	}
	if (!ReadFile(hReadPipe, &size2, sizeof(size2), &bytesWritten, NULL))
	{
		cout << "Array2 size read is failed " << endl;
		return GetLastError();
	}
	double* arr2 = new double[size2];
	if (!ReadFile(hReadPipe, arr2, sizeof(double) * size2, &bytesWritten, NULL))
	{
		cout << "Array2 elements read is failed " << endl;
		return GetLastError();
	}
	cout << "Received information from Server:" << endl;
	cout << "Size of array 1: " << size1 << endl;
	cout << "Elements of array 1: ";
	for (int i = 0; i < size1; i++)
	{
		cout << arr1[i] << " ";
	}
	cout << endl;
	cout << "Size of array 2: " << size2 << endl;
	cout << "Elements of array 2: ";
	for (int i = 0; i < size2; i++)
	{
		cout << arr2[i] << " ";
	}
	cout << endl;

	set<double> uniqueElements(arr1, arr1 + size1);
	vector<double> result;

	for (int i = 0; i < size2; i++) {
		if (uniqueElements.count(arr2[i]) > 0) {
			uniqueElements.erase(arr2[i]);
		}
		else {
			result.push_back(arr2[i]);
		}
	}
	if (!uniqueElements.empty()) {
		for (auto un : uniqueElements)
			result.push_back(un);
	}
	else
		cout << "Result array is empty";
	
	cout << "Result array\n";
	for (int i = 0; i < result.size(); i++)
	{
		cout << result[i] << " ";
	}

	int resultSize = result.size();
	if (!WriteFile(hWritePipe, &resultSize, sizeof(resultSize), &bytesWritten, NULL))
	{
		cout << "Result array size write is failed " << endl;
		return GetLastError();
	}
	if (!WriteFile(hWritePipe, result.data(), resultSize * sizeof(double), &bytesWritten, NULL))
	{
		cout << "Result array elements write is failed " << endl;
		return GetLastError();
	}

	cout << "\nTo exit press any key ";
	_getch();
}