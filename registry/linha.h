/**
 * @file linha.h
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef LINHA_H
#define LINHA_H

    #define TRUE 1
    #define FALSE 0
    #define boolean int
    #define FILE_MODE "rb"
    struct _cabecalhoLinha
    {
        char status;
        long byteProxReg;
        int nroRegistros;
        int nroRegRemovidos;
        char descreveCodigo[15];
        char descreveCartao[13];
        char descreveNome[13];
        char descreveLinha[24];
    };

    struct _linha
    {
        char removido;
        int tamanhoRegistro;
        int codLinha;
        char aceitaCartao;
        int tamanhoNome;
        int tamanhoCor;
        char* corLinha;
    };

    typedef struct _cabecalhoLinha CABECALHOL;
    typedef struct _linha LINHA;

#endif