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
#include "../utils/func/sort.h"

/**
 * @brief Algoritmo de inserção de novas chaves no arquivo de indices
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 * @param chave 
 * @param enderecoBin 
 */
void insereChave(FILE *fp, CABECALHOI *cabecalho, INDEX *indice, int chave, int64 enderecoBin)
{
    // Primeiro caso: Árvore vazia
    // Checar se o noRaiz da struct de cabeçalho está com valor -1
    if (cabecalho->noRaiz == -1)
    {
        // Criar um nó página, inserir no arquivo binário de indice, colocar o RRN no cabecalho->noRaiz
        // Inserir a chave ordenadamente na primeira página criado
        inicializarNovaPagina(indice, cabecalho->noRaiz, 1);

        indice->C1 = chave;
        indice->Pr1 = enderecoBin;
        indice->nroChavesIndexadas++;

        // Escrever a nova página no arquivo
        escreverBINIndex(fp, cabecalho, indice);

        cabecalho->noRaiz = 0; // TAMANHO_PAGINA*1
        cabecalho->RRNproxNo = 1;
    }

    // Inicia uma pesquisa que desce até o nível dos nós folhas
    int rrnAtual = cabecalho->noRaiz;
    int isFolha = 0;    // controle (para quando acha folha)
    int ponteiroAnterior = 0;

    // Posiciona o ponteiro para a página correspondente ao rrn
    lerBINIndice(fp, indice, rrnAtual);
    if(indice->folha == '1')
        isFolha = true;
        
    while(isFolha != 1)
    {
        // Achar qual será a página que deverá ir
        if(chave < indice->C1)
        {
            ponteiroAnterior = ((rrnAtual+1)*TAMANHO_NO) + 9;
            rrnAtual = indice->P1;
        }
        else if(chave > indice->C1 && chave < indice->C2)
        {
            ponteiroAnterior = ((rrnAtual+1)*TAMANHO_NO) + 25;
            rrnAtual = indice->P2;
        }
        else if(chave < indice->C3)
        {
            ponteiroAnterior = ((rrnAtual+1)*TAMANHO_NO) + 41;
            rrnAtual = indice->P3;
        }
        else if(chave > indice->C3 && chave < indice->C4)
        {
            ponteiroAnterior = ((rrnAtual+1)*TAMANHO_NO) + 57;
            rrnAtual = indice->P4;
        }
        else
        {
            ponteiroAnterior = ((rrnAtual+1)*TAMANHO_NO) + 73;
            rrnAtual = indice->P5;
        }

        lerBINIndice(fp, indice, rrnAtual);
        if(indice->folha == '1')
            isFolha = TRUE;
    }
    
    // Verificar se a página do rrnAtual está cheia
    if(indice->nroChavesIndexadas == NUM_CHAVES_NO)
    {
        // Fazer o split
        
        // Se for o nó raiz, o nó promovido do split será o novo nó raiz
        if(rrnAtual == cabecalho->noRaiz)
            divideNo(fp, cabecalho, indice, chave, ponteiroAnterior, TRUE);
        else
            divideNo(fp, cabecalho, indice, chave, ponteiroAnterior, FALSE);
    } 
    else
    {
        // Inserir ordenado

    }
}

/**
 * @brief 
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 * @param novaChave nova chave a ser inserida
 */
void divideNo(FILE *fp, CABECALHOI *cabecalho, INDEX *indice, int novaChave, int ponteiroAnterior, boolean raiz)
{

    // a e h k (g)
    // [a e] (g) [h k]

    // Se o nó a ser feito split for o nó raiz, ao final do split fazer com que cabecalho->noRaiz aponte para o INDEX paginaPromocao
    INDEX novaPagina;
    INDEX paginaPromocao; // equivalente ao NewPage  

    // Distribuir as chaves mais uniformemente possível
    int chaves[5];
    chaves[0] = indice->C1;
    chaves[1] = indice->C2;
    chaves[2] = indice->C3;
    chaves[3] = indice->C4;
    chaves[4] = novaChave;
    insertionSort(&chaves, 5);

    // Agora, o elemento do meio será aquele que será promovido

    inicializarNovaPagina(paginaPromocao); // pega o elemento do meio
    paginaPromocao.nroChavesIndexadas = 1;
    paginaPromocao.folha = '0';
    paginaPromocao.RRNdoNo = cabecalho->RRNproxNo;
    cabecalho->RRNproxNo++;
    paginaPromocao.C1 = chaves[2];
    
    

}

/**
 * @brief 
 * 
 * @param fp 
 * @param indice 
 * @param novaChave 
 */
void inserirChaveOrdenada(FILE *fp, INDEX *indice, int novaChave, int64 enderecoBin)
{
    // Se fosse vetor:
    /*for(int i=0; i<4; i++)
    {
        if(indice->C[i] == -1)
        {
            indice->C[i] = novaChave;
            indice->Pr[i] = enderecoBin;
        }
    }
    ordenarChaves();*/

    if(novaChave < indice->C1 || indice->C1 == -1)
    {
        // novaChave será o novo C1
        int aux1 = indice->C1;
        int64 aux2 = indice->Pr1;
        indice->C1 = novaChave;
        indice->Pr1 = enderecoBin;

        // Mover todos uma posição para frente
        int aux3 = indice->C2;
        int64 aux4 = indice->Pr2;

        indice->C2 = aux1;
        indice->Pr2 = aux2;

        aux5 = indice->C3;
        aux6 = indice->Pr3

        indice->C3 = aux3;
        indice->Pr3 = aux4

        indice->C4 = aux5;
        indice->Pr4 = aux6;
    }
    else if(novaChave < indice->C2 || indice->C2 == -1)
    {
        // novaChave será o novo C2
        int aux1 = indice->C2;
        int64 aux2 = indice->Pr2;
        indice->C2 = novaChave;
        indice->Pr2 = enderecoBin;

        // Mover todos uma posição para frente
        int aux3 = indice->C3;
        int64 aux4 = indice->Pr3;

        indice->C3 = aux1;
        indice->Pr3 = aux2;

        indice->C3 = aux3;
        indice->Pr3 = aux4
    }
    else if(novaChave < indice->C3 || indice->C3 == -1)
    {
        // novaChave será o novo C2
        int aux1 = indice->C3;
        int64 aux2 = indice->Pr3;
        indice->C3 = novaChave;
        indice->Pr3 = enderecoBin;

        // Mover todos uma posição para frente
        int aux3 = indice->C4;
        int64 aux4 = indice->Pr4;

        indice->C4 = aux1;
        indice->Pr4 = aux2;
    }
    else
    {
        indice->C4 = novaChave;
        indice->Pr4 = enderecoBin;
    }
}

