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

    typedef unsigned long long int int64;
    
    #define TAMANHO_NO 77
    #define NUM_CHAVES_NO 4

    #define boolean int
    #define TRUE 1
    #define FALSE 0

    struct _cabecalhoIndex
    {
        char status;
        int noRaiz;
        int RRNproxNo; // aqui invocamos a func preenchendoLixo do fileManager.c
    };

    struct _index
    {
        char folha;
        int nroChavesIndexadas;
        int RRNdoNo;

        // int P[5];
        // int C[4];
        // int64 Pr[4];

        int P1;
        int C1;
        int64 Pr1;

        int P2;
        int C2;
        int64 Pr2;

        int P3;
        int C3;
        int64 Pr3;

        int P4;
        int C4;
        int64 Pr4;

        int P5;
    };

    typedef struct _cabecalhoIndex CABECALHOI;
    typedef struct _index INDEX;    


#endif