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
 * @brief procedimento Driver da inserção. Rotina inicializadora e de tratamento da raiz
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 * @param chave 
 * @param enderecoBin 
 */
void inserirIndex(FILE *fp, CABECALHOI *cabecalho, INDEX *indice, int chave, int64 enderecoBin)
{
    // Na primeira vez, a árvore estará vazia, então criar o no raiz
    if (cabecalho->noRaiz == -1)
    {
        // Criar um nó página, inserir no arquivo binário de indice, colocar o RRN no cabecalho->noRaiz
        // Inserir a chave ordenadamente na primeira página criado
        cabecalho->noRaiz = 0;

        indice->C[0] = chave;
        indice->Pr[0] = enderecoBin;
        indice->nroChavesIndexadas++;
        indice->folha = '1';

        // Escrever a nova página no arquivo
        escreverBINIndex(fp, cabecalho->RRNproxNo, indice);

        cabecalho->RRNproxNo = cabecalho->noRaiz + 1;
        return;
    }
    else
    {
        // Colocar em indice o nó raiz
        lerBINIndice(fp, indice, cabecalho->noRaiz);

        int rrnPromoRChild = 0;
        int promoKey = 0;
        int64 enderecoBinPromoKey = 0;

        if (inserirChave(fp, cabecalho, indice, chave, enderecoBin, &rrnPromoRChild, &promoKey, &enderecoBinPromoKey) == PROMOTION)
        {
            // Criar uma nova raiz
            // create a new root page with key := PROMO_KEY
            INDEX novaRaiz;
            inicializarNovaPagina(&novaRaiz, cabecalho->RRNproxNo, FALSE);
            cabecalho->RRNproxNo++;

            novaRaiz.C[0] = promoKey;
            novaRaiz.Pr[0] = enderecoBinPromoKey;
            novaRaiz.nroChavesIndexadas += 1;

            // left child = root
            novaRaiz.P[0] = cabecalho->noRaiz;

            // right child = rrnPromoRChild
            novaRaiz.P[1] = rrnPromoRChild;

            identificaFolha(&novaRaiz);
            escreverBINIndex(fp, novaRaiz.RRNdoNo, &novaRaiz);

            // Set ROOT to RRN of new root page
            cabecalho->noRaiz = novaRaiz.RRNdoNo;
        }
    }
}

/**
 * @brief insere uma chave em um nó, utilizando ou não o SPLIT
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 * @param chave 
 * @param enderecoBin 
 * @param rrnPromoRChild  retorna o ponteiro para o filho direito de PROMO_KEY
 * @param promoKey 
 * @param enderecoBinPromoKey 
 */
