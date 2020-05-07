#include "deRigorich.h"

/*  */

void TestFunction(void) {
    printf("Hello, World!\n");
}

void FatalError(const char* message) { 
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
        printf("Failed allocation of %lld bytes!\n", ByteCount);
        printf("Not enough heap memory!\n");
        printf("Please close Google Chrome or that program!\n");
        printf("And press any key\n");
        getch();
        ptr = malloc(ByteCount);
    }
    return ptr;
}

void swap(void* a, void* b, int size) {
    if (a == b) return;
    void* tmp = Malloc(size);
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
    free(tmp);
}

void Subsort(void* arr, int elemsize, bool(*comp)(void*,void*), int* tmp, int* inds, int L, int R) {
    if (L == R)
        return;
    int M = (L + R) / 2 + 1;
    Subsort(arr, elemsize, comp, tmp, inds, L, M - 1);
    Subsort(arr, elemsize, comp, tmp, inds, M, R);
    int i = L, j = M, k = L;
    while (i < M && j <= R) {
        if (comp(arr + inds[j] * elemsize, arr + inds[i] * elemsize)) {
            tmp[k] = inds[j];
            k++;
            j++;
        } else {
            tmp[k] = inds[i];
            k++;
            i++;
        }
    }
    while (i < M) {
        tmp[k] = inds[i];
        k++;
        i++;
    }
    while (j <= R) {
        tmp[k] = inds[j];
        k++;
        j++;
    }
    for (k = L; k <= R; k++)
        inds[k] = tmp[k];
}

void sort(void* begin, void* end, int sizeof_element, bool (*comp) (void* a, void* b)) {
    int n = (end - begin) / sizeof_element;
    if (n < 2)
        return;
    int* tmp = (int*)Malloc(n * sizeof(int));
    int* inds = (int*)Malloc(n * sizeof(int));
    int i;
    for (i = 0; i < n; i++)
        inds[i] = i;
    Subsort(begin, sizeof_element, comp, tmp, inds, 0, n - 1);
    for (i = 0; i < n; i++)
        tmp[inds[i]] = i;
    for (i = 0; i < n; i++) {
        while (i != tmp[i]) {
            swap(begin + i * sizeof_element, begin + tmp[i] * sizeof_element, sizeof_element);
            swap(&(tmp[i]), &(tmp[tmp[i]]), sizeof(int));
        }
    }
    free(tmp);
    free(inds);
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
    Vector* t = (Vector*)Malloc(sizeof(Vector));
    t->elemsize = sizeof_element;
    t->size = 0;
    t->begin = NULL;
    t->alloc = 0;
    t->comp = NULL;
    return t;
}

void VectorClear(Vector* that) { 
    free(that->begin);
    that->begin = NULL;
    that->size = 0;
    that->alloc = 0;
    that->comp = NULL;
}

void VectorDelete(Vector* that) { 
    VectorClear(that);
    free(that);
    /* that = NULL; */
}

void VectorAddElem(Vector* that, void* what) {
    if (that->size + 1 > that->alloc) {
        VectorRealloc(that, that->alloc * 2 + 1);
    }
    that->size += 1;
    VectorSetElem(that, that->size - 1, what);
}

void VectorDelElem(Vector* that, int index) {
    if (index < 0 || VectorSize(that) <= index) {
        printf("Error: Wrond index (in VectorDelElem)");
        return;
    }
    int i;
    for (i = index + 1; i < VectorSize(that); i++) {
        VectorSetElem(that, i-1, VectorGetElem(that, i));
    }
    that->size -= 1;
}

int VectorSize(Vector* that) { 
    return that->size; 
}

void* VectorBegin(Vector* that) {
    return that->begin;
}

void* VectorGetElem(Vector* that, int index) { 
    if (index < 0 || VectorSize(that) <= index) {
        printf("Error: Wrond index (in VectorGetElem)");
        return NULL;
    }
    return (void*)((char*)(that->begin) + index * that->elemsize);
}

void VectorSetElem(Vector* that, int index, void* what) { 
    if (index < 0 || VectorSize(that) <= index) {
        printf("Error: Wrond index (in VectorSetElem)");
        return;
    }
    memcpy(((char*)(that->begin) + index * that->elemsize), what, that->elemsize);
}

