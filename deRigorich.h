
/* Version 12.03.2020 BETA */
/* �����: ������ ����������� (�������), ��.��. 953501 */

/*
    ������� ���� ��� ����� � ������:
    - ��������� ����� -- ANSI
    - #include <windows.h>
    - SetConsoleCP(1251); 
    - SetConsoleOutputCP(1251);
*/

#ifndef DERIGORICH_H
#define DERIGORICH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ��������������� ��������� � ���� // TO(int)(void* p) == *(int*)(void* p) */
#define TO(T) *(T*)

/* ��� ������ bool */
#define bool int
#define true 1
#define false 0

/* ������� ������������� ����� */
#define MAXINT 1000000000
#define MAXLL 1000000000000000000
#define INTBOUNDS -MAXINT, MAXINT
#define LLBOUNDS -MAXLL, MAXLL

/* ������� �������� � ��������� */
#define min(x,y) (((x) < (y)) ? (x) : (y))
#define max(x,y) (((x) > (y)) ? (x) : (y))

/* ���� ��������� ����������� ������ */
#define ESC 27
#define SPACE 32
#define ENTER 13
#define TAB 9

/* ����-������� */
void TestFunction(void); 

/* ������� ����������� ���������� ��������� */
void FatalError(char* message);

/* ��������� ������ */
void* Malloc(long long ByteCount); /* ��������� ������ � ��������� */
void MallocCheck(void* ptr); /* �������� �� �������� ��������� ������ */

/* ���������� */
void swap(void* a, void* b, int size); /* ����� ������� ���� ��������� ������� size */

/* ������ */
typedef struct {
    int elemsize;
    int size;
    void* begin;
    int alloc;
    bool (*comp) (void* a, void* b);
} Vector;

Vector* VectorNew(int sizeof_element); /* ������������ ������ � ���������� ��������� �� ���� */
void VectorDelete(Vector* this); /* ���������� ������� */
void VectorClear(Vector* this); /* ������� ������ */
void VectorAddElem(Vector* this, void* what); /* ��������� � ����� ������� ������� ���� ����������� */
void VectorDelElem(Vector* this, int index); /* ������� ������� ������� � ��������� �������� */
int VectorSize(Vector* this); /* ���������� ���������� ��������� ������� */
void* VectorBegin(Vector* this); /* ���������� ��������� �� ������ ������� */
void* VectorGetElem(Vector* this, int index); /* ���������� ��������� �� ������� � ��������� �������� // ����������� TO(type) */
void VectorSetElem(Vector* this, int index, void* what); /* �������� ������� � ��������� �������� ���� ����������� */
int VectorFindElem(Vector* this, void* what); /* ���������� ������ ������� ��������� �������� ��� -1 */
int VectorFindLastElem(Vector* this, void* what); /* ���������� ������ ���������� ��������� �������� ��� -1 */
bool VectorContainElem(Vector* this, void* what); /* ���������� ������������� ��������� �������� */
void VectorSort(Vector* this); /* ��������� ������ */
void VectorSortComp(Vector* this, bool (*comp) (void* a, void* b)); /* ��������� ������ � �������������� ����������� */
void VectorResize(Vector* this, int newsize); /* �������� ���������� ��������� ������� */
void VectorRealloc(Vector* this, int newalloc); /* �������� ���������� ������, ���������� ��� ������� */
/* ������ */

/* ������ */
Vector* StringNew(void); /* ������������ ������ � ���������� ��������� �� �� */
void StringDelete(Vector* this); /* ���������� ������ */
void StringClear(Vector* this); /* ������� ������ */
void StringAddElem(Vector* this, char what); /* ��������� � ����� ������ ������ */
/*UPD*/void StringPutElem(Vector* this, int index, char what); /* ��������� ������ � ����� � ��������� �������� */
void StringDelElem(Vector* this, int index); /* ������� ������ � ��������� �������� */
char* StringBegin(Vector* this); /* ���������� ��������� �� ������ ������ */
int StringSize(Vector* this); /* ���������� ����� ������ */
char StringGetElem(Vector* this, int index); /* ���������� ������ ������ � ��������� �������� */
void StringSetElem(Vector* this, int index, char what); /* �������� ������ ������ � ��������� �������� */
int StringFindElem(Vector* this, char what); /* ���������� ������ ������� ��������� ������� ��� -1 */
int StringFindLastElem(Vector* this, char what); /* ���������� ������ ���������� ��������� ������� ��� -1 */
bool StringContainElem(Vector* this, char what); /* ���������� ������������� ��������� ������� */

void StringRead(Vector* this, char until, FILE* from); /* ������ ������ ����� ����� ���������, ����������� ���������, �������� ��� until-�������� */
void StringReadLn(Vector* this, FILE* from); /* ������ ������ ������ � ��������� ������ �� �������� */
void StringIgnore(FILE* from); /* ��������� ������ ������ �� ��������� ������ */

bool StringToInt(char* from, long long* towhat, long long MinBound, long long MaxBound); /* �������� ������������� ������ � ����� ����� � �������� [MinBound, MaxBound] */
bool StringToFloat(char* from, double* towhat); /* �������� ������������� ������ � ������������ ����� */

long long StringReadInt(FILE* from, long long MinBound, long long MaxBound); /* ����������� ����� ����� � �������� [MinBound, MaxBound] */
double StringReadFloat(FILE* from); /* ����������� ������������ ����� */
/* ������ */

/* ���������� */
double absf(double x); /* ���������� ������ ������������� ����� */
long long fact_int(int x); /* ���������� ������������� ��������� ����� x */
double fact_float(int x); /* ���������� ������������ ��������� ����� x */
/* ���������� */

/* ��������� ������ */
bool OperLess(void* a, void* b, bool (*cmp)(void*, void*));
bool OperEqual(void* a, void* b, bool (*cmp)(void*, void*));
bool OperGreat(void* a, void* b, bool (*cmp)(void*, void*));

bool CmpInt(void* a, void* b);
bool CmpDouble(void* a, void* b);
bool CmpChar(void* a, void* b);
bool CmpCharArray(void* a, void* b);
/* ��������� ������ */

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