
/* Version 07.05.2020 BETA */
/* Автор: Никита Григорьевич (Гришаев), ст.гр. 953501 */

/*
    Русский язык при вводе и выводе:
    Кодировка файла -- ANSI
    #include <windows.h>
    SetConsoleCP(1251); 
    SetConsoleOutputCP(1251);
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
void FatalError(const char* message);

/* Выделение памяти */
void* Malloc(long long ByteCount); /* Выделение памяти с проверкой */
void MallocCheck(void* ptr); /* Проверка на успешное выделение памяти */

/* Полезности */
void swap(void* a, void* b, int size); /* Обмен местами двух элементов размера size */
void sort(void* begin, void* end, int sizeof_element, bool (*comp) (void* a, void* b)); /* Сортировка */

/* Вектор */
typedef struct Vector Vector;
struct Vector {
    int elemsize;
    int size;
    void* begin;
    int alloc;
    bool (*comp) (void* a, void* b);
};

Vector* VectorNew(int sizeof_element); /* Конструирует вектор и возвращает указатель на него */
void VectorDelete(Vector* that); /* Деструктор вектора */
void VectorClear(Vector* that); /* Очищает вектор */
void VectorAddElem(Vector* that, void* what); /* Добавляет в конец вектора элемент путём копирования */
void VectorDelElem(Vector* that, int index); /* Удаляет элемент вектора с указанным индексом */
int VectorSize(Vector* that); /* Возвращает количество элементов вектора */
void* VectorBegin(Vector* that); /* Возвращает указатель на начало массива */
void* VectorGetElem(Vector* that, int index); /* Возвращает указатель на элемент с указанным индексом // Используйте TO(type) */
void VectorSetElem(Vector* that, int index, void* what); /* Изменяет элемент с указанным индексом путём копирования */
int VectorFindElem(Vector* that, void* what); /* Возвращает индекс первого вхождения элемента или -1 */
int VectorFindLastElem(Vector* that, void* what); /* Возвращает индекс последнего вхождения элемента или -1 */
bool VectorContainElem(Vector* that, void* what); /* Возвращает существование вхождения элемента */
void* VectorGetComp(Vector* that); /* Возвращает компаратор вектора */
void VectorSetComp(Vector* that, bool (*comp) (void* a, void* b)); /* Устанавливает компаратор вектора */
void VectorSort(Vector* that); /* Сортирует вектор */
void VectorSortComp(Vector* that, bool (*comp) (void* a, void* b)); /* Сортирует вектор с использованием компаратора */
void VectorResize(Vector* that, int newsize); /* Изменяет количество элементов массива */
void VectorRealloc(Vector* that, int newalloc); /* Изменяет количество памяти, выделенной для массива */
/* Вектор */

/* Строка */
typedef Vector String;
String* StringNew(void); /* Конструирует строку и возвращает указатель на неё */
void StringDelete(String* that); /* Деструктор строки */
void StringClear(String* that); /* Очищает строку */
void StringAddElem(String* that, char what); /* Добавляет в конец строки символ */
/*UPD*/void StringPutElem(String* that, int index, char what); /* Добавляет символ в место с указанным индексом */
void StringDelElem(String* that, int index); /* Удаляет символ с указанным индексом */
char* StringBegin(String* that); /* Возвращает указатель на начало строки */
int StringSize(String* that); /* Возвращает длину строки */
char StringGetElem(String* that, int index); /* Возвращает символ строки с указанным индексом */
void StringSetElem(String* that, int index, char what); /* Изменяет символ строки с указанным индексом */
int StringFindElem(String* that, char what); /* Возвращает индекс первого вхождения символа или -1 */
int StringFindLastElem(String* that, char what); /* Возвращает индекс последнего вхождения символа или -1 */
bool StringContainElem(String* that, char what); /* Возвращает существование вхождения символа */

void StringRead(String* that, char until, FILE* from); /* Читает строку между двумя символами, являющимися переносом, пробелом или until-символом */
void StringReadLn(String* that, FILE* from); /* Читает строку вместе с пробелами вплоть до переноса */
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

/* AA-Tree */
typedef struct TreeVertex TreeVertex;
struct TreeVertex {
    void* key;
    void* value;
    int level;
    TreeVertex* left;
    TreeVertex* right;
};

typedef struct Tree Tree;
struct Tree {
    int keysize;
    int valuesize;
    int size;
    bool (*comp) (void* a, void* b);
    void (*nuller) (void* a);
    TreeVertex* root;
};

Tree* TreeNew(int sizeof_key, int sizeof_value, bool (*key_comparator) (void* a, void* b), void (*value_nullator) (void* a));
void TreeClear(Tree* that);
void TreeDelete(Tree* that);
void TreeErase(Tree* that, void* index);
void TreeInsert(Tree* that, void* index);
void* TreeGetElem(Tree* that, void* index);

TreeVertex* TreeBegin(Tree* that);
TreeVertex* TreeLast(Tree* that);
TreeVertex* TreeEnd(Tree* that);
TreeVertex* TreeNext(Tree* that, TreeVertex* it);
TreeVertex* TreePrev(Tree* that, TreeVertex* it);
TreeVertex* TreeFind(Tree* that, void* index);

void NullInt(void* a);
/* AA-Tree */

/* Set */
typedef Tree Set;

Set* SetNew(int sizeof_element, bool (*comparator) (void* a, void* b));
void SetClear(Set* that) ;
void SetDelete(Set* that);
void SetAddElem(Set* that, void* what);
void SetDelElem(Set* that, void* what);
bool SetContainElem(Set* that, void* what);
int SetSize(Set* that);
/* Set */

#endif