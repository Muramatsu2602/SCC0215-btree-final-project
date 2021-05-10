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
    char *input = NULL;

    if (!fp || !veiculo)
        return FALSE;

    // lendo linha
    input = readLine(fp);

    // verificando se a linha lida no CSV esta excluida
    int aux = 0;
    if (input[0] == '*')
    {
        veiculo->removido = '1';
        aux++;
    }
    else
        veiculo->removido = '0';

    // LENDO VEICULO e tokenizando

    // Prefixo do veiculo
    token = strtok(&input[aux], delim);
    printf("PREFIXO: %s\t",token);
    strcpy(veiculo->prefixo, token);

    // Data de entrada do veiculo na frota
    token = strtok(NULL, delim);
    printf("DATA_ENTRADA: %s\t",token);
    strcpy(veiculo->data, token);

    // Quantidade de lugares sentados disponiveis
    token = strtok(NULL, delim);
    printf("QUANTIDADE_LUGARES: %s\t",token);
    veiculo->quantidadeLugares = (int)atoi(token);

    // Linha associada ao veiculo
    token = strtok(NULL, delim);
    printf("LINHA: %s\t",token);
    veiculo->codLinha = (int)atoi(token);

    // Modelo do veiculo e seu tamanho
    token = strtok(NULL, delim);
    printf("MODELO: %s\t",token);
    veiculo->tamanhoModelo = (int)strlen(token) + 1;

    // como o struct VEICULO vai ser reaproveitado, utilizamos realloc para caso de variacoes no tamanho do campo
    veiculo->modelo = NULL;
    veiculo->modelo = (char *)realloc(veiculo->modelo, veiculo->tamanhoModelo * sizeof(char));
    strcpy(veiculo->modelo, token);

    //  Categoria do veiculo e seu tamanho
    token = strtok(NULL, delim);
    printf("CATEGORIA: %s\t",token);
    printf("\n");
    veiculo->tamanhoCategoria = (int)strlen(token) + 1;

    // como o struct VEICULO vai ser reaproveitado, utilizamos realloc para caso de variacoes no tamanho do campo
    veiculo->categoria = NULL;
    veiculo->categoria = (char *)realloc(veiculo->categoria, veiculo->tamanhoCategoria * sizeof(char));
    strcpy(veiculo->categoria, token);

    // char removido;
    veiculo->removido = '0';
    // int tamanhoRegistro;
    veiculo->tamanhoRegistro = sizeof(veiculo->removido) + sizeof(veiculo->tamanhoRegistro) + sizeof(veiculo->prefixo) + sizeof(veiculo->data) + sizeof(veiculo->quantidadeLugares) + sizeof(veiculo->codLinha) + sizeof(veiculo->tamanhoModelo) + strlen(veiculo->modelo) + sizeof(veiculo->tamanhoCategoria) + strlen(veiculo->categoria);

    free(input);
    return TRUE;
}

/**
 * @brief Le a primeira linha de CSV e preenche os campos descritivos
 * 
 * @param fp 
 * @param cabVeiculos 
 * @return boolean 
 */
boolean lerCabecalhoCSVVeiculo(FILE *fp, CABECALHOV *cabVeiculos)
{
    const char delim[2] = ",";
    char *token = NULL;
    char *input = NULL;

    if (!fp || !cabVeiculos)
        return FALSE;

    // lendo cabeçalho
    input = readLine(fp);

    // Descreve prefixo
    token = strtok(input, delim);
    strcpy(cabVeiculos->descrevePrefixo, token);

    // Descreve Data
    token = strtok(NULL, delim);
    strcpy(cabVeiculos->descreveData, token);

    // Descreve Lugares
    token = strtok(NULL, delim);
    strcpy(cabVeiculos->descreveLugares, token);

    // Descreve Linha
    token = strtok(NULL, delim);
    strcpy(cabVeiculos->descreveLinha, token);

    // Descreve Modelo
    token = strtok(NULL, delim);
    strcpy(cabVeiculos->descreveModelo, token);

    // Descreve Categoria
    token = strtok(NULL, delim);
    strcpy(cabVeiculos->descreveCategoria, token);

    // Status
    cabVeiculos->status = '0'; // 0: inconsistente, 1: consistente

    // Calculando Tamanho do cabeçalho
    cabVeiculos->byteProxReg = sizeof(cabVeiculos->status) + sizeof(cabVeiculos->byteProxReg) + sizeof(cabVeiculos->nroRegistros) + sizeof(cabVeiculos->nroRegRemovidos) + strlen(cabVeiculos->descrevePrefixo) + strlen(cabVeiculos->descreveData) + strlen(cabVeiculos->descreveLugares) + strlen(cabVeiculos->descreveLinha) + strlen(cabVeiculos->descreveModelo) + strlen(cabVeiculos->descreveCategoria);
    cabVeiculos->nroRegistros = 0;
    cabVeiculos->nroRegRemovidos = 0;

    free(input);
    return TRUE;
}

