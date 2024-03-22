#ifndef view_h
#define view_h

#define head "                            VECTORS CALCULATOR\n/=========================================================================\\"
#define end "|                                                                         |\n\\=========================================================================/"
#define funcs "|                        1.___Scalar multiply____                         |\n|                        2.______Addition________                         |\n|                        3._______Delete_________                         |\n|                        4._____Set element______                         |\n|                        5.________Back__________                         |"
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include "../vector/vector.h"

typedef enum {
    MENU_ST,
    FUNC_CHOICE_ST,
    VECT_INFO_ST,
    VECT_LIST_ST,
    CRT_ST
} interfaceState_e;

typedef struct Interface{
    interfaceState_e state;
    char *logs;
    vector *vecs;
    int vecsCnt;
    int currVectIndex;
} Interface;

char *getText(char *filename) {
    FILE *file;
    char menuText[900];
    file = fopen(filename, "r");
    fseek(file, 0L, SEEK_END);
    long size = ftello(file); 
    fseek(file, 0L, SEEK_SET); 
    char* buf = (char*)malloc(sizeof(char) * size);
    fread(buf, 1, size, file);
    fclose(file);
    return buf;
}

Interface * createInterface(interfaceState_e state) {
    Interface * newInterface = malloc(sizeof(Interface));
    newInterface->state = state;
    newInterface->vecs = malloc(sizeof(vector));
    return newInterface;
}

void CreateVectorView(Interface *interface) {
    char* text = getText("templates/createVec.txt");
    fprintf(stdout, "%s", text);
    printf("\n\n >>> ");
    char vecName[15];
    int vecSize;
    char vecType[7];
    scanf("%s %s %d", &vecName, &vecType, &vecSize);
    vector *vcts = interface->vecs;
    interface->vecs = malloc(sizeof(vector) * (interface->vecsCnt + 1));
    for (int i = 0; i < interface->vecsCnt; ++i) {
        interface->vecs[i] = vcts[i];
    }
    if (!strcmp(vecType, "int")){
        interface->vecs[(interface->vecsCnt == 0) ? 0:interface->vecsCnt] = \
        *createVector(vecSize, INT_VEC, &vecSize, 0, vecName);
        interface->vecsCnt += 1;
        interface->state = MENU_ST;
    } else
    if (!strcmp(vecType, "double")){
        interface->vecs[(interface->vecsCnt == 0) ? 0:interface->vecsCnt] = \
        *createVector(vecSize, DOUBLE_VEC, &vecSize, 0, vecName);
        interface->vecsCnt += 1;
        interface->state = MENU_ST;
    } else
    if (!strcmp(vecType, "complex")){
        interface->vecs[(interface->vecsCnt == 0) ? 0:interface->vecsCnt] = \
        *createVector(vecSize, COML_VEC, &vecSize, 0, vecName);
        interface->vecsCnt += 1;
        interface->state = MENU_ST;
    }
    free(vcts);
}

void deleteVectorByIndex(Interface *interface, int index) {
    if (index >= 0 && index < interface->vecsCnt) {
        for (int i = index; i < interface->vecsCnt - 1; ++i) {
            interface->vecs[i] = interface->vecs[i + 1];
        }
        interface->vecs = realloc(interface->vecs, interface->vecsCnt - 1);
        interface->vecsCnt --;
    }
}

char * makeString(int n, char *txt, int center) {
    if (!center) {
        printf("|");
        for (int i = 0; i < 9; ++i) printf(" ");
        printf("%d. %s", n, txt);
        int spCnt = strlen(txt); do { spCnt++; } while (n /= 10);
        for (int i = 0; i < 62 - spCnt; ++i) printf(" ");
        printf("|\n");
    }
    else {
        printf("|");
        for (int i = 0; i < (73 - strlen(txt)) / 2; ++i) printf(" ");
        printf("%s", txt);
        for (int i = 0; i < (73 - strlen(txt)) / 2 + ((strlen(txt) % 2) ? 0 : 1); ++i) printf(" ");
        printf("|\n");
    }
}

vector * VecListView(Interface *interface, int page) {
    char * beg = getText("templates/Head.txt");
    while (1) {
        system("clear");
        printf("%s\n", beg);
        makeString(-1, "", 1);
        int stringCnt = 0;
        for (int v = (page - 1) * 4; v < MIN(page * 4, interface->vecsCnt); ++v) {
            makeString(v + 1, interface->vecs[v].name, 0);
            stringCnt ++;
        }
        for (int i = 0; i < 4 - stringCnt; ++i) 
            makeString(-1, "", 1);
        makeString(-1, "M.__Menu___", 1);
        if (interface->vecsCnt > 4) makeString(-1, "<   >", 1);
        else makeString(-1, "", 1);
        printf("%s\n >>> ", end);
        char userAnswer[10];
        scanf("%s", &userAnswer);
        if (!strcmp(userAnswer, ">")) {
            if ((interface->vecsCnt+4) / ((page + 1) * 4)) page ++;
        } else if (!strcmp(userAnswer, "<")) {
            if (page > 1) page --;
        } else if (!strcmp(userAnswer, "M")) {
            interface->state = MENU_ST;
            break;
        } else if (isdigit(userAnswer)) {
            int vecNum = atoi(userAnswer) - 1;
            if (vecNum >= 0 && vecNum < interface->vecsCnt) {
                interface->currVectIndex = vecNum;
                interface->state = VECT_INFO_ST;
            } 
            break;
        } else {}
    }
}

