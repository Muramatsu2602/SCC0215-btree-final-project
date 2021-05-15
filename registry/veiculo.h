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

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    #include "../utils/fileManager.h"

    struct _veiculo
    {	
        char removido;
        int tamanhoRegistro;
        char prefixo[6];
        char data[11];
        int	quantidadeLugares;
        int codLinha;
        int tamanhoModelo;
        char* modelo;
        int tamanhoCategoria;
        char* categoria;
    };

    // Todos as string possuem um byte a mais para ter o \0 ao final da string
    // Porém ao ser gravado no arquivo binário, esse \0 é desconsiderado
    struct _cabecalhoVeiculo
    {
        char status;
        int64 byteProxReg;
        int nroRegistros;
        int nroRegRemovidos;
        char descrevePrefixo[19];
        char descreveData[36];
        char descreveLugares[43];
        char descreveLinha[27];
        char descreveModelo[18];
        char descreveCategoria[21];
    };

    typedef struct _cabecalhoVeiculo CABECALHOV;
    typedef struct _veiculo VEICULO;

    boolean lerLinhaCSVVeiculo(FILE *fp, VEICULO *veiculo, CABECALHOV *cabecalho);
    boolean lerCabecalhoCSVVeiculo(FILE *fp, CABECALHOV *cabVeiculos);
    boolean escreverCabecalhoBINVeiculo(FILE *bin, CABECALHOV *cabVeiculos);
    boolean escreverBINVeiculo(FILE *bin, VEICULO *veiculos);
    void atualizaCabecalhoVeiculo(FILE *bin, CABECALHOV *cabecalho);
    boolean lerCabecalhoBINVeiculo(FILE *bin, CABECALHOV *cabVeiculos);
    boolean lerBINVeiculo(FILE *bin, VEICULO *veiculos, boolean flag, char *campo, char *valor);
    boolean exibirRegistrosVeiculo(CABECALHOV *cabVeiculos, VEICULO *veiculo);
    void exibirData(char *data);
#endif