int inserirChave(FILE *fp, CABECALHOI *cabecalho, INDEX *indice, int chave, int64 enderecoBin, int *rrnPromoRChild, int *promoKey, int64 *enderecoBinPromoKey)
{
    // garantindo construção a partir das folhas (bottom)
    if (indice == NULL || indice->RRNdoNo == -1)
    {
        *promoKey = chave;
        *enderecoBinPromoKey = enderecoBin;
        *rrnPromoRChild = -1;
        return PROMOTION;
    }
    // se a página não é folha,  a função é chamada de modo recursivo até encontrar uma KEY ou que chegue a um nó folha
    else
    {
        // O índice já contem o conteudo de CURRENT_RRN

        // i guarda a posição da última chave inserida em raiz
        int i = indice->nroChavesIndexadas - 1;

        // procurando no folha
        while (i >= 0 && indice->C[i] > chave)
            i--;

        // Colocar em aux a página do RRN encontrado
        INDEX aux;
        if(lerBINIndice(fp, &aux, indice->P[(i + 1)]) == FALSE)
        {
            inicializarNovaPagina(&aux, -1, FALSE);
        }

        int chavePromovidaNivelInferior = 0; // P_B_KEY
        int64 enderecoBinChavePromovidaNivelInferior = 0;
        int rrnChavePromovidaNivelInferior = 0; // P_B_RRN (filho a direita de P_B_KEY)
        int RETURN_VALUE = inserirChave(fp, cabecalho, &aux, chave, enderecoBin, &rrnChavePromovidaNivelInferior, &chavePromovidaNivelInferior, &enderecoBinChavePromovidaNivelInferior);
        
        if(RETURN_VALUE == NO_PROMOTION)
        {
            return RETURN_VALUE;
        }
        else if(indice->nroChavesIndexadas < 4)
        {
            // Tem espaço em indice para inserir a chave
            // Então faz a inserção e retorna NO_PROMOTION
            i = indice->nroChavesIndexadas - 1;

            while (i >= 0 && indice->C[i] > chavePromovidaNivelInferior)
            {
                indice->C[i + 1] = indice->C[i];
                indice->Pr[i + 1] = indice->Pr[i];
                indice->P[i + 2] = indice->P[i+1];
                i--;
            }

            // Inserir a nova chave na posição encontrada
            indice->C[i + 1] = chavePromovidaNivelInferior;
            indice->Pr[i + 1] = enderecoBinChavePromovidaNivelInferior;
            indice->P[i + 2] = rrnChavePromovidaNivelInferior;
            indice->nroChavesIndexadas += 1;
            identificaFolha(indice);
            atualizarBINIndex(fp, indice);

            return NO_PROMOTION; // inserção sem particionamento
        }
        // inserção com particionamento (split), indicando chave promovida 
        else
        {
            INDEX novaPagina;
            //void divideNo(FILE *fp, int chave, int64 enderecoBinChave, int rrnDirChave, INDEX *pagina, int *promoKey, int64 *enderecoBinPromoKey, int *rrnPromoRChild, INDEX *novaPagina, CABECALHOI *cabecalho)
            // divideNo(ARQ, CABECALGO, PAGINA DO DISCO CORRENTE, CHAVE A SER INSERIDA, RRN DO FILHO A DIREITA DA CHAVE A SER INSERIDA, CHAVE PROMOVIDA, FILHO A DIREITA DA CHAVE PROMOVIDA, NOVA PAGINA DE DISCO CRIADA)
            divideNo(fp, chavePromovidaNivelInferior, enderecoBinChavePromovidaNivelInferior, rrnChavePromovidaNivelInferior, indice, promoKey, enderecoBinPromoKey, rrnPromoRChild, &novaPagina, cabecalho);

            // Write PAGE to file at CURRENT_RRN
            identificaFolha(indice);
            escreverBINIndex(fp, indice->RRNdoNo, indice);

            // Wrtite NEWPAGE to file at RRN PROMO_R_CHILD
            identificaFolha(&novaPagina);
            escreverBINIndex(fp, *rrnPromoRChild, &novaPagina);

            return PROMOTION;
        }
    }

    return -1;
}

/**
 * @brief  Verifica se possui um filho, se possuir não será no folha
 * 
 * @param indice 
 */
void identificaFolha(INDEX *indice)
{
    for (int i = 0; i < 5; i++)
    {
        if (indice->P[i] != -1)
        {
            indice->folha = '0';
            return;
        }
    }
    indice->folha = '1';
    return;
}

/**
 * @brief @brief  promovendo um dos nos da pagina original.  Quando vou promover uma chave,
 * essa chave pode ir para uma página já existente e o <i> serve para controlar em qual posição em C e em P irá referenciar a nova página.  i é índice da última chave presente em novaRaiz
 * 
 * @param fp
 * @param chave  nova chave a ser inserida
 * @param enderecoBinChave 
 * @param rrnDirChave filho a direita da nova chave a ser inserida
 * @param pagina página de disco corrente
 * @param promoKey  chave promovida
 * @param enderecoBinPromoKey 
 * @param rrnPromoRChild filho a direita da chave promovida
 * @param novaPagina nova página de disco
 */
