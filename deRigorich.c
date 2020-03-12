#include "deRigorich.h"

/* */

void TestFunction(void) {
    printf("Hello, World!\n");
}

void FatalError(char* message) { 
    printf("%s", message); 
    sleep(5); 
    getch(); 
    exit(42); 
};

void MallocCheck(void* ptr) {
    if (!ptr) {
        FatalError("Not enough heap memory!");
    }
}

void* Malloc(long long ByteCount) {
    if (ByteCount < 0) {
        printf("Warning: try to alloc wrong byte amount!\n");
        return NULL;
    }
    void* ptr = malloc(ByteCount);
    while (!ptr) {
        printf("\n");
        printf("Not enough heap memory!\n");
        printf("Please close Google Chrome or this program!\n");
        printf("And press any key\n");
        getch();
        ptr = malloc(ByteCount);
    }
    return ptr;
}

void swap(void* a, void* b, int size) {
    void* tmp = Malloc(size);
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
    free(tmp);
}

/* */

/* Comparators */ 

bool CmpInt(void* a, void* b) {
    return TO(int)a < TO(int)b;
}

bool CmpDouble(void* a, void* b) {
    return TO(double)a < TO(double)b;
}

bool CmpChar(void* a, void* b) {
    return TO(char)a < TO(char)b;
}

bool CmpCharArray(void* a, void* b) {
    if(strcmp(TO(char*)a, TO(char*)b) < 0) return true;
    return false;
}

bool OperLess(void* a, void* b, bool (*cmp)(void*, void*)) {
    return cmp(a, b);
}

bool OperEqual(void* a, void* b, bool (*cmp)(void*, void*)) {
    return (!cmp(a, b) && !cmp(b, a));
}

bool OperGreat(void* a, void* b, bool (*cmp)(void*, void*)) {
    return cmp(b, a);
}

/* Comparators */

/* Vector */

Vector* VectorNew(int sizeof_element) {
    Vector* t = Malloc(sizeof(Vector));
    t->elemsize = sizeof_element;
    t->size = 0;
    t->begin = NULL;
    t->alloc = 0;
    t->comp = NULL;
    return t;
}

void VectorClear(Vector* this) { 
    free(this->begin);
    this->begin = NULL;
    this->size = 0;
    this->alloc = 0;
    this->comp = NULL;
}

void VectorDelete(Vector* this) { 
    VectorClear(this);
    free(this);
    /* this = NULL; */
}

void VectorAddElem(Vector* this, void* what) {
    if (this->size + 1 > this->alloc) {
        VectorRealloc(this, this->alloc * 2 + 1);
    }
    this->size += 1;
    VectorSetElem(this, this->size - 1, what);
}

void VectorDelElem(Vector* this, int index) {
    if (index < 0 || VectorSize(this) <= index) {
        printf("Error: Wrond index (in VectorDelElem)");
        return;
    }
    int i;
    for (i = index + 1; i < VectorSize(this); i++) {
        VectorSetElem(this, i-1, VectorGetElem(this, i));
    }
    this->size -= 1;
}

int VectorSize(Vector* this) { 
    return this->size; 
}

void* VectorBegin(Vector* this) {
    return this->begin;
}

void* VectorGetElem(Vector* this, int index) { 
    if (index < 0 || VectorSize(this) <= index) {
        printf("Error: Wrond index (in VectorGetElem)");
        return;
    }
    return (void*)(this->begin + index * this->elemsize);
}

void VectorSetElem(Vector* this, int index, void* what) { 
    if (index < 0 || VectorSize(this) <= index) {
        printf("Error: Wrond index (in VectorSetElem)");
        return;
    }
    memcpy((this->begin + index * this->elemsize), what, this->elemsize);
}

void* VectorGetComp(Vector* this) {
    if (!this->comp) {
        FatalError("Error: Vector has no Comparator");
    }
    return this->comp;
}

void VectorSetComp(Vector* this, bool (*comparator) (void* a, void* b)) {
    this->comp = comparator;
}

int VectorFindElem(Vector* this, void* what) {
    int i;
    for (i = 0; i < VectorSize(this); i++) {
        if (OperEqual(VectorGetElem(this, i), what, VectorGetComp(this))) {
            return i;
        }
    }
    return -1;
}

int VectorFindLastElem(Vector* this, void* what) {
    int i;
    for (i = VectorSize(this)-1; i >= 0; i--) {
        if (OperEqual(VectorGetElem(this, i), what, VectorGetComp(this))) {
            return i;
        }
    }
    return -1;
}

bool VectorContainElem(Vector* this, void* what) {
    return VectorFindElem(this, what) != -1;
}

