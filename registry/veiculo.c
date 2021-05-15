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

/**
 * @brief Ler uma linha do veiculo.csv e preenche o VEICULO *veiculo
 * 
 * @param fp 
 * @param veiculo 
 * @return boolean 
 */
boolean lerLinhaCSVVeiculo(FILE *fp, VEICULO *veiculo, CABECALHOV *cabecalho)
{
    const char delim[2] = ",";
    char *token = NULL;
    char *input = NULL;

    if (!fp || !veiculo)
        return FALSE;

    // lendo linha
    input = readLine(fp);

    // Verificar se chegou na última linha do arquivo CSV
    if (feof(fp))
    {
        free(input);
        return FALSE;
    }
    
    // verificando se a linha lida no CSV esta excluida
    int aux = 0;
    if (input[0] == '*')
    {
        veiculo->removido = '0';
        cabecalho->nroRegRemovidos += 1;
        aux++;
    }
    else
    {
        veiculo->removido = '1';
        cabecalho->nroRegistros += 1;
    }

    // LENDO VEICULO e tokenizando

    // Prefixo do veiculo
    token = strtok(&input[aux], delim);
    strcpy(veiculo->prefixo, token);

    // Data de entrada do veiculo na frota
    token = strtok(NULL, delim);
    if (strcmp(token, "NULO") == 0)
    {
        veiculo->data[0] = '\0';
        preenchendoLixo(strlen(veiculo->data)+1, 10, veiculo->data);
    }
    else
        strcpy(veiculo->data, token);

    // Quantidade de lugares sentados disponiveis
    token = strtok(NULL, delim);
    if (strcmp(token, "NULO") == 0)
        veiculo->quantidadeLugares = -1;
    else
        veiculo->quantidadeLugares = (int)atoi(token);

    // Linha associada ao veiculo
    token = strtok(NULL, delim);
    if (strcmp(token, "NULO") == 0)
        veiculo->codLinha = -1;
    else
        veiculo->codLinha = (int)atoi(token);

    // Modelo do veiculo e seu tamanho
    token = strtok(NULL, delim);
    veiculo->tamanhoModelo = (int)strlen(token);

    // como o struct VEICULO vai ser reaproveitado, utilizamos realloc para caso de variacoes no tamanho do campo
    if (strcmp(token, "NULO") == 0)
    {
        free(veiculo->modelo);
        veiculo->modelo = NULL;
        veiculo->tamanhoModelo = 0;
    }
    else
    {
        veiculo->modelo = (char *)realloc(veiculo->modelo, ((veiculo->tamanhoModelo)+1) * sizeof(char));
        strcpy(veiculo->modelo, token);
    }

    //  Categoria do veiculo e seu tamanho
    token = strtok(NULL, delim);
    veiculo->tamanhoCategoria = (int)strlen(token);

    // como o struct VEICULO vai ser reaproveitado, utilizamos realloc para caso de variacoes no tamanho do campo
    if (strcmp(token, "NULO") == 0)
    {
        free(veiculo->categoria);
        veiculo->categoria = NULL;
        veiculo->tamanhoCategoria = 0;
    }
    else
    {
        veiculo->categoria = (char *)realloc(veiculo->categoria, ((veiculo->tamanhoCategoria)+1) * sizeof(char));
        strcpy(veiculo->categoria, token);
    }

    // int tamanhoRegistro;
    veiculo->tamanhoRegistro = (sizeof(veiculo->prefixo)-1) + (sizeof(veiculo->data)-1) + sizeof(veiculo->quantidadeLugares) + sizeof(veiculo->codLinha) + sizeof(veiculo->tamanhoModelo) + sizeof(veiculo->tamanhoCategoria);

    if(veiculo->modelo != NULL)
    {
        veiculo->tamanhoRegistro += strlen(veiculo->modelo);
    }
    if(veiculo->categoria != NULL)
        veiculo->tamanhoRegistro += strlen(veiculo->categoria);

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
    // preenchendoLixo(strlen(cabVeiculos->descrevePrefixo), 18, cabVeiculos->descrevePrefixo);

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
    fwrite(&cabVeiculos->status, sizeof(char), 1, bin);

    // byteProxreg
    fwrite(&cabVeiculos->byteProxReg, sizeof(int64), 1, bin);

    // nroRegistros
    fwrite(&cabVeiculos->nroRegistros, sizeof(int), 1, bin);

    // nroRegRemovidos
    fwrite(&cabVeiculos->nroRegRemovidos, sizeof(int), 1, bin);

    // descrevePrefixo
    fwrite(cabVeiculos->descrevePrefixo, sizeof(char), 18, bin);

    // descreveData
    fwrite(cabVeiculos->descreveData, sizeof(char), 35, bin);

    // descreveLugares
    fwrite(cabVeiculos->descreveLugares, sizeof(char), 42, bin);

    // descreveLinha
    fwrite(cabVeiculos->descreveLinha, sizeof(char), 26, bin);

    // descreveModelo
    fwrite(cabVeiculos->descreveModelo, sizeof(char), 17, bin);

    // descreveCategoria
    fwrite(cabVeiculos->descreveCategoria, sizeof(char), 20, bin);

    return TRUE;
}

