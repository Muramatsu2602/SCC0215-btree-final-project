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

/**
 * @brief Le uma linha do linha.csv e preenche uma struct homonima, bem como atualiza o cabeçalho do .bin correspondente
 * 
 * @param fp 
 * @param linha 
 * @param cabecalho 
 * @return boolean 
 */
boolean lerLinhaCSVLinha(FILE *fp, LINHA *linha, CABECALHOL *cabecalho)
{
    const char delim[2] = ",";
    char *token = NULL;
    char *input = NULL;

    if (!fp || !linha)
        return FALSE;

    // Ler a linha do CSV
    input = readLine(fp);

    // Verificar se chegou na última linha do arquivo CSV
    if(feof(fp))
    {
        free(input);
        return FALSE;
    }

    // Codigo da linha,Aceita cartao,Nome da linha,Cor que descreve a linha

    // Verificando se a linha lido no CSV está excluída
    int aux = 0;
    if (input[0] == '*')
    {
        linha->removido = '0';
        cabecalho->nroRegRemovidos += 1;
        aux++;
    }
    else
    {
        linha->removido = '1';
        cabecalho->nroRegistros += 1;
    }

    // Codigo da linha
    token = strtok(&input[aux], delim);
    linha->codLinha = (int)atoi(token);

    // aceita cartao
    token = strtok(NULL, delim);
    linha->aceitaCartao = token[0];

    // Nome da linha
    token = strtok(NULL, delim);
    linha->tamanhoNome = (int)strlen(token);
    if(strcmp(token, "NULO") == 0)
    {
        free(linha->nomeLinha);
        linha->nomeLinha = NULL;
        linha->tamanhoNome = 0;
    }
    else
    {
        linha->nomeLinha = (char *) realloc (linha->nomeLinha, ((linha->tamanhoNome)+1) * sizeof(char));
        strcpy(linha->nomeLinha, token);
    }

    // Cor da linha
    token = strtok(NULL, delim);
    linha->tamanhoCor = (int)strlen(token);
    if(strcmp(token, "NULO") == 0)
    {
        free(linha->corLinha);
        linha->corLinha = NULL;
        linha->tamanhoCor = 0;
    }
    else
    {
        linha->corLinha = (char *) realloc (linha->corLinha, ((linha->tamanhoCor)+1) * sizeof(char));
        strcpy(linha->corLinha, token);
    }

    // int tamanhoRegistro
    linha->tamanhoRegistro = sizeof(linha->codLinha) + sizeof(linha->aceitaCartao) + sizeof(linha->tamanhoNome) + sizeof(linha->tamanhoCor);

    if(linha->nomeLinha != NULL)
    {
        linha->tamanhoRegistro += strlen(linha->nomeLinha);
    }
    if(linha->corLinha != NULL)
    {
        linha->tamanhoRegistro += strlen(linha->corLinha);
    }
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
    strcpy(cabLinhas->descreveCartao, token);

    // Descreve Nome
    token = strtok(NULL, delim);
    strcpy(cabLinhas->descreveNome, token);

    // Descreve Linha
    token = strtok(NULL, delim);
    strcpy(cabLinhas->descreveLinha, token);

    // Status
    cabLinhas->status = '0';

    // Calculando o tamanho do cabeçalho
    cabLinhas->byteProxReg = sizeof(cabLinhas->status) + sizeof(cabLinhas->byteProxReg) + sizeof(cabLinhas->nroRegistros) + sizeof(cabLinhas->nroRegRemovidos) + strlen(cabLinhas->descreveCodigo) + strlen(cabLinhas->descreveCartao) + strlen(cabLinhas->descreveNome) + strlen(cabLinhas->descreveLinha);
    cabLinhas->nroRegistros = 0;
    cabLinhas->nroRegRemovidos = 0;

    free(input);
    return TRUE;
}

/**
 * @brief escreve os dados do cabeçalho no arq bin correspondente
 * 
 * @param bin 
 * @param cabLinhas 
 * @return boolean 
 */
