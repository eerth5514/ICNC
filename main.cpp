#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<ctype.h>
#include <time.h>
//#include <locale.h>
using namespace std;

#include"classes.h"


/*void make_gab ()
{
    FILE *p;

    p = fopen("Gabarito iCNC.txt", "w");
    if (p == NULL)
    {
        printf ("Erro gabarito arquivo de gabarito n%co pode ser criado, confira as permissoes de acesso da pasta\n", 198);
        exit(1);
    }
    else
    {
        fprintf(p, "Torno\n\n-T 8 17\nG28# T# M06! | G99# G98# | | G96! G97! | S# | M03! M04! | M08# G00# F!-G71 5 13\n| G41! G42! | X! Z# G71# U! R# G71# P! Q! U# W# F! G00! X!\n-G72 5 13\n| G41# G42# | X! Z! G72# W! R# G72# P! Q! U! W# F# G00# Z!\n-G73 5 13\n| G41# G42# | X! Z! G73# U! W! R# G73# P! Q# U! W# F! G00# X# Z!\n-G70 5 5\n| G41 G42 | X! Z# G70! P# Q! F\n-G75 5 7\nG75# X! Z! P# Q! F#\n-G76 7 13\nX! Z! G76# P! Q! R! G76# X! Z! P# Q# F#\n-M00 5 5\nG28 M09 M05 M00 | M03 M04 | M08\n-M30 6 0\nG28! M05# M09# M30!\n-M02 6 0\nG28! M05# M09# M02!");
    }
}*/

Gabarito* Read_Gabarito(char nome[], int *ngab, char tipoequip[])
{
    FILE *fp;
    Gabarito *gab;
    int flag=1, i;
    char ch;

    fp=fopen(nome, "r");
    if(fp == NULL)
    {
        printf("O Gabarito n%co pode ser encontrado\n\n", 198);
        system("Pause");
        exit(1);
    }

    i=0;
    do
    {
        ch=getc(fp);
        tipoequip[i]=ch;
        i++;
    }while(ch!='\n');
    i--;
    tipoequip[i]='\0';

    ch=getc(fp);
    while(ch!=EOF){
        if(ch=='-'){
            *ngab=*ngab+1;

            if(flag==1){
                gab = (Gabarito*) malloc(*ngab*sizeof(Gabarito));
                flag = 0;
            }
            else{
                gab = (Gabarito*) realloc(gab, *ngab*sizeof(Gabarito));
            }

            if(!gab){
                printf("Falha na aloca%c%co de mem%cria!\n", 135, 198,162);
                system("Pause");
                exit(1);
            }

            ch = gab[*ngab-1].Salve_Gabarito(fp);
        }
        else{
            ch=getc(fp);
        }
    }
    fclose(fp);

    *ngab=*ngab-1;

    return gab;
}

char* Read_Codigo(char nome_cod[])
{
    FILE *fp;
    int b;
    int i;
    char ch, *cd;
    char nome[50];

    while (1)
    {
        printf ("Digite o nome do arquivo do programa: ");
        fflush(stdin);
        gets(nome);

        fp=fopen(nome,"r");
        if(fp == NULL)
        {
            printf("O arquivo n%co pode ser encontrado, confira se o nome esta correto\n\n", 198);
        }
        else
        {
            strcpy(nome_cod, nome);

            i = 0;
            ch=getc(fp);
            while(ch!=EOF)
            {
                i++;
                ch = getc(fp);
            }
            i++;

            cd = (char*) malloc((i)*sizeof(char));

            if(!cd)
            {
                printf("Falha na aloca%c%co de mem%cria 1.1 !\n", 135, 198, 162);
                system("Pause");
                exit(1);
            }

            rewind(fp);

            b=0;
            while (!feof(fp))
            {
                fgets((cd+b), (i), fp);
                b = strlen(cd); //existe um fp para cada linha do arquivo .txt, então tive que fazer a gravação por partes
            }


            fclose(fp);

            cd[i] = '\0';

            return cd;
        }
    }


}

