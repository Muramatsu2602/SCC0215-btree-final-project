/**
 * @file merge.c
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @brief funcoes auxiliares para os procedimentos de Ordenação e de Processamento Cosequencial
 * @version 0.1
 * @date 2021-07-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "merge.h"

/**
 * @brief libera os dois campos dinamicos presentes no struct VEICULO e struct LINHA
 * 
 * @param campo1 
 * @param campo2 
 */
void freeCamposDinamicos(char **campo1, char **campo2)
{
    if (*campo1 != NULL)
    {
        free(*campo1);
        *campo1 = NULL;
    }
    else if (*campo2 != NULL)
    {
        free(*campo2);
        *campo2 = NULL;
    }
    return;
}

/**
 * @brief Função de comparação utilizada pelo algoritmo de ordenação Quick Sort
 * para comparar codLinha de cada veículo
 * 
 * @param p 
 * @param q 
 * @return int 
 */
int comparadorVeiculo(const void *p, const void *q)
{
    int l = ((VEICULO *)p)->codLinha;
    int r = ((VEICULO *)q)->codLinha;
    return (l - r);
}

/**
 * @brief Função de comparação utilizada pelo algoritmo de ordenação Quick Sort
 * para comparar codLinha de cada linha de ônibus
 * 
 * @param p 
 * @param q 
 * @return int 
 */
int comparadorLinha(const void *p, const void *q)
{
    int l = ((LINHA *)p)->codLinha;
    int r = ((LINHA *)q)->codLinha;
    return (l - r);
}

/**
 * @brief copia entre duas structs do tipo VEICULO. Nesse caso, do veiculoAux (temporario)
 * para o veiculos[i] (vetor de todos os registros de veiculo.bin)
 * 
 * @param veiculoOrigem 
 * @param veiculoDestino 
 */
void copiarCamposVeiculo(VEICULO *veiculoOrigem, VEICULO *veiculoDestino)
{
    veiculoOrigem->removido = veiculoDestino->removido;
    veiculoOrigem->tamanhoRegistro = veiculoDestino->tamanhoCategoria;
    strcpy(veiculoOrigem->prefixo, veiculoDestino->prefixo);
    strcpy(veiculoOrigem->data, veiculoDestino->data);
    veiculoOrigem->quantidadeLugares = veiculoDestino->quantidadeLugares;
    veiculoOrigem->codLinha = veiculoDestino->codLinha;
    veiculoOrigem->tamanhoModelo = veiculoDestino->tamanhoModelo;
    veiculoOrigem->tamanhoCategoria = veiculoDestino->tamanhoCategoria;
    veiculoOrigem->modelo = (char *)malloc(veiculoOrigem->tamanhoCategoria * sizeof(char));
    veiculoOrigem->categoria = (char *)malloc(veiculoOrigem->tamanhoModelo * sizeof(char));
    strcpy(veiculoOrigem->modelo, veiculoDestino->modelo);
    strcpy(veiculoOrigem->categoria, veiculoDestino->categoria);
}

/**
 * @brief copia entre duas structs do tipo LINHA. Nesse caso, do linhaAux (temporario)
 * para o linhas[i] (vetor de todos os registros de linha.bin)
 * 
 * @param linhaOrigem 
 * @param linhaDestino 
 */
void copiarCamposLinha(LINHA *linhaOrigem, LINHA *linhaDestino)
{
    linhaOrigem->tamanhoRegistro = linhaDestino->tamanhoRegistro;
    linhaOrigem->codLinha = linhaDestino->codLinha;
    linhaOrigem->aceitaCartao = linhaDestino->aceitaCartao;
    linhaOrigem->tamanhoNome = linhaDestino->tamanhoNome;
    linhaOrigem->nomeLinha = (char *)malloc(linhaOrigem->tamanhoNome * sizeof(char));
    linhaOrigem->tamanhoCor = linhaDestino->tamanhoCor;
    linhaOrigem->corLinha = (char *)malloc(linhaOrigem->tamanhoCor * sizeof(char));
    strcpy(linhaOrigem->nomeLinha, linhaDestino->nomeLinha);
    strcpy(linhaOrigem->corLinha, linhaDestino->corLinha);
}