/**
 * @brief insere os dados de um veiculo no .bin
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
    fwrite(&veiculos->prefixo, 1, 5, bin);

    // data
    fwrite(&veiculos->data, 1, 10, bin);

    // quantidadeLugares
    fwrite(&veiculos->quantidadeLugares, sizeof(veiculos->quantidadeLugares), 1, bin);

    // codeLinha
    fwrite(&veiculos->codLinha, sizeof(veiculos->codLinha), 1, bin);

    // tamanhoModelo
    fwrite(&veiculos->tamanhoModelo, sizeof(veiculos->tamanhoModelo), 1, bin);

    // modelo
    if(veiculos->modelo != NULL)
        fwrite(veiculos->modelo, 1, veiculos->tamanhoModelo, bin);

    // tamanhoCategoria
    fwrite(&veiculos->tamanhoCategoria, sizeof(veiculos->tamanhoCategoria), 1, bin);

    // Categoria
    if(veiculos->categoria != NULL)
        fwrite(veiculos->categoria, 1, veiculos->tamanhoCategoria, bin);


    return TRUE;
}

/**
 * @brief atualiza no .bin de veiculos os dados que dizem respeito ao conteudo do arquivo.
 * Sao eles: byteProxReg, nroRegistros e nroRegistrosRemovidos
 * @param bin 
 * @param cabecalho 
 */
void atualizaCabecalhoVeiculo(FILE *bin, CABECALHOV *cabecalho)
{
    // Atualizar byteProxReg
    int64 byteProxReg = (int64)ftell(bin);
    fseek(bin, 1, SEEK_SET);
    fwrite(&byteProxReg, sizeof(byteProxReg), 1, bin);

    // Atualizar nroRegistros
    fseek(bin, 9, SEEK_SET);
    fwrite(&cabecalho->nroRegistros, sizeof(cabecalho->nroRegistros), 1, bin);

    // Atualizar nroRegRemovidos
    fseek(bin, 13, SEEK_SET);
    fwrite(&cabecalho->nroRegRemovidos, sizeof(cabecalho->nroRegRemovidos), 1, bin);
}

/**
 * @brief le o cabecalho do veiculo do .bin e preenche a struct CABECALHOV
 * 
 * @param bin 
 * @param cabVeiculos 
 * @return boolean 
 */
boolean lerCabecalhoBINVeiculo(FILE *bin, CABECALHOV *cabVeiculos)
{
    if (!bin || !cabVeiculos)
        return FALSE;

    // Posicionar o ponteiro no inicio do arquivo binário
    fseek(bin, 0, SEEK_SET);

    // Fazer a leitura dos campos e colocar na struct

    // Status
    fread(&cabVeiculos->status, sizeof(cabVeiculos->status), 1, bin);

    // byteProxReg
    fread(&cabVeiculos->byteProxReg, sizeof(cabVeiculos->byteProxReg), 1, bin);

    // nroRegistros
    fread(&cabVeiculos->nroRegistros, sizeof(cabVeiculos->nroRegistros), 1, bin);

    // nroRegRemovidos
    fread(&cabVeiculos->nroRegRemovidos, sizeof(cabVeiculos->nroRegRemovidos), 1, bin);

    // descrevePrefixo
    fread(cabVeiculos->descrevePrefixo, sizeof(char), 18, bin);
    cabVeiculos->descrevePrefixo[18] = '\0';

    // descreveData
    fread(cabVeiculos->descreveData, sizeof(char), 35, bin);
    cabVeiculos->descreveData[35] = '\0';

    // descreveLugares
    fread(cabVeiculos->descreveLugares, sizeof(char), 42, bin);
    cabVeiculos->descreveLugares[42] = '\0';

    // descreveLinha
    fread(cabVeiculos->descreveLinha, sizeof(char), 26, bin);
    cabVeiculos->descreveLinha[26] = '\0';

    // descreveModelo
    fread(cabVeiculos->descreveModelo, sizeof(char), 17, bin);
    cabVeiculos->descreveModelo[17] = '\0';

    // descreveCategoria
    fread(cabVeiculos->descreveCategoria, sizeof(char), 20, bin);
    cabVeiculos->descreveCategoria[20] = '\0';

    // Agora, o ponteiro do arquivo estará apontado para o primeiro registro do arquivo após o cabeçalho

    return TRUE;
}

/**
 * @brief Le linha do .bin e preenche uma struct do tipo VEICULO
 * 
 * @param bin 
 * @param veiculos 
 * @return boolean 
 */
