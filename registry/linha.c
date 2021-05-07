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
#include "linha.h"
#include "../utils/fileManager.h"

boolean lerLinhaCSVLinha(FILE *fp, LINHA *linha)
{
    const char delim[2] = ",";
    char *token = NULL;
    char *input = NULL;

    if (!fp || !linha)
        return FALSE;

    // Ler a linha do CSV
    input = readLine(fp);
    // Verificando se a linha lida no CSV está excluida
    if (input[0] == '*')
    {
        free(input);
        return TRUE;
    }

    // Codigo da linha,Aceita cartao,Nome da linha,Cor que descreve a linha

    // código da linha
    token = strtok(input, delim);
    linha->codLinha = atoi(token);

    // aceita cartao
    token = strtok(NULL, delim);
    linha->aceitaCartao = (char)token;

    // Nome da linha
    token = strtok(NULL, delim);
    linha->tamanhoNome = (int)strlen(token) + 1;

    linha->nomeLinha = (char *)realloc(linha->nomeLinha, linha->tamanhoNome * sizeof(char));
    strcpy(linha->nomeLinha, token);

    // Cor da linha
    token = strtok(NULL, delim);
    linha->tamanhoCor = (int)strlen(token) + 1;

    linha->corLinha = (char *)realloc(linha->corLinha, linha->tamanhoCor * sizeof(char));
    strcpy(linha->corLinha, token);

    // Calcular o tamanho do registro através do tamanho das variáveis da struct
    linha->tamanhoRegistro = sizeof(linha->removido) + sizeof(linha->tamanhoRegistro) + sizeof(linha->codLinha) + sizeof(linha->aceitaCartao) + sizeof(linha->tamanhoNome) + strlen(linha->nomeLinha) + sizeof(linha->tamanhoCor) + strlen(linha->corLinha);

    // Colocar removido como 0
    linha->removido = '0';

    free(input);
    return TRUE;
}

// boolean escreverBinarioCabecalhoLinha

boolean escreverBinarioLinha(const char *filename)
{
    if (!filename)
        return FALSE;

    return TRUE;
}
