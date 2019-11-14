#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

class Gabarito
{
    public:
        char PripComand[5];
        char **comand;
        int comandAntes;
        int comandDep;
        int numcomand;

        Gabarito();
        void Inicializa_Gabarito(int comantes, int comdep);
        char Salve_Gabarito(FILE *fp);
};


/*class Codigo
{
    public:
        char **comanCod;
        int qualgab;
        int ncomand;
        int comenta[2]; //Se tiver comentari antes ou depois o é igual a 1, se não igual a 0

        Codigo();
        void Inicializa_Codigo(int comentario);
        void Armazena_Codigo(FILE *fp, Gabarito gab, int num, char tipoequip[], int *i, int *j, char **cd, int nlinhas);
};*/


#endif // MAIN_H_INCLUDED
