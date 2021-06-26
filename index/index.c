/**
 * @file index.c
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @version 0.2
 * @date 2021-06-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "index.h"

/**
 * @brief 
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 * @param chave 
 * @param enderecoBin 
 */
void inserirIndex(FILE *fp, CABECALHOI *cabecalho, INDEX *indice, int chave, int64 enderecoBin)
{
    printf("%d | %lld\n",chave,enderecoBin);
    // Primeiro caso: Árvore vazia
    // Checar se o noRaiz da struct de cabeçalho está com valor -1
    if (cabecalho->noRaiz == -1)
    {
        printf("Entrou aqui.\n");
        // Criar um nó página, inserir no arquivo binário de indice, colocar o RRN no cabecalho->noRaiz
        // Inserir a chave ordenadamente na primeira página criado
        cabecalho->noRaiz = 0;

        indice->C[0] = chave;
        indice->Pr[0] = enderecoBin;
        indice->nroChavesIndexadas++;

        // Escrever a nova página no arquivo
        escreverBINIndex(fp, cabecalho, indice);

        cabecalho->RRNproxNo = cabecalho->noRaiz + 1;
    }
    // Se a árvore existe
    else
    {
        lerBINIndice(fp, indice, cabecalho, cabecalho->noRaiz);

        // se o nó está cheio, então a árvore B cresce em altura
        if (indice->nroChavesIndexadas == NUM_CHAVES_NO)
        {
            // Criar a nova raiz
            INDEX novaRaiz;
            inicializarNovaPagina(&novaRaiz, cabecalho->RRNproxNo, FALSE);
            escreverBINIndex(fp, cabecalho, &novaRaiz);
            cabecalho->noRaiz = cabecalho->RRNproxNo;
            cabecalho->RRNproxNo++;

            // Página a esquerda da novaRaiz será a raiz original
            novaRaiz.P[0] = indice->RRNdoNo;

            int chaves[5];
            int64 enderecosBin[5];
            int j = 0;
            for (int i = 0; i < 5; i++)
            {
                if (chave < indice->C[j])
                {
                    enderecosBin[i] = enderecoBin;
                    chaves[i] = chave;
                    chave = INT_MAX;
                }
                else
                {
                    enderecosBin[i] = indice->Pr[j];
                    chaves[i] = indice->C[j];
                    j++;
                }
            }

            // Dar split na raiz antiga e promover o valor central
            divideNo(fp, cabecalho, 0, indice, &novaRaiz, chaves, enderecosBin);

            cabecalho->noRaiz = novaRaiz.RRNdoNo;
        }
        else
        {
            inserirChave(fp, cabecalho, indice, chave, enderecoBin);
        }
    }
}

/**
 * @brief insere uma chave em um nó, utilizando ou não o SPLIT
 * 
 * @param fp 
 * @param cabecalho 
 * @param raiz 
 * @param chave 
 * @param enderecoBin 
 */
void inserirChave(FILE *fp, CABECALHOI *cabecalho, INDEX *raiz, int chave, int64 enderecoBin)
{
    printf("ENTROU AQUI!");
    // i guarda a posição da última chave inserida em raiz
    int i = raiz->nroChavesIndexadas - 1;

    if (raiz->folha == 1)
    {
        // Encontrar a posição em que a chave será inserida
        while (i >= 0 && raiz->C[i] > chave)
        {
            raiz->C[i + 1] = raiz->C[i];
            raiz->Pr[i + 1] = raiz->Pr[i];
            i--;
        }

        // Inserir a nova chave na posição encontrada
        raiz->C[i + 1] = chave;
        raiz->Pr[i + 1] = enderecoBin;
        raiz->nroChavesIndexadas++;
    }
    else
    {
        // Se não for folha

        // Encontrar o RRN da página que irá receber a chave
        while (i >= 0 && raiz->C[i] > chave)
            i--;

        // Colocar em aux a página do RRN encontrado
        INDEX aux;
        lerBINIndice(fp, &aux ,cabecalho, raiz->P[(i + 1)]);

        int chaves[5];
        int64 enderecosBin[5];
        int j = 0;
        for (int i = 0; i < 5; i++)
        {
            if (chave < aux.C[j])
            {
                enderecosBin[i] = enderecoBin;
                chaves[i] = chave;
                chave = INT_MAX;
            }
            else
            {
                enderecosBin[i] = aux.Pr[j];
                chaves[i] = aux.C[j];
                j++;
            }
        }

        // Ver se a página está cheia
        if (aux.nroChavesIndexadas == NUM_CHAVES_NO)
        {
            // Se está cheio, então deve fazer o split
            divideNo(fp, cabecalho, (i + 1), &aux, raiz, chaves, enderecosBin);
        }
        inserirChave(fp, cabecalho, &aux, chave, enderecoBin);
    }
}