void VectorSort(Vector* this){
    int i, j;
    for(i = 0; i < VectorSize(this); i++)
    for(j = i + 1; j < VectorSize(this); j++)
    if (OperLess(VectorGetElem(this, j), VectorGetElem(this, i), VectorGetComp(this))) {
        swap(VectorGetElem(this, j), VectorGetElem(this, i), this->elemsize);
    }
}

void VectorSortComp(Vector* this, bool (*comp) (void* a, void* b)){
    void* tmp = this->comp;
    VectorSetComp(this, comp);
    VectorSort(this);
    VectorSetComp(this, tmp);
}

void VectorRealloc(Vector* this, int newalloc) {
    if (newalloc < this->size) {
        printf("Warning: Wrong Vector Realloc!\n");
        return;
    }
    this->alloc = newalloc;
    if (newalloc == 0) {
        free(this->begin);
        this->begin = NULL;
        return;
    }
    void* t = Malloc(newalloc * this->elemsize);
    memcpy(t, this->begin, this->size * this->elemsize);
    free(this->begin);
    this->begin = t;
}

void VectorResize(Vector* this, int newsize) {
    if (newsize < 0) {
        printf("Warning: New Vector size < 0\n");
        newsize = 0;
    }
    this->size = newsize;
    VectorRealloc(this, newsize);
}

/* Vector */


/* String */

char NullChar = 0;

Vector* StringNew(void) {
    Vector* t = VectorNew(sizeof(char));
    VectorSetComp(t, CmpChar);
    VectorAddElem(t, &NullChar);
    return t;
}

void StringClear(Vector* this) { 
    VectorClear(this);
    VectorAddElem(this, &NullChar);
}

void StringDelete(Vector* this) { 
    VectorDelete(this);
}

void StringAddElem(Vector* this, char what) { 
    VectorAddElem(this, &NullChar);
    VectorSetElem(this, StringSize(this) - 1, &what);
}

void StringDelElem(Vector* this, int index) { 
    VectorDelElem(this, index);
}

char* StringBegin(Vector* this) {
    return this->begin;
}

int StringSize(Vector* this) { 
    return this->size - 1; 
}

char StringGetElem(Vector* this, int index) { 
    return TO(char)VectorGetElem(this, index); 
}

void StringSetElem(Vector* this, int index, char what) { 
    VectorSetElem(this, index, &what); 
}

int StringFindElem(Vector* this, char what) {
    return VectorFindElem(this, &what);
}

int StringFindLastElem(Vector* this, char what) {
    return VectorFindLastElem(this, &what);
}

bool StringContainElem(Vector* this, char what) {
    if (!what) {
        return false;
    }
    return VectorContainElem(this, &what);
}

void StringIgnore(FILE* from) {
    char sym;
    fscanf(from, "%c", &sym);
    while (sym != '\n') {
        fscanf(from, "%c", &sym);
    }
}

void StringRead(Vector* this, char until, FILE* from) {
    StringClear(this);
    char sym;
    if (!feof(from)) {
        fscanf(from, "%c", &sym);
    }
    while ((sym == ' ' || sym == '\n' || sym == until) && !feof(from)) { 
        fscanf(from, "%c", &sym);
    }
    while (sym != until && sym != '\n' && !feof(from)) {
        StringAddElem(this, sym);
        fscanf(from, "%c", &sym);
    }
    if (!feof(from)) {
        ungetc(sym, from);
    }
}

void StringReadLn(Vector* this, FILE* from) {
    StringClear(this);
    char sym;
    if (!feof(from)) {
        fscanf(from, "%c", &sym);
    }
    while (sym != '\n' && !feof(from)) {
        StringAddElem(this, sym);
        fscanf(from, "%c", &sym);
    }
}

bool StringToInt(char* from, long long* towhat, long long MinBound, long long MaxBound) {
    if (MinBound > MaxBound || MinBound < -MAXLL || MAXLL < MaxBound) {
        printf("Wrong StringToInt bounds!\n");
        return false;
    }
    if (!from[0]) {
        return false;
    }
    bool Minus = false;
    if (from[0] == '-') {
        Minus = true;
    }
    unsigned long long t = 0;
    int i;
    for (i = Minus; from[i]; i++) {
        if (from[i] < '0' || '9' < from[i]) {
            return false;
        }
        else if (t * 10 + (from[i] - '0') > MAXLL) {
            return false;
        }
        else {
            t *= 10;
            t += (from[i] - '0');
        }
    }
    if (t > MAXLL) {
        return false;
    }
    long long tmp = t;
    if (Minus) {
        tmp = -tmp;
    }
    if (tmp < MinBound || MaxBound < tmp) {
        return false;
    }
    *towhat = tmp;
    return true;
}

