/**
 * @file index.h
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @version 0.2
 * @date 2021-06-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef INDEX_H
#define INDEX_H

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <limits.h>

    typedef unsigned long long int int64;
    
    #define TAMANHO_NO 77
    #define NUM_CHAVES_NO 4

    #define boolean int
    #define TRUE 1
    #define FALSE 0

    struct _cabecalhoIndex
    {
        char status;
        int RRNnoRaiz;
        int RRNproxNo; // aqui invocamos a func preenchendoLixo do fileManager.c
    };

    struct _index
    {
        char folha;
        int nroChavesIndexadas;
        int RRNdoNo;

        int P[5];   // ponteiros para uma subárvore ou assume o valor -1 caso não exista subárvore
        int C[4];   // chaves de busca
        int64 Pr[4];// campo de referência para o registro no arquivo de dados que contém o registro de dados correspondente a C
    };

    typedef struct _cabecalhoIndex CABECALHOI;
    typedef struct _index INDEX;    


#endif