char* typeToString(Type type) {
    switch (type)
    {
    case INT_VEC:
        return "int";
        break;
    case DOUBLE_VEC:
        return "double";
        break;
    case COML_VEC:
        return "complex";
        break;
    }
}

void setValueView(vector *vector) {
    printf(" Index of element >>> ");
    int elemIndex;
    scanf("%d", &elemIndex);
    elemIndex--;
    switch (vector->type)
    {
    case INT_VEC:
        printf(" Value of element >>> ");
        int valueI;
        scanf("%d", &valueI);
        vector->setValue(vector, 
                        &valueI, elemIndex, 
                        INT_VEC);
        break;
    case DOUBLE_VEC:
        printf(" Value of element >>> ");
        double valueD;
        scanf("%lf", &valueD);
        vector->setValue(&vector, 
                        &valueD, elemIndex, 
                        DOUBLE_VEC);
        break;
    case COML_VEC:
        ComplexNumber valueC;
        printf(" Real value of element >>> ");
        scanf("%lf", &valueC.re);
        printf("\n Imaginary value of element >>> ");
        scanf("%lf", &valueC.re);
        vector->setValue(&vector, 
                        &valueC, elemIndex, 
                        COML_VEC);
        break;
    }
}

int isNum(char str[15]) {
    for (int i = 0; i < strlen(str); ++i) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

vector * findVector(Interface *interface, char *By) {
    if (isNum(By)) {
        int vecNum = atoi(By) - 1;
        if (vecNum >= 0 && vecNum < interface->vecsCnt)
            return &interface->vecs[vecNum];
    } else {
        for (int i = 0; i < interface->vecsCnt; ++i) {
            if (strcmp(interface->vecs[i].name, By) == 0) {
                return &interface->vecs[i];
            }
        }
    }
}

void * voidToString(void *value, Type type, char result[100]) {
    switch (type)
    {
    case INT_VEC:
        sprintf(result, "%d", *((int*)value));
        break;
    case DOUBLE_VEC:
        sprintf(result, "%lf", *((double*)value));
        break;
    case COML_VEC:
        ComplexNumber tmpNum;
        tmpNum.re = ((ComplexNumber*)value)->re;
        tmpNum.im = ((ComplexNumber*)value)->im;
        sprintf(result,"(%lf, %lf)", tmpNum.re, tmpNum.im);
        break;
    }
    return result;
}

void VectInfoView(Interface *interface, int vectIndex) {
    system("clear");
    printf("%s\n", head);
    makeString(-1, "", 1);
    vector *thisVector = &interface->vecs[vectIndex];
    char vecChar[30] = "", typeChar[30] = "", sizeChar[30] = "";
    sprintf(vecChar, "Vector %s", thisVector->name);
    sprintf(typeChar, "Type: %s", typeToString(thisVector->type));
    sprintf(sizeChar, "Size: %d", thisVector->size);
    makeString(-1, vecChar, 1);
    makeString(-1, typeChar, 1);
    makeString(-1, sizeChar, 1);
    char *vectorString = thisVector->toString(thisVector);
    makeString(-1, vectorString, 1);
    printf("%s\n", funcs);
    makeString(-1, "", 1);
    printf("%s\n >>> ", end);
    char userAnswer[15];
    scanf("%s", &userAnswer);
    switch (atoi(userAnswer))
    {\
    case 1:
        if (interface->vecsCnt > 1) {
            printf(" Enter the name or number of vector >>> ");
            scanf("%s", &userAnswer);
            vector *multVec = findVector(interface, userAnswer);
            if (multVec != NULL) {
                void *voidResult = thisVector->scalarMultiply(thisVector, multVec);
                char charResult[100];
                voidToString(voidResult, thisVector->type, charResult);
                printf(" RESULT OF MULTIPLYING: %s\n", charResult);
                printf(" Press any key to continue >>> ");
                scanf("%s", &userAnswer);
            }
        }
        break;
    case 2:
        /* code */
        break;
    case 3:
        deleteVectorByIndex(interface, vectIndex);
        interface->currVectIndex = -1;
        interface->state = VECT_LIST_ST;
        break;
    case 4:
        setValueView(thisVector);
        break;
    case 5:
        interface->state = VECT_LIST_ST;
        interface->currVectIndex = -1;
        break;
    
    default:
        break;
    }
}

void RunMenuView(Interface* interface) {
    char *text = getText("templates/menu.txt");
    fprintf(stdout, "%s", text);
    printf("\n\n >>> ");
    char userAnswer;
    scanf("%c", &userAnswer);
    printf("\n");
    switch (userAnswer)
    {
    case '1':
        interface->state = VECT_LIST_ST;
        break;
    case '2':
        interface->state = CRT_ST;
        break;
    case '3':
        free(interface->vecs);
        interface->vecsCnt = 0;
        break;
    case '4':
        exit(0);
        break;
    default:
        break;
    }
}

void RunProcess(Interface* interface) {
    while(1) {
        switch (interface->state)
        {
        case MENU_ST:
            RunMenuView(interface);
            break;
        case VECT_INFO_ST:
            VectInfoView(interface, interface->currVectIndex);
            break;
        case VECT_LIST_ST:
            VecListView(interface, 1);
            break;
        case CRT_ST:
            CreateVectorView(interface);
            break;
        default:
            break;
        }
        system("clear");
    }
};

#endif