void* VectorGetComp(Vector* that) {
    if (!that->comp) {
        FatalError("Error: Vector has no Comparator");
    }
    return that->comp;
}

void VectorSetComp(Vector* that, bool (*comparator) (void* a, void* b)) {
    that->comp = comparator;
}

void VectorSort(Vector* that) {
    sort(that->begin, that->begin + that->size * that->elemsize, that->elemsize, VectorGetComp(that));
}

void VectorSortComp(Vector* that, bool (*comp) (void* a, void* b)){
    bool(*tmp)(void*,void*) = that->comp;
    VectorSetComp(that, comp);
    VectorSort(that);
    VectorSetComp(that, tmp);
}

int VectorFindElem(Vector* that, void* what) {
    int i;
    for (i = 0; i < VectorSize(that); i++) {
        if (OperEqual(VectorGetElem(that, i), what, (bool(*)(void*,void*))VectorGetComp(that))) {
            return i;
        }
    }
    return -1;
}

int VectorFindLastElem(Vector* that, void* what) {
    int i;
    for (i = VectorSize(that)-1; i >= 0; i--) {
        if (OperEqual(VectorGetElem(that, i), what, (bool(*)(void*,void*))VectorGetComp(that))) {
            return i;
        }
    }
    return -1;
}

bool VectorContainElem(Vector* that, void* what) {
    return VectorFindElem(that, what) != -1;
}

void VectorRealloc(Vector* that, int newalloc) {
    if (newalloc < that->size) {
        printf("Warning: Wrong Vector Realloc!\n");
        return;
    }
    that->alloc = newalloc;
    if (newalloc == 0) {
        free(that->begin);
        that->begin = NULL;
        return;
    }
    void* t = Malloc(newalloc * that->elemsize);
    memcpy(t, that->begin, that->size * that->elemsize);
    free(that->begin);
    that->begin = t;
}

void VectorResize(Vector* that, int newsize) {
    if (newsize < 0) {
        printf("Warning: New Vector size < 0\n");
        newsize = 0;
    }
    that->size = newsize;
    VectorRealloc(that, newsize);
}

/* Vector */


/* String */

char NullChar = 0;

String* StringNew(void) {
    String* t = VectorNew(sizeof(char));
    VectorSetComp(t, CmpChar);
    VectorAddElem(t, &NullChar);
    return t;
}

void StringClear(String* that) { 
    VectorClear(that);
    VectorAddElem(that, &NullChar);
}

void StringDelete(String* that) { 
    VectorDelete(that);
}

void StringAddElem(String* that, char what) { 
    VectorAddElem(that, &NullChar);
    VectorSetElem(that, StringSize(that) - 1, &what);
}

void StringDelElem(String* that, int index) { 
    VectorDelElem(that, index);
}

char* StringBegin(String* that) {
    return (char*)(that->begin);
}

int StringSize(String* that) { 
    return that->size - 1; 
}

char StringGetElem(String* that, int index) { 
    return TO(char)VectorGetElem(that, index); 
}

void StringSetElem(String* that, int index, char what) { 
    VectorSetElem(that, index, &what); 
}

int StringFindElem(String* that, char what) {
    return VectorFindElem(that, &what);
}

int StringFindLastElem(String* that, char what) {
    return VectorFindLastElem(that, &what);
}

bool StringContainElem(String* that, char what) {
    if (!what) {
        return false;
    }
    return VectorContainElem(that, &what);
}

void StringIgnore(FILE* from) {
    char sym;
    fscanf(from, "%c", &sym);
    while (sym != '\n') {
        fscanf(from, "%c", &sym);
    }
}

void StringRead(String* that, char until, FILE* from) {
    StringClear(that);
    char sym;
    if (!feof(from)) {
        fscanf(from, "%c", &sym);
    }
    while ((sym == ' ' || sym == '\n' || sym == until) && !feof(from)) { 
        fscanf(from, "%c", &sym);
    }
    while (sym != until && sym != '\n' && !feof(from)) {
        StringAddElem(that, sym);
        fscanf(from, "%c", &sym);
    }
    if (!feof(from)) {
        ungetc(sym, from);
    }
}