void divideNo(FILE *fp, int chave, int64 enderecoBinChave, int rrnDirChave, INDEX *pagina, int *promoKey, int64 *enderecoBinPromoKey, int *rrnPromoRChild, INDEX *novaPagina, CABECALHOI *cabecalho)
{
    // Vetor de chaves, enderecosBin e ponteiros para a realizão do split
    int chaves[5];
    int64 enderecosBin[5];
    int ponteiros[6];

    // Ordenando as chaves junto com a nova chave a ser inserida
    ordenarChavesDivideNo(chaves, enderecosBin, ponteiros, chave, enderecoBinChave, rrnDirChave, pagina);

    // Allocate and initialize a new page in the B-tree file to hold NEWPAGE
    inicializarNovaPagina(novaPagina, cabecalho->RRNproxNo, FALSE);
    cabecalho->RRNproxNo += 1;

    // Set PROMO_KEY to the value of middle key, which will be promoted after the split
    *promoKey = chaves[2];
    *enderecoBinPromoKey = enderecosBin[2];

    // set PROMO_R_CHILD to RRN of NEWPAGE
    *rrnPromoRChild = novaPagina->RRNdoNo;

    // Copy keys and child pointers preceding PROMO_KEY from working page to PAGE 
    // Ou seja, tudo que está antes da posição 2
    //[]A[]B[]X[]C[]D[]
    int rrnAntigo = pagina->RRNdoNo;
    inicializarNovaPagina(pagina, pagina->RRNdoNo, FALSE);
    pagina->RRNdoNo = rrnAntigo;
    pagina->P[0] = ponteiros[0];
    pagina->P[1] = ponteiros[1];
    pagina->P[2] = ponteiros[2];
    pagina->C[0] = chaves[0];
    pagina->Pr[0] = enderecosBin[0];
    pagina->C[1] = chaves[1];
    pagina->Pr[1] = enderecosBin[1];
    pagina->nroChavesIndexadas = 2;

    // Copy keys and child pointers following PROMO_KEY from working page to NEWPAGE
    // Ou seja, tudo que está depois da posição 2
    novaPagina->P[0] = ponteiros[3];
    novaPagina->P[1] = ponteiros[4];
    novaPagina->P[2] = ponteiros[5];
    novaPagina->C[0] = chaves[3];
    novaPagina->Pr[0] = enderecosBin[3];
    novaPagina->C[1] = chaves[4];
    novaPagina->Pr[1] = enderecosBin[4];
    novaPagina->nroChavesIndexadas = 2;

    return;
}

/**
 * @brief esta função é responsável pela ordenação das chaves no nó
 * em que será realizado o Split juntamente com a nova chav e a ser inserida. 
 * 
 * @param chaves 
 * @param enderecosBin 
 * @param ponteiros 
 * @param chave 
 * @param enderecoBinChave 
 * @param rrnDirChave 
 * @param pagina 
 */
