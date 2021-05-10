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
    linha->aceitaCartao = token[0];

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

/**
 * @brief Le a primeira liha do CSV e preenche os campos descritivos
 * 
 * @param fp 
 * @param cabVeiculos 
 * @return boolean 
 */
boolean lerCabecalhoCSVLinha(FILE *fp, CABECALHOL *cabLinhas)
{
    const char delim[2] = ",";
    char *token = NULL;
    char *input = NULL;

    if(!fp || !cabLinhas)
        return FALSE;

    // lendo cabeçalho
    input = readLine(fp);

    // Descreve código
    token = strtok(input, delim);
    strcpy(cabLinhas->descreveCodigo, token);

    // Descrece cartão
    token = strtok(NULL, delim);
    strcpy(cabLinhas->descreveCartao token);

    // Descreve Nome
    token = strtok(NULL, delim);
    strcpy(cabLinhas->descreveNome token);

    // Descreve Linha
    token = strtok(NULL, delim);
    strcpy(cabLinhas->descreveNome token);

    // Status
    cabLinhas->status = '0';

    // Calculando o tamanho do cabeçalho
    cabLinhas->byteProxReg = sizeof(cabLinhas->status) + sizeof(cabLinhas->byteProxReg) + sizeof(cabLinhas->nroRegistros) + sizeof(cabLinhas->nroRegRemovidos) + strlen(cabLinhas->descreveCodigo) + strlen(cabLinhas->descreveCartao) + strlen(cabLinhas->descreveNome) + strlen(cabLinhas->descreveLinha);
    cabLinhas->nroRegistros = 0;
    cabLinhas->nroRegRemovidos = 1;

    free(input);
    return TRUE;
}

boolean escreveCabecalhoBINLinhas(FILE *bin, CABECALHOL *cabLinhas)
{
    if (!bin)
        return FALSE;

    // Status
    fwrite($cabLinhas->status, sizeof(char), 1, bin);

    // ByteProxReg
    fwrite($cabLinhas->byteProxReg, sizeof(long), 1, bin);

    // nroRegistros
    fwrite($cabLinhas->nroRegistros, sizeof(int), 1, bin);

    // nroRegRemovidos
    fwrite($cabLinhas->nroRegRemovidos, sizeof(int), 1, bin);

    // descreveCodigo
    fwrite($cabLinhas->descreveCodigo, sizeof(char)*15, 1, bin);

    // descreveCartao
    fwrite($cabLinhas->descreveCartao, sizeof(char)*13, 1, bin);

    // descreveNome
    fwrite($cabLinhas->descreveNome, sizeof(char)*13, 1, bin);

    // descreveLinha
    fwrite($cabLinhas->descreveLinha, sizeof(char)*24, 1, bin);
    
    return TRUE;
}

boolean escreverBINLinha(FILE *bin, LINHA *linhas)
{
    if (!bin)
        return FALSE;

    // Removido

    // tamanhoRegistro

    // codLinha

    // aceitaCartao

    // tamanhoNome

    // nomeLinha

    // tamanhoCor

    // corLinha

    // ------------------------- ATUALIZANDO CABEÇALHO -------------------------//

    // byteProxReg
    // sizeof(status) = 1 byte

    lonh byteProxReg = 0;

    // Mover o ponteiro para byteProxReg no cabeçalho
    fseek(bin, 1, SEEK_SET);
    fread(&byteProxReg, sizeof(long), 1, bin);

    // Atualizando o offset do registro atual
    byteProxReg += linhas->tamanhoRegistro;
    fseek(bin, 1, SEEK_SET);
    fwrite($byteProxReg, sizeof(long), 1, bin);

    

    return TRUE;
}
