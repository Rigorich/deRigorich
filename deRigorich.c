#include "deRigorich.h"

/*  */

void TestFunction(void) {
	printf("Hello, World!\n");
}

void MallocCheck(void* ptr) {
    if (!ptr) {
        printf("Not enough heap memory!");
        ERROR
    }
}

/* */


/* Vector */

Vector* VectorNew(int sizeof_element) {
	Vector* t = malloc(sizeof(Vector));
	MallocCheck(t);
	t->elemsize = sizeof_element;
	t->size = 0;
	t->begin = NULL;
	t->alloc = 0;
	return t;
}

void VectorClear(Vector* this) { 
	free(this->begin);
	this->begin = NULL;
	this->size = 0;
	this->alloc = 0;
}

void VectorDelete(Vector* this) { 
	VectorClear(this);
	free(this);
	this = NULL;
}

void VectorAddElem(Vector* this, void* what) {
    if (this->size + 1 > this->alloc) {
		VectorRealloc(this, this->alloc * 2 + 1);
    }
	this->size += 1;
	VectorSetElem(this, this->size - 1, what);
}

int VectorSize(Vector* this) { 
	return this->size; 
}

void* VectorGetElem(Vector* this, int index) { 
	if (index < 0 || VectorSize(this) <= index) {
		printf("Error: Wrond index (in Vector)");
		return;
	}
	return (void*)(this->begin + index * this->elemsize);
}

void VectorSetElem(Vector* this, int index, void* what) { 
	if (index < 0 || VectorSize(this) <= index) {
		printf("Error: Wrond index (in Vector)");
		return;
	}
	memcpy((this->begin + index * this->elemsize), what, this->elemsize);
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
	void* t = malloc(newalloc * this->elemsize);
	MallocCheck(t);
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
	fscanf(from, "%c", &sym);
	while (sym == ' ' || sym == '\n' || sym == until) { 
		fscanf(from, "%c", &sym);
    }
	while (sym != until && sym != '\n') {
		StringAddElem(this, sym);
		fscanf(from, "%c", &sym);
	}
	ungetc(sym, from);
}

void StringReadLn(Vector* this, FILE* from) {
	StringClear(this);
	char sym;
	fscanf(from, "%c", &sym);
	while (sym != '\n') {
		StringAddElem(this, sym);
		fscanf(from, "%c", &sym);
	}
}

bool StringToInt(char* from, long long* towhat, long long MinBound, long long MaxBound) {
    if (MinBound > MaxBound || MinBound < -MAXLL || MAXLL < MaxBound) {
        printf("Wrong StringToInt bounds!\n");
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
	bool Minus = false;
	if (from[0] == '-') {
		Minus = true;
    }
	double t = 0.0;
	int i;
	for (i = Minus; from[i]; i++){
        if (from[i] == '.' || from[i] == ',') {
            if (i == Minus) {
                return false;
            }
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
	if (from[i] == '.' || from[i] == ',') {
        i++;
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


/* */



/* */