/**
 * @brief Carrega os registros de veiculos.bin na memoria RAM e ordena por codLinha utilizando qsort()
 * 
 * @param binVeiculoDesordenado 
 * @param veiculos 
 * @param totalRegistroVeiculos 
 */
void ordenarVeiculos(FILE *binVeiculoDesordenado, VEICULO *veiculos, int totalRegistrosVeiculos)
{
    int tamVetorVeiculos = 0; // iterador do vetor de veiculos.
    // Struct veiculo auxiliar para inserção no vetor final
    VEICULO veiculoAux;
    veiculoAux.modelo = NULL;
    veiculoAux.categoria = NULL;

    // Leitura de todos os veículos NÃO REMOVIDOS do disco para o array em RAM
    for (int i = 0; i < totalRegistrosVeiculos; i++)
    {
        lerBINVeiculo(binVeiculoDesordenado, &veiculoAux, FALSE, NULL, NULL);

        // Se não está removido, inserir os dados no vetor de veiculos
        if (veiculoAux.removido == '1')
        {
            // Colocar os dados de veiculoAux no índice atual do vetor de veículos
            copiarCamposVeiculo(&veiculos[tamVetorVeiculos], &veiculoAux);
            tamVetorVeiculos++;
        }

        // liberando memoria dos campos dinamicos da variável auxiliar de veiculos
        freeCamposDinamicos(&veiculoAux.categoria, &veiculoAux.modelo);
    }

    // Agora, é póssível realizar a ordenação dos veiculos por meio do codLinha
    qsort(veiculos, tamVetorVeiculos, sizeof(VEICULO), comparadorVeiculo);
}

/**
 * @brief  Carrega os registros de linhas.bin na memoria RAM e ordena por codLinha utilizando qsort()
 * 
 * @param binLinhaDesordenado
 * @param linhas 
 * @param totalRegistrosLinhas 
 */
void ordenarLinhas(FILE *binLinhaDesordenado, LINHA *linhas, int totalRegistrosLinhas)
{
    int tamVetorLinhas = 0; // iterador do vetor de linhas.

    // Struct veiculo auxiliar para inserção no vetor final
    LINHA linhaAux;
    linhaAux.corLinha = NULL;
    linhaAux.nomeLinha = NULL;

    // Leitura de todos as linhas NÃO REMOVIDOS do disco para o array em RAM
    for (int i = 0; i < totalRegistrosLinhas; i++)
    {
        lerBINLinha(binLinhaDesordenado, &linhaAux, FALSE, NULL, NULL);

        // Se não está removido, inserir os dados no vetor de veiculos
        if (linhaAux.removido == '1')
        {
            // Colocar os dados de linhaAux no índice atual do vetor de linhas de onibus
            copiarCamposLinha(&linhas[tamVetorLinhas], &linhaAux);
            tamVetorLinhas++;
        }

        // liberando memoria dos campos dinamicos da variável auxiliar de veiculos
        freeCamposDinamicos(&linhaAux.corLinha, &linhaAux.nomeLinha);
    }

    // Agora, é póssível realizar a ordenação dos veiculos por meio do codLinha
    qsort(linhas, tamVetorLinhas, sizeof(LINHA), comparadorLinha);
}

/**
 * @brief Função responsável pelas junções de loop aninhado e de loop único quando há o auxilio do arquivo de índices para realizar o merging
 * 
 * @param binVeiculo 
 * @param binLinha 
 * @param binIndex 
 * @param cabVeiculos 
 * @param cabLinhas 
 * @param cabIndex 
 * @param flag para diferenciar o tratamento entre VEICULO e LINHA
 */
