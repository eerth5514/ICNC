#include"classes.h"

Gabarito::Gabarito()
{
    Inicializa_Gabarito(0,0);
}

void Gabarito::Inicializa_Gabarito(int comantes, int comdep)
{
    comandAntes=comantes;
    comandDep=comdep;
}

char Gabarito::Salve_Gabarito(FILE*fp)
{
    char ch, str[200];
    int i, j, k;

    ch=getc(fp);
    i=0;

    //Salva: Comando Principal, numero de comandos antes e depois
    while(ch!=' ')
    {
        PripComand[i]=ch;
        ch=getc(fp);
        i++;
    }
    PripComand[i]='\0';

    fscanf(fp,"%i %i\n", &comandAntes, &comandDep);

    //Salva os comandos a serem analizados e o tipo de erro
    fgets(str, sizeof(str), fp);

    i=0;
    numcomand=0;
    while(str[i]!='\0')
    {
        if(str[i]==' ')
        {
            numcomand++;
        }
        i++;
    }
    numcomand++;

    comand = (char**) calloc(numcomand, sizeof(char*));

    for(i=0; i<numcomand; i++)
    {
        comand[i] = (char*) calloc(5, sizeof(char));
        if (comand[i] == NULL){
            printf ("Erro na alocacao de memoria");
            system("Pause");
            exit (1);
        }
    }

    i=0;
    j=0;
    k=0;
    while(str[k]==' ')
    {
        k++;
    }
    do
    {
        if(str[k]==' '){
            comand[j][i]='\0';
            j++;
            i=0;
        }
        else{
            if(str[k]!='\n' && str[k]!='\0')
            {
                comand[j][i]=str[k];
                i++;
            }
        }


        k++;
    }while(str[k]!='\0');

    return ch=getc(fp);
}


/*Codigo::Codigo()
{
    Inicializa_Codigo(0);
}

void Codigo::Inicializa_Codigo(int comentario)
{
    comenta[0]=comentario;
    comenta[1]=comentario;
}

void Codigo::Armazena_Codigo(FILE *fp, Gabarito gab, int num, char tipoequip[], int *i, int *j, char **cd, int nlinhas)
{
    int antes=0, depois=0;
    int k;
    int x=0, i_ini, j_ini;

    qualgab=num;

    comenta[0]=0;
    comenta[1]=0;

    i_ini=*i;
    j_ini=*j;

    if(gab.comandAntes>0)
    {
        *j=*j-1;
        k=0;

        while(antes!=gab.comandAntes)
        {
            if(cd[*i][*j]!=')')
            {
                if(cd[*i][*j]>=65 && cd[*i][*j]<=90)
                {
                    antes++;
                }
            }
            else
            {
                comenta[0]=1;
                while(cd[*i][*j]!='(')
                {
                    if(*j<=0)
                    {
                        *i=*i-1;
                        *j=strlen(cd[*i]);
                    }
                    *j=*j-1;
                }
            }

            if(antes!=gab.comandAntes)
            {
                if(*j<=0)
                {
                    *i=*i-1;

                    if(*i>=0)
                    {
                        *j=strlen(cd[*i]);
                    }
                }
                *j=*j-1;
            }

            if(*i<0)
            {
                *i=0;
                *j=0;
                break;
            }
        }
    }

    comanCod =(char**) calloc((antes+gab.comandDep+1), sizeof(char*));
    if(comanCod==NULL)
    {
        printf("Erro na alocacao de memoria 5 !");
        system("Pause");
        exit (1);
    }

    for(k=0; k<(antes+gab.comandDep+1); k++)
    {
        comanCod[k]=(char*) calloc(6,sizeof(char*));
        if(comanCod[k]==NULL)
        {
            printf("Erro na alocacao de memoria 6 !");
            system("Pause");
            exit (1);
        }
    }

    k=0;
    while(*i!=i_ini || *j!=j_ini)
    {
        if(cd[*i][*j]!='(')
        {
            if(cd[*i][*j]>=65 && cd[*i][*j]<=90)
            {
                if((cd[*i][*j]=='G') || (cd[*i][*j]=='M') || (cd[*i][*j]=='S') || (cd[*i][*j]=='F') || (cd[*i][*j]=='N'))  //Condição dentro do if; antes do princ
                {
                    while(k<7)
                    {
                        comanCod[x][k]=cd[*i][*j];
                        *j=*j+1;
                        k++;
                        if((cd[*i][*j]>=65 && cd[*i][*j]<=90)  || cd[*i][*j]==' ' || cd[*i][*j]=='\n')
                        {
                            break;
                        }
                    }
                    comanCod[x][k]='\0';
                    *j=*j-1;
                    x++;
                    k=0;
                }
                else
                {
                    comanCod[x][0]=cd[*i][*j];
                    comanCod[x][1]='\0';
                    x++;
                }
            }
        }
        else
        {
            while(cd[*i][*j]!=')')
            {
                *j=*j+1;
                if(cd[*i][*j]=='\n')
                {
                    *i=*i+1;
                    *j=0;
                }
            }
        }

        *j=*j+1;
        if(cd[*i][*j]=='\n')
        {
            *i=*i+1;
            *j=0;
        }
    }

    if(cd[*i][*j]=='G' || cd[*i][*j]=='M')
    {
        while(k<3)
        {
            comanCod[x][k]=cd[*i][*j];
            *j=*j+1;
            k++;
            if((cd[*i][*j]>=65 && cd[*i][*j]<=90)  || cd[*i][*j]==' ' || cd[*i][*j]=='\n')
            {
                break;
            }
        }
        comanCod[x][k]='\0';
        x++;
        k=0;
    }
    else
    {
        comanCod[x][0]=cd[*i][*j];
        comanCod[x][1]='\0';
        x++;
        *j=*j+1;
    }

    if(gab.comandDep>0)
    {
        while(depois!=gab.comandDep)
        {
            if(cd[*i][*j]!='(')
            {
                if(cd[*i][*j]>=65 && cd[*i][*j]<=90)
                {
                    if((cd[*i][*j]=='G') || (cd[*i][*j]=='M') || (cd[*i][*j]=='S') || (cd[*i][*j]=='F') || (cd[*i][*j]=='N'))  //Condição dentro do if; depois do princ
                    {
                        while(k<7)
                        {
                            comanCod[x][k]=cd[*i][*j];
                            *j=*j+1;
                            k++;
                            if((cd[*i][*j]>=65 && cd[*i][*j]<=90)  || cd[*i][*j]==' ' || cd[*i][*j]=='\n')
                            {
                                break;
                            }
                        }
                        comanCod[x][k]='\0';
                        *j=*j-1;
                        x++;
                        k=0;
                    }
                    else
                    {
                        comanCod[x][0]=cd[*i][*j];
                        comanCod[x][1]='\0';
                        x++;
                    }
                    depois++;
                }
            }
            else
            {
                while(cd[*i][*j]!=')')
                {
                    *j=*j+1;
                    if(cd[*i][*j]=='\n')
                    {
                        *i=*i+1;
                        *j=0;
                    }
                }
                comenta[1]=1;
            }

            *j=*j+1;
            if(cd[*i][*j]=='\n')
            {
                *i=*i+1;
                *j=0;
            }

            if(*i>=nlinhas)
            {
                if(depois<gab.comandDep)
                {
                    for(k=antes+depois+1; k<antes+gab.comandDep+1; k++)
                    {
                        free(comanCod[k]);
                    }
                }

                break;
            }
        }
    }

    *j=j_ini;
    *i=i_ini;
    ncomand = antes + depois;
}*/
