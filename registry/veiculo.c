/**
 * @file veiculo.c
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

#include "veiculo.h"
#include "../utils/fileManager.h"

boolean lerLinhaCSVVeiculo(FILE *fp, VEICULO *veiculo)
{
    const char delim[2] = ",";
    char *token = NULL;
    char *linha = NULL;

    if (!fp || !veiculo)
        return FALSE;

    // lendo linha
    linha = readLine(fp);
    // verificando se a linha lida no CSV esta excluida
    if (linha[0] == '*')
    {
        free(linha);
        return TRUE;
    }
    // Tokenizando linha e preenchendo veiculo
    
}

// boolean escreverBinarioCabecalhoVeiculo()

boolean escreverBinarioVeiculo(const char *filename)
{
    if (!filename)
        return FALSE;
}
