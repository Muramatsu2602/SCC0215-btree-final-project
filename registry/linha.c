/**
 * @file linha.c
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "linha.h"

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