void juncoesLoop(FILE *binVeiculo, FILE *binLinha, FILE *binIndex, CABECALHOV *cabVeiculos, CABECALHOL *cabLinhas, CABECALHOI *cabIndex, int flag)
{
    // Se flag = 0, fazer a junção de loop aninhado (sem o arquivo de index)
    // Se flag = 1, fazer a junção de loop unico (por meio do arquivo de index)

    // Receber o total de registros dos arquivos de veiculo e linha
    int totalRegistrosVeiculos = cabVeiculos->nroRegistros + cabVeiculos->nroRegRemovidos;
    int totalRegistrosLinhas = cabLinhas->nroRegistros + cabLinhas->nroRegRemovidos;

    VEICULO veiculo;
    veiculo.modelo = NULL;
    veiculo.categoria = NULL;

    LINHA linha;
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;

    INDEX index;
    inicializarIndex(&index);

    // Variável para verificar se algum registro foi gerado na junção dos dois arquivos
    int encontrados = 0;

    // Fazer a busca do registro com a chave buscada (caso do loop único)
    int rrnEncontrado = 0;
    int posEncontrado = 0;

    for (int i = 0; i < totalRegistrosVeiculos; i++)
    {
        // Ver se a leitura funcionou e o registro não está logicamente removido
        if(lerBINVeiculo(binVeiculo, &veiculo, FALSE, NULL, NULL) && veiculo.removido == '1')
        {
            if (flag == 0)
            {
                // Junção de loop aninhado
                
                // Voltar o arquivo de linhas para o primeiro registro após o cabecalho
                rewind(binLinha);
                lerCabecalhoBINLinha(binLinha, cabLinhas);

                for (int j = 0; j < totalRegistrosLinhas; j++)
                {
                    lerBINLinha(binLinha, &linha, FALSE, NULL, NULL);

                    // Se veiculo.codLinha = linha.codLinha então mostre os campos de veiculo e linha conforme solicitado
                    if (veiculo.codLinha == linha.codLinha)
                    {
                        encontrados++;
                        // Mostrar os 2
                        exibirRegistrosVeiculo(cabVeiculos, &veiculo);
                        exibirRegistrosLinha(cabLinhas, &linha);
                        printf("\n");
                    }

                    // liberando memoria os campos dinamicos
                    freeCamposDinamicos(&linha.nomeLinha, &linha.corLinha);
                }
            }
            else
            {
                // Junção de loop único

                // Procurar a linha que contém o codLinha do veiculo atual
                if (procuraIndex(binIndex, cabIndex->noRaiz, veiculo.codLinha, &rrnEncontrado, &posEncontrado) == FOUND)
                {
                    // Encontrou, então mostrar o veiculo e a linha atuais
                    encontrados++;

                    // Primeiro ler a Linha no arquivo de linhas de onibus
                    lerBINIndice(binIndex, &index, rrnEncontrado);

                    // pegando o byteoffset correspondente à chave na posição encontrada
                    int byteOffSet = index.Pr[posEncontrado];

                    // Mover o ponteiro do arquivo de linhas para o byteOffSet
                    fseek(binLinha, byteOffSet, SEEK_SET);
                    lerBINLinha(binLinha, &linha, FALSE, NULL, NULL);

                    // Por fim, mostrar o veiculo e a linha atual
                    exibirRegistrosVeiculo(cabVeiculos, &veiculo);
                    exibirRegistrosLinha(cabLinhas, &linha);
                    printf("\n");

                    // liberando memoria os campos dinamicos
                    freeCamposDinamicos(&linha.nomeLinha, &linha.corLinha);
                }
            }
            // liberando memoria dos campos dinamicos
            freeCamposDinamicos(&veiculo.modelo, &veiculo.categoria);
        }
    }

    freeCamposDinamicos(&linha.nomeLinha, &linha.corLinha);
    freeCamposDinamicos(&veiculo.modelo, &veiculo.categoria);

    // Se não foi gerado nenhum registro na junção dos dois arquivos
    if (encontrados == 0)
    {
        printf("Registro inexistente.\n");
    }
}