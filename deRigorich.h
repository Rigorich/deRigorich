
/* Version 12.03.2020 BETA */
/* Автор: Никита Григорьевич (Гришаев), ст.гр. 953501 */

/*
    Русский язык при вводе и выводе:
    - Кодировка файла -- ANSI
    - #include <windows.h>
    - SetConsoleCP(1251); 
    - SetConsoleOutputCP(1251);
*/

#ifndef DERIGORICH_H
#define DERIGORICH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Преобразовывает указатель к типу // TO(int)(void* p) == *(int*)(void* p) */
#define TO(T) *(T*)

/* Тип данных bool */
#define bool int
#define true 1
#define false 0

/* Границы целочисленных типов */
#define MAXINT 1000000000
#define MAXLL 1000000000000000000
#define INTBOUNDS -MAXINT, MAXINT
#define LLBOUNDS -MAXLL, MAXLL

/* Функции минимума и максимума */
#define min(x,y) (((x) < (y)) ? (x) : (y))
#define max(x,y) (((x) > (y)) ? (x) : (y))

/* Коды некоторых специальных клавиш */
#define ESC 27
#define SPACE 32
#define ENTER 13
#define TAB 9

/* Тест-функция */
void TestFunction(void); 

/* Функция экстренного завершения программы */
void FatalError(char* message);

/* Выделение памяти */
void* Malloc(long long ByteCount); /* Выделение памяти с проверкой */
void MallocCheck(void* ptr); /* Проверка на успешное выделение памяти */

/* Полезности */
void swap(void* a, void* b, int size); /* Обмен местами двух элементов размера size */

/* Вектор */
typedef struct {
    int elemsize;
    int size;
    void* begin;
    int alloc;
    bool (*comp) (void* a, void* b);
} Vector;

Vector* VectorNew(int sizeof_element); /* Конструирует вектор и возвращает указатель на него */
void VectorDelete(Vector* this); /* Деструктор вектора */
void VectorClear(Vector* this); /* Очищает вектор */
void VectorAddElem(Vector* this, void* what); /* Добавляет в конец вектора элемент путём копирования */
void VectorDelElem(Vector* this, int index); /* Удаляет элемент вектора с указанным индексом */
int VectorSize(Vector* this); /* Возвращает количество элементов вектора */
void* VectorBegin(Vector* this); /* Возвращает указатель на начало массива */
void* VectorGetElem(Vector* this, int index); /* Возвращает указатель на элемент с указанным индексом // Используйте TO(type) */
void VectorSetElem(Vector* this, int index, void* what); /* Изменяет элемент с указанным индексом путём копирования */
int VectorFindElem(Vector* this, void* what); /* Возвращает индекс первого вхождения элемента или -1 */
int VectorFindLastElem(Vector* this, void* what); /* Возвращает индекс последнего вхождения элемента или -1 */
bool VectorContainElem(Vector* this, void* what); /* Возвращает существование вхождения элемента */
void VectorSort(Vector* this); /* Сортирует вектор */
void VectorSortComp(Vector* this, bool (*comp) (void* a, void* b)); /* Сортирует вектор с использованием компаратора */
void VectorResize(Vector* this, int newsize); /* Изменяет количество элементов массива */
void VectorRealloc(Vector* this, int newalloc); /* Изменяет количество памяти, выделенной для массива */
/* Вектор */

/* Строка */
Vector* StringNew(void); /* Конструирует строку и возвращает указатель на неё */
void StringDelete(Vector* this); /* Деструктор строки */
void StringClear(Vector* this); /* Очищает строку */
void StringAddElem(Vector* this, char what); /* Добавляет в конец строки символ */
/*UPD*/void StringPutElem(Vector* this, int index, char what); /* Добавляет символ в место с указанным индексом */
void StringDelElem(Vector* this, int index); /* Удаляет символ с указанным индексом */
char* StringBegin(Vector* this); /* Возвращает указатель на начало строки */
int StringSize(Vector* this); /* Возвращает длину строки */
char StringGetElem(Vector* this, int index); /* Возвращает символ строки с указанным индексом */
void StringSetElem(Vector* this, int index, char what); /* Изменяет символ строки с указанным индексом */
int StringFindElem(Vector* this, char what); /* Возвращает индекс первого вхождения символа или -1 */
int StringFindLastElem(Vector* this, char what); /* Возвращает индекс последнего вхождения символа или -1 */
bool StringContainElem(Vector* this, char what); /* Возвращает существование вхождения символа */

void StringRead(Vector* this, char until, FILE* from); /* Читает строку между двумя символами, являющимися переносом, пробелом или until-символом */
void StringReadLn(Vector* this, FILE* from); /* Читает строку вместе с пробелами вплоть до переноса */
void StringIgnore(FILE* from); /* Переносит курсор чтения на следующую строку */

bool StringToInt(char* from, long long* towhat, long long MinBound, long long MaxBound); /* Пытается преобразовать строку в целое число в пределах [MinBound, MaxBound] */
bool StringToFloat(char* from, double* towhat); /* Пытается преобразовать строку в вещественное число */

long long StringReadInt(FILE* from, long long MinBound, long long MaxBound); /* Запрашивает целое число в пределах [MinBound, MaxBound] */
double StringReadFloat(FILE* from); /* Запрашивает вещественное число */
/* Строка */

/* Математика */
double absf(double x); /* Возвращает модуль вещественного числа */
long long fact_int(int x); /* Возвращает целочисленный факториал числа x */
double fact_float(int x); /* Возвращает вещественный факториал числа x */
/* Математика */

/* Сравнение данных */
bool OperLess(void* a, void* b, bool (*cmp)(void*, void*));
bool OperEqual(void* a, void* b, bool (*cmp)(void*, void*));
bool OperGreat(void* a, void* b, bool (*cmp)(void*, void*));

bool CmpInt(void* a, void* b);
bool CmpDouble(void* a, void* b);
bool CmpChar(void* a, void* b);
bool CmpCharArray(void* a, void* b);
/* Сравнение данных */

/* Set */
typedef struct {
    int elemsize;
    int size;
    Vector* base;
    bool (*comp) (void* a, void* b);
} Set;

Set* SetNew(int sizeof_element, bool (*comparator) (void* a, void* b));
void SetClear(Set* this) ;
void SetDelete(Set* this);
void SetAddElem(Set* this, void* what);
void SetDelElem(Set* this, void* what);
bool SetContainElem(Set* this, void* what);
int SetSize(Set* this);
/* Set */


/* Map */
typedef struct {
    int keysize;
    int valuesize;
    int size;
    Vector* keys;
    Vector* values;
    bool (*comp) (void* a, void* b);
    void (*nuller) (void* a);
} Map;

Map* MapNew(int sizeof_key, int sizeof_value, bool (*comparator) (void* a, void* b), void (*value_nullator) (void* a));
void MapClear(Map* this);
void MapDelete(Map* this);
bool MapContainElem(Map* this, void* what);
void* MapValue(Map* this, void* what);
int MapSize(Map* this);

void NullInt(void* a);
/* Map */

#endif