boolean lerBINVeiculo(FILE *bin, VEICULO *veiculos)
{
    if (!bin || !veiculos)
        return FALSE;

    // Removido
    fread(&veiculos->removido, sizeof(veiculos->removido), 1, bin);
    
    // tamanhoRegistro
    fread(&veiculos->tamanhoRegistro, sizeof(veiculos->tamanhoRegistro), 1, bin);

    // prefixo
    fread(veiculos->prefixo, sizeof(char), 5, bin);
    veiculos->prefixo[5] = '\0';

    // data
    fread(veiculos->data, sizeof(char), 10, bin);
    veiculos->data[10] = '\0';

    // quantidadeLugares
    fread(&veiculos->quantidadeLugares, sizeof(veiculos->quantidadeLugares), 1, bin);

    // codLinha
    fread(&veiculos->codLinha, sizeof(veiculos->codLinha), 1, bin);

    // tamanhoModelo
    fread(&veiculos->tamanhoModelo, sizeof(veiculos->tamanhoModelo), 1, bin);

    // modelo
    // Checar se o campo Modelo é nulo
    if (veiculos->tamanhoModelo != 0)
    {
        veiculos->modelo = (char *)realloc(veiculos->modelo, ((veiculos->tamanhoModelo)+1) * sizeof(char));
        fread(&veiculos->modelo[0], sizeof(char), veiculos->tamanhoModelo, bin);
        veiculos->modelo[veiculos->tamanhoModelo] = '\0';
    }

    // tamanhoCategoria
    fread(&veiculos->tamanhoCategoria, sizeof(veiculos->tamanhoCategoria), 1, bin);

    // categoria
    if (veiculos->tamanhoCategoria != 0)
    {
        veiculos->categoria = (char *)realloc(veiculos->categoria, ((veiculos->tamanhoCategoria)+1) * sizeof(char));
        fread(&veiculos->categoria[0], sizeof(char), veiculos->tamanhoCategoria, bin);
        veiculos->categoria[veiculos->tamanhoCategoria] = '\0';
    }

    return TRUE;
}

boolean exibirRegistrosVeiculo(CABECALHOV *cabVeiculos, VEICULO *veiculo)
{
    if(!cabVeiculos || !veiculo)
        return FALSE;

    // Prefixo
    printf("%s: ",cabVeiculos->descrevePrefixo);
    printf("%s\n",veiculo->prefixo);

    // Modelo
    printf("%s: ",cabVeiculos->descreveModelo);
    if(veiculo->modelo == NULL)
    {
        printf("campo com valor nulo\n");
    }
    else
    {
        printf("%s\n",veiculo->modelo);
    }

    // Categoria
    printf("%s: ",cabVeiculos->descreveCategoria);
    if(veiculo->categoria == NULL)
    {
        printf("campo com valor nulo\n");
    }
    else
    {
        printf("%s\n",veiculo->categoria);
    }

    // Data
    printf("%s: ",cabVeiculos->descreveData);
    if(veiculo->data[0] == '\0')
    {
        printf("campo com valor nulo\n");
    }
    else
    {
        // Formato: 2004-11-17
        exibirData(veiculo->data);
    }

    // Quantidade Lugares
    printf("%s: ",cabVeiculos->descreveLugares);
    if(veiculo->quantidadeLugares == -1)
    {
        printf("campo com valor nulo\n");
    }
    else
    {
        printf("%d\n",veiculo->quantidadeLugares);
    }

    // Pular uma linha entre registros
    printf("\n");
    return TRUE;
}

/**
 * @brief Recebe a data desformatada e exibe na tela 
 * 
 * @param data
 */
void exibirData(char *data)
{
    const char delim[2] = "-";
    char *token = NULL;

    int ano;
    int mes;
    int dia;

    // Ano
    token = strtok(data, delim);
    ano = atoi(token);

    // Mês
    token = strtok(NULL, delim);
    mes = atoi(token);

    // Dia
    token = strtok(NULL, delim);
    dia = atoi(token);

    printf("%d de ",dia);
    
    switch(mes)
    {
        case 1:
            printf("janeiro de ");
            break;
        case 2:
            printf("fevereiro de ");
            break;
        case 3:
            printf("março de ");
            break;
        case 4:
            printf("abril de ");
            break;
        case 5:
            printf("maio de ");
            break;
        case 6:
            printf("junho de ");
            break;
        case 7:
            printf("julho de ");
            break;
        case 8:
            printf("agosto de ");
            break;
        case 9:
            printf("setembro de ");
            break;
        case 10:
            printf("outrubro de ");
            break;
        case 11:
            printf("novembro de ");
            break;
        case 12:
            printf("dezembro de ");
            break;
    }

    printf("%d\n",ano);
}