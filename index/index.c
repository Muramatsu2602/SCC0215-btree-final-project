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
#include "../utils/auxFunc/indexManager.h"

void insercao(FILE *fp, ABECALHOI *cabecalho, INDEX *indice, int chave, int64 enderecoBin)
{
    // Primeiro caso: Árvore vazia
    // Checar se o noRaiz da struct de cabeçalho está com valor -1
    if (cabecalho->noRaiz == -1)
    {
        // Criar um nó página, inserir no arquivo binário de indice, colocar o RRN no cabecalho->noRaiz
        // Inserir a chave ordenadamente na primeira página criado
        cabecalho->noRaiz = 0;
        inicializarNovaPagina(indice, cabecalho->noRaiz, 1);

        indice->C1 = chave;
        indice->Pr1 = enderecoBin;
        indice->nroChavesIndexadas++;

        // Escrever a nova página no arquivo
        escreverBINIndex(fp, cabecalho, indice);

        cabecalho->RRNproxNo = cabecalho->noRaiz + 1;
    }
    else
    {
        // Inicializar o indice como a raiz da árvore
        lerBINIndice(fp, indice, cabecalho, cabecalho->RRNnoRaiz);
    }
}

void inicializarNovaPagina(INDEX *indice, int RRN, boolean folha)
{
    // indica se o nó é um nó folha ou não, representado por uma string de 1
    // byte (0 indica que o nó não é folha e 1 indica que o nó é folha);
    if (folha == 1)
        indice->folha = '1';
    else
        indice->folha = '0';

    indice->RRNdoNo = RRN;
    indice->nroChavesIndexadas = 0;

    for (int i = 0; i < 4; i++)
    {
        indice->C[i] = -1;
        indice->Pr[i] = -1;
    }

    for (int i = 0; i < 5; i++)
    {
        indice->P[i] = -1;
    }
}

