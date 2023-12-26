/*5.6. Написать программы двух консольных процессов Server и Searh, которые обмениваются сообщениями анонимным каналам (2 шт.): 1 процесс записывает в первый канал, читает из второго канала,  2 процесс записывает во второй канал, читает из первого канала .Создать наследуемые дескрипторы канала.

Одновременно сообщение может передаваться только одним из процессов.

Процесс- Server, который выполняет следующие действия:
- Размер массива1 и элементы массива1 вводятся с консоли. Тип массива: _ int16
- Размер массива2 и элементы массива2 вводятся с консоли. Тип массива: __int8
- Запускает процесс Searh.
- Передает процессу- Searh по анонимному каналу размеры массивов и элементы массивов. 
- Получает от процесса- Searh по анонимному каналу итоговый массив (пары индексов). Выводит полученную и переданную информацию на консоль. 


Процесс- Searh, который выполняет следующие действия.
- Получает каналу размеры массивов и элементы массивов по анонимному каналу от процесса-сервера.
- Выводит полученные данные на консоль.
- Определяет не совпадающие элементы из массивов, удалить дубли.
- Выводит выводит новый массив на консоль
- Передает  новый массив   по анонимному каналу процессу-серверу.
*/
#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

int main() {
	int size1, size2;
	cout << "-Server-\nEnter array1 size: ";
	cin >> size1;
	double* arr1 = new double[size1];
	cout << "Enter array1 elements: ";
	for (int i = 0; i < size1; i++) {
		cin >> arr1[i];
	}
	cout << endl;
	cout << "Enter array2 size: ";
	cin >> size2;
	double* arr2 = new double[size2];
	cout << "Enter array2 elements: ";
	for (int i = 0; i < size2; i++) {
		cin >> arr2[i];
	}

	HANDLE hWritePipe, hReadPipe, hInheritWritePipe;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
		cout << "-Server-\n Pipe was not created";
		return GetLastError();
	}
	if (!DuplicateHandle(GetCurrentProcess(), hWritePipe, GetCurrentProcess(), &hInheritWritePipe, 0, TRUE, DUPLICATE_SAME_ACCESS))
	{
		cout << "Duplicate write pipe is failed" << endl;
		return GetLastError();
	}

	DWORD bytesWritten;
	if (!WriteFile(hInheritWritePipe, &size1, sizeof(int), &bytesWritten, NULL))
	{
		cout << "Array1 size write is failed " << endl;
		return GetLastError();
	}
	if (!WriteFile(hInheritWritePipe, arr1, sizeof(double) * size1, &bytesWritten, NULL))
	{
		cout << "Array1 elements write is failed " << endl;
		return GetLastError();
	}
	if (!WriteFile(hInheritWritePipe, &size2, sizeof(int), &bytesWritten, NULL))
	{
		cout << "Array2 size write is failed " << endl;
		return GetLastError();
	}
	if (!WriteFile(hInheritWritePipe, arr2, sizeof(double) * size2, &bytesWritten, NULL))
	{
		cout << "Array2 elements write is failed " << endl;
		return GetLastError();
	}

	string cmdLine = "Search.exe " + to_string((int)hWritePipe) + " " + to_string((int)hReadPipe) + " ";
	wstring wcmdLine = wstring(cmdLine.begin(), cmdLine.end());
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL, (LPWSTR) &wcmdLine[0], NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "Search.exe was not launched.\n";
		return GetLastError();
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD byteRead;
	int resultSize;
	if (!ReadFile(hReadPipe, &resultSize, sizeof(resultSize), &byteRead, NULL))
	{
		cout << "Size of result array read is failed " << endl;
		return GetLastError();
	}
	double* result = new double[resultSize];
	if (!ReadFile(hReadPipe, result, sizeof(double) * resultSize, &byteRead, NULL))
	{
		cout << "Result array elements read is failed " << endl;
		return GetLastError();
	}

	cout << "\nResult array\n";
	for (int i = 0; i < resultSize; i++)
	{
		cout << result[i] << " ";
	}

	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}