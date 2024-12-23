#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>   
#include <math.h>  

// Определение типа Size как псевдонима для unsigned int
typedef unsigned int Size;
// Определение типа SudokuField как указателя на двумерный массив целых чисел
typedef int** SudokuField;

// Прототипы функций
SudokuField createField(Size size);
void freeField(SudokuField field, Size size);
void drawField(SudokuField field, Size size, int curRow, int curCol, int** modified);
void handleInput(char input, int* curRow, int* curCol, Size size);
int isValid(SudokuField field, Size size, int row, int col, int num);
void generateRandomField(SudokuField field, Size size);
int isSolved(SudokuField field, Size size);

// Функция для создания игрового поля и его инициализации нулями
SudokuField createField(Size size) 
{
    SudokuField field = (SudokuField)malloc(size * sizeof(int*)); // Выделение памяти для строк
    for (Size i = 0; i < size; i++) 
    {
        *(field + i) = (int*)malloc(size * sizeof(int)); // Выделение памяти для столбцов
        for (Size j = 0; j < size; j++) 
        {
            *(*(field + i) + j) = 0; // Инициализация каждой ячейки нулём
        }
    }
    return field;
}

// Функция для освобождения памяти, выделенной под игровое поле
void freeField(SudokuField field, Size size) 
{
    for (Size i = 0; i < size; i++) 
    {
        free(*(field + i)); // Освобождение памяти для каждой строки
    }
    free(field); // Освобождение памяти для указателя на строки
}

// Функция для случайного заполнения игрового поля
void generateRandomField(SudokuField field, Size size) 
{
    srand(time(NULL)); // Инициализация генератора случайных чисел
    Size filledCells = size * size / 4; // Количество заполненных ячеек (25% от всех ячеек)

    for (Size i = 0; i < filledCells; i++) 
    {
        int row = rand() % size;
        int col = rand() % size;
        int num = 1 + rand() % size;

        // Если ячейка пуста и число допустимо, добавляем его
        if (*(*(field + row) + col) == 0 && isValid(field, size, row, col, num)) 
        {
            *(*(field + row) + col) = num;
        } 
        else i--; // Если не удалось добавить число, повторяем попытку
    }
}

// Функция для отрисовки игрового поля
void drawField(SudokuField field, Size size, int curRow, int curCol, int** modified) 
{
    for (Size i = 0; i < size; i++) 
    {
        for (Size j = 0; j < size; j++) 
        { // Отображение текущей позиции курсора
            if (i == curRow && j == curCol) 
            {
                if (*(*(field + i) + j) == 0) 
                {
                    printf("[.]"); // Пустая текущая ячейка
                } 
                else 
                {
                    printf("\033[0;36m[%d]\033[0m", *(*(field + i) + j)); // Подсветка текущей ячейки
                }
            } 
            else 
            { // Отображение обычной ячейки
                if (*(*(field + i) + j) == 0) 
                {
                    printf(" . "); // Пустая ячейка
                } 
                else 
                { // Если ячейка была изменена пользователем, подсвечиваем её
                    if (*(*(modified + i) + j)) 
                    {
                        printf("\033[0;36m %d \033[0m", *(*(field + i) + j));
                    } 
                    else 
                    {
                        printf("\033[0;32m %d \033[0m", *(*(field + i) + j)); // Ячейка, заданная игрой
                    }
                }
            }
        }
        printf("\n");
    }
}

// Функция для обработки ввода от пользователя (перемещение курсора)
void handleInput(char input, int* curRow, int* curCol, Size size) 
{ 
    switch (input) 
    {
        case 'w': case 'W': if (*curRow > 0) (*curRow)--; break;
        case 's': case 'S': if (*curRow < size - 1) (*curRow)++; break;
        case 'a': case 'A': if (*curCol > 0) (*curCol)--; break;
        case 'd': case 'D': if (*curCol < size - 1) (*curCol)++; break;
    }
}

