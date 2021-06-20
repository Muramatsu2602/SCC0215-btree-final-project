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

/**
 * @brief Algoritmo de inserção de novas chaves no arquivo de indices
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 * @param rrnAtual 
 * @param chave 
 * @param enderecoBin 
 * @param rrnDireitoChavePromovida 
 * @param chavePromovida 
 * @param enderecoBinPromovida 
 * @param rrnPromovido 
 * @return int  PROMOTION  
    Parâmetros de retorno
        0 -> promotion: quando uma inserção é feita e a chave é promovida
        1 -> no promotion: quando a inserção é feita e nenhuma chave é promovida
        2 -> error: não irá acontecer segundo as especificações do projeto
 */

int insereChave(FILE *fp, CABECALHOI *cabecalho, INDEX *indice, int rrnAtual, int chave, int64 enderecoBin, int rrnDireitoChavePromovida, int chavePromovida, int64 enderecoBinPromovida, int rrnPromovido)
{
    INDEX PAGE;
    INDEX NEWPAGE;
    int pos = -1; // Posição na página na qual a chave ocorre ou deveria ocorrer

    int p_b_key; //  chave promovida do nível inferir para ser inserida em PAGE
    int p_b_rrn; //  rrn promovido do nível anterior para ser inserido em PAGE

    if ()

        // se o rrnAtual é um nó folha
        if (rrnAtual == -1)
        {
            chavePromovida = chave;
            rrnDireitoChavePromovida = -1;
            return 0;
        }
        else
        {
            // Achar qual será a página que deverá ir
            boolean flag = 0;
            int rrnAtual2 = rrnAtual;
            int aux = 9;

            for (int i = 0; i < 4 && flag == 0; i += 2)
            {
                if (chave < indice->C[i])
                {
                    ponteiroAnterior = ((rrnAtual + 1) * TAMANHO_NO) + aux;
                    rrnAtual2 = indice->P[i];
                    flag == 1;
                }
                else if (chave > indice->C[i] && chave < indice->C[i + 1])
                {
                    ponteiroAnterior = ((rrnAtual + 1) * TAMANHO_NO) + aux + 4;
                    rrnAtual2 = indice->P[i + 1];
                    flag == 1;
                }
                aux += 8;
            }

            // Se a chave for maior do que a última chave presente na página atual
            if (flag == 0)
            {
                ponteiroAnterior = ((rrnAtual + 1) * TAMANHO_NO) + aux;
                rrnAtual2 = indice->P[4];
            }
            
            // a chave de busca não foi encontrada, portanto
            // procura a chave de busca no nó filho
            int returnValue = insereChave(fp, cabecalho, indice, rrnAtual2, chave, enderecoBin,);

            // NO PROMOTION
            if(returnValue == 1)
        }
}

void insereChave(FILE *fp, CABECALHOI *cabecalho, INDEX *indice, int chave, int64 enderecoBin)
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

    // Inicia uma pesquisa que desce até o nível dos nós folhas
    int rrnAtual = cabecalho->noRaiz;
    int isFolha = 0;    // controle (para quando acha folha)
    int ponteiroAnterior = 0;

    // Posiciona o ponteiro para a página correspondente ao rrn
    lerBINIndice(fp, indice, rrnAtual);
    if(indice->folha == '1')
        isFolha = TRUE;
        
    while(isFolha != 1)
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
 * @brief realiza o procedimento de SPLIT, criando duas novas paginas e 
 * promovendo um dos nos da pagina original
 * 
 * @param fp 
 * @param cabecalho 
 * @param indice 
 * @param novaChave nova chave a ser inserida
 */
void divideNo(FILE *fp, CABECALHOI *cabecalho, INDEX *indice, int novaChave, int ponteiroAnterior, boolean raiz)
{
    // Primeiro, verificar se o ponteiro anterior

    // a e h k (g)
    // [a e] (g) [h k]

    // Se o nó a ser feito split for o nó raiz, ao final do split fazer com que cabecalho->noRaiz aponte para o INDEX paginaPromocao
    INDEX novaPagina; // Nova página que receberá o que está a direita da página a ser promovida
    inicializarNovaPagina(novaPagina, cabecalho->RRNproxNo, TRUE);
    cabecalho->RRNproxNo++;

    INDEX paginaPromocao; // Nova página que conterá a chave promovida
    inicializarNovaPagina(paginaPromocao, cabecalho->RRNproxNo, FALSE);
    cabecalho->RRNproxNo++;

    // Distribuir as chaves mais uniformemente possível ordenando o array chaves
    int chaves[5];
    for (int i = 0, int j = 0; i < 5; i++)
    {
        if (indice->C[j] > novaChave)
        {
            chaves[i] = novaChave;
        }
        else
        {
            chaves[i] = indice->C[j];
            j++;
        }
    }

    // Agora, o elemento do meio será aquele que será promovido
    paginaPromocao.nroChavesIndexadas = 1;
    paginaPromocao.C[0] = chaves[2];
    paginaPromocao.P[0] = indice->RRNdoNo;
    paginaPromocao.P[1] = novaPagina.RRNdoNo;

    // As chaves a esquerda da chave promovida continuarão na página original

    // As chaves a direita da chave promovida ficarão na nova página criada
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
    fseek(fp, (RRN*TAMANHO_NO)+TAMANHO_NO, SEEK_SET);

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
    fseek(fp, (cabecalho->RRNproxNo+1)*TAMANHO_NO, SEEK_SET);

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