void StringReadLn(String* that, FILE* from) {
    StringClear(that);
    char sym;
    if (!feof(from)) {
        fscanf(from, "%c", &sym);
    }
    while (sym != '\n' && !feof(from)) {
        StringAddElem(that, sym);
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
    String* s = StringNew();
    StringRead(s, ' ', from);
    long long t;
    while (!StringToInt((char*)(s->begin), &t, MinBound, MaxBound)) {
        printf("Wrong integer \"%s\".\n Try again:\n", s->begin);
        if (feof(from)) {
            printf("Not enough data in file!\n");
            sleep(5); getch(); exit(42);
        }
        StringRead(s, ' ', from);
    }
    StringDelete(s);
    return t;
}

double StringReadFloat(FILE* from) {
    String* s = StringNew();
    StringRead(s, ' ', from);
    double t;
    while (!StringToFloat((char*)(s->begin), &t)) {
        printf("Wrong float \"%s\".\n Try again:\n", s->begin);
        StringRead(s, ' ', from);
    }
    StringDelete(s);
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


/* AA-Tree */

TreeVertex* AATreeSkew(Tree* that, TreeVertex* vrt){
    if (!vrt)
        return NULL;
    if (!vrt->left)
        return vrt;
    if (vrt->level == vrt->left->level)
    {
        TreeVertex* rtrn = vrt->left;
        vrt->left = rtrn->right;
        rtrn->right = vrt;
        return rtrn;
    }
    return vrt;
}

TreeVertex* AATreeSplit(Tree* that, TreeVertex* vrt){
    if (!vrt)
        return NULL;
    if (!vrt->right)
        return vrt;
    if (!vrt->right->right)
        return vrt;
    if (vrt->level == vrt->right->right->level)
    {
        TreeVertex* rtrn = vrt->right;
        rtrn->level++;
        vrt->right = rtrn->left;
        rtrn->left = vrt;
        return rtrn;
    }
    return vrt;
}

TreeVertex* AATreeInsert(Tree* that, TreeVertex* vrt, void* x){
    if (!vrt) 
    {
        TreeVertex* tmp = (TreeVertex*)Malloc(sizeof(TreeVertex));
        tmp->key = Malloc(that->keysize);
        tmp->value = Malloc(that->valuesize);
        memcpy(tmp->key, x, that->keysize);
        that->nuller(tmp->value);
        tmp->level = 1;
        tmp->left = NULL;
        tmp->right = NULL;
        return tmp;
    }
    if (OperLess(x, vrt->key, that->comp))
        vrt->left = AATreeInsert(that, vrt->left, x);
    if (OperLess(vrt->key, x, that->comp))
        vrt->right = AATreeInsert(that, vrt->right, x);
    vrt = AATreeSkew(that, vrt);
    vrt = AATreeSplit(that, vrt);
    return vrt;
}

TreeVertex* AATreeFind(Tree* that, TreeVertex* vrt, void* x){
    if (!vrt)
        return NULL;
    if (OperLess(x, vrt->key, that->comp))
        return AATreeFind(that, vrt->left, x);
    if (OperLess(vrt->key, x, that->comp))
        return AATreeFind(that, vrt->right, x);
    return vrt;
}

TreeVertex* AATreeFindMore(Tree* that, TreeVertex* vrt, void* x){
    static TreeVertex* last;
    if (vrt == that->root)
        last = NULL;
    if (!vrt)
        return last;
    if (OperLess(x, vrt->key, that->comp))
    {
        last = vrt;
        return AATreeFindMore(that, vrt->left, x);
    }
    if(!OperGreat(vrt->key, x, that->comp))
    {
        return AATreeFindMore(that, vrt->right, x);
    }
}

TreeVertex* AATreeFindLess(Tree* that, TreeVertex* vrt, void* x){
    static TreeVertex* last;
    if (vrt == that->root)
        last = NULL;
    if (!vrt)
        return last;
    if (!OperGreat(x, vrt->key, that->comp))
    {
        return AATreeFindLess(that, vrt->left, x);
    }
    if (OperLess(vrt->key, x, that->comp))
    {
        last = vrt;
        return AATreeFindLess(that, vrt->right, x);
    }
}

TreeVertex* AATreeErase(Tree* that, TreeVertex* vrt, void* x){
    if (!vrt)
        return NULL;
    static TreeVertex* last;
    static TreeVertex* del = NULL;

    last = vrt;
    if (OperLess(x, vrt->key, that->comp))
    {
        vrt->left = AATreeErase(that, vrt->left, x);
    }
    else
    {
        del = vrt;
        vrt->right = AATreeErase(that, vrt->right, x);
    }

    if (vrt == last)
    {
        if (!del || !OperEqual(del->key, x, that->comp))
            return vrt;
        free(del->key);
        free(del->value);
        del->key = vrt->key;
        del->value = vrt->value;
        del = NULL;
        vrt = vrt->right;
        free(last);
        last = NULL;
    }

    else
    {
        if (((vrt->left ? vrt->left->level : 0) < vrt->level - 1) ||
            ((vrt->right ? vrt->right->level : 0) < vrt->level - 1))
        {
            vrt->level--;
            if (vrt->right && vrt->right->level > vrt->level)
                vrt->right->level = vrt->level;
            vrt = AATreeSkew(that, vrt);
            if (vrt->right)
            {
                vrt->right = AATreeSkew(that, vrt->right);
                if (vrt->right->right)
                    vrt->right->right = AATreeSkew(that, vrt->right->right);
            }
            vrt = AATreeSplit(that, vrt);
            if (vrt->right)
                vrt->right = AATreeSplit(that, vrt->right);
        }
    }
    return vrt;
}

void AATreeClear(Tree* that, TreeVertex* vrt){
    if (vrt)
    {
        AATreeClear(that, vrt->left);
        AATreeClear(that, vrt->right);
        free(vrt->key);
        free(vrt->value);
        free(vrt);
    }
}
    
    
Tree* TreeNew(int sizeof_key, int sizeof_value, bool (*key_comparator) (void* a, void* b), void (*value_nullator) (void* a)){
    Tree* t = (Tree*)Malloc(sizeof(Tree));
    t->keysize = sizeof_key;
    t->valuesize = sizeof_value;
    t->size = 0;
    t->comp = key_comparator;
    t->nuller = value_nullator;
    t->root = NULL;
    return t;
}

void TreeClear(Tree* that){
    AATreeClear(that, that->root);
    that->root = NULL;
}

void TreeDelete(Tree* that){
    TreeClear(that);
    free(that);
    /* that = NULL; */
}

void TreeErase(Tree* that, void* index){
    if(TreeFind(that, index) != TreeEnd(that))
    {
        that->root = AATreeErase(that, that->root, index);
        that->size--;
    }
}

void TreeInsert(Tree* that, void* index){
    if(TreeFind(that, index) == TreeEnd(that))
    {
        that->root = AATreeInsert(that, that->root, index);
        that->size++;
    }
}

void* TreeGetElem(Tree* that, void* index){
    TreeInsert(that, index);
    return AATreeFind(that, that->root, index)->value;
}

TreeVertex* TreeBegin(Tree* that){
    TreeVertex* vrt = that->root;
    while (vrt && vrt->left)
        vrt = vrt->left;
    return vrt;
}

TreeVertex* TreeLast(Tree* that){
    TreeVertex* vrt = that->root;
    while (vrt && vrt->right)
        vrt = vrt->right;
    return vrt;
}

TreeVertex* TreeEnd(Tree* that){
    return NULL;
}

TreeVertex* TreeNext(Tree* that, TreeVertex* it){
    return AATreeFindMore(that, that->root, it->key);
}

TreeVertex* TreePrev(Tree* that, TreeVertex* it){
    return AATreeFindLess(that, that->root, it->key);
}

TreeVertex* TreeFind(Tree* that, void* index){
    return AATreeFind(that, that->root, index);
}


void NullInt(void* a) {
    (TO(int) a) = 0;
}
void NullVoid(void* a) { }

/* AA-Tree */


/* Set */

Set* SetNew(int sizeof_element, bool (*comparator) (void* a, void* b)){
    return TreeNew(sizeof_element, 0, comparator, NullVoid);
}

void SetClear(Set* that) { 
    TreeClear(that);
}

void SetDelete(Set* that) { 
    TreeDelete(that);
}

void SetAddElem(Set* that, void* what) {
    TreeInsert(that, what);
}

void SetDelElem(Set* that, void* what) {
    TreeErase(that, what);
}

bool SetContainElem(Set* that, void* what) {
    return TreeFind(that, what) != TreeEnd(that);
}

int SetSize(Set* that) { 
    return that->size; 
}

/* Set */