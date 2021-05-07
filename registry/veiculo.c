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
#include <string.h>

#include "veiculo.h"
#include "../utils/fileManager.h"

/**
 * @brief Ler uma linha do veiculo.csv e preenche o VEICULO *veiculo
 * 
 * @param fp 
 * @param veiculo 
 * @return boolean 
 */
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
    // LENDO VEICULO e tokenizando

    // Prefixo do veiculo
    token = strtok(linha, delim);
    strcpy(veiculo->prefixo, token);

    // Data de entrada do veiculo na frota
    token = strtok(NULL, delim);
    strcpy(veiculo->data, token);

    // Quantidade de lugares sentados disponiveis
    token = strtok(NULL, delim);
    veiculo->quantidadeLugares = (int)atoi(token);

    // Linha associada ao veiculo
    token = strtok(NULL, delim);
    veiculo->codLinha = (int)atoi(token);

    // Modelo do veiculo e seu tamanho
    token = strtok(NULL, delim);
    veiculo->tamanhoModelo = (int)strlen(token) + 1;

    // como o struct VEICULO vai ser reaproveitado, utilizamos realloc para caso de variacoes no tamanho do campo
    veiculo->modelo = (char *)realloc(veiculo->modelo, veiculo->tamanhoModelo * sizeof(char));
    strcpy(veiculo->modelo, token);

    //  Categoria do veiculo e seu tamanho
    token = strtok(NULL, delim);
    veiculo->tamanhoCategoria = (int)strlen(token) + 1;

    // como o struct VEICULO vai ser reaproveitado, utilizamos realloc para caso de variacoes no tamanho do campo
    veiculo->categoria = (char *)realloc(veiculo->categoria, veiculo->tamanhoCategoria * sizeof(char));
    strcpy(veiculo->categoria, token);

    // char removido;
    veiculo->removido = '0';
    // int tamanhoRegistro;
    veiculo->tamanhoRegistro = sizeof(veiculo->removido) + sizeof(veiculo->tamanhoRegistro) + sizeof(veiculo->prefixo) + sizeof(veiculo->data) + sizeof(veiculo->quantidadeLugares) + sizeof(veiculo->codLinha) + sizeof(veiculo->tamanhoModelo) + strlen(veiculo->modelo) + sizeof(veiculo->tamanhoCategoria) + strlen(veiculo->categoria);

    return TRUE;
}

boolean lerCabecalhoCSVVeiculo(FILE *fp, CABECALHOV *cabVeiculos)
{
    const char delim[2] = ",";
    char *token = NULL;
    char *linha = NULL;

    if (!fp || !cabVeiculos)
        return FALSE;

    // lendo cabeçalho
    linha = readLine(fp);

    // 

    // ByteProxReg
    cabVeiculos->byteProxReg = '0'; // 0: inconsistente, 1: consistente

    // Calculando Tamanho do cabeçalho
    cabVeiculos->byteProxReg = sizeof(cabVeiculos->status) + sizeof(cabVeiculos->byteProxReg) + sizeof(cabVeiculos->nroRegistros) + sizeof(cabVeiculos->nroRegRemovidos) + strlen(cabVeiculos->descrevePrefixo) + strlen(cabVeiculos->descreveData) + strlen(cabVeiculos->descreveLugares) + strlen(cabVeiculos->descreveLinha) + strlen(cabVeiculos->descreveModelo) + strlen(cabVeiculos->descreveCategoria);
    cabVeiculos->nroRegistros = 0;
    cabVeiculos->nroRegRemovidos = 0;
}

boolean escreverBinarioCabecalhoVeiculo(FILE *fp)
{
    if (!fp)
        return FALSE;

    return TRUE;
}

boolean escreverBinarioVeiculo(FILE *fp)
{
    if (!fp)
        return FALSE;

    return TRUE;
}