/**
 * @brief realiza o procedimento de SPLIT, criando duas novas paginas e 
 * promovendo um dos nos da pagina original
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 * @param novaChave nova chave a ser inserida
 */

// i é índice da última chave presente em novaRaiz
/*
    Quando vou promover uma chave, essa chave pode ir para uma página já existente
    e o <i> serve para controlar em qual posição em C e em P irá referenciar a nova página
*/
void divideNo(FILE *fp, CABECALHOI *cabecalho, int i, INDEX *raiz, INDEX *novaRaiz, int chaves[5], int64 enderecosBin[5])
{
    // Inicializando uma nova página
    // Atualizando as informações no cabeçalho
    INDEX novaPagina;
    inicializarNovaPagina(&novaPagina, cabecalho->RRNproxNo, FALSE);
    escreverBINIndex(fp, cabecalho, &novaPagina);
    cabecalho->noRaiz = cabecalho->RRNproxNo;
    cabecalho->RRNproxNo++;

    novaPagina.nroChavesIndexadas = 2;

    // P[0] C[0] P[1] C[1] P[2] C[2] P[3] C[3] P[4]

    // Copiar as 2 últimas chaves para o novaPagina
    // Copiar os 2 últimos ponteiros das subárvores
    novaPagina.P[0] = raiz->P[2];
    novaPagina.C[0] = chaves[3];
    novaPagina.Pr[0] = enderecosBin[3];
    novaPagina.P[1] = raiz->P[3];
    novaPagina.C[1] = chaves[4];
    novaPagina.Pr[1] = enderecosBin[4];
    novaPagina.P[2] = raiz->P[4];

    // Tirar as duas chaves da raiz
    raiz->C[2] = -1;
    raiz->Pr[2] = -1;
    raiz->C[3] = -1;
    raiz->Pr[3] = -1;
    raiz->P[2] = -1;
    raiz->P[3] = -1;
    raiz->P[4] = -1;

    raiz->nroChavesIndexadas -= 2;

    // Adicionar a chave do meio no novaRaiz
    novaRaiz->C[i] = chaves[2];
    novaRaiz->Pr[i] = enderecosBin[2];

    // Página a direita da novaRaiz será a nova pagina criada
    novaRaiz->P[i + 1] = novaPagina.RRNdoNo;
    novaRaiz->nroChavesIndexadas = 1;
}

/**
 * @brief Lê a página de índice do RRN específicado e insere seus dados temporariamente na struct INDEX
 * 
 * @param fp arquivo Binário de índices
 * @param indice struct de páginas do indice
 * @param RRN RRN da página a ser lida
 */
boolean lerBINIndice(FILE *fp, INDEX *indice, CABECALHOI *cabecalho, int RRN)
{
    if (!fp || !cabecalho || !indice)
        return FALSE;

    // posiciona o ponteiro do arquivo logo depois do fim do cabeçalho
    fseek(fp, (RRN + 1) * TAMANHO_NO, SEEK_SET);

    // Folha
    fread(&indice->folha, sizeof(char), 1, fp);

    // nroChavesIndexadas
    fread(&indice->nroChavesIndexadas, sizeof(int), 1, fp);

    // RRNdoNo
    fread(&indice->RRNdoNo, sizeof(int), 1, fp);

    for (int i = 0; i < NUM_CHAVES_NO; i++)
    {
        // Array de ponteiros das subárvores de um dado nó
        fread(&indice->P[i], sizeof(int), 1, fp);
        // Array de chaves localizadas em um dado nó
        fread(&indice->C[i], sizeof(int), 1, fp);
        // Array dos byteoffset dos arquivos bin (veiculo ou linha) correspondentes às chaves
        fread(&indice->Pr[i], sizeof(int64), 1, fp);
    }

    // P[4]
    fread(&indice->P[4], sizeof(int), 1, fp);

    return TRUE;
}