/**
 * @brief Lê a página de índice do RRN específicado e insere seus dados temporariamente na struct INDEX
 * 
 * @param fp arquivo Binário de índices
 * @param indice struct de páginas do indice
 * @param RRN RRN da página a ser lida
 */
boolean lerBINIndice(FILE *fp, INDEX *indice, int RRN)
{
    if (!fp || !cabecalho || !indice)
        return FALSE;
    
    // posiciona o ponteiro do arquivo logo depois do fim do cabeçalho
    fseek(fp, (RRN*TAMANHO_NO)+TAMANHO_NO);

    // Folha
    fread(&indice->folha, sizeof(char), 1, bin);

    // nroChavesIndexadas
    fread(&indice->nroChavesIndexadas, sizeof(int), 1, bin);

    // RRNdoNo
    fread(&indice->RRNdoNo, sizeof(int), 1, bin);

    // P1
    fread(&indice->P1, sizeof(int), 1, bin);
    // C1
    fread(&indice->C1, sizeof(int), 1, bin);
    // Pr1
    fread(&indice->Pr1, sizeof(int64), 1, bin);

    // P2
    fread(&indice->P2, sizeof(int), 1, bin);
    // C2
    fread(&indice->C2, sizeof(int), 1, bin);
    // Pr2
    fread(&indice->Pr2, sizeof(int64), 1, bin);

    // P3
    fread(&indice->P3, sizeof(int), 1, bin);
    // C3
    fread(&indice->C3, sizeof(int), 1, bin);
    // Pr3
    fread(&indice->Pr3, sizeof(int64), 1, bin);

    // P4
    fread(&indice->P4, sizeof(int), 1, bin);
    // C4
    fread(&indice->C4, sizeof(int), 1, bin);
    // Pr4
    fread(&indice->Pr4, sizeof(int64), 1, bin);

    // P5
    fread(&indice->P5, sizeof(int), 1, bin);
    
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
        return FALSE;

    // Posicionar o ponteiro do arquivo binário na posição do RRN do próximo nó
    fseek(fp, (cabecalho->RRNproxNo+1)*TAMANHO_NO);

    // Inserção do nó no arquivo binário

    // Folha
    fwrite(&indice->folha, sizeof(char), 1, bin);

    // nroChavesIndexadas
    fwrite(&indice->nroChavesIndexadas, sizeof(int), 1, bin);

    // RRNdoNo
    fwrite(&indice->RRNdoNo, sizeof(int), 1, bin);

    // P1
    fwrite(&indice->P1, sizeof(int), 1, bin);
    // C1
    fwrite(&indice->C1, sizeof(int), 1, bin);
    // Pr1
    fwrite(&indice->Pr1, sizeof(int64), 1, bin);

    // P2
    fwrite(&indice->P2, sizeof(int), 1, bin);
    // C2
    fwrite(&indice->C2, sizeof(int), 1, bin);
    // Pr2
    fwrite(&indice->Pr2, sizeof(int64), 1, bin);

    // P3
    fwrite(&indice->P3, sizeof(int), 1, bin);
    // C3
    fwrite(&indice->C3, sizeof(int), 1, bin);
    // Pr3
    fwrite(&indice->Pr3, sizeof(int64), 1, bin);

    // P4
    fwrite(&indice->P4, sizeof(int), 1, bin);
    // C4
    fwrite(&indice->C4, sizeof(int), 1, bin);
    // Pr4
    fwrite(&indice->Pr4, sizeof(int64), 1, bin);

    // P5
    fwrite(&indice->P5, sizeof(int), 1, bin);

    return TRUE;
}

/**
 * @brief Inicializa os valores da struct auxiliar de página de índice para a criação e inserção de uma nova página no arquivo binário
 * 
 * @param indice 
 * @param RRN 
 * @param folha 
 */
void inicializarNovaPagina(INDEX *indice, int RRN, int folha)
{
    // indica se o nó é um nó folha ou não, representado por uma string de 1
    // byte (0 indica que o nó não é folha e 1 indica que o nó é folha); 
    if(folha == 1)
        indice->folha = '1';
    else   
        indice->folha = '0';

    indice->RRNdoNo = RRN;
    indice->nroChavesIndexadas = 0;

    indice->P1 = -1;
    indice->C1 = -1;
    indice->Pr1 = -1;
    indice->P2 = -1;
    indice->C2 = -1;
    indice->Pr2 = -1;
    indice->P3 = -1;
    indice->C3 = -1;
    indice->Pr3 = -1;
    indice->P4 = -1;
    indice->C4 = -1;
    indice->Pr4 = -1;
    indice->P5 = -1;
}