bool StringToFloat(char* from, double* towhat) {
    if (!from[0]) {
        return false;
    }
    bool Minus = false;
    if (from[0] == '-') {
        Minus = true;
    }
    double t = 0.0;
    int i = Minus;
    for ( ; from[i]; i++){
        if (from[i] == '.' || from[i] == ',') {
            if (i == Minus) {
                return false;
            }
            i++;
            break;
        }
        else if (from[i] < '0' || '9' < from[i]) {
            return false;
        }
        else {
            t *= 10.0;
            t += (from[i] - '0');
        }
    }
    double p = 1.0;
    for ( ; from[i]; i++) {
        if (from[i] < '0' || '9' < from[i]) {
            return false;
        }
        else {
            p /= 10.0;
            t += p * (from[i] - '0');
        }
    }
    if (Minus) {
        t = -t;
    }
    *towhat = t;
    return true;
}

long long StringReadInt(FILE* from, long long MinBound, long long MaxBound) {
    Vector* s = StringNew();
    StringRead(s, ' ', from);
    long long t;
    while (!StringToInt(s->begin, &t, MinBound, MaxBound)) {
        printf("Wrong integer \"%s\".\n Try again:\n", s->begin);
        if (feof(from)) {
            printf("Not enough data in file!\n");
            sleep(5); getch(); exit(42);
        }
        StringRead(s, ' ', from);
    }
    VectorDelete(s);
    return t;
}

double StringReadFloat(FILE* from) {
    Vector* s = StringNew();
    StringRead(s, ' ', from);
    double t;
    while (!StringToFloat(s->begin, &t)) {
        printf("Wrong float \"%s\".\n Try again:\n", s->begin);
        StringRead(s, ' ', from);
    }
    VectorDelete(s);
    return t;
}

/* String */


/* SomeMath */

double absf(double x) {
    if (x < 0.0) return -x;
    return x;
}

long long fact_int(int x) {
    long long t = 1;
    if (x < 0 || 20 < x) {
        return 0;
    }
    for ( ; x > 0; x--) t *= x;
    return t;
}

double fact_float(int x) {
    double t = 1.0;
    for ( ; x > 0; x--) t *= x;
    if (!finite(t) || x < 0) {
        return 0;
    }
    return t;
}

/* SomeMath */


/* Set */

Set* SetNew(int sizeof_element, bool (*comparator) (void* a, void* b)){
    Set* t = Malloc(sizeof(Set));
    t->elemsize = sizeof_element;
    t->size = 0;
    t->base = VectorNew(sizeof_element);
    VectorSetComp(t->base, comparator);
    t->comp = comparator;
    return t;
}

void SetClear(Set* this) { 
    VectorClear(this->base);
    VectorSetComp(this->base, this->comp);
    this->size = 0;
}

void SetDelete(Set* this) { 
    SetClear(this);
    free(this);
    /* this = NULL; */
}

void SetAddElem(Set* this, void* what) {
    if (!VectorContainElem(this->base, what)) {
        VectorAddElem(this->base, what);
        this->size += 1;
        VectorSort(this->base);
    }
}

void SetDelElem(Set* this, void* what) {
    if (VectorContainElem(this->base, what)) {
        VectorDelElem(this->base, VectorFindElem(this->base, what));
        this->size -= 1;
    }
}

bool SetContainElem(Set* this, void* what) {
    return VectorContainElem(this->base, what);
}

int SetSize(Set* this) { 
    return this->size; 
}

/* Set */


/* Map */

Map* MapNew(int sizeof_key, int sizeof_value, bool (*comparator) (void* a, void* b), void (*value_nullator) (void* a)){
    Map* t = Malloc(sizeof(Map));
    t->keysize = sizeof_key;
    t->valuesize = sizeof_value;
    t->size = 0;
    t->keys = VectorNew(sizeof_key);
    VectorSetComp(t->keys, comparator);
    t->values = VectorNew(sizeof_value);
    t->comp = comparator;
    t->nuller = value_nullator;
    return t;
}

void MapClear(Map* this) { 
    VectorClear(this->keys);
    VectorSetComp(this->keys, this->comp);
    VectorClear(this->values);
    this->size = 0;
}

void MapDelete(Map* this) { 
    MapClear(this);
    free(this);
    /* this = NULL; */
}

void* MapValue(Map* this, void* what) {
    if (VectorContainElem(this->keys, what)) {
        return VectorGetElem(this->values, VectorFindElem(this->keys, what));
    } else {
        this->size += 1;
        VectorAddElem(this->keys, what);
        void* tmp = Malloc(this->valuesize);
        VectorAddElem(this->values, tmp);
        free(tmp);
        this->nuller(VectorGetElem(this->values, VectorSize(this->values)-1));
        return VectorGetElem(this->values, VectorFindElem(this->keys, what));
    }
}

bool MapContainElem(Map* this, void* what) {
    return VectorContainElem(this->keys, what);
}

int MapSize(Map* this) { 
    return this->size; 
}

void NullInt(void* a) {
    (TO(int) a) = 0;
}

/* Map */