void ordenarChavesDivideNo(int *chaves, int64 *enderecosBin, int *ponteiros, int chave, int enderecoBinChave, int rrnDirChave, INDEX *pagina)
{
    // Inicializar ponteiros com o valor -1
    for(int i=0; i<6; i++)
    {
        ponteiros[i] = -1;
    }

    int j = 0;
    int flag = 0;
    for (int i = 0; i < 5; i++)
    {
        if (chave < pagina->C[j])
        {
            ponteiros[i] = pagina->P[i];
            enderecosBin[i] = enderecoBinChave;
            chaves[i] = chave;
            ponteiros[i+1] = rrnDirChave;
            flag = 1;

            // Passar os ponteiros que estão a direita do ponteiro antigo para uma posição a direita
            for(int k=(i+1); k<5; k++)
            {
                ponteiros[k+1] = pagina->P[k];
            }
            chave = INT_MAX;
        }
        else
        {
            enderecosBin[i] = pagina->Pr[j];
            chaves[i] = pagina->C[j];
            
            // copiando ponteiros se não estes não estão inicializados
            if(ponteiros[i] == -1)
                ponteiros[i] = pagina->P[j];
            j++;
        }
    }
    // posicionando as informações no ultimo slot da pagina
    if(flag == 0)
    {
        enderecosBin[4] = enderecoBinChave;
        chaves[4] = chave;
        ponteiros[5] = rrnDirChave;
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
    if (!fp || !indice || RRN == -1)
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

/**
 * @brief preenche uma struct do tipo CABECALHOI com os dados do arquivo .bin
 * @param fp 
 * @param cabecalho 
 * @return boolean 
 */
boolean lerBinCabIndex(FILE *fp, CABECALHOI *cabecalho)
{
    if(cabecalho == NULL)
        return FALSE;

    fseek(fp, 0, SEEK_SET);

    // Status
    fread(&cabecalho->status, sizeof(char), 1, fp);

    // noRaiz
    fread(&cabecalho->noRaiz, sizeof(int), 1, fp);

    // RRNproxNo
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, fp);

    return TRUE;
}

/**
 * @brief insere em disco o cabecalho da arvore-B
 * 
 * @param fp 
 * @param cabecalho 
 * @return boolean 
 */
boolean escreverBinCabIndex(FILE *fp, CABECALHOI *cabecalho)
{
    if (cabecalho == NULL || fp == NULL)
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
    // 68 = TAMANHO_REGISTRO - (sizeof(cabecalho->status) + sizeof(cabecalho->noRaiz) + sizeof(cabecalho->RRNproxNo) )
    char lixo[68];
    if(!preenchendoLixo(0, 68, lixo))
        return FALSE;

    fwrite(lixo, sizeof(char), 68, fp);

    return TRUE;
}

/**
 * @brief insere uma nova pagina na arvore binaria
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 */
boolean escreverBINIndex(FILE *fp, int RRN, INDEX *indice)
{
    if (!fp || !indice)
    {
        return FALSE;
    }

    // Posicionar o ponteiro do arquivo binário na posição do RRN do próximo nó
    fseek(fp, (RRN + 1) * TAMANHO_NO, SEEK_SET);

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
 * @brief Atualiza a pagina com novas chaves e novos ponteiros.
 *  É chamada sempre que uma nova chave for inserida em alguma das
 *  paginas da arvore b.
 *
 * @param fp 
 * @param indice 
 * @return boolean 
 */
boolean atualizarBINIndex(FILE *fp, INDEX *indice)
{
    if (!fp || !indice)
    {
        return FALSE;
    }

    // Posicionar o ponteiro do arquivo binário na posição do RRN do próximo nó
    fseek(fp, (indice->RRNdoNo + 1) * TAMANHO_NO, SEEK_SET);

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
 * @brief Inicializa um nó da árvore B, colocando valores iniciais em cada uma de
 * suas prioridades
 * 
 * @param indice 
 * @param RRN 
 * @param folha 
 */
boolean inicializarNovaPagina(INDEX *indice, int RRN, boolean folha)
{
    if (indice == NULL)
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
 * @brief inicializa uma struct do tipo INDEX, atribuindo valores NIL,
 * que podem variar de acordo com cada propriedade (0 ou -1)
 * 
 * @param index 
 * @return boolean 
 */
boolean inicializarIndex(INDEX *index)
{
    if (index == NULL)
        return FALSE;

    index->folha = -1;
    index->nroChavesIndexadas = 0;
    index->RRNdoNo = 0;

    for (int i = 0; i < 4; i++)
    {
        index->P[i] = -1;
        index->C[i] = -1;
        index->Pr[i] = -1;
    }
    index->P[4] = -1;

    return TRUE;
}

/**
 * @brief inicializa o cabecalho do index, atribuindo valores NIL,
 * que podem variar dependendo da natureza da propriedade (0, '0' ou -1)
 * 
 * @param cabecalho 
 * @return boolean 
 */
boolean inicializarCabecalhoIndex(CABECALHOI *cabecalho)
{
    if (cabecalho == NULL)
        return FALSE;

    cabecalho->status = '0';
    cabecalho->noRaiz = -1;
    cabecalho->RRNproxNo = 0;

    return TRUE;
}

/**
 * @brief algoritmo de pesquisa em arvore b
 * Começa pela raiz, ou seja rrnAtual = cabecalho->noRaiz
 * @param rrnAtual 
 * @param chave 
 * @param rrnEncontrado 
 * @param posEncontrado 
 * @return boolean 
 */
boolean procuraIndex(FILE *fp, int rrnAtual, int chave, int *rrnEncontrado, int *posEncontrado)
{
    // rrnAtual = -1 caso a chave não tenha sido encontrada
    if(rrnAtual == -1)
        return NOT_FOUND;
        
    // read page RRN into PAGE
    INDEX pagina;
    inicializarNovaPagina(&pagina, rrnAtual, FALSE);
    lerBINIndice(fp, &pagina, rrnAtual);

    // pesquisa a página procurando a chave de busca
    // i guarda a posição da última chave inserida em raiz
    int i = pagina.nroChavesIndexadas - 1;

    // procurando no folha
    while (i >= 0 && pagina.C[i] > chave)
        i--;

    if(chave == pagina.C[i])
    {
        *rrnEncontrado = rrnAtual;
        *posEncontrado = i;
        return FOUND;
    }
    else
    {
        //a chave de busca não foi encontrada, portanto procura a chave de busca no nó filho
        if(lerBINIndice(fp, &pagina, pagina.P[(i + 1)]) == FALSE)
        {
            // Caso não exista nó filho, a busca foi má sucedida, portanto RRNdoNo = -1
            pagina.RRNdoNo = -1;
        }

        return procuraIndex(fp, pagina.RRNdoNo, chave, rrnEncontrado, posEncontrado);
    }  
}