// Функция проверки валидности числа в заданной позиции
int isValid(SudokuField field, Size size, int row, int col, int num) 
{
    for (Size i = 0; i < size; i++) 
    {
        if (*(*(field + row) + i) == num || *(*(field + i) + col) == num) 
        {
            return 0; // Число уже есть в строке или столбце
        }
    }
    
    // Проверка в пределах блока
    int boxSize = (int)sqrt(size);
    int boxRow = (row / boxSize) * boxSize;
    int boxCol = (col / boxSize) * boxSize;
    for (int i = 0; i < boxSize; i++) 
    {
        for (int j = 0; j < boxSize; j++) 
        {
            if (*(*(field + boxRow + i) + boxCol + j) == num) 
            {
                return 0; // Число уже есть в блоке
            }
        }
    }

    return 1; // Число допустимо
}

// Функция проверки, решена ли головоломка
int isSolved(SudokuField field, Size size) 
{
    for (Size i = 0; i < size; i++) 
    {
        for (Size j = 0; j < size; j++) 
        {
            if (*(*(field + i) + j) == 0) 
            {
                return 0; 
            }

            for (Size k = 0; k < size; k++) 
            {
                if (k != j && *(*(field + i) + k) == *(*(field + i) + j)) return 0;
                if (k != i && *(*(field + k) + j) == *(*(field + i) + j)) return 0;
            }

            int boxSize = (int)sqrt(size);
            int boxRow = (i / boxSize) * boxSize;
            int boxCol = (j / boxSize) * boxSize;
            for (int x = 0; x < boxSize; x++) 
            {
                for (int y = 0; y < boxSize; y++) 
                {
                    if ((boxRow + x != i || boxCol + y != j) && 
                        *(*(field + boxRow + x) + boxCol + y) == *(*(field + i) + j)) 
                        {
                        return 0; // Есть пустые ячейки
                    }
                }
            }
        }
    }
    return 1; // Поле заполнено корректно
}

// Главная функция
int main() 
{
    Size size;
    printf("Select the field size (4 for 4x4, 9 for 9x9): ");
    scanf("%u", &size);

    if (size != 4 && size != 9) 
    {
        printf("The size is incorrect. Acceptable sizes: 4, 9.\n");
        return 1;
    }
    
    // Создание игрового поля и массива модификаций
    SudokuField field = createField(size);
    int** modified = (int**)malloc(size * sizeof(int*));
    for (Size i = 0; i < size; i++) 
    {
        *(modified + i) = (int*)calloc(size, sizeof(int));
    }

    generateRandomField(field, size);

    int curRow = 0, curCol = 0; // Начальные координаты курсора
    char input;

    // Основной игровой цикл
    while (1) 
    {
        system("cls");
        drawField(field, size, curRow, curCol, modified);

        if (size == 4) 
        {
            printf("Enter the command (WASD to move, Q to exit, 1-4 to enter a number, X to erase): ");
        } 
        else if (size == 9) 
        {
            printf("Enter the command (WASD to move, Q to exit, 1-9 to enter a number, X to erase): ");
        }

        scanf(" %c", &input);

        if (input == 'q' || input == 'Q') 
        {
            break;
        } 
        else if ((input >= '1' && input <= '9') && 
                   ((size == 9) || (size == 4 && input <= '4'))) 
                   {
            int num = input - '0';
            if (curRow < size && curCol < size && *(*(field + curRow) + curCol) == 0) 
            {
                *(*(field + curRow) + curCol) = num;
                *(*(modified + curRow) + curCol) = 1; 
            } else 
            {
                printf("error\n");
            }
        } 
        else if (input == 'x' || input == 'X') 
        {
            if (curRow < size && curCol < size && *(*(field + curRow) + curCol) != 0) 
            {
                *(*(field + curRow) + curCol) = 0;
                *(*(modified + curRow) + curCol) = 0;  
            }
        } 
        else 
        {
            handleInput(input, &curRow, &curCol, size);
        }

        if (isSolved(field, size)) 
        {
            system("cls");
            drawField(field, size, curRow, curCol, modified);
            printf("\nSudoku solved!\n");
            break;
        }
    }
    
    // Очистка памяти
    for (Size i = 0; i < size; i++) 
    {
        free(*(modified + i));
    }
    free(modified);
    freeField(field, size);
    return 0;
}
#endif // USER_H