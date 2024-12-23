#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h> 

#define SIZE_4X4 4
#define SIZE_9X9 9

// Определение SudokuField как указателя на указатель на int
typedef int** SudokuField;

// Прототипы функций
SudokuField createField(int size);
void freeField(SudokuField field, int size);
void printSudoku(SudokuField field, SudokuField original, int size, int currentRow, int currentCol);
void setCursorPosition(int row, int col);
int isValid(SudokuField field, int size, int row, int col, int num);
int solveSudoku(SudokuField field, int size, SudokuField original);
void generateRandomSudoku(SudokuField field, int size, int filledPercentage);

// Функция задержки для имитации обработки и визуализации
void my_sleep(int seconds, int nanoseconds) 
{
    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds;
    nanosleep(&req, NULL);
}

// Функция для создания динамического поля судоку
SudokuField createField(int size) 
{
    SudokuField field = (SudokuField)malloc(size * sizeof(int*));// Выделение памяти для строк
    for (int i = 0; i < size; i++) 
    {
        *(field + i) = (int*)malloc(size * sizeof(int));// Выделение памяти для столбцов
        for (int j = 0; j < size; j++) 
        {
            *(*(field + i) + j) = 0; // Инициализация всех ячеек нулями
        }
    }
    return field;
}
// Освобождение выделенной памяти для поля судоку
void freeField(SudokuField field, int size) 
{
    for (int i = 0; i < size; i++) 
    {
        free(*(field + i)); // Освобождение строк
    }
    free(field); // Освобождение всего поля
}

// Установка позиции курсора в консоли для визуальных обновлений
void setCursorPosition(int row, int col) 
{
    printf("\033[%d;%dH", row + 1, (col + 1) * 3 - 2);
}
// Очистка экрана консоли
void clearScreen() 
{
    printf("\033[H\033[J");
}

// Печать сетки судоку с цветами для визуализации
void printSudoku(SudokuField field, SudokuField original, int size, int currentRow, int currentCol) 
{
    clearScreen(); // Очистка экрана перед печатью

    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            setCursorPosition(i, j); // Переместить курсор в нужное место
            
            if (*(*(original + i) + j) != 0) // Печать исходных чисел зеленым цветом
            {
                printf("\033[32m%d\033[0m", *(*(field + i) + j) == 0 ? '.' : *(*(field + i) + j)); 
            } 
            else if (i == currentRow && j == currentCol) // Выделить текущую позицию желтым цветом
            {
                printf("\033[33m%d\033[0m", *(*(field + i) + j) == 0 ? '.' : *(*(field + i) + j));
            } 
            else if (*(*(field + i) + j) == 0) // Печать пустых ячеек как '.' белым цветом
            {
                printf("\033[37m.\033[0m");
            } 
            else // Печать других чисел синим цветом
            {
                printf("\033[34m%d\033[0m", *(*(field + i) + j));
            }

            fflush(stdout); // Обновить вывод для плавных изменений
        }
    }
    fflush(stdout);
}

// Проверка, можно ли разместить число в указанной ячейке
int isValid(SudokuField field, int size, int row, int col, int num) 
{ // Проверка строки и столбца
    for (int i = 0; i < size; i++) 
    {
        if (*(*(field + row) + i) == num || *(*(field + i) + col) == num) 
        {
            return 0; // Невозможно разместить
        }
    }
    // Проверка квадрата
    int boxSize = (int)sqrt(size);
    int boxRow = (row / boxSize) * boxSize;
    int boxCol = (col / boxSize) * boxSize;
    for (int i = 0; i < boxSize; i++) 
    {
        for (int j = 0; j < boxSize; j++) 
        {
            if (*(*(field + boxRow + i) + boxCol + j) == num) 
            {
                return 0; // Невозможно разместить в квадрате
            }
        }
    }

    return 1; // Размещение допустимо
}

// Решение судоку методом перебора
int solveSudoku(SudokuField field, int size, SudokuField original) 
{
    for (int row = 0; row < size; row++) 
    {
        for (int col = 0; col < size; col++) 
        {
            if (*(*(field + row) + col) == 0) // Найти пустую ячейку
            {
                for (int num = 1; num <= size; num++) 
                {
                    if (isValid(field, size, row, col, num)) 
                    {
                        *(*(field + row) + col) = num; // Разместить число
                        printSudoku(field, original, size, row, col);
                        my_sleep(0, 15000000);  // Задержка визуализации
                        if (solveSudoku(field, size, original)) // Рекурсивный вызов
                        {
                            return 1;
                        }
                        *(*(field + row) + col) = 0; // Сброс, если нет решения
                    }
                }
                return 0; // Возврат
            }
        }
    }
    return 1; // Судоку решено
}

// Функция для случайного заполнения игрового поля
void generateRandomSudoku(SudokuField field, int size, int filledPercentage) 
{

    srand(time(NULL)); // Инициализация генератора случайных чисел
    
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            if (rand() % 100 < filledPercentage) 
            { 
                int num = rand() % size + 1;
                while (!isValid(field, size, i, j, num)) 
                { 
                    num = rand() % size + 1;
                }
                *(*(field + i) + j) = num; 
            }
        }
    }
}

// Главная функция
int main() 
{
    int size;
    
    printf("Select the field size (4 for 4x4, 9 for 9x9): ");
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 4) 
    {
        size = SIZE_4X4;
    } 
    else if (choice == 9) 
    {
        size = SIZE_9X9;
    } 
    else 
    {
        printf("Invalid choice. Defaulting to 9x9.\n");
        size = SIZE_9X9;
    }

    SudokuField field = createField(size);
    SudokuField original = createField(size); 

    generateRandomSudoku(field, size, 25);

    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            *(*(original + i) + j) = *(*(field + i) + j);
        }
    }

    printSudoku(field, original, size, -1, -1);
    my_sleep(0, 15000000);

    if (solveSudoku(field, size, original)) 
    {
        printf("\nSudoku solved!\n");
    } 
    else 
    {
        printf("\nNo solution for Sudoku.\n");
    }

    freeField(field, size);
    freeField(original, size);
    return 0;
}

#endif // BRUTEFORCE_H