boolean escreverBinCabIndex(FILE *fp, CABECALHOI *cabecalho)
{
    if(cabecalho == NULL)
    {
        return FALSE;
    }

    fseek(fp, 0, SEEK_SET);

    // Status
    fwrite(&cabecalho->status, sizeof(char), 1, fp);

    // noRaiz
    fwrite(&cabecalho->noRaiz, sizeof(int), 1, fp);

    // RRNproxNo
    fwrite(&cabecalho->RRNproxNo, sizeof(int), 1, fp);

    // Inserir lixo no restante do registro, completando os 77 bytes
    char lixo[68];
    preenchendoLixo(0, 68, lixo);

    fwrite(lixo, sizeof(char), 68, fp);
   
    return TRUE;
}

/**
 * @brief 
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 */
boolean escreverBINIndex(FILE *fp, CABECALHOI *cabecalho, INDEX *indice)
{
    if (!fp || !cabecalho || !indice)
    {
        //printf("Deu ruim.\n");
        return FALSE;
    }

    // Posicionar o ponteiro do arquivo binário na posição do RRN do próximo nó
    fseek(fp, (cabecalho->RRNproxNo + 1) * TAMANHO_NO, SEEK_SET);

    // Inserção do nó no arquivo binário

    // Folha
    fwrite(&indice->folha, sizeof(char), 1, fp);
    
    // nroChavesIndexadas
    fwrite(&indice->nroChavesIndexadas, sizeof(int), 1, fp);

    // RRNdoNo
    fwrite(&indice->RRNdoNo, sizeof(int), 1, fp);

    for (int i = 0; i < NUM_CHAVES_NO; i++)
    {
        // Array de ponteiros das subárvores de um dado nó
        fwrite(&indice->P[i], sizeof(int), 1, fp);
        // Array de chaves localizadas em um dado nó
        fwrite(&indice->C[i], sizeof(int), 1, fp);
        // Array de ponteiros 
        fwrite(&indice->Pr[i], sizeof(int64), 1, fp);
    }

    // P[4]
    fwrite(&indice->P[4], sizeof(int), 1, fp);
    
    return TRUE;
}

/**
 * @brief Inicializa um nó da árvore B
 * 
 * @param indice 
 * @param RRN 
 * @param folha 
 */
boolean inicializarNovaPagina(INDEX *indice, int RRN, boolean folha)
{
    if(indice == NULL)
        return FALSE;
    
    // indica se o nó é um nó folha ou não, representado por uma string de 1
    // byte (0 indica que o nó não é folha e 1 indica que o nó é folha);
    if (folha == 1)
        indice->folha = '1';
    else
        indice->folha = '0';

    indice->RRNdoNo = RRN;
    indice->nroChavesIndexadas = 0;

    for (int i = 0; i < NUM_CHAVES_NO; i++)
    {
        indice->C[i] = -1;
        indice->Pr[i] = -1;
    }

    for (int i = 0; i < 5; i++)
    {
        indice->P[i] = -1;
    }

    return TRUE;
}

/**
 * @brief 
 * 
 * @param index 
 * @return boolean 
 */
boolean inicializarIndex(INDEX *index)
{
    if(index == NULL)
        return FALSE;
    
    index->folha = -1;
    index->nroChavesIndexadas = 0;
    index->RRNdoNo = 0;

    for(int i=0; i<4; i++)
    {
        index->P[i] = -1;
        index->C[i] = -1;
        index->Pr[i] = -1;
    }
    index->P[4] = -1;

    return TRUE;
}

/**
 * @brief 
 * 
 * @param cabecalho 
 * @return boolean 
 */
boolean inicializarCabecalhoIndex(CABECALHOI *cabecalho)
{
    if(cabecalho == NULL)
        return FALSE;

    cabecalho->status = '0';
    cabecalho->noRaiz = -1;
    cabecalho->RRNproxNo = 0;
    
    return TRUE;
}