boolean escreveCabecalhoBINLinhas(FILE *bin, CABECALHOL *cabLinhas)
{
    if (!bin || !cabLinhas)
        return FALSE;
    // Status
    fwrite(&cabLinhas->status, sizeof(char), 1, bin);

    // ByteProxReg
    fwrite(&cabLinhas->byteProxReg, sizeof(int64), 1, bin);

    // nroRegistros
    fwrite(&cabLinhas->nroRegistros, sizeof(int), 1, bin);

    // nroRegRemovidos
    fwrite(&cabLinhas->nroRegRemovidos, sizeof(int), 1, bin);
    
    // descreveCodigo
    fwrite(&cabLinhas->descreveCodigo, sizeof(char), 15, bin);
    
    // descreveCartao
    fwrite(&cabLinhas->descreveCartao, sizeof(char), 13, bin);
    
    // descreveNome
    fwrite(&cabLinhas->descreveNome, sizeof(char), 13, bin);
    
    // descreveLinha
    fwrite(&cabLinhas->descreveLinha, sizeof(char), 24, bin);
    
    return TRUE;
}

/**
 * @brief escreve 1 registro no final do arquivo .bin correspondente aos dados das linhas de ônibus
 * 
 * @param bin 
 * @param linhas 
 * @return boolean 
 */
boolean escreverBINLinha(FILE *bin, LINHA *linhas)
{
    if (!bin || !linhas)
        return FALSE;

    // Removido
    fwrite(&linhas->removido, sizeof(linhas->removido), 1, bin);

    // tamanhoRegistro
    fwrite(&linhas->tamanhoRegistro, sizeof(linhas->tamanhoRegistro), 1, bin);

    // codLinha
    fwrite(&linhas->codLinha, sizeof(linhas->codLinha), 1, bin);

    // aceitaCartao
    fwrite(&linhas->aceitaCartao, sizeof(linhas->aceitaCartao), 1, bin);

    // tamanhoNome
    fwrite(&linhas->tamanhoNome, sizeof(linhas->tamanhoNome), 1, bin);

    // nomeLinha
    if(linhas->nomeLinha != NULL)
        fwrite(linhas->nomeLinha, 1, linhas->tamanhoNome, bin);

    // tamanhoCor
    fwrite(&linhas->tamanhoCor, sizeof(linhas->tamanhoCor), 1, bin);

    // corLinha
    if(linhas->corLinha != NULL)
        fwrite(linhas->corLinha, 1, linhas->tamanhoCor, bin);


    return TRUE;
}

/**
 * @brief atualiza no cabeçalho do arq .bin os campos que descrevem características do conteúdo do arquivo
 * São eles: byteProxReg, nroRegistros, nroRegistrosRemovidos
 * @param bin 
 * @param cabecalho 
 */
void atualizaCabecalhoLinha(FILE *bin, CABECALHOL *cabecalho)
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
 * @brief le o cabecalho da do .bin da linha e preenche a struct CABECALHOL
 * 
 * @param bin 
 * @param cabLinhas 
 * @return boolean 
 */
boolean lerCabecalhoBINLinha(FILE*bin, CABECALHOL *cabLinhas){
    /*
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
    */
   if (!bin || !cabLinhas)
        return FALSE;

    
    // Posicionar o ponteiro no inicio do arquivo binário
    fseek(bin, 0, SEEK_SET);

    // Fazer a leitura dos campos e colocar na struct

    // Status
    fread(&cabLinhas->status, sizeof(cabLinhas->status), 1, bin);

    // byteProxReg
    fread(&cabLinhas->byteProxReg, sizeof(cabLinhas->byteProxReg), 1, bin);

    // nroRegistros
    fread(&cabLinhas->nroRegistros, sizeof(cabLinhas->nroRegistros), 1, bin);

    // nroRegRemovidos
    fread(&cabLinhas->nroRegRemovidos, sizeof(cabLinhas->nroRegRemovidos), 1, bin);

    // descreveCodigo
    fread(cabLinhas->descreveCodigo, sizeof(cabLinhas->descreveCodigo), 15, bin);

    // descreveCartao
    fread(cabLinhas->descreveCartao, sizeof(cabLinhas->descreveCartao), 13, bin);

    // descreveNome
    fread(cabLinhas->descreveNome, sizeof(cabLinhas->descreveNome), 13, bin);

    // descreveLinha
    fread(cabLinhas->descreveLinha, sizeof(cabLinhas->descreveLinha), 24, bin);

    // Agora, o ponteiro do arquivo estará apontado para o primeiro registro do arquivo após o cabeçalho

    return TRUE;
}


/**
 * @brief Le linha do .bin e preenche uma struct do tipo LINHA
 * 
 * @param bin 
 * @param linhas 
 * @return boolean 
 */
boolean lerBINLinha(FILE*bin, LINHA *linhas){

    /*
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
     */
    if(!bin || !linhas)
        return FALSE;
    
    


}