/**
 * @brief escreve no arquivo binario a primeira linha do csv, que descreve cada campo do registro do veiculo
 * 
 * @param bin 
 * @param cabVeiculos 
 * @return boolean 
 */
boolean escreverCabecalhoBINVeiculo(FILE *bin, CABECALHOV *cabVeiculos)
{
    if (!bin || !cabVeiculos)
        return FALSE;

    // status
    fwrite(&cabVeiculos->status, sizeof(cabVeiculos->status), 1, bin);

    // byteProxreg
    fwrite(&cabVeiculos->byteProxReg, sizeof(cabVeiculos->byteProxReg), 1, bin);

    // nroRegistros
    fwrite(&cabVeiculos->nroRegistros, sizeof(cabVeiculos->nroRegistros), 1, bin);

    // nroRegRemovidos
    fwrite(&cabVeiculos->nroRegRemovidos, sizeof(cabVeiculos->nroRegRemovidos), 1, bin);

    // descrevePrefixo
    fwrite(cabVeiculos->descrevePrefixo, sizeof(char) * 18, 1, bin);

    // descreveData
    fwrite(cabVeiculos->descreveData, sizeof(char) * 32, 1, bin);

    // descreveLugares
    fwrite(cabVeiculos->descreveLugares, sizeof(char) * 42, 1, bin);

    // descreveLinha
    fwrite(cabVeiculos->descreveLinha, sizeof(char) * 17, 1, bin);

    // descreveModelo
    fwrite(cabVeiculos->descreveModelo, sizeof(char) * 20, 1, bin);

    // descreveCategoria
    fwrite(cabVeiculos->descreveCategoria, sizeof(char) * 26, 1, bin);

    return TRUE;
}

/**
 * @brief atualiza o header (quanto a byteProxReg. nroRegistros, nroRegRemovidos) e insere os dados de um veiculo no .bin
 * 
 * @param bin 
 * @param veiculos 
 * @return boolean 
 */
boolean escreverBINVeiculo(FILE *bin, VEICULO *veiculos)
{
    if (!bin || !veiculos)
        return FALSE;

    // removido
    fwrite(&veiculos->removido, sizeof(veiculos->removido), 1, bin);

    // tamanhoRegistro
    fwrite(&veiculos->tamanhoRegistro, sizeof(veiculos->tamanhoRegistro), 1, bin);

    // prefixo
    fwrite(&veiculos->prefixo, sizeof(veiculos->prefixo), 1, bin);

    // data
    fwrite(&veiculos->data, sizeof(veiculos->data), 1, bin);

    // quantidadeLugares
    fwrite(&veiculos->quantidadeLugares, sizeof(veiculos->quantidadeLugares), 1, bin);

    // codeLinha
    fwrite(&veiculos->codLinha, sizeof(veiculos->codLinha), 1, bin);

    // tamanhoModelo
    fwrite(&veiculos->tamanhoCategoria, sizeof(veiculos->tamanhoCategoria), 1, bin);

    // modelo
    fwrite(veiculos->modelo, strlen(veiculos->modelo), 1, bin);

    // tamanhoCategoria
    fwrite(veiculos->categoria, strlen(veiculos->categoria), 1, bin);

    // ------------------------- ATUALIZANDO CABEÇALHO -------------------------//

    // byteProxReg
    long byteProxReg = 0;

    // pegando o valor de byteProxReg Atual
    // sizeof(status) = 1 byte
    fseek(bin, 1, SEEK_SET);
    fread(&byteProxReg, sizeof(byteProxReg), 1, bin);

    // atualizando o offset do registro atual
    byteProxReg += veiculos->tamanhoRegistro;
    fseek(bin, 1, SEEK_SET);
    fwrite(&byteProxReg, sizeof(byteProxReg), 1, bin);

    // nroRegristros
    int nroRegistros = 0;

    // pegando o valor de nroRegistros Atual
    // sizeof(status) + sizeof(byteProxReg) = 9 bytes
    fseek(bin, 9, SEEK_SET);
    fread(&nroRegistros, sizeof(nroRegistros), 1, bin);

    // contabilizando o registro
    nroRegistros++;
    fseek(bin, 9, SEEK_SET);
    fread(&nroRegistros, sizeof(nroRegistros), 1, bin);

    // nroRegRemovidos
    if (!veiculos->removido)
        return TRUE;

    int nroRegRemovidos = 0;

    // pegando o valor de nroRegRemovidos atual
    // sizeof(status) + sizeof(byteProxReg) + sizeof(nroRegistros) = 13 bytes
    fseek(bin, 13, SEEK_SET);
    fread(&nroRegRemovidos, sizeof(nroRegRemovidos), 1, bin);

    // contabilizando registro se removido
    nroRegRemovidos++;
    fseek(bin, 13, SEEK_SET);
    fread(&nroRegRemovidos, sizeof(nroRegRemovidos), 1, bin);

    // retornar ao fim do arquivo, para escrever o proximo registro
    fseek(bin, byteProxReg, SEEK_SET);

    return TRUE;
}