void insereChave(FILE *fp, CABECALHOI *cabecalho, INDEX *indice, int chave, int64 enderecoBin)
{
    INDEX raiz;
    // Primeiro caso: Árvore vazia
    // Checar se o noRaiz da struct de cabeçalho está com valor -1
    if (cabecalho->RRNnoRaiz == -1)
    {
        // Criar um nó página, inserir no arquivo binário de indice, colocar o RRN no cabecalho->noRaiz
        // Inserir a chave ordenadamente na primeira página criado
        cabecalho->noRaiz = 0;
        inicializarNovaPagina(raiz, cabecalho->noRaiz, 1);

        raiz->C[0] = chave;
        raiz->Pr[0] = enderecoBin;
        raiz->nroChavesIndexadas++;

        // Escrever a nova página no arquivo
        escreverBINIndex(fp, cabecalho, raiz);

        cabecalho->RRNproxNo = cabecalho->noRaiz + 1;
    }
    // Se a árvore existe
    else
    {
        lerBINIndice(fp, raiz, cabecalho, cabecalho->RRNnoRaiz);

        // se o nó está cheio, então a árvore B cresce em altura
        if (raiz->nroChavesIndexadas == NUM_CHAVES_NO)
        {
            // Criar a nova raiz
            INDEX novaRaiz;
            inicializarNovaPagina(novaRaiz, cabecalho->RRNproxNo, FALSE);
            escreverBINIndex(fp, cabecalho, novaRaiz);
            cabecalho->RRNnoRaiz = cabecalho->RRNproxNo;
            cabecalho->RRNproxNo++;

            // Página a esquerda da novaRaiz será a raiz original
            novaRaiz->P[0] = raiz.RRNdoNo;

            int chaves[5];
            int64 enderecosBin[5];
            for (int i = 0, int j = 0; i < 5; i++)
            {
                if (chave < raiz->C[j])
                {
                    enderecoBin[i] = enderecoBin;
                    chaves[i] = chave;
                    chave = INT_MAX;
                }
                else
                {
                    enderecoBin[i] = raiz->Pr[j];
                    chaves[i] = raiz->c[j];
                    j++;
                }
            }

            // Dar split na raiz antiga e promover o valor central
            divideNo(fp, cabecalho, 0, raiz, novaRaiz, chaves, enderecosBin);

            // TODO: Verificar a linha intermediaria que aparece no geek for geeks

            cabecalho->RRNnoRaiz = novaRaiz.RRNdoNo;
        }
        else
        {
            inserirChave(fp, cabecalho, raiz, chave, enderecoBin);
        }
    }

    // Inicia uma pesquisa que desce até o nível dos nós folhas
    int rrnAtual = cabecalho->noRaiz;
    int isFolha = 0; // controle (para quando acha folha)
    int ponteiroAnterior = 0;

    // Posiciona o ponteiro para a página correspondente ao rrn
    lerBINIndice(fp, indice, rrnAtual);
    if (indice->folha == '1')
        isFolha = TRUE;

    while (isFolha != 1)
    {
        // Inicia uma pesquisa que desce até o nível dos nós folhas
        int rrnAtual = cabecalho->noRaiz;
        boolean isFolha = 0; // controle (para quando acha folha)
        int ponteiroAnterior = -1;

        // Posiciona o ponteiro para a página correspondente ao rrn
        lerBINIndice(fp, indice, rrnAtual);
        if (indice->folha == '1')
            isFolha = true;

        while (isFolha != 1)
        {
            int aux = 9; // Byte inicial para onde o P0 aponta.
            boolean flag = 0;

            // Achar qual será a página que deverá ir
            for (int i = 0; i < 4 && flag == 0; i += 2)
            {
                if (chave < indice->C[i])
                {
                    ponteiroAnterior = ((rrnAtual + 1) * TAMANHO_NO) + aux;
                    rrnAtual = indice->P[i];
                    flag == 1;
                }
                else if (chave > indice->C[i] && chave < indice->C[i + 1])
                {
                    ponteiroAnterior = ((rrnAtual + 1) * TAMANHO_NO) + aux + 4;
                    rrnAtual = indice->P[i + 1];
                    flag == 1;
                }
                aux += 8;
            }

            // Se a chave for maior do que a última chave presente na página atual
            if (flag == 0)
            {
                ponteiroAnterior = ((rrnAtual + 1) * TAMANHO_NO) + aux;
                rrnAtual = indice->P[4];
            }

            // ler
            lerBINIndice(fp, indice, rrnAtual);
            if (indice->folha == '1')
                isFolha = TRUE;
        }

        // Depois que encontrou o nó folha, tentará fazer a inserção
        // Verificar se a página do rrnAtual está cheia
        if (indice->nroChavesIndexadas == NUM_CHAVES_NO)
        {
            // A página está cheia, portanto fazer o split

            // Se for o nó raiz, o nó promovido do split será o novo nó raiz
            if (rrnAtual == cabecalho->noRaiz)
                divideNo(fp, cabecalho, indice, chave, ponteiroAnterior, TRUE);
            else
                divideNo(fp, cabecalho, indice, chave, ponteiroAnterior, FALSE);
        }
        else
        {
            // Inserir ordenado
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
    // i guarda a posição da última chave inserida em raiz
    int i = raiz->nroChavesIndexadas - 1;

    if (raiz->folha == 1)
    {
        // Encontrar a posição em que a chave será inserida
        while (i >= 0 && raiz->C[i] > chave)
        {
            raiz->C[i+1] = raiz->C[i];
            raiz->Pr[i+1] = raiz->Pr[i];
            i--;
        }

        // Inserir a nova chave na posição encontrada
        raiz->C[i+1] = chave;
        raiz->Pr[i+1] = enderecoBin;
        raiz->nroChavesIndexadas++;
    }
    else
    {
        // Se não for folha

        // Encontrar o RRN da página que irá receber a chave
        while(i >= 0 && raiz->C[i] > chave)
            i--;

        // Colocar em aux a página do RRN encontrado
        INDEX aux;
        aux = lerBINIndice(fp, cabecalho, raiz->P[(i+1)]);

        int chaves[5];
        int64 enderecosBin[5];
        for (int i = 0, int j = 0; i < 5; i++)
        {
            if (chave < aux->C[j])
            {
                enderecoBin[i] = enderecoBin;
                chaves[i] = chave;
                chave = INT_MAX;
            }
            else
            {
                enderecoBin[i] = aux->Pr[j];
                chaves[i] = aux->c[j];
                j++;
            }
        }

        // Ver se a página está cheia
        if(aux.nroChavesIndexadas ==  NUM_CHAVES_NO)
        {
            // Se está cheio, então deve fazer o split
            divideNo(fp, cabecalho, (i+1), aux, raiz, chaves, enderecosBin);
        }
        inserirChave(fp, cabecalho, aux, chave, enderecoBin);
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
void divideNo(FILE *fp, CABECALHOI *cabecalho, int i, INDEX *raiz, INDEX *novaRaiz, int chaves[5], int enderecosBin[5])
{
    // Inicializando uma nova página
    // Atualizando as informações no cabeçalho
    INDEX novaPagina;
    inicializarNovaPagina(novaPagina, cabecalho->RRNproxNo, FALSE);
    escreverBINIndex(fp, cabecalho, novaPagina);
    cabecalho->RRNnoRaiz = cabecalho->RRNproxNo;
    cabecalho->RRNproxNo++;

    novaPagina.nroChavesIndexadas = 2;

    // P[0] C[0] P[1] C[1] P[2] C[2] P[3] C[3] P[4]

    // Copiar as 2 últimas chaves para o novaPagina
    // Copiar os 2 últimos ponteiros das subárvores
    novaPagina->P[0] = raiz->P[2];
    novaPagina->C[0] = chaves[3];
    novaPagina->Pr[0] = enderecosBin[3];
    novaPagina->P[1] = raiz->P[3];
    novaPagina->C[1] = chaves[4];
    novaPagina->Pr[1] = enderecosBin[4];
    novaPagina->P[2] = raiz->P[4];

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
 * @brief essa função é convocada quando a inserção se dá em uma página não cheia, ou seja,
 * sem a necessidade de um SPLIT
 * 
 * @param fp 
 * @param indice 
 * @param novaChave 
 */
void inserirChaveOrdenada(FILE *fp, INDEX *indice, int novaChave, int64 enderecoBin)
{
    // Se fosse vetor:
    for (int i = 0; i < 4; i++)
    {
        // so podemos inserir em uma folha
        if (indice->C[i] == -1)
        {
            indice->C[i] = novaChave;
            indice->Pr[i] = enderecoBin;
        }
    }
    ordenarChaves();
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
    fseek(fp, (RRN * TAMANHO_NO) + TAMANHO_NO, SEEK_SET);

    // Folha
    fread(&indice->folha, sizeof(char), 1, fp);

    // nroChavesIndexadas
    fread(&indice->nroChavesIndexadas, sizeof(int), 1, fp);

    // RRNdoNo
    fread(&indice->RRNdoNo, sizeof(int), 1, fp);

    // P1
    fread(&indice->P1, sizeof(int), 1, fp);
    // C1
    fread(&indice->C1, sizeof(int), 1, fp);
    // Pr1
    fread(&indice->Pr1, sizeof(int64), 1, fp);

    // P2
    fread(&indice->P2, sizeof(int), 1, fp);
    // C2
    fread(&indice->C2, sizeof(int), 1, fp);
    // Pr2
    fread(&indice->Pr2, sizeof(int64), 1, fp);

    // P3
    fread(&indice->P3, sizeof(int), 1, fp);
    // C3
    fread(&indice->C3, sizeof(int), 1, fp);
    // Pr3
    fread(&indice->Pr3, sizeof(int64), 1, fp);

    // P4
    fread(&indice->P4, sizeof(int), 1, fp);
    // C4
    fread(&indice->C4, sizeof(int), 1, fp);
    // Pr4
    fread(&indice->Pr4, sizeof(int64), 1, fp);

    // P5
    fread(&indice->P5, sizeof(int), 1, fp);

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
    fseek(fp, (cabecalho->RRNproxNo + 1) * TAMANHO_NO, SEEK_SET);

    // Inserção do nó no arquivo binário

    // Folha
    fwrite(&indice->folha, sizeof(char), 1, fp);

    // nroChavesIndexadas
    fwrite(&indice->nroChavesIndexadas, sizeof(int), 1, fp);

    // RRNdoNo
    fwrite(&indice->RRNdoNo, sizeof(int), 1, fp);

    // P1
    fwrite(&indice->P1, sizeof(int), 1, fp);
    // C1
    fwrite(&indice->C1, sizeof(int), 1, fp);
    // Pr1
    fwrite(&indice->Pr1, sizeof(int64), 1, fp);

    // P2
    fwrite(&indice->P2, sizeof(int), 1, fp);
    // C2
    fwrite(&indice->C2, sizeof(int), 1, fp);
    // Pr2
    fwrite(&indice->Pr2, sizeof(int64), 1, fp);

    // P3
    fwrite(&indice->P3, sizeof(int), 1, fp);
    // C3
    fwrite(&indice->C3, sizeof(int), 1, fp);
    // Pr3
    fwrite(&indice->Pr3, sizeof(int64), 1, fp);

    // P4
    fwrite(&indice->P4, sizeof(int), 1, fp);
    // C4
    fwrite(&indice->C4, sizeof(int), 1, fp);
    // Pr4
    fwrite(&indice->Pr4, sizeof(int64), 1, fp);

    // P5
    fwrite(&indice->P5, sizeof(int), 1, fp);

    return TRUE;
}

/**
 * @brief Inicializa os valores da struct auxiliar de página de índice para a criação e inserção de uma nova página no arquivo binário
 * 
 * @param indice 
 * @param RRN 
 * @param folha booleano "é Folha".
 */
void inicializarNovaPagina(INDEX *indice, int RRN, boolean folha)
{
    // indica se o nó é um nó folha ou não, representado por uma string de 1
    // byte (0 indica que o nó não é folha e 1 indica que o nó é folha);
    if (folha == 1)
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