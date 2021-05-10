/**
 * @file veiculo.h
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef VEICULO_H
#define VEICULO_H

    #define TRUE 1
    #define FALSE 0
    #define boolean int
    #define FILE_MODE "rb"

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    struct _veiculo
    {	
        char removido;
        int tamanhoRegistro;
        char prefixo[5];
        char data[10];
        int	quantidadeLugares;
        int codLinha;
        int tamanhoModelo;
        char* modelo;
        int tamanhoCategoria;
        char* categoria;
    };

    struct _cabecalhoVeiculo
    {
        char status;
        long byteProxReg;
        int nroRegistros;
        int nroRegRemovidos;
        char descrevePrefixo[18];
        char descreveData[32];
        char descreveLugares[42];
        char descreveLinha[17];
        char descreveModelo[20];
        char descreveCategoria[26];
    };

    typedef struct _cabecalhoVeiculo CABECALHOV;
    typedef struct _veiculo VEICULO;

    boolean lerLinhaCSVVeiculo(FILE *fp, VEICULO *veiculo);
    boolean escreverCabecalhoBINVeiculo(FILE *bin, CABECALHOV cabVeiculos);
    boolean escreverBINVeiculo(FILE *BIN, VEICULO veiculos);

#endif