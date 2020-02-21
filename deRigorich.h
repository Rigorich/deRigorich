
/* Version 21.02.2020 */
/* Автор: Никита Григорьевич (Гришаев), ст.гр. 953501 */

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

/* Функция экстренного завершения программы */
#define ERROR sleep(5); exit(42);

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

/* Проверка на успешное выделение памяти */
void MallocCheck(void* ptr);

/* Вектор */
typedef struct {
	int elemsize;
	int size;
	void* begin;
	int alloc;
} Vector;

Vector* VectorNew(int sizeof_element); /* Конструирует вектор и возвращает указатель на него */
void VectorDelete(Vector* this); /* Деструктор вектора */
void VectorClear(Vector* this); /* Очищает вектор */
void VectorAddElem(Vector* this, void* what); /* Добавляет в конец вектора элемент путём копирования */
int VectorSize(Vector* this); /* Возвращает количество элементов вектора */
void* VectorGetElem(Vector* this, int index); /* Возвращает указатель на элемент с указанным индексом // Используйте TO(type) */
void VectorSetElem(Vector* this, int index, void* what); /* Изменяет элемент с указанным индексом путём копирования */
void VectorResize(Vector* this, int newsize); /* Изменяет количество элементов массива */
void VectorRealloc(Vector* this, int newalloc); /* Изменяет количество памяти, выделенной для массива */
/* Вектор */

/* Строка */
Vector* StringNew(void); /* Конструирует строку и возвращает указатель на неё */
void StringDelete(Vector* this); /* Деструктор строки */
void StringClear(Vector* this); /* Очищает строку */
void StringAddElem(Vector* this, char what); /* Добавляет в конец строки символ */
char* StringBegin(Vector* this); /* Возвращает указатель на начало строки */
int StringSize(Vector* this); /* Возвращает длину строки */
char StringGetElem(Vector* this, int index); /* Возвращает символ строки с указанным индексом */
void StringSetElem(Vector* this, int index, char what); /* Изменяет символ строки с указанным индексом */

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


#endif