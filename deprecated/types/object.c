#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "object.h"
#include "value.h"


#define ALLOCATE_OBJECT(type, oType) \
    (type*)allocateObject(sizeof(type), oType)

static Obj* allocateObject (size_t size, ObjectT type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    return object;
}


// STRING STUFF //
static OString* allocateString (char* chars, int len, uint32_t hash) {
    OString* string = ALLOCATE_OBJECT(OString, O_STRING);
    string->length = len;
    string->chars = chars;
    string->hash = hash;
    return string;
}

uint32_t hashString (const char* chars, int len) {
    uint32_t hash = 2166136261u;
    for (int key = 0; key < len; key++) {
        hash ^= (uint8_t)chars[key];
        hash *= 16777619;
    }
    return hash;
}

OString* genString (char* chars, int len) {
    uint32_t hash = hashString(chars, len);

/*
    OString* intern = findString(&vm.strings, chars, len, hash);

    if (intern != NULL) {
        FREE_ARRAY(char, chars, len + 1);
        return intern;
    }
*/

    return allocateString(chars, len, hash);
}

OString* copyString (const char* chars, int len) {
    uint32_t hash = hashString(chars, len);
/*
    OString* intern = findString(&vm.strings, chars, len, hash);

    if (intern != NULL) { return intern; }
*/
    char* heapChars = ALLOCATE(char, len + 1);

    memcpy(heapChars, chars, len);
    heapChars[len] = '\0';

    return allocateString(heapChars, len, hash);
}

 // FUNCTION STUFF //
OOperation* newOperation () {
    OOperation* op = ALLOCATE_OBJECT (OOperation, O_OPERATION);
    op->arity = 0;
    op->name = NULL;
    //initSequence(&op->sequence);
    return op;
}

static void printOperation (OOperation* op) {
    if (op->name == NULL) {
        printf("<script>");
        return;
    }

    printf("<op %s>", op->name->chars);
    return;
}

void printObject (Value value) {
    switch (OBJECT_TYPE(value)) {
        case O_OPERATION: {
            printOperation(AS_OPERATION(value));
            break;
        }
        case O_STRING:
            printf("%s", AS_CSTRING(value));
            break;
    }
    return;
}

// NUMERAL STUFF //

