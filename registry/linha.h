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

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    #include "../utils/fileManager.h"
    struct _cabecalhoLinha
    {
        char status;
        int64 byteProxReg;
        int nroRegistros;
        int nroRegRemovidos;
        char descreveCodigo[16];
        char descreveCartao[14];
        char descreveNome[14];
        char descreveLinha[25];
    };

    struct _linha
    {
        char removido;
        int tamanhoRegistro;
        int codLinha;
        char aceitaCartao;
        int tamanhoNome;
        char* nomeLinha;
        int tamanhoCor;
        char* corLinha;
    };

    typedef struct _cabecalhoLinha CABECALHOL;
    typedef struct _linha LINHA;

    boolean lerLinhaCSVLinha(FILE *fp, LINHA *linha);
    boolean lerCabecalhoCSVLinha(FILE *fp, CABECALHOL *cabLinhas);
    boolean escreveCabecalhoBINLinhas(FILE *bin, CABECALHOL *cabLinhas);
    boolean escreverBINLinha(FILE *bin, LINHA *linhas);

#endif