void print (Gabarito *gab, int i, int coman, int flag_M08, int flag_M05, int d)
{
    int flag = 0;

    if (gab[i].PripComand[0] == 'T')
    {
        if (strcmp("G28", gab[i].comand[coman]) == 0)
        {
            printf ("   Erro: Afastar a torre da pe%ca antes da troca de ferramenta (G28)\n\n", 135);
            flag = 1;
        }
        else
        {
            if(strcmp("M06", gab[i].comand[coman]) == 0)
            {
                printf ("   Erro: N%co foi ligado o motor da torre para a troca da ferramenta (M06)\n\n", 198);
                flag = 1;
            }
            else
            {
                if(strcmp("G99", gab[i].comand[coman]) == 0)
                {
                    printf("    Erro: N%co foi definido se o avan%co ser%c em mm/min ou mm/rot (G98 ou G99)\n\n", 198, 135, 160);
                    flag = 1;
                }
                else
                {
                    if(strcmp("G96", gab[i].comand[coman]) == 0)
                    {
                        printf ("   Erro: N%co foi definido se a rota%c%co ser%c constante [rpm] ou vari%cvel [Vc constante - m/min] (G96 ou G97)\n\n", 198, 135, 198, 160, 160);
                        flag = 1;
                    }
                    else
                    {
                        if(strcmp("S", gab[i].comand[coman]) == 0)
                        {
                            printf("    Erro: N%co foi definido o valor da rota%c%co ou da Vc para esta ferramenta (S)\n\n", 198, 135, 198);
                            flag = 1;
                        }
                        else
                        {
                            if(strcmp("M03", gab[i].comand[coman]) == 0)
                            {
                                printf ("   Erro: N%co foi ligado o motor da %crvore (rota%c%co) para esta ferramenta (M03 ou M04)\n\n", 198, 160, 135, 198);
                                flag = 1;
                            }
                            else
                            {
                                if (strcmp("G00", gab[i].comand[coman]) == 0)
                                {
                                    printf ("   Erro: Ferramenta n%co foi aproximada da pe%ca em velocidade r%cpida (G00)\n\n", 198, 135, 160);
                                    flag = 1;
                                }
                                else
                                {
                                    if (strcmp("M08", gab[i].comand[coman]) == 0)
                                    {
                                        printf ("   Aviso: Usinagem sendo realizada sem lubri-refrigerante (M08)\n\n");
                                        flag = 1;
                                    }
                                    else
                                    {
                                        if (strcmp("F", gab[i].comand[coman]) == 0)
                                        {
                                            printf ("   Erro: N%co foi definido o valor do avan%co para esta ferramenta (F)\n\n", 198, 135);
                                            flag = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (strcmp(gab[i].PripComand, "M30") == 0 || strcmp(gab[i].PripComand, "M02") == 0)
        {
            if (strcmp("M05", gab[i].comand[coman]) == 0)
            {
                printf ("   Erro: Desligar rota%c%co da %crvore ao final do programa (M05)\n\n", 135, 198, 160);
                flag = 1;
            }
            else
            {
                if (strcmp("G28", gab[i].comand[coman]) == 0)
                {
                    printf ("   Erro: Afastar a ferramenta da pe%ca ao final do programa (G28)\n\n", 135);
                    flag = 1;
                }
                else
                {
                    if (flag_M08 == 1 && strcmp("M09", gab[i].comand[coman]) == 0)
                    {
                        printf ("   Erro: Desligar refrigera%c%co ao final do programa (M09)\n\n", 135, 198);
                        flag = 1;
                    }

                }
            }
        }
        else
        {
            if (strcmp("G71", gab[i].PripComand) == 0 || strcmp("G72", gab[i].PripComand) == 0 || strcmp("G73", gab[i].PripComand) == 0)
            {
                if (strcmp("G41", gab[i].comand[coman]) == 0)
                {
                    printf ("   Aviso: Recomenda-se utilizar compensa%c%co do raio da ferramenta nos ciclos de desbaste com perfil - Verificar (G41 OU G42)\n\n", 135, 198);
                    flag = 1;
                }
                else
                {
                    if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 0)
                    {
                        printf ("   Aviso: Recomenda-se definir as coordenadas X e Z do ponto de in%ccio do ciclo %s antes da chamada do mesmo\n\n", 214, gab[i].PripComand);
                        flag = 1;
                    }
                    else
                    {
                        if (strcmp("F", gab[i].comand[coman]) == 0)
                        {
                            printf ("   Aviso: Recomenda-se definir um F para desbaste na defini%c%co do ciclo %s\n\n", 135, 198, gab[i].PripComand);
                            flag = 1;
                        }
                        else
                        {
                            if (strcmp("G00", gab[i].comand[coman]) == 0)
                            {
                                printf ("   Aviso: Recomenda-se aproximar a ferramenta do ponto de in%ccio de perfil em G00\n\n", 214);
                                flag = 1;
                            }
                            else
                            {
                                if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 1)
                                {
                                    printf ("   Erro: A aproxima%c%co da ferramenta ao ponto de in%ccio de perfil do ciclo %s deve ser feita no eixo %s e, de prefer%cncia, em G00\n\n", 135, 198, 135, gab[i].PripComand, gab[i].comand[coman], 136);
                                    flag = 1;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (strcmp("G70", gab[i].PripComand) == 0)
                {
                    if (strcmp("G41", gab[i].comand[coman]) == 0)
                    {
                        printf ("   Aviso: Recomenda-se utilizar compensa%c%co do raio da ferramenta no ciclo de acabamento com perfil  - Verificar (G41 ou G42)\n\n", 135, 198);
                        flag = 1;
                    }
                    else
                    {
                        if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 0)
                        {
                            printf ("   Aviso: Recomenda-se definir as coordenadas %s do ponto de in%ccio do ciclo G70 antes da chamada do mesmo e, de prefer%cncia, que sejam iguais as do ciclo de desbaste\n\n", gab[i].comand[coman], 214, 136);
                            flag = 1;
                        }
                        else
                        {
                            if (strcmp("F", gab[i].comand[coman]) == 0)
                            {
                                printf ("   Aviso: Certifique-se que o F de acabamento foi programado no ciclo de desbaste, caso contr%crio defina um F de acabamento na chamada do G70\n\n", 160);
                                flag = 1;
                            }
                        }
                    }
                }
                else
                {
                    if (strcmp("M00", gab[i].PripComand) == 0)
                    {
                        if (strcmp("G28", gab[i].comand[coman]) == 0)
                        {
                            printf ("   Aviso: Afastar a ferramenta o suficiente da pe%ca para permitir o acesso do operador (G28)\n\n", 135);
                            flag = 1;
                        }
                        else
                        {
                            if (strcmp("M09", gab[i].comand[coman]) == 0 && flag_M08 == 1)
                            {
                                printf ("   Aviso: Desligar refrigerante antes da parada programada(M09)\n\n");
                                flag = 1;
                            }
                            else
                            {
                                if (strcmp("M05", gab[i].comand[coman]) == 0)
                                {
                                    printf ("   Aviso: Desligar %crvore antes da parada programada (M05)\n\n", 160);
                                    flag = 1;
                                }
                                else
                                {
                                    if (strcmp("M03", gab[i].comand[coman]) == 0 && flag_M05 == 1)
                                    {
                                        printf ("   Aviso: Ligar %crvore ap%cs a parada programada (M03)\n\n", 160, 162);
                                        flag = 1;
                                    }
                                    else
                                    {
                                        if (strcmp("M08", gab[i].comand[coman]) == 0)
                                        {
                                            printf ("   Aviso: Ligar refrigerante ap%cs a parada programada (M08)\n\n", 162);
                                            flag = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if (strcmp("G75", gab[i].PripComand) == 0)
                        {
                            if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 1)
                            {
                                printf ("   Erro: %c necess%crio definir as coordenadas X e Z do ponto final do canal no ciclo G75\n\n", 144, 160);
                                flag = 1;
                            }
                            else
                            {
                                if (strcmp("F", gab[i].comand[coman]) == 0)
                                {
                                    printf ("   Aviso: Certifique-se que o F para usinagem do canal foi devidamente informado no ciclo G75\n\n");
                                    flag = 1;
                                }
                            }
                        }
                        else
                        {
                            if (strcmp("G76", gab[i].PripComand) == 0)
                            {
                                if ((strcmp("X", gab[i].comand[coman]) == 0) && d == 0)
                                {
                                    printf ("   Aviso: Recomenda-se definir as coordenadas X do ponto de in%ccio do ciclo G76 antes da chamada do mesmo e afastadas o suficiente, principalmente no eixo Z\n\n", 214);
                                    flag = 1;
                                }
                                else
                                {
                                    if (strcmp("F", gab[i].comand[coman]) == 0)
                                    {
                                        printf ("   Erro: O ciclo G76 requer o par%cmetro F, que corresponde ao passo da rosca quando o avanço for programado em G99 [mm/rot]\n\n", 131);
                                        flag = 1;
                                    }
                                    else
                                    {
                                        if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 1)
                                        {
                                            printf ("   Erro: O ciclo G76 requer as coordenadas X do fundo do filete da rosca e Z do final da rosca\n\n");
                                            flag = 1;
                                        }
                                        else
                                        {
                                            if ((strcmp("Z", gab[i].comand[coman]) == 0) && d == 0)
                                            {
                                                printf ("   Aviso: Recomenda-se definir as coordenadas Z do ponto de in%ccio do ciclo G76 antes da chamada do mesmo e afastadas o suficiente, principalmente no eixo Z\n\n", 214);
                                                flag = 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (flag_M08 == 0 && strcmp("M09", gab[i].comand[coman]) == 0)
    {
        flag = 1;
    }
    else
    {
        if (flag == 0 && strcmp("M03", gab[i].comand[coman]) == 0)
        {
            flag = 1;
        }
    }

    if (flag == 0)
    {
        printf ("   Erro: O ciclo %s requer o par%cmetro %s depois da chamada do mesmo\n\n", gab[i].PripComand, 131, gab[i].comand[coman]);
    }

}

void salva_print (Gabarito *gab, int i, int coman, int flag_M08, int flag_M05, int d, FILE *p)
{
    int flag = 0;

    if (gab[i].PripComand[0] == 'T')
    {
        if (strcmp("G28", gab[i].comand[coman]) == 0)
        {
            fprintf (p, "   Erro: Afastar a torre da peça antes da troca de ferramenta (G28)\n\n");
            flag = 1;
        }
        else
        {
            if(strcmp("M06", gab[i].comand[coman]) == 0)
            {
                fprintf (p, "   Erro: Não foi ligado o motor da torre para a troca da ferramenta (M06)\n\n");
                flag = 1;
            }
            else
            {
                if(strcmp("G99", gab[i].comand[coman]) == 0)
                {
                    fprintf(p, "    Erro: Não foi definido se o avanço será em mm/min ou mm/rot (G98 ou G99)\n\n");
                    flag = 1;
                }
                else
                {
                    if(strcmp("G96", gab[i].comand[coman]) == 0)
                    {
                        fprintf (p, "   Erro: Não foi definido se a rotação será constante [rpm] ou variável [Vc constante - m/min] (G96 ou G97)\n\n");
                        flag = 1;
                    }
                    else
                    {
                        if(strcmp("S", gab[i].comand[coman]) == 0)
                        {
                            fprintf(p, "    Erro: Não foi definido o valor da rotação ou da Vc para esta ferramenta (S)\n\n");
                            flag = 1;
                        }
                        else
                        {
                            if(strcmp("M03", gab[i].comand[coman]) == 0)
                            {
                                fprintf (p, "   Erro: Não foi ligado o motor da árvore (rotação) para esta ferramenta (M03 ou M04)\n\n");
                                flag = 1;
                            }
                            else
                            {
                                if (strcmp("G00", gab[i].comand[coman]) == 0)
                                {
                                    fprintf (p, "   Erro: Ferramenta não foi aproximada da peça em velocidade rápida (G00)\n\n");
                                    flag = 1;
                                }
                                else
                                {
                                    if (strcmp("M08", gab[i].comand[coman]) == 0)
                                    {
                                        fprintf (p, "   Aviso: Usinagem sendo realizada sem lubri-refrigerante (M08)\n\n");
                                        flag = 1;
                                    }
                                    else
                                    {
                                        if (strcmp("F", gab[i].comand[coman]) == 0)
                                        {
                                            fprintf (p, "   Erro: Não foi definido o valor do avanço para esta ferramenta (F)\n\n");
                                            flag = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (strcmp(gab[i].PripComand, "M30") == 0 || strcmp(gab[i].PripComand, "M02") == 0)
        {
            if (strcmp("M05", gab[i].comand[coman]) == 0)
            {
                fprintf (p, "   Erro: Desligar rotação da árvore ao final do programa (M05)\n\n");
                flag = 1;
            }
            else
            {
                if (strcmp("G28", gab[i].comand[coman]) == 0)
                {
                    fprintf (p, "   Erro: Afastar a ferramenta da peça ao final do programa (G28)\n\n");
                    flag = 1;
                }
                else
                {
                    if (flag_M08 == 1 && strcmp("M09", gab[i].comand[coman]) == 0)
                    {
                        fprintf (p, "   Erro: Desligar refrigeração ao final do programa (M09)\n\n");
                        flag = 1;
                    }

                }
            }
        }
        else
        {
            if (strcmp("G71", gab[i].PripComand) == 0 || strcmp("G72", gab[i].PripComand) == 0 || strcmp("G73", gab[i].PripComand) == 0)
            {
                if (strcmp("G41", gab[i].comand[coman]) == 0)
                {
                    fprintf (p, "   Aviso: Recomenda-se utilizar compensação do raio da ferramenta nos ciclos de desbaste com perfil - Verificar (G41 OU G42)\n\n");
                    flag = 1;
                }
                else
                {
                    if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 0)
                    {
                        fprintf (p, "   Aviso: Recomenda-se definir as coordenadas X e Z do ponto de início do ciclo %s antes da chamada do mesmo\n\n", gab[i].PripComand);
                        flag = 1;
                    }
                    else
                    {
                        if (strcmp("F", gab[i].comand[coman]) == 0)
                        {
                            fprintf (p, "   Aviso: Recomenda-se definir um F para desbaste na definição do ciclo %s\n\n", gab[i].PripComand);
                            flag = 1;
                        }
                        else
                        {
                            if (strcmp("G00", gab[i].comand[coman]) == 0)
                            {
                                fprintf (p, "   Aviso: Recomenda-se aproximar a ferramenta do ponto de início de perfil em G00\n\n");
                                flag = 1;
                            }
                            else
                            {
                                if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 1)
                                {
                                    fprintf (p, "   Erro: A aproximação da ferramenta ao ponto de início de perfil do ciclo %s deve ser feita no eixo %s e, de preferência, em G00\n\n", gab[i].PripComand, gab[i].comand[coman]);
                                    flag = 1;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (strcmp("G70", gab[i].PripComand) == 0)
                {
                    if (strcmp("G41", gab[i].comand[coman]) == 0)
                    {
                        fprintf (p, "   Aviso: Recomenda-se utilizar compensação do raio da ferramenta no ciclo de acabamento com perfil  - Verificar (G41 ou G42)\n\n");
                        flag = 1;
                    }
                    else
                    {
                        if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 0)
                        {
                            fprintf (p, "   Aviso: Recomenda-se definir a coordenada %s do ponto de início do ciclo G70 antes da chamada do mesmo e, de preferência, que sejam iguais as do ciclo de desbaste\n\n", gab[i].comand[coman]);
                            flag = 1;
                        }
                        else
                        {
                            if (strcmp("F", gab[i].comand[coman]) == 0)
                            {
                                fprintf (p, "   Aviso: Certifique-se que o F de acabamento foi programado no ciclo de desbaste, caso contrário defina um F de acabamento na chamada do G70\n\n");
                                flag = 1;
                            }
                        }
                    }
                }
                else
                {
                    if (strcmp("M00", gab[i].PripComand) == 0)
                    {
                        if (strcmp("G28", gab[i].comand[coman]) == 0)
                        {
                            fprintf (p, "   Aviso: Afastar a ferramenta o suficiente da peça para permitir o acesso do operador (G28)\n\n");
                            flag = 1;
                        }
                        else
                        {
                            if (strcmp("M09", gab[i].comand[coman]) == 0 && flag_M08 == 1)
                            {
                                fprintf (p, "   Aviso: Desligar refrigerante antes da parada programada(M09)\n\n");
                                flag = 1;
                            }
                            else
                            {
                                if (strcmp("M05", gab[i].comand[coman]) == 0)
                                {
                                    fprintf (p, "   Aviso: Desligar árvore antes da parada programada (M05)\n\n");
                                    flag = 1;
                                }
                                else
                                {
                                    if (strcmp("M03", gab[i].comand[coman]) == 0 && flag_M05 == 1)
                                    {
                                        fprintf (p, "   Aviso: Ligar árvore após a parada programada (M03)\n\n");
                                        flag = 1;
                                    }
                                    else
                                    {
                                        if (strcmp("M08", gab[i].comand[coman]) == 0)
                                        {
                                            fprintf (p, "   Aviso: Ligar refrigerante após a parada programada (M08)\n\n");
                                            flag = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if (strcmp("G75", gab[i].PripComand) == 0)
                        {
                            if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 1)
                            {
                                fprintf (p, "   Erro: É necessário definir as coordenadas X e Z do ponto final do canal no ciclo G75\n\n");
                                flag = 1;
                            }
                            else
                            {
                                if (strcmp("F", gab[i].comand[coman]) == 0)
                                {
                                    fprintf (p, "   Aviso: Certifique-se que o F para usinagem do canal foi devidamente informado no ciclo G75\n\n");
                                    flag = 1;
                                }
                            }
                        }
                        else
                        {
                            if (strcmp("G76", gab[i].PripComand) == 0)
                            {
                                if ((strcmp("X", gab[i].comand[coman]) == 0) && d == 0)
                                {
                                    fprintf (p, "   Aviso: Recomenda-se definir as coordenadas X do ponto de início do ciclo G76 antes da chamada do mesmo e afastadas o suficiente, principalmente no eixo Z\n\n");
                                    flag = 1;
                                }
                                else
                                {
                                    if (strcmp("F", gab[i].comand[coman]) == 0)
                                    {
                                        fprintf (p, "   Erro: O ciclo G76 requer o parâmetro F, que corresponde ao passo da rosca quando o avanço for programado em G99 [mm/rot]\n\n");
                                        flag = 1;
                                    }
                                    else
                                    {
                                        if ((strcmp("X", gab[i].comand[coman]) == 0 || strcmp("Z", gab[i].comand[coman]) == 0) && d == 1)
                                        {
                                            fprintf (p, "   Erro: O ciclo G76 requer as coordenadas X do fundo do filete da rosca e Z do final da rosca\n\n");
                                            flag = 1;
                                        }
                                        else
                                        {
                                            if ((strcmp("Z", gab[i].comand[coman]) == 0) && d == 0)
                                            {
                                                fprintf (p, "   Aviso: Recomenda-se definir as coordenadas Z do ponto de início do ciclo G76 antes da chamada do mesmo e afastadas o suficiente, principalmente no eixo Z\n\n");
                                                flag = 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (flag_M08 == 0 && strcmp("M09", gab[i].comand[coman]) == 0)
    {
        flag = 1;
    }
    else
    {
        if (flag == 0 && strcmp("M03", gab[i].comand[coman]) == 0)
        {
            flag = 1;
        }
    }

    if (flag == 0)
    {
        fprintf (p, "   Erro: O ciclo %s requer o parâmetro %s depois da chamada do mesmo\n\n", gab[i].PripComand, gab[i].comand[coman]);
    }

}


void testa_cod (char **ca, Gabarito *gab, int inicio, int fim, int *flag, int k, int i)
{
    int x;
    for (x = inicio; x < fim; x++)
    {
        if (gab[i].comand[k][0] == 'G' || gab[i].comand[k][0] == 'M')
        {
            *flag = strcmp(gab[i].comand[k], ca[x]);
        }
        else
        {
            if (gab[i].comand[k][0] == ca[x][0])
            {
                *flag = 0;
            }
            else
            {
                *flag = 1;
            }
        }

        if (*flag == 0)
        {
            break;
        }
    }
}

float pega_valor (int i, char **ca)
{
    char c[7];
    float v;
    int j;


    for (j = 1; ca[i][j] != '\0'; j++)
    {
        c[j-1] = ca[i][j];
    }

    fflush(stdin);

    v = atof(c);

    return v;
}

void testa_comb (char**ca, Gabarito *gab, int inicio, int fim, int i, int *flag)
{
    int j, G99, G98, G97, G96;
    float S, F;

    G99 = 0;
    G98 = 0;
    G97 = 0;
    G96 = 0;
    S = -1.0;
    F = -1.0;

    for (j = inicio; j < fim; j++)
    {
        if (strcmp("G99", ca[j]) == 0)
        {
            G99 = 1;
        }
        else
        {
            if (strcmp("G98", ca[j]) == 0)
            {
                G98 = 1;
            }
            else
            {
                if (strcmp("G97", ca[j]) == 0)
                {
                    G97 = 1;
                }
                else
                {
                    if (strcmp("G96", ca[j]) == 0)
                    {
                        G96 = 1;
                    }
                    else
                    {
                        if ('S' == ca[j][0])
                        {
                            fflush(stdin);
                            S = pega_valor(j, ca);
                        }
                        else
                        {
                            if ('F' == ca[j][0])
                            {
                                fflush(stdin);
                                F = pega_valor(j, ca);
                            }
                        }
                    }
                }
            }
        }
        //printf ("G99=%i, G98=%i, G97=%i, G96=%i, S=%f, F=%f\n", G99, G98, G97, G96, S, F);
    }

    if (G96 == 1 && S > 1000.0 && S > 0)
    {
        printf ("   Aviso: Usinagem sendo realizada com Vc elevada - Verificar! (G96 e S)\n\n");
        *flag = 1;
    }
    if (G97 == 1 && S < 100.0 && S > 0)
    {
        printf ("   Aviso: Usinagem sendo realizada com rota%c%co muito baixa - Verificar! (G97 e S)\n\n", 135, 198);
        *flag = 1;
    }
    if (G98 == 1 && F<5.0 && F > 0)
    {
        printf ("   Aviso: Usinagem sendo realizada com avan%co muito baixo - Verificar! (G98 e F)\n\n", 135);
        *flag = 1;
    }
    if (G99 == 1 && F>5.0 && F > 0)
    {
        printf("    Aviso: Usinagem sendo realizada com avan%co muito alto - Verificar! (G99 e F)\n\n", 135);
        *flag = 1;
    }
}

void salva_testa_comb (char**ca, Gabarito *gab, int inicio, int fim, int i, int *flag, FILE *p)
{
    int j, G99, G98, G97, G96;
    float S, F;

    G99 = 0;
    G98 = 0;
    G97 = 0;
    G96 = 0;
    S = -1.0;
    F = -1.0;

    for (j = inicio; j < fim; j++)
    {
        if (strcmp("G99", ca[j]) == 0)
        {
            G99 = 1;
        }
        else
        {
            if (strcmp("G98", ca[j]) == 0)
            {
                G98 = 1;
            }
            else
            {
                if (strcmp("G97", ca[j]) == 0)
                {
                    G97 = 1;
                }
                else
                {
                    if (strcmp("G96", ca[j]) == 0)
                    {
                        G96 = 1;
                    }
                    else
                    {
                        if ('S' == ca[j][0])
                        {
                            fflush(stdin);
                            S = pega_valor(j, ca);
                        }
                        else
                        {
                            if ('F' == ca[j][0])
                            {
                                fflush(stdin);
                                F = pega_valor(j, ca);
                            }
                        }
                    }
                }
            }
        }
        //printf ("G99=%i, G98=%i, G97=%i, G96=%i, S=%f, F=%f\n", G99, G98, G97, G96, S, F);
    }

    if (G96 == 1 && S > 1000.0 && S > 0)
    {
        fprintf (p, "   Aviso: Usinagem sendo realizada com Vc elevada - Verificar! (G96 e S)\n\n");
        *flag = 1;
    }
    if (G97 == 1 && S < 100.0 && S > 0)
    {
        fprintf (p, "   Aviso: Usinagem sendo realizada com rotação muito baixa - Verificar! (G97 e S)\n\n");
        *flag = 1;
    }
    if (G98 == 1 && F<5.0 && F > 0)
    {
        fprintf (p, "   Aviso: Usinagem sendo realizada com avanço muito baixo - Verificar! (G98 e F)\n\n");
        *flag = 1;
    }
    if (G99 == 1 && F>5.0 && F > 0)
    {
        fprintf(p, "    Aviso: Usinagem sendo realizada com avanço muito alto - Verificar! (G99 e F)\n\n");
        *flag = 1;
    }
}

void analisador (Gabarito *gab, int ngab, char cd[])    //ngab é o número de linhas do gabarito, ncod do código
{
    int i, j, cont, k, z, linha, fim_codigo, h, h2, inicio, fim, flag, flag3, cont2, flag_M08 = 0, flag_G41, flag_G42, w, flag_G40, flag_G71, cont3, u, w1, flag_G94, flag_G81, flag_G54, flag_G70, flag_G00, flag_G97, flag_G99, flag_S, flag_Suprema, flag_M05=0, flag_letra = 0, flag_T = 0, flag_comp;
    char *cod, **ca;
    float valor;

    //teste para ver se tem cabeçalho
    i=0;
    j=0;
    flag = 0;
    while (j<=5)
    {
        if (cd[i] == '\n')
        {
            j++;
        }
        else
        {
            if (cd[i] == ')')
            {
                flag++;
            }
        }

        i++;
    }
    if (flag < 2)
    {
        printf ("Aviso: Incluir cabe%calho no in%ccio do programa\n\n", 135, 214);
    }

    i = strlen(cd);
    cod = (char *) malloc(i*sizeof(char));
    if (cod == NULL)
    {
        printf ("Erro na aloca%c%co de mem%cria!", 135, 198, 162);
        system("Pause");
        exit(1);
    }
    //cont = arruma_cod (cd, cod, ca);

    i=0;
    j=0;
    cont =0;
    while (cd[i]!='\0')
    {
        if (cd[i] == 'O' || cd[i] == 'o')
        {
            while (cd[i] != ' ' && cd[i] != '\n')
            {
                i++;
            }
            i++;
        }
        else
        {
            if (cd[i] == '(')
            {
                while (cd[i] != ')')
                {
                    i++;
                }
                i = i + 2;
            }
            else
            {
                if (cd[i] == '[')
                {
                    while (cd[i]!=' ' && cd[i] != '\n')
                    {
                        i++;
                    }
                    i = i+1;
                }
                else
                {
                    if (cd[i] >= 'A' && cd[i] <= 'Z')
                    {
                        cont++;
                    }
                    else
                    {
                        if (cd[i] >= 'a' && cd[i] <= 'z')
                        {
                            cd[i] = toupper(cd[i]);
                            cont++;
                            flag_letra = 1;
                        }
                    }
                    cod[j] = cd[i];
                    i++;
                    j++;
                }
            }
        }
    }
    cod[j] = '\0';

    //printf ("%s", cod);

    ca = (char**) calloc (cont , sizeof(char*));

    if(ca==NULL)
    {
        printf("Erro na aloca%c%co de mem%cria!", 135, 198, 162);
        system("Pause");
        exit (1);
    }

    for (j = 0; j < cont; j++)
    {
        ca[j] = (char*) calloc (10 , sizeof(char));
        if(ca[j]==NULL)
        {
            printf("Erro na aloca%c%co de mem%cria!", 135, 198, 162);
            system("Pause");
            exit (1);
        }//printf ("HERE %i %i\n", cont, j);
    }

    j = 0;
    for (i = 0;  cod[i] != '\0'; i++)
    {
        if (cod[i] >= 'A' && cod[i]<= 'Z')
        {
            k = 0;
            if (cod[i] == 'T')
            {
                ca[j][k] = 'T';
                k++;
            }
            else
            {
                while (cod[i] != ' ' && cod[i] != '\n')
                {
                    ca[j][k] = cod[i];
                    i++;
                    k++;
                }
            }

            ca[j][k] = '\0';
            j++;
        }
    }

    free(cod);

    /*for (i = 0; i< cont; i++)
    {
        printf ("%s\n", ca[i]);
    }*/
    //printf("%s", ca[0]);


    //caso o cidadão tenha feito o código em letras minúsculas
    if (flag_letra == 1)
    {
        printf ("Aviso: Recomenda-se o uso de letras mai%csculas no c%cdigo\n\n", 163, 162);
    }

    //Testando para a presença de alguns códigos.
    flag_G94 = 0;
    flag_G81 = 0;
    flag_G71 = 0;
    flag_G54 = 0;
    fim_codigo = 0;
    flag_G00 = 0;

    for (j = 0; j < cont; j++)
    {
        if (strcmp("G94", ca[j]) == 0)
        {
            flag_G94 = 1;
            flag_G00 = 0;
            valor = 0.0;
            i = j;
            while (flag_G00 == 0)
            {
                if (strcmp(ca[i], "G00") == 0)
                {
                    flag_G00 = 1;
                    printf ("\n");
                }
                else
                {
                    if (ca[i][0] == 'Z')
                    {
                        valor = pega_valor(i, ca);

                        if (valor < 0.0)
                        {
                            printf ("Bloco %s:\n    Aviso: Recomenda-se posicionar o ZP na face acabada da pe%ca (Z0 no in%ccio da pe%ca) (Z deve ser positivo depois de G94 at%c G00)\n\n", ca[linha], 135, 214, 135, 130);
                        }
                    }
                }
                i++;
            }
        }
        else
        {
            if (strcmp("G81", ca[j]) == 0 || strcmp("G74", ca[j]) == 0 || strcmp("G83", ca[j]) == 0)
            {
                flag_G81 = 1;
                flag_G97 = 0;
                i = j;
                valor = 0.0;
                while (flag_G97 == 0 || flag_S == 0)
                {
                    if (strcmp("G97", ca[i]) == 0)
                    {
                        flag_G97 = 1;
                        printf ("\n");
                    }
                    else
                    {
                        if (strcmp(ca[i], "G96") == 0)
                        {
                            flag_G97 = 2;
                            printf("Bloco %s - Comando %s\n  Erro: A usinagem est%c sendo realizada com a rota%c%co m%cxima da m%cquina (Use G97 n%co G96)\n\n", ca[linha], ca[j], 160, 135, 198, 160, 160, 198);
                        }
                        else
                        {
                            if ('S' == ca[i][0])
                            {
                                flag_S = 1;
                                valor = pega_valor(i, ca);
                                if (valor<100)
                                {
                                    printf("Bloco %s - Comando %s e %s\n    Aviso: Usinagem sendo realizada com rota%c%co muito baixa - Verificar!\n\n", ca[linha], ca[j], ca[i], 135, 198);
                                }
                            }
                        }
                    }
                    if (i == 0)
                    {
                        printf ("G97 n%co foi encontrado no c%cdigo, %s no bloco %s necessita deste complemento - Verificar!\n\n", 198, 162, ca[j], ca[linha]);
                        flag_G97 = -1;
                        flag_S = -1;
                    }
                    i--;
                }

            }
            else
            {
                if (strcmp("G71", ca[j]) == 0 || strcmp("G72", ca[j]) == 0 || strcmp("G73", ca[j]) == 0)
                {
                    flag_G71 = 1;
                }
                else
                {
                    if (strcmp("G54", ca[j]) == 0 || strcmp("G55", ca[j]) == 0 || strcmp("G56", ca[j]) == 0 || strcmp("G57", ca[j]) == 0)
                    {
                        flag_G54 = 1;
                    }
                    else
                    {
                        if ((strcmp(ca[j], "M30")) == 0 || (strcmp(ca[j], "M02")) == 0)
                        {
                            fim_codigo = 1;
                        }
                        else
                        {
                            if (strcmp("G41", ca[j]) == 0 || strcmp("G42", ca[j]) == 0)
                            {
                                flag_G41 = 1;
                            }
                            else
                            {
                                if (ca[j][0] == 'N')
                                {
                                    linha = j;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (j = 0; j<cont; j++)
    {
        if (strcmp("G81", ca[j]) == 0 || strcmp("G74", ca[j]) == 0 || strcmp("G75", ca[j]) == 0 || strcmp("G76", ca[j]) == 0)
        {
            i = j;
            flag_comp = 0;
            while (i>=0 && flag_comp == 0)
            {
                if (strcmp("G41", ca[i]) == 0 || strcmp("G42", ca[i]) == 0)
                {
                    flag_comp = 1;
                }
                else
                {
                    if (strcmp("G40", ca[i]) == 0)
                    {
                        flag_comp = -1;
                    }
                }
                i--;
            }

            if (flag_comp == 1)
            {
                printf("Bloco %s - Comando %s\n    Erro: N%co utilizar compensa%c%co de raio com este ciclo de usinagem\n\n", ca[linha], ca[j], 198, 135, 198);
            }
        }
        else
        {
            if (ca[j][0] == 'N')
            {
                linha = j;
            }
        }
    }


    if (flag_G94 == 0)
    {
        printf ("Aviso: Recomenda-se utilizar ciclo de faceamento (G94)\n\n");
    }
    if (flag_G71 == 0)
    {
        printf("Aviso: Recomenda-se utilizar ciclos de desbaste (G71 ou G72 ou G73)\n\n");
    }
    else
    {
        flag_G70 = 0;
        for (j = 0; j < cont; j++)
        {
            if (strcmp("G70", ca[j]) == 0)
            {
                flag_G70 = 1;
                break;
            }
        }
        if (flag_G70 == 0)
        {
            printf ("Aviso: Como foi utilizado um dos ciclos de desbaste (G71, G72 ou G73), recomenda-se utilizar o ciclo para acabamento - G70\n\n");
        }
    }
    if (flag_G41 == 0)
    {
        printf ("Aviso: Recomenda-se fortemente utilizar compensa%c%co do raio da ferramenta - Verificar (G41 ou G42)\n\n", 135, 198);
    }
    if (flag_G81 == 0)
    {
        printf ("Aviso: Se houver fura%c%co no programa recomenda-se utilizar ciclos de fura%c%co, etc. (G81 ou G74 ou G83)\n\n", 135, 198, 135, 198);

    }
    if (flag_G54 == 0)
    {
        printf ("Aviso: %c altamente recomendado indicar qual ZP ser%c utilizado em cada fase do processo (G54 ou G55 ou G56 ou G57)\n\n", 144, 160);
    }
    if (fim_codigo == 0)
    {
        printf ("Erro: Necess%crio encerrar o programa ao seu final (M02 ou M30)\n\n", 160);
    }






    for (i = 0; i <= ngab; i++)
    {
        flag3 = 0;
        flag_G71 = 0;
        cont3 = 0;
        cont2 = 0;
        u = 0;
        w1 = 0;

        for (j = 0; j < cont; j++)
        {

            if (ca[j][0] == 'N')
            {
                linha = j;
            }

            h = strcmp(gab[i].PripComand, ca[j]);

            if ((strcmp("G71", gab[i].PripComand) == 0) || (strcmp("G72", gab[i].PripComand)==0) || (strcmp("G73", gab[i].PripComand)== 0) || (strcmp("G76", gab[i].PripComand) == 0))
            {
                if ((strcmp("G71", ca[j]) == 0) || (strcmp("G72", ca[j])==0) || (strcmp("G73", ca[j])== 0) || (strcmp("G76", ca[j]) == 0))
                {
                    cont2++;
                    if (cont2 == 1)
                    {
                        flag3 = 0;
                    }
                    else
                    {
                        flag3 = 1;
                        cont2 = 0;
                    }
                }

            }

            if (h == 0 && flag3 == 0)
            {

                flag_G71 = 0;
                flag_Suprema = 0;

                printf ("\n\nBloco %s - Comando %s:\n", ca[linha], gab[i].PripComand);

                for (z = 0; z < gab[i].numcomand; z++)
                {
                    h2 = strcmp(gab[i].PripComand, gab[i].comand[z]);
                    if (h2==0)
                    {
                        inicio = j - gab[i].comandAntes;
                        fim = j+gab[i].comandDep;

                        if (inicio < 0)
                        {
                            inicio = 0;
                        }
                        if (fim >= cont)
                        {
                            fim = cont - 1 ;
                        }

                        if (strcmp("G76", gab[i].PripComand) == 0)
                        {
                            k = j;
                            flag_G99 = 0;
                            while (flag_G99 == 0)
                            {
                                if (strcmp("G99", ca[k]) == 0)
                                {
                                    flag_G99 = 1;
                                }
                                else
                                {
                                    if (strcmp("G98", ca[k]) == 0)
                                    {
                                        flag_G99 = -1;
                                        printf ("   Erro: Avan%co na abertura de rosca deve ser igual ao passo da rosca (deve-se usar G99, n%co G98)\n\n", 135, 198);
                                        flag_Suprema = 1;
                                    }
                                }
                                if (k == 0)
                                {
                                    printf ("   G99 n%co foi encontrado no c%cdigo - Verificar!\n\n", 198, 162);
                                    flag_G99 = -1;
                                    flag_Suprema = 1;
                                }
                                k--;
                            }
                        }
                        else
                        {
                            if ('T' == gab[i].PripComand[0])
                            {
                                testa_comb(ca, gab, j, fim, i, &flag_Suprema);
                                flag_T++;
                            }
                        }

                        //testando antes do codigo principal no codigo e gabarito
                        for (k = 0; k < z; k++)
                        {
                            if (gab[i].comand[k][0] == '|')
                            {
                                testa_cod(ca, gab, inicio, j, &flag, (k+1), i);

                                if (flag != 0)
                                {
                                    testa_cod(ca, gab, inicio, j, &flag, (k+2), i);;
                                }

                                //G41, G42 em tese devem estar antes do codigo principal
                                if (strcmp("G71", gab[i].PripComand) == 0 || strcmp("G72", gab[i].PripComand) == 0 || strcmp("G73", gab[i].PripComand) == 0 || strcmp("G70", gab[i].PripComand) == 0)
                                {
                                    flag_G71 = 1;
                                    flag_G41 = 0;
                                    if(strcmp("G41", gab[i].comand[(k+1)]) == 0 && flag == 0)
                                    {
                                        flag_G41 = 1;
                                        flag_G40 = 0;
                                    }
                                }

                                if (flag != 0)
                                {
                                    print(gab, i, (k+1), flag_M08, flag_M05, 0);
                                    flag_Suprema = 1;
                                }
                                k = k + 3;


                            }
                            else
                            {
                                testa_cod(ca, gab, inicio, j, &flag, k, i);
                                if (flag != 0)
                                {
                                    print (gab, i, k, flag_M08, flag_M05, 0);
                                    flag_Suprema = 1;
                                }

                                if (strcmp("M09", gab[i].comand[k]) == 0 && flag == 0)
                                {
                                    flag_M08 = 0;   //M09 desliga a refrigeração (M08)
                                }
                                else
                                {
                                    if (strcmp("M05", gab[i].comand[k]) == 0 && flag == 0)
                                    {
                                        flag_M05 = 1;
                                    }
                                }
                            }
                        }

                        flag_G42 = 0;
                        for (w=j-1; w>=0; w--)
                        {
                            if (strcmp("G41", ca[w]) == 0 || strcmp("G42", ca[w]) == 0)
                            {
                                flag_G42 = 1;
                            }
                            else
                            {
                                if ('T' == ca[w][0])
                                {
                                    break;
                                }
                            }
                        }


                        if (flag_G41 == 1 && 'T' == gab[i].PripComand[0])
                        {
                            w=inicio;
                            while (w<cont)
                            {
                                if (ca[w][0] == 'T')
                                {
                                    break;
                                }
                                else
                                {
                                    if (strcmp("G40", ca[w]) == 0)
                                    {
                                        flag_G40 = 1;
                                        break;
                                    }
                                }
                                w++;
                            }
                            if (w < cont)   //quer dizer que teve algum T, ou que algum break entrou em ação
                            {
                                if (flag_G40 != 1 && flag_T != 1 && flag_G42 == 1)
                                {
                                    printf ("   Erro: Desligar a compensa%c%co do raio da ferramenta antes da troca da ferramenta (G40 antes do G28)\n\n", 135, 198);
                                    flag_Suprema = 1;
                                }
                            }
                        }



                        //testando depois do codigo principal no codigo e gabarito
                        for (k = (z+1); k < gab[i].numcomand; k++)
                        {
                            if (gab[i].comand[k][0] == '|')
                            {
                                testa_cod(ca, gab, (j+1), (fim+1), &flag, (k+1), i);

                                if (flag != 0)
                                {
                                    testa_cod(ca, gab, (j+1), (fim+1), &flag, (k+2), i);
                                }

                                if (flag != 0)
                                {
                                    print(gab, i, (k+1), flag_M08, flag_M05, 1);
                                    flag_Suprema = 1;
                                }
                                k = k+3;

                            }
                            else
                            {
                                testa_cod(ca, gab, (j+1), (fim+1), &flag, k, i);

                                if (flag != 0)
                                {
                                    print (gab, i, k, flag_M08, flag_M05, 1);
                                    flag_Suprema = 1;
                                }

                                if (flag == 0 && strcmp("M08", gab[i].comand[k]) == 0)
                                {
                                    flag_M08 = 1;
                                }
                            }
                        }
                        break;
                    }
                }
                if (flag_Suprema == 0)
                {
                    printf ("   OK\n");
                }
            }

            valor = 0.0;
            if (flag_G71 == 1)
            {
                if (ca[j][0] == 'U' || ca[j][0] == 'W')
                {

                    if (ca[j][0] == 'U')
                    {
                        u++;
                    }
                    else
                    {
                        if (ca[j][0] == 'W')
                        {
                            w1++;
                        }
                    }

                    cont3++;

                    if (u == 2 || w1 == 2)
                    {
                        flag_G71 = 0;
                        u = 0;
                        w1 = 0;
                    }
                    else
                    {
                        if (cont3 == 2)
                        {
                            cont3 = 0;
                            flag_G71 = 0;
                            u = 0;
                            w1 = 0;

                            valor = pega_valor(j, ca);

                            if (valor == 0.0)
                            {
                                printf("    Erro: Ciclo de desbaste, sendo assim, deixar sobrematerial para o acabamento posterior (U e W n%co podem ser 0)\n", 198);
                                flag_Suprema = 1;
                            }
                            printf ("\n");
                        }
                        else
                        {
                            valor = pega_valor(j, ca);
                            if (valor == 0.0)
                            {
                                printf("    Erro: Ciclo de desbaste, sendo assim, deixar sobrematerial para o acabamento posterior (U e W n%co podem ser 0)\n", 198);
                                flag_Suprema = 1;
                            }
                            printf ("\n");
                        }
                    }
                }
            }
        }
    }

    free(ca);

}

void txt (Gabarito *gab, int ngab, char cd[], char nome[], char _nome[])
{
    FILE *p;

    p = fopen(nome, "w");
    if (p == NULL)
    {
        printf ("Erro, o arquivo n%co pode ser aberto!!\n\n", 198);
    }
    else
    {
        int i, j, cont, k, z, linha, fim_codigo, h, h2, inicio, fim, flag, flag3, cont2, flag_M08 = 0, flag_G41, flag_G42, w, flag_G40, flag_G71, cont3, u, w1, flag_G94, flag_G81, flag_G54, flag_G70, flag_G00, flag_G97, flag_G99, flag_S, flag_Suprema, flag_M05=0, flag_letra = 0, flag_T = 0, flag_comp;
        char *cod, **ca;
        float valor;
        time_t t = time(NULL);

        struct tm tm = *localtime(&t);



        fprintf(p, "UTFPR - NUFER\nInspetor CNC - ICNC\n\nRelatório  de inspeção do programa CNC %s realizado em %d/%d/%d às %d:%d:%d\n\n\n\n", _nome, tm.tm_mday, tm.tm_mon+1, (tm.tm_year+1900), tm.tm_hour, tm.tm_min, tm.tm_sec);

        //teste para ver se tem cabeçalho
        i=0;
        j=0;
        flag = 0;
        while (j<=5)
        {
            if (cd[i] == '\n')
            {
                j++;
            }
            else
            {
                if (cd[i] == ')')
                {
                    flag++;
                }
            }

            i++;
        }
        if (flag < 2)
        {
            fputs ("Aviso: Incluir cabeçalho no início do programa\n\n", p);
        }

        i = strlen(cd);
        cod = (char *) malloc(i*sizeof(char));
        if (cod == NULL)
        {
            printf ("Erro na aloca%c%co de mem%cria!", 135, 198, 162);
            system("Pause");
            exit(1);
        }
        //cont = arruma_cod (cd, cod, ca);

        i=0;
        j=0;
        cont =0;
        while (cd[i]!='\0')
        {
            if (cd[i] == 'O' || cd[i] == 'o')
            {
                while (cd[i] != ' ' && cd[i] != '\n')
                {
                    i++;
                }
                i++;
            }
            else
            {
                if (cd[i] == '(')
                {
                    while (cd[i] != ')')
                    {
                        i++;
                    }
                    i = i + 2;
                }
                else
                {
                    if (cd[i] == '[')
                    {
                        while (cd[i]!=' ' && cd[i] != '\n')
                        {
                            i++;
                        }
                        i = i+1;
                    }
                    else
                    {
                        if (cd[i] >= 'A' && cd[i] <= 'Z')
                        {
                            cont++;
                        }
                        else
                        {
                            if (cd[i] >= 'a' && cd[i] <= 'z')
                            {
                                cd[i] = toupper(cd[i]);
                                cont++;
                                flag_letra = 1;
                            }
                        }
                        cod[j] = cd[i];
                        i++;
                        j++;
                    }
                }
            }
        }
        cod[j] = '\0';

        //printf ("%s", cod);

        ca = (char**) calloc (cont , sizeof(char*));

        if(ca==NULL)
        {
            printf("Erro na aloca%c%co de mem%cria!", 135, 198, 162);
            system("Pause");
            exit (1);
        }

        for (j = 0; j < cont; j++)
        {
            ca[j] = (char*) calloc (10 , sizeof(char));
            if(ca[j]==NULL)
            {
                printf("Erro na aloca%c%co de mem%cria!", 135, 198, 162);
                system("Pause");
                exit (1);
            }//printf ("HERE %i %i\n", cont, j);
        }

        j = 0;
        for (i = 0;  cod[i] != '\0'; i++)
        {
            if (cod[i] >= 'A' && cod[i]<= 'Z')
            {
                k = 0;
                if (cod[i] == 'T')
                {
                    ca[j][k] = 'T';
                    k++;
                }
                else
                {
                    while (cod[i] != ' ' && cod[i] != '\n')
                    {
                        ca[j][k] = cod[i];
                        i++;
                        k++;
                    }
                }

                ca[j][k] = '\0';
                j++;
            }
        }

        free(cod);

        /*for (i = 0; i< cont; i++)
        {
            printf ("%s\n", ca[i]);
        }*/
        //printf("%s", ca[0]);


        //caso o cidadão tenha feito o código em letras minúsculas
        if (flag_letra == 1)
        {
            fputs ("Aviso: Recomenda-se o uso de letras maiúsculas no código\n\n", p);
        }

        //Testando para a presença de alguns códigos.
        flag_G94 = 0;
        flag_G81 = 0;
        flag_G71 = 0;
        flag_G54 = 0;
        fim_codigo = 0;
        flag_G00 = 0;

        for (j = 0; j < cont; j++)
        {
            if (strcmp("G94", ca[j]) == 0)
            {
                flag_G94 = 1;
                flag_G00 = 0;
                valor = 0.0;
                i = j;
                while (flag_G00 == 0)
                {
                    if (strcmp(ca[i], "G00") == 0)
                    {
                        flag_G00 = 1;
                        printf ("\n");
                    }
                    else
                    {
                        if (ca[i][0] == 'Z')
                        {
                            valor = pega_valor(i, ca);

                            if (valor < 0.0)
                            {
                                fprintf (p, "Bloco %s:\n    Aviso: Recomenda-se posicionar o ZP na face acabada da peça (Z0 no início da peça) (Z deve ser positivo depois de G94 até G00)\n\n", ca[linha]);
                            }
                        }
                    }
                    i++;
                }
            }
            else
            {
                if (strcmp("G81", ca[j]) == 0 || strcmp("G74", ca[j]) == 0 || strcmp("G83", ca[j]) == 0)
                {
                    flag_G81 = 1;
                    flag_G97 = 0;
                    i = j;
                    valor = 0.0;
                    while (flag_G97 == 0 || flag_S == 0)
                    {
                        if (strcmp("G97", ca[i]) == 0)
                        {
                            flag_G97 = 1;
                            printf ("\n");
                        }
                        else
                        {
                            if (strcmp(ca[i], "G96") == 0)
                            {
                                flag_G97 = 2;
                                fprintf(p, "Bloco %s - Comando %s\n  Erro: A usinagem está sendo realizada com a rotação máxima da máquina (Use G97 não G96)\n\n", ca[linha], ca[j]);
                            }
                            else
                            {
                                if ('S' == ca[i][0])
                                {
                                    flag_S = 1;
                                    valor = pega_valor(i, ca);
                                    if (valor<100)
                                    {
                                        fprintf(p, "Bloco %s - Comando %s e %s\n    Aviso: Usinagem sendo realizada com rotação muito baixa - Verificar!\n\n", ca[linha], ca[j], ca[i]);
                                    }
                                }
                            }
                        }
                        if (i == 0)
                        {
                            fprintf (p, "G97 não foi encontrado no código, %s no bloco %s necessita deste complemento - Verificar!\n\n", ca[j], ca[linha]);
                            flag_G97 = -1;
                            flag_S = -1;
                        }
                        i--;
                    }

                }
                else
                {
                    if (strcmp("G71", ca[j]) == 0 || strcmp("G72", ca[j]) == 0 || strcmp("G73", ca[j]) == 0)
                    {
                        flag_G71 = 1;
                    }
                    else
                    {
                        if (strcmp("G54", ca[j]) == 0 || strcmp("G55", ca[j]) == 0 || strcmp("G56", ca[j]) == 0 || strcmp("G57", ca[j]) == 0)
                        {
                            flag_G54 = 1;
                        }
                        else
                        {
                            if ((strcmp(ca[j], "M30")) == 0 || (strcmp(ca[j], "M02")) == 0)
                            {
                                fim_codigo = 1;
                            }
                            else
                            {
                                if (strcmp("G41", ca[j]) == 0 || strcmp("G42", ca[j]) == 0)
                                {
                                    flag_G41 = 1;
                                }
                                else
                                {
                                    if (ca[j][0] == 'N')
                                    {
                                        linha = j;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        for (j = 0; j<cont; j++)
        {
            if (strcmp("G81", ca[j]) == 0 || strcmp("G74", ca[j]) == 0 || strcmp("G75", ca[j]) == 0 || strcmp("G76", ca[j]) == 0)
            {
                i = j;
                flag_comp = 0;
                while (i>=0 && flag_comp == 0)
                {
                    if (strcmp("G41", ca[i]) == 0 || strcmp("G42", ca[i]) == 0)
                    {
                        flag_comp = 1;
                    }
                    else
                    {
                        if (strcmp("G40", ca[i]) == 0)
                        {
                            flag_comp = -1;
                        }
                    }
                    i--;
                }

                if (flag_comp == 1)
                {
                    fprintf(p, "Bloco %s - Comando %s\n    Erro: Não utilizar compensação de raio com este ciclo de usinagem\n\n", ca[linha], ca[j]);
                }
            }
            else
            {
                if (ca[j][0] == 'N')
                {
                    linha = j;
                }
            }
        }


        if (flag_G94 == 0)
        {
            fprintf (p, "Aviso: Recomenda-se utilizar ciclo de faceamento (G94)\n\n");
        }
        if (flag_G71 == 0)
        {
            fprintf(p, "Aviso: Recomenda-se utilizar ciclos de desbaste (G71 ou G72 ou G73)\n\n");
        }
        else
        {
            flag_G70 = 0;
            for (j = 0; j < cont; j++)
            {
                if (strcmp("G70", ca[j]) == 0)
                {
                    flag_G70 = 1;
                    break;
                }
            }
            if (flag_G70 == 0)
            {
                fprintf (p, "Aviso: Como foi utilizado um dos ciclos de desbaste (G71, G72 ou G73), recomenda-se utilizar o ciclo para acabamento - G70\n\n");
            }
        }
        if (flag_G41 == 0)
        {
            fprintf (p, "Aviso: Recomenda-se fortemente utilizar compensação do raio da ferramenta - Verificar (G41 ou G42)\n\n");
        }
        if (flag_G81 == 0)
        {
            fprintf (p, "Aviso: Se houver furação no programa recomenda-se utilizar ciclos de furação, etc. (G81 ou G74 ou G83)\n\n");
        }
        if (flag_G54 == 0)
        {
            fprintf (p, "Aviso: É altamente recomendado indicar qual ZP será utilizado em cada fase do processo (G54 ou G55 ou G56 ou G57)\n\n");
        }
        if (fim_codigo == 0)
        {
            fprintf (p, "Erro: Necessário encerrar o programa ao seu final (M02 ou M30)\n\n");
        }





        for (i = 0; i <= ngab; i++)
        {
            flag3 = 0;
            flag_G71 = 0;
            cont3 = 0;
            cont2 = 0;
            u = 0;
            w1 = 0;

            for (j = 0; j < cont; j++)
            {

                if (ca[j][0] == 'N')
                {
                    linha = j;
                }

                h = strcmp(gab[i].PripComand, ca[j]);

                if ((strcmp("G71", gab[i].PripComand) == 0) || (strcmp("G72", gab[i].PripComand)==0) || (strcmp("G73", gab[i].PripComand)== 0) || (strcmp("G76", gab[i].PripComand) == 0))
                {
                    if ((strcmp("G71", ca[j]) == 0) || (strcmp("G72", ca[j])==0) || (strcmp("G73", ca[j])== 0) || (strcmp("G76", ca[j]) == 0))
                    {
                        cont2++;
                        if (cont2 == 1)
                        {
                            flag3 = 0;
                        }
                        else
                        {
                            flag3 = 1;
                            cont2 = 0;
                        }
                    }

                }

                if (h == 0 && flag3 == 0)
                {

                    flag_G71 = 0;
                    flag_Suprema = 0;

                    fprintf (p, "\n\nBloco %s - Comando %s:\n", ca[linha], gab[i].PripComand);

                    for (z = 0; z < gab[i].numcomand; z++)
                    {
                        h2 = strcmp(gab[i].PripComand, gab[i].comand[z]);
                        if (h2==0)
                        {
                            inicio = j - gab[i].comandAntes;
                            fim = j+gab[i].comandDep;

                            if (inicio < 0)
                            {
                                inicio = 0;
                            }
                            if (fim >= cont)
                            {
                                fim = cont - 1 ;
                            }

                            if (strcmp("G76", gab[i].PripComand) == 0)
                            {
                                k = j;
                                flag_G99 = 0;
                                while (flag_G99 == 0)
                                {
                                    if (strcmp("G99", ca[k]) == 0)
                                    {
                                        flag_G99 = 1;
                                    }
                                    else
                                    {
                                        if (strcmp("G98", ca[k]) == 0)
                                        {
                                            flag_G99 = -1;
                                            fprintf (p, "   Erro: Avanço na abertura de rosca deve ser igual ao passo da rosca (deve-se usar G99, não G98)\n\n");
                                            flag_Suprema = 1;
                                        }
                                    }
                                    if (k == 0)
                                    {
                                        fprintf (p, "   G99 não foi encontrado no código - Verificar!\n\n");
                                        flag_G99 = -1;
                                        flag_Suprema = 1;
                                    }
                                    k--;
                                }
                            }
                            else
                            {
                                if ('T' == gab[i].PripComand[0])
                                {
                                    salva_testa_comb(ca, gab, j, fim, i, &flag_Suprema, p);
                                    flag_T++;
                                }
                            }

                            //testando antes do codigo principal no codigo e gabarito
                            for (k = 0; k < z; k++)
                            {
                                if (gab[i].comand[k][0] == '|')
                                {
                                    testa_cod(ca, gab, inicio, j, &flag, (k+1), i);

                                    if (flag != 0)
                                    {
                                        testa_cod(ca, gab, inicio, j, &flag, (k+2), i);;
                                    }

                                    //G41, G42 em tese devem estar antes do codigo principal
                                    if (strcmp("G71", gab[i].PripComand) == 0 || strcmp("G72", gab[i].PripComand) == 0 || strcmp("G73", gab[i].PripComand) == 0 || strcmp("G70", gab[i].PripComand) == 0)
                                    {
                                        flag_G71 = 1;
                                        flag_G41 = 0;
                                        if(strcmp("G41", gab[i].comand[(k+1)]) == 0 && flag == 0)
                                        {
                                            flag_G41 = 1;
                                            flag_G40 = 0;
                                        }
                                    }

                                    if (flag != 0)
                                    {
                                        salva_print(gab, i, (k+1), flag_M08, flag_M05, 0, p);
                                        flag_Suprema = 1;
                                    }
                                    k = k + 3;


                                }
                                else
                                {
                                    testa_cod(ca, gab, inicio, j, &flag, k, i);
                                    if (flag != 0)
                                    {
                                        salva_print (gab, i, k, flag_M08, flag_M05, 0, p);
                                        flag_Suprema = 1;
                                    }

                                    if (strcmp("M09", gab[i].comand[k]) == 0 && flag == 0)
                                    {
                                        flag_M08 = 0;   //M09 desliga a refrigeração (M08)
                                    }
                                    else
                                    {
                                        if (strcmp("M05", gab[i].comand[k]) == 0 && flag == 0)
                                        {
                                            flag_M05 = 1;
                                        }
                                    }
                                }
                            }


                            flag_G42 = 0;
                            for (w=j-1; w>=0; w--)
                            {
                                if (strcmp("G41", ca[w]) == 0 || strcmp("G42", ca[w]) == 0)
                                {
                                    flag_G42 = 1;
                                }
                                else
                                {
                                    if ('T' == ca[w][0])
                                    {
                                        break;
                                    }
                                }
                            }


                            if (flag_G41 == 1 && 'T' == gab[i].PripComand[0])
                            {
                                w=inicio;
                                while (w<cont)
                                {
                                    if (ca[w][0] == 'T')
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        if (strcmp("G40", ca[w]) == 0)
                                        {
                                            flag_G40 = 1;
                                            break;
                                        }
                                    }
                                    w++;
                                }
                                if (w < cont)   //quer dizer que teve algum T, ou que algum break entrou em ação
                                {
                                    if (flag_G40 != 1 && flag_T != 1 && flag_G42 == 1)
                                    {
                                        fprintf (p, "   Erro: Desligar a compensação do raio da ferramenta antes da troca da ferramenta (G40 antes do G28)\n\n");
                                        flag_Suprema = 1;
                                    }
                                }
                            }



                            //testando depois do codigo principal no codigo e gabarito
                            for (k = (z+1); k < gab[i].numcomand; k++)
                            {
                                if (gab[i].comand[k][0] == '|')
                                {
                                    testa_cod(ca, gab, (j+1), (fim+1), &flag, (k+1), i);

                                    if (flag != 0)
                                    {
                                        testa_cod(ca, gab, (j+1), (fim+1), &flag, (k+2), i);
                                    }

                                    if (flag != 0)
                                    {
                                        salva_print(gab, i, (k+1), flag_M08, flag_M05, 1, p);
                                        flag_Suprema = 1;
                                    }
                                    k = k+3;

                                }
                                else
                                {
                                    testa_cod(ca, gab, (j+1), (fim+1), &flag, k, i);

                                    if (flag != 0)
                                    {
                                        salva_print (gab, i, k, flag_M08, flag_M05, 1, p);
                                        flag_Suprema = 1;
                                    }

                                    if (flag == 0 && strcmp("M08", gab[i].comand[k]) == 0)
                                    {
                                        flag_M08 = 1;
                                    }
                                }
                            }
                            break;
                        }
                    }
                    if (flag_Suprema == 0)
                    {
                        fprintf (p, "   OK\n");
                    }
                }

                valor = 0.0;
                if (flag_G71 == 1)
                {
                    if (ca[j][0] == 'U' || ca[j][0] == 'W')
                    {

                        if (ca[j][0] == 'U')
                        {
                            u++;
                        }
                        else
                        {
                            if (ca[j][0] == 'W')
                            {
                                w1++;
                            }
                        }

                        cont3++;

                        if (u == 2 || w1 == 2)
                        {
                            flag_G71 = 0;
                            u = 0;
                            w1 = 0;
                        }
                        else
                        {
                            if (cont3 == 2)
                            {
                                cont3 = 0;
                                flag_G71 = 0;
                                u = 0;
                                w1 = 0;

                                valor = pega_valor(j, ca);

                                if (valor == 0.0)
                                {
                                    fprintf(p, "    Erro: Ciclo de desbaste, sendo assim, deixar sobrematerial para o acabamento posterior (U e W não podem ser 0)\n");
                                    flag_Suprema = 1;
                                }
                                printf ("\n");
                            }
                            else
                            {
                                valor = pega_valor(j, ca);
                                if (valor == 0.0)
                                {
                                    fprintf(p, "    Erro: Ciclo de desbaste, sendo assim, deixar sobrematerial para o acabamento posterior (U e W não podem ser 0)\n");
                                    flag_Suprema = 1;
                                }
                                printf ("\n");
                            }
                        }
                    }
                }
            }
        }

        free(ca);
        fclose(p);

        printf ("Arquivo salvo com sucesso\n\n");
    }



    free(p);

}
int main()
{
    char /*nomegab[50], nomecod[50],*/ *cd, nome[50], nome_gab[50], nome_cod[50];
    Gabarito *gab;
    //char **codigo;
    int i=0, j=0, ngab=0;
    //int *ncolunascod;
    int flag1=0, flag2=0;
    char tipoequip[12];
    int op=0;

    FILE *p;

    p = fopen("Gabarito iCNC.txt", "w");
    if (p == NULL)
    {
        printf ("Erro gabarito arquivo de gabarito n%co pode ser criado, confira as permissoes de acesso da pasta\n", 198);
        exit(1);
    }
    else
    {
        fprintf(p, "Torno\n\n-T 8 24\nG28 T M06 | G99 G98 | | G96 G97 | S | M03 M04 | M08 G00 F\n-G71 10 16\n| G41 G42 | X Z G71 U R G71 P Q U W F G00 X\n-G72 10 16\n| G41 G42 | X Z G72 W R G72 P Q U W F G00 Z\n-G73 10 16\n| G41 G42 | X Z G73 U W R G73 P Q U W F G00 X Z\n-G70 5 5\n| G41 G42 | X Z G70 P Q F\n-G75 5 7\nG75 X Z P Q F\n-G76 7 13\nX Z G76 P Q R G76 X Z P Q F\n-M00 5 5\nG28 M09 M05 M00 | M03 M04 | M08\n-M30 6 0\nG28 M05 M09 M30\n-M02 6 0\nG28 M05 M09 M02");
    }

    //setlocale(LC_ALL, "Portuguese");

    /*if(flag1==1)
    {
        for(i=0; i<=ngab; i++)
        {
            for(j=0; j<gab[i].numcomand; j++)
            {
                free(gab[i].comand[j]);
            }
            free(gab[i].comand);
            free(gab[i].tipoerro);
        }

        free(gab);
        ngab = 0;
    }*/

    //make_gab();

    fclose(p);
    free(p);

    setbuf(stdin,NULL);
    //printf("Digite o nome do arquivo do Gabarito: ");
    //gets(nomegab);
    printf("\n");


    //gab = Read_Gabarito(nomegab, &ngab, tipoequip);
    gab = Read_Gabarito("Gabarito iCNC.txt", &ngab, tipoequip);

    /*printf("Gabarito Salvo com sucesso\n\n");
    system("pause");
    system("cls");*/

    flag1=1;

    while(op!=8)
    {
        if(op!=0)
        {
            system("Pause");
            system("cls");
        }

        printf ("Inspetor CNC - iCNC\n\n");
        printf("   Menu  \n\n");
       // printf("1-Load Gabarito (basta digitar 1)\n");
        printf("1-Ler Programa CNC\n");
        printf("2-Listar Programa CNC\n");
        printf("3-Analisar Programa \n");
        printf("4-Listar Gabarito\n");
        printf("5-Salvar an%clise em um .txt\n", 160);
        printf("6-Trocar Gabarito\n");
        printf("7-About\n");
        printf("8-Sair\n\n");

        setbuf(stdin,NULL);
        printf("Qual %c a sua op%c%co: ", 130, 135, 198);
        fflush(stdin);
        scanf("%i", &op);

        printf("\n");

        switch (op)
        {
        case 1:
            {
                if(flag1==0)
                {
                    printf("N%co tem nenhum gabarito salvo!\n", 198);
                    printf("Escolha um gabarito antes de ler programa\n");
                }
                else
                {
                    if(flag2==1)
                    {
                        free(cd);
                    }

                   // setbuf(stdin,NULL);
                    //printf("Digite o nome do arquivo do Programa: ");
                    //gets(nomecod);
                    printf("\n");

                    cd = Read_Codigo(nome_cod);

                    printf("Programa CNC salvo com sucesso\n\n");

                    flag2=1;
                }

            }break;
        case 2:
            {
                if(flag2==1)
                {
                    printf ("%s", cd);
                }
                else
                {
                    printf("N%co tem nenhum Programa CNC salvo\n\n", 198);
                }

            }break;
        case 3:
            {
                if (flag2 == 1)
                {
                    analisador(gab, ngab, cd);
                    printf ("\n");
                }
                else
                {
                    printf ("N%co tem nenhum Programa CNC salvo\n\n", 198);
                }

            }break;
        case 4:
            {
                if(flag1==1)
                {
                    printf("%s\n",tipoequip);
                    for(i=0; i<=ngab; i++)
                    {
                        printf("%s %i %i\n", gab[i].PripComand, gab[i].comandAntes, gab[i].comandDep);

                        for(j=0; j<gab[i].numcomand; j++)
                        {
                            printf("%s ", gab[i].comand[j]);
                        }
                        printf("\n\n");
                    }
                }
                else
                {
                    printf("N%co tem nenhum gabarito salvo!\n\n", 198);
                }
            }break;
        case 5:
            {
                printf ("Digite o nome do arquivo (digite .txt tamb%cm): ", 130);
                fflush(stdin);
                gets(nome);
                txt(gab, ngab, cd, nome, nome_cod);
            }break;
        case 6:
            {
                while (1)
                {
                    printf ("Digite o nome do novo gabarito: ");
                    fflush(stdin);
                    gets(nome_gab);
                    p = fopen(nome_gab, "r");
                    if (p == NULL)
                    {
                        printf ("Erro ao abrir novo gabarito\n\n");
                    }
                    else
                    {
                        fclose(p);
                        free(gab);
                        gab = Read_Gabarito(nome_gab, &ngab, tipoequip);
                        break;
                    }
                }


            }break;
        case 7:
            {
                printf("Inspetor CNC (iCNC)\n   Institui%c%co: UTFPR --- Autores: Renan Erthmann Arantes, Neri Volpato\n\n", 135, 198);
            }break;
        case 8:
            {
                remove("Gabarito iCNC.txt");
                exit(0);
            }break;
        default:
            {
                printf("Op%c%co Inv%clida!\n\n", 135, 198, 160);
            }
        }
    }

   /* if(flag1==1)
    {
        for(i=0; i<=ngab; i++)
        {
            for(j=0; j<gab[i].numcomand; j++)
            {
                free(gab[i].comand[j]);
            }
            free(gab[i].comand);
            free(gab[i].tipoerro);
        }
        free(gab);
    }

    if(flag2==1)
    {
        for(i=0; i<=ncod; i++)
        {
            for(j=0; j<(cod[i].ncomand+1); j++)
            {
                free(cod[i].comanCod[j]);
            }
            free(cod[i].comanCod);
        }
        free(cod);
    }*/

    system("Pause");

    return 0;
}
