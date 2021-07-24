/**
 * @file main.c
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "registry/linha.h"
#include "registry/veiculo.h"
#include "index/index.h"
#include "merge/merge.h"
#include "utils/convertePrefixo.h"

/**
 * @brief Realiza a leitura do arquivo CSV e cria e insere um arquivo binário contendo o cabeçalho
 * e os veiculos lidos.
 * 
 * @param nomeCSV nome do arquivo CSV a ser lido
 * @param nomeBIN nome do arquivo binário a ser criado
 */
void funcionalidade1(char *nomeCSV, char *nomeBIN)
{
    // Abrir arquivo CSV para leitura
    FILE *csv = abrirArquivo(nomeCSV, FILE_MODE2);
    if (csv == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Escrever o cabeçalho do arquivo binário de veiculos
    CABECALHOV cabVeiculos;
    if (!lerCabecalhoCSVVeiculo(csv, &cabVeiculos))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Abrir o arquivo binário para escrita
    FILE *bin = abrirArquivo(nomeBIN, FILE_MODE3);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Escrever o cabeçalho no arquivo binário
    if (!escreverCabecalhoBINVeiculo(bin, &cabVeiculos))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Criar a struct para armazenamento temporário dos dados do veiculo
    VEICULO veiculos;

    // Ler linha a linha do arquivo csv e inserir no arquivo binário
    // Lembrando que a struct VEICULO conterá temporariamente os dados da linha do arquivo CSV lida
    veiculos.modelo = NULL;
    veiculos.categoria = NULL;

    // Ler cada linha do arquivo CSV e inserir os veiculos no arquivo binário
    while (lerLinhaCSVVeiculo(csv, &veiculos, &cabVeiculos))
    {
        if (!escreverBINVeiculo(bin, &veiculos))
        {
            printf("Falha no processamento do arquivo.\n");
            return;
        }
    }

    // limpando a mem heap
    free(veiculos.modelo);
    free(veiculos.categoria);

    // Atualizar cabeçalho do arquivo binário
    atualizaCabecalhoVeiculo(bin, &cabVeiculos);

    // fechando arquivos
    fecharArquivoBin(&bin);
    fclose(csv);

    binarioNaTela(nomeBIN);
    return;
}

/**
 * @brief Realiza a leitura do arquivo CSV e cria e insere um arquivo binário contendo o cabeçalho e
 * as linhas de ônibus lidas.
 * 
 * @param nomeCSV nome do arquivo CSV a ser lido
 * @param nomeBIN nome do arquivo binário a ser criado
 */
void funcionalidade2(char *nomeCSV, char *nomeBIN)
{
    // Abrir arquivo CSV para leitura
    FILE *csv = abrirArquivo(nomeCSV, FILE_MODE2);
    if (csv == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Escrever o cabeçalho do arquivo binário de linhas de onibus
    CABECALHOL cabLinhas;
    if (!lerCabecalhoCSVLinha(csv, &cabLinhas))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Abrir o arquivo binário para escrita
    FILE *bin = abrirArquivoBin(nomeBIN, FILE_MODE3);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Escrever o cabeçalho no arquivo binário
    if (!escreveCabecalhoBINLinhas(bin, &cabLinhas))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Criar a struct para armazenamento temporário dos dados da linha de onibus
    LINHA linha;

    // Ler linha a linha do arquivo csv e inserir no arquivo binário
    // Lembrando que a struct LINHA conterá temporariamente os dados da linha do arquivo CSV lida
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;

    // Ler cada linha do arquivo CSV e inserir as linhas de obibus no arquivo binário
    while (lerLinhaCSVLinha(csv, &linha, &cabLinhas))
    {
        if (!escreverBINLinha(bin, &linha))
        {
            printf("Falha no processamento do arquivo.\n");
            return;
        }
    }

    // limpando a mem heap
    free(linha.nomeLinha);
    free(linha.corLinha);

    // Atualizar cabeçalho do arquivo binário
    atualizaCabecalhoLinha(bin, &cabLinhas);

    // fechando arquivos
    fecharArquivoBin(&bin);
    fclose(csv);

    binarioNaTela(nomeBIN);
    return;
}

/**
 * @brief Realiza a recuperação dos dados de todos os registros armazenados no arquivo BINÁRIO de veiculos
 * e os exibe de maneira organizada.
 * 
 * @param nomeBIN nome do arquivo binário de veiculos
 */
void funcionalidade3(char *nomeBIN)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivoBin(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho de veiculos do arquivo binário
    CABECALHOV cabVeiculos;
    lerCabecalhoBINVeiculo(bin, &cabVeiculos);

    // Chegar se não há registros no arquivo
    if (feof(bin) || cabVeiculos.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
    }

    // Agora ler os registros e exibir na tela
    VEICULO veiculo;
    veiculo.modelo = NULL;
    veiculo.categoria = NULL;

    int totalRegistros = cabVeiculos.nroRegistros + cabVeiculos.nroRegRemovidos;

    for (int i = 0; i < totalRegistros; i++)
    {
        // Ler o registro
        lerBINVeiculo(bin, &veiculo, FALSE, NULL, NULL);

        // Só exibir os veiculos que não estão marcados logicamente como excluidos
        if (veiculo.removido == '1')
            exibirRegistrosVeiculo(&cabVeiculos, &veiculo);

        // liberando memoria dos campos dinamicos
        free(veiculo.modelo);
        free(veiculo.categoria);
        veiculo.modelo = NULL;
        veiculo.categoria = NULL;
    }

    // Fechando arquivo binário
    fecharArquivoBin(&bin);

    return;
}

/**
 * @brief Realiza a recuperação dos dados de todos os registros armazenados no arquivo BINÁRIO
 * de linhas de ônibus e os exibe de maneira organizada.
 * 
 * @param nomeBIN nome do arquivo binário de linhas de ônibus
 */
void funcionalidade4(char *nomeBIN)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivoBin(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho de veiculos do arquivo binário
    CABECALHOL cabLinhas;
    lerCabecalhoBINLinha(bin, &cabLinhas);

    // Chegar se não há registros no arquivo
    if (feof(bin) || cabLinhas.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
    }

    // Agora ler os registros e exibir na tela
    LINHA linha;
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;

    int totalRegistros = cabLinhas.nroRegistros + cabLinhas.nroRegRemovidos;

    for (int i = 0; i < totalRegistros; i++)
    {
        // Ler o registro
        lerBINLinha(bin, &linha, FALSE, NULL, NULL);

        // Só exibir os veiculos que não estão marcados logicamente como excluidos
        if (linha.removido == '1')
            exibirRegistrosLinha(&cabLinhas, &linha);

        // liberando memoria os campos dinamicos
        free(linha.nomeLinha);
        free(linha.corLinha);
        linha.nomeLinha = NULL;
        linha.corLinha = NULL;
    }

    // Fechando arquivo binário
    fecharArquivoBin(&bin);

    return;
}

/**
 * @brief Faz a recuperação de dados de todos os registros do arquivo BINÁRIO que satisfaçam 
 * um critério de busca determinado pelo usuário.
 * Exibe apenas os registros cujo valor do campo bate com o determinado pelo usuário.
 * 
 * @param nomeBIN
 * @param campo
 * @param valor
 */
void funcionalidade5(char *nomeBIN, char *campo, char *valor)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivoBin(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho de veiculos do arquivo binário
    CABECALHOV cabVeiculos;
    lerCabecalhoBINVeiculo(bin, &cabVeiculos);

    // Chegar se não há registros no arquivo
    if (feof(bin) || cabVeiculos.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
    }

    // Agora ler os registros e exibir na tela
    VEICULO veiculo;
    veiculo.modelo = NULL;
    veiculo.categoria = NULL;

    int totalRegistros = cabVeiculos.nroRegistros + cabVeiculos.nroRegRemovidos;

    for (int i = 0; i < totalRegistros; i++)
    {
        // Ler o registro
        if (lerBINVeiculo(bin, &veiculo, TRUE, campo, valor))
        {
            // Exibir apenas os veiculos que não estão marcadas logicamente como excluidos
            if (veiculo.removido == '1')
            {
                exibirRegistrosVeiculo(&cabVeiculos, &veiculo);
                // liberando memoria dos campos dinamicos
                free(veiculo.modelo);
                free(veiculo.categoria);
                veiculo.modelo = NULL;
                veiculo.categoria = NULL;
            }
        }
    }

    // Fechando arquivo binário
    fecharArquivoBin(&bin);

    return;
}

/**
 * @brief Faz a recuperação de dados de todos os registros do arquivo BINÁRIO que satisfaçam 
 * um critério de busca determinado pelo usuário.
 * Exibe apenas os registros cujo valor do campo bate com o determinado pelo usuário.
 * 
 * @param nomeBIN
 * @param campo 
 * @param valor
 */
void funcionalidade6(char *nomeBIN, char *campo, char *valor)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivoBin(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho de veiculos do arquivo binário
    CABECALHOL cabLinhas;
    lerCabecalhoBINLinha(bin, &cabLinhas);

    // Chegar se não há registros no arquivo
    if (feof(bin) || cabLinhas.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
    }

    // Agora ler os registros e exibir na tela
    LINHA linha;
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;

    int totalRegistros = cabLinhas.nroRegistros + cabLinhas.nroRegRemovidos;

    for (int i = 0; i < totalRegistros; i++)
    {
        // Ler o registro
        if (lerBINLinha(bin, &linha, TRUE, campo, valor))
        {
            // Exibir apenas as linhas que não estão marcadas logicamente como excluidas
            if (linha.removido == '1')
            {
                exibirRegistrosLinha(&cabLinhas, &linha);
                // liberando memoria os campos dinamicos
                free(linha.nomeLinha);
                free(linha.corLinha);
                linha.nomeLinha = NULL;
                linha.corLinha = NULL;
            }
        }
    }

    // Fechando arquivo binário
    fecharArquivoBin(&bin);

    return;
}

/**
 * @brief Permite a inserção de N novos registros no arquivo binário de veiculos. 
 * Utiliza da função scan_quote_string para a leitura dos campos.
 * 
 * @param nomeBIN
 * @param N número de novos registros a serem inseridos.
 */
void funcionalidade7(char *nomeBIN, int N)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivoBin(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho de veiculos do arquivo binário
    CABECALHOV cabVeiculos;
    if (!lerCabecalhoBINVeiculo(bin, &cabVeiculos))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&bin);
        return;
    }

    // Mover o ponteiro para o final do arquivo
    fseek(bin, 0, SEEK_END);

    VEICULO veiculo;
    veiculo.modelo = NULL;
    veiculo.categoria = NULL;
    for (int i = 0; i < N; i++)
    {
        // Ler dados do veiculo a ser inserido no arquivo binário
        if (!lerEntradaVeiculo(&veiculo))
        {
            printf("Falha no processamento do arquivo.\n");
            fecharArquivoBin(&bin);
            return;
        }

        // Escrever no arquivo binário o novo veiculo inserido
        escreverBINVeiculo(bin, &veiculo);
        cabVeiculos.nroRegistros++;

        if (veiculo.modelo != NULL)
        {
            free(veiculo.modelo);
            veiculo.modelo = NULL;
        }
        if (veiculo.categoria != NULL)
        {
            free(veiculo.categoria);
            veiculo.categoria = NULL;
        }
    }

    // Atualizar cabeçalho do arquivo binário com o novo número de registros, byteOffSet e fechar o arquivo, atualizando o status como 1
    atualizaCabecalhoVeiculo(bin, &cabVeiculos);
    fecharArquivoBin(&bin);

    binarioNaTela(nomeBIN);
    return;
}

/**
 * @brief Permite a inserção de N novos registros no arquivo binário de linhas de ônibus. Utiliza da função scan_quote_string para a leitura dos campos.
 * 
 * @param nomeBIN
 * @param N número de novos registros a serem inseridos.
 */
void funcionalidade8(char *nomeBIN, int N)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivoBin(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHOL cabLinhas;
    // Ler o cabeçalho de veiculos do arquivo binário
    if (!lerCabecalhoBINLinha(bin, &cabLinhas))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&bin);
        return;
    }

    // Mover o ponteiro para o final do arquivo
    fseek(bin, 0, SEEK_END);

    LINHA linha;
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;
    for (int i = 0; i < N; i++)
    {
        // Ler dados da linha de onibus a ser inserida no arquivo binário
        if (!lerEntradaLinha(&linha))
        {
            printf("Falha no processamento do arquivo.\n");
            fecharArquivoBin(&bin);
            return;
        }

        // Escrever no arquivo binário a nova linha de onibus inserida
        escreverBINLinha(bin, &linha);
        linha.removido == '1' ? cabLinhas.nroRegistros++ : cabLinhas.nroRegRemovidos++;

        if (linha.nomeLinha != NULL)
        {
            free(linha.nomeLinha);
            linha.nomeLinha = NULL;
        }
        if (linha.corLinha != NULL)
        {
            free(linha.corLinha);
            linha.corLinha = NULL;
        }
    }

    // Atualizar cabeçalho do arquivo binário com o novo número de registros, byteOffSet e fechar o arquivo, atualizando o status como 1
    atualizaCabecalhoLinha(bin, &cabLinhas);
    fecharArquivoBin(&bin);

    binarioNaTela(nomeBIN);
    return;
}

/**
 * @brief Cria um arquivo de índice árvore-B para o arquivo de dados veiculo definido 
 * de acordo com a especificação do primeiro trabalho prático
 * 
 * @param arqVeiculoBIN 
 * @param arqIndicePrefixo 
 */
void funcionalidade9(char *arqVeiculoBIN, char *arqIndicePrefixo)
{
    FILE *binVeiculo = abrirArquivoBin(arqVeiculoBIN, FILE_MODE1);
    FILE *binIndex = abrirArquivo(arqIndicePrefixo, FILE_MODE3);

    if (binVeiculo == NULL || binIndex == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho de veiculos do arquivo binário
    CABECALHOV cabVeiculos;
    lerCabecalhoBINVeiculo(binVeiculo, &cabVeiculos);

    // Agora ler os registros e exibir na tela
    VEICULO veiculo;
    veiculo.modelo = NULL;
    veiculo.categoria = NULL;

    int totalRegistros = cabVeiculos.nroRegistros + cabVeiculos.nroRegRemovidos;

    // Armazena o byteoffset do arquivo de veiculos atual
    int64 byteoffset = ftell(binVeiculo);

    // Inicializar e escrever o cabeçalho no arquivo de índice
    CABECALHOI cabIndex;
    if (!inicializarCabecalhoIndex(&cabIndex) || !escreverBinCabIndex(binIndex, &cabIndex))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binIndex);
        return;
    }

    INDEX index;
    if (!inicializarIndex(&index))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binIndex);
        return;
    }

    for (int i = 0; i < totalRegistros; i++)
    {
        // Ler o registro
        if (lerBINVeiculo(binVeiculo, &veiculo, FALSE, NULL, NULL))
        {
            // Inserir no arquivo de índices apenas os veículos que não estão marcados como logicamente removidos
            if (veiculo.removido == '1')
            {
                int chave = convertePrefixo(veiculo.prefixo);

                inserirIndex(binIndex, &cabIndex, &index, chave, byteoffset);

                // liberando memoria dos campos dinamicos
                free(veiculo.modelo);
                free(veiculo.categoria);
                veiculo.modelo = NULL;
                veiculo.categoria = NULL;
            }
            byteoffset = ftell(binVeiculo);
        }
    }

    // Atualizar o cabecalho de index
    escreverBinCabIndex(binIndex, &cabIndex);

    // Fechando arquivos binários
    fecharArquivoBin(&binVeiculo);
    fecharArquivoBin(&binIndex);

    binarioNaTela(arqIndicePrefixo);
    return;
}

/**
 * @brief Cria um arquivo de índice árvore-B para o arquivo de dados linha definido de acordo
 * com a especificação do primeiro trabalho prático. O campo a ser indexado é codLinha
 * 
 * @param arqLinhaBIN 
 * @param arqIndicePrefixo 
 */
void funcionalidade10(char *arqLinhaBIN, char *arqIndicePrefixo)
{
    FILE *binLinha = abrirArquivoBin(arqLinhaBIN, FILE_MODE1);
    FILE *binIndex = abrirArquivo(arqIndicePrefixo, FILE_MODE3);

    if (binLinha == NULL || binIndex == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho de linhas do arquivo binário
    CABECALHOL cabLinhas;
    lerCabecalhoBINLinha(binLinha, &cabLinhas);

    // Agora ler os registros e exibir na tela
    LINHA linha;
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;

    int totalRegistros = cabLinhas.nroRegistros + cabLinhas.nroRegRemovidos;

    // Armazena o byteoffset do arquivo de linhas atual
    int64 byteoffset = ftell(binLinha);

    // Inicializar e escrever o cabeçalho no arquivo de índice
    CABECALHOI cabIndex;
    if (!inicializarCabecalhoIndex(&cabIndex) || !escreverBinCabIndex(binIndex, &cabIndex))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
    }

    INDEX index;
    if (!inicializarIndex(&index))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
    }

    for (int i = 0; i < totalRegistros; i++)
    {
        // Ler o registro
        if (lerBINLinha(binLinha, &linha, FALSE, NULL, NULL))
        {
            // Exibir apenas as linhas que não estão marcadas logicamente como excluidas
            if (linha.removido == '1')
            {
                inserirIndex(binIndex, &cabIndex, &index, linha.codLinha, byteoffset);

                // liberando memoria os campos dinamicos
                freeCamposDinamicos(&linha.nomeLinha, &linha.corLinha);
            }
            byteoffset = ftell(binLinha);
        }
    }

    // Atualizar o cabecalho de index
    escreverBinCabIndex(binIndex, &cabIndex);

    // Fechando arquivos binários
    fecharArquivoBin(&binLinha);
    fecharArquivoBin(&binIndex);

    binarioNaTela(arqIndicePrefixo);
    return;
}

/**
 * @brief Permite a recuperação dos dados de todos os registros do arquivo de dados veiculo que 
 * satisfaçam um critério de busca determinado pelo usuário sobre o campo prefixo, usando
 * o índice árvore-B criado na funcionalidade
 *
 * @param arqVeiculoBIN 
 * @param arqIndicePrefixo 
 * @param valor chave do registro buscado
 */
void funcionalidade11(char *arqVeiculoBIN, char *arqIndicePrefixo, int valor)
{
    FILE *binVeiculo = abrirArquivoBin(arqVeiculoBIN, FILE_MODE1);
    FILE *binIndex = abrirArquivo(arqIndicePrefixo, FILE_MODE1);

    // Registros marcados como logicamente removidos não devem ser exibidos.
    if (!binVeiculo || !binIndex)
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Inicializando  o cabecalho do indice
    CABECALHOI cabIndex;
    inicializarCabecalhoIndex(&cabIndex);
    lerBinCabIndex(binIndex, &cabIndex);

    // Fazer a busca do registro com a chave buscada
    int rrnEncontrado = 0;
    int posEncontrado = 0;

    if (procuraIndex(binIndex, cabIndex.noRaiz, valor, &rrnEncontrado, &posEncontrado) == NOT_FOUND)
    {
        printf("Registro inexistente.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Encontrou, então ler o registro no arquivo binário de veiculos e exibir na tela
    INDEX index;
    inicializarIndex(&index);
    lerBINIndice(binIndex, &index, rrnEncontrado);

    // pegando o byteoffset correspondente à chave na posição encontrada
    int byteOffSet = index.Pr[posEncontrado];

    // Ler o cabeçalho de veiculos do arquivo binário
    VEICULO veiculo;
    veiculo.modelo = NULL;
    veiculo.categoria = NULL;

    CABECALHOV cabVeiculo;
    if (!lerCabecalhoBINVeiculo(binVeiculo, &cabVeiculo))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Mover o ponteiro do arquivo de Veiculos para o byteOffSet
    fseek(binVeiculo, byteOffSet, SEEK_SET);
    lerBINVeiculo(binVeiculo, &veiculo, FALSE, NULL, NULL);

    // Exibir o veiculo somente se não estiver marcado logicamente como removido
    if (veiculo.removido == '0')
    {
        printf("Registro inexistente.\n");
    }
    else
    {
        exibirRegistrosVeiculo(&cabVeiculo, &veiculo);
    }

    // liberando memoria dos campos dinamicos
    freeCamposDinamicos(&veiculo.modelo, &veiculo.categoria);

    // Fechando arquivos binários
    fecharArquivoBin(&binVeiculo);
    fecharArquivoBin(&binIndex);

    return;
}

/**
 * @brief Permita a recuperação dos dados de todos os registros do arquivo de dados linha que 
 * satisfaçam um critério de busca determinado pelo usuário sobre o campo codLinha, usando
 * o índice árvore-B criado na funcionalidade
 *
 * @param arqVeiculoLIN
 * @param arqIndicePrefixo
 * @param valor 
 */
void funcionalidade12(char *arqLinhaBIN, char *arqIndicePrefixo, int valor)
{
    FILE *binLinha = abrirArquivoBin(arqLinhaBIN, FILE_MODE1);
    FILE *binIndex = abrirArquivo(arqIndicePrefixo, FILE_MODE1);
    // Registros marcados como logicamente removidos não devem ser exibidos.
    if (!binLinha || !binIndex)
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Inicializando  o cabecalho do indice
    CABECALHOI cabIndex;
    inicializarCabecalhoIndex(&cabIndex);
    lerBinCabIndex(binIndex, &cabIndex);

    // Fazer a busca do registro com a chave buscada
    int rrnEncontrado = 0;
    int posEncontrado = 0;

    if (procuraIndex(binIndex, cabIndex.noRaiz, valor, &rrnEncontrado, &posEncontrado) == NOT_FOUND)
    {
        printf("Registro inexistente.\n");
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Encontrou, então ler o registro no arquivo binário de linhas e exibir na tela
    INDEX index;
    inicializarIndex(&index);
    lerBINIndice(binIndex, &index, rrnEncontrado);

    int byteOffSet = index.Pr[posEncontrado];

    // Ler o cabeçalho de linhas do arquivo binário
    LINHA linha;
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;

    CABECALHOL cabLinha;
    if (!lerCabecalhoBINLinha(binLinha, &cabLinha))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Mover o ponteiro do arquivo de linhas para o byteOffSet
    fseek(binLinha, byteOffSet, SEEK_SET);
    lerBINLinha(binLinha, &linha, FALSE, NULL, NULL);

    // Exibir a linha somente se não estiver marcada logicamente como removida
    if (linha.removido == '0')
    {
        printf("Registro inexistente.\n");
    }
    else
    {
        exibirRegistrosLinha(&cabLinha, &linha);
    }

    // liberando memoria dos campos dinamicos
    freeCamposDinamicos(&linha.nomeLinha, &linha.corLinha);

    // Fechando arquivos binários
    fecharArquivoBin(&binLinha);
    fecharArquivoBin(&binIndex);

    return;
}

/**
 * @brief Estende a funcionalidade [7] de forma que, a cada inserção de um registro no arquivo de 
 * dados veiculo, a chave de busca correspondente a essa inserção seja inserida no arquivo de índice árvore-B
 * 
 * @param arqVeiculoBIN 
 * @param arqIndicePrefixo 
 * @param n Número de registros a serem inseridos
 */
void funcionalidade13(char *arqVeiculoBIN, char *arqIndicePrefixo, int n)
{
    // Abrir arquivo binário para leitura
    FILE *binVeiculo = abrirArquivoBin(arqVeiculoBIN, FILE_MODE1);
    FILE *binIndex = abrirArquivoBin(arqIndicePrefixo, FILE_MODE1);
    if (binVeiculo == NULL || binIndex == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho de veiculos do arquivo binário
    CABECALHOV cabVeiculos;
    if (!lerCabecalhoBINVeiculo(binVeiculo, &cabVeiculos))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Inicializar e escrever o cabeçalho no arquivo de índice
    CABECALHOI cabIndex;
    inicializarCabecalhoIndex(&cabIndex);
    if (!lerBinCabIndex(binIndex, &cabIndex))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binIndex);
        return;
    }

    INDEX index;
    inicializarIndex(&index);

    // Mover o ponteiro para o final do arquivo
    fseek(binVeiculo, 0, SEEK_END);

    VEICULO veiculo;
    veiculo.modelo = NULL;
    veiculo.categoria = NULL;
    for (int i = 0; i < n; i++)
    {
        // Ler dados do veiculo a ser inserido no arquivo binário
        if (!lerEntradaVeiculo(&veiculo))
        {
            printf("Falha no processamento do arquivo.\n");
            fecharArquivoBin(&binVeiculo);
            fecharArquivoBin(&binIndex);
            return;
        }

        // Escrever no arquivo binário o novo veiculo inserido
        int byteOffSet = ftell(binVeiculo);
        escreverBINVeiculo(binVeiculo, &veiculo);
        cabVeiculos.nroRegistros++;

        // Escrever no arquivo de index o novo registro
        inserirIndex(binIndex, &cabIndex, &index, convertePrefixo(veiculo.prefixo), byteOffSet);

        freeCamposDinamicos(&veiculo.modelo, &veiculo.categoria);
    }

    // Atualizar cabeçalho do arquivo binário com o novo número de registros, byteOffSet e fechar o arquivo, atualizando o status como 1
    atualizaCabecalhoVeiculo(binVeiculo, &cabVeiculos);
    // Atualizar o cabecalho de index
    escreverBinCabIndex(binIndex, &cabIndex);

    // Fechar arquivos binários de veículo de index
    fecharArquivoBin(&binVeiculo);
    fecharArquivoBin(&binIndex);

    binarioNaTela(arqIndicePrefixo);
    return;
}

/**
 * @brief  Estende a funcionalidade [8] de forma que, a cada inserção de um registro no
 * arquivo de dados linha, a chave de busca correspondente a essa inserção eh inserida
 * no arquivo de índice árvore-B
 * 
 * @param arqVeiculoBIN 
 * @param arqIndicePrefixo 
 * @param n Número de registros a serem inseridos
 */
void funcionalidade14(char *arqLinhaBIN, char *arqIndicePrefixo, int n)
{
    // Abrir arquivo binário para leitura
    FILE *binLinha = abrirArquivoBin(arqLinhaBIN, FILE_MODE1);
    FILE *binIndex = abrirArquivoBin(arqIndicePrefixo, FILE_MODE1);
    if (binLinha == NULL || binIndex == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHOL cabLinhas;
    // Ler o cabeçalho de linhas do arquivo binário
    if (!lerCabecalhoBINLinha(binLinha, &cabLinhas))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Inicializar e escrever o cabeçalho no arquivo de índice
    CABECALHOI cabIndex;
    inicializarCabecalhoIndex(&cabIndex);
    if (!lerBinCabIndex(binIndex, &cabIndex))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
        return;
    }

    INDEX index;
    inicializarIndex(&index);

    // Mover o ponteiro para o final do arquivo
    fseek(binLinha, 0, SEEK_END);

    LINHA linha;
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;
    for (int i = 0; i < n; i++)
    {
        // Ler dados da linha de onibus a ser inserida no arquivo binário
        if (!lerEntradaLinha(&linha))
        {
            printf("Falha no processamento do arquivo.\n");
            fecharArquivoBin(&binLinha);
            fecharArquivoBin(&binIndex);
            return;
        }

        // Escrever no arquivo binário a nova linha de onibus inserida
        int byteOffSet = ftell(binLinha);
        escreverBINLinha(binLinha, &linha);
        linha.removido == '1' ? cabLinhas.nroRegistros++ : cabLinhas.nroRegRemovidos++;

        // Escrever no arquivo de index o novo registro
        inserirIndex(binIndex, &cabIndex, &index, linha.codLinha, byteOffSet);

        freeCamposDinamicos(&linha.nomeLinha, &linha.corLinha);
    }

    // Atualizar cabeçalho do arquivo binário com o novo número de registros, byteOffSet e fechar o arquivo, atualizando o status como 1
    atualizaCabecalhoLinha(binLinha, &cabLinhas);
    // Atualizar o cabecalho de index
    escreverBinCabIndex(binIndex, &cabIndex);

    // Fechar arquivos binários de linhas de index
    fecharArquivoBin(&binLinha);
    fecharArquivoBin(&binIndex);

    binarioNaTela(arqIndicePrefixo);
    return;
}

/**
 * @brief Permite a recuperação dos dados de todos os registros armazenados no arquivo de dados veiculo.bin,
 * juntando-os de forma apropriada com os dados de linha.bin.
 * 
 * @param arqVeiculoBIN 
 * @param arqLinhaBIN 
 * @param nomeCampoVeiculo 
 * @param nomeCampoLinha 
 */
void funcionalidade15(char *arqVeiculoBIN, char *arqLinhaBIN, char *nomeCampoVeiculo, char *nomeCampoLinha)
{
    // Abrir arquivos binários para leitura
    FILE *binVeiculo = abrirArquivoBin(arqVeiculoBIN, FILE_MODE1);
    FILE *binLinha = abrirArquivoBin(arqLinhaBIN, FILE_MODE1);
    if (binVeiculo == NULL || binLinha == NULL)
    {
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binLinha);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler os cabeçalhos de veiculos e de linhas em seus respsectivos arquivos binários
    CABECALHOV cabVeiculos;
    CABECALHOL cabLinhas;

    if (!lerCabecalhoBINVeiculo(binVeiculo, &cabVeiculos) || !lerCabecalhoBINLinha(binLinha, &cabLinhas))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binLinha);
        return;
    }

    // Checar se não há registros no arquivo
    if (feof(binVeiculo) || feof(binLinha) || cabVeiculos.nroRegistros == 0 || cabLinhas.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binLinha);
        return;
    }

    // Função para a realização da junção de loop aninhado
    juncoesLoop(binVeiculo, binLinha, NULL, &cabVeiculos, &cabLinhas, NULL, 0);

    // Fechando arquivos binários
    fecharArquivoBin(&binVeiculo);
    fecharArquivoBin(&binLinha);

    return;
}

/**
 * @brief  Permite a recuperação dos dados de todos os registros armazenados no arquivo de
 * dados veiculo.bin, juntando-os de forma apropriada com os dados de linha.bin.
 * 
 * @param arqVeiculoBIN 
 * @param arqLinhaBIN 
 * @param nomeCampoVeiculo 
 * @param nomeCampoLinha 
 * @param arqIndiceLinhas 
 */
void funcionalidade16(char *arqVeiculoBIN, char *arqLinhaBIN, char *nomeCampoVeiculo, char *nomeCampoLinha, char *arqIndiceLinhas)
{
    // Abrir arquivos binários para leitura
    FILE *binVeiculo = abrirArquivoBin(arqVeiculoBIN, FILE_MODE1);
    FILE *binLinha = abrirArquivoBin(arqLinhaBIN, FILE_MODE1);
    FILE *binIndex = abrirArquivoBin(arqIndiceLinhas, FILE_MODE1);
    if (binVeiculo == NULL || binLinha == NULL || binIndex == NULL)
    {
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalhos de veiculos, linhas e index dos respectivos arquivos binários
    CABECALHOV cabVeiculos;
    CABECALHOL cabLinhas;
    CABECALHOI cabIndex;

    inicializarCabecalhoIndex(&cabIndex);

    if (!lerCabecalhoBINVeiculo(binVeiculo, &cabVeiculos) || !lerCabecalhoBINLinha(binLinha, &cabLinhas) || !lerBinCabIndex(binIndex, &cabIndex))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Checar se não há registros nos arquivos
    if (feof(binVeiculo) || feof(binLinha) || feof(binIndex) || cabVeiculos.nroRegistros == 0 || cabLinhas.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
        fecharArquivoBin(&binVeiculo);
        fecharArquivoBin(&binLinha);
        fecharArquivoBin(&binIndex);
        return;
    }

    // Função para a realização da junção de loop único por meio do arquivo de índice
    juncoesLoop(binVeiculo, binLinha, binIndex, &cabVeiculos, &cabLinhas, &cabIndex, 1);

    // Fechando arquivos binários
    fecharArquivoBin(&binVeiculo);
    fecharArquivoBin(&binLinha);
    fecharArquivoBin(&binIndex);

    return;
}

/**
 * @brief Ordena o arquivo de dados veiculo.bin de acordo com um campo de ordenação codLinha
 * 
 * @param arqDesordenadoBIN 
 * @param arqOrdenadoBIN 
 * @param campoOrdenacao 
 */
void funcionalidade17(char *arqDesordenadoBIN, char *arqOrdenadoBIN, char *campoOrdenacao)
{
    // Abrir arquivos binários para leitura
    FILE *binVeiculoDesordenado = abrirArquivoBin(arqDesordenadoBIN, FILE_MODE1);
    FILE *binVeiculoOrdenado = abrirArquivoBin(arqOrdenadoBIN, FILE_MODE3);
    if (binVeiculoDesordenado == NULL || binVeiculoOrdenado == NULL)
    {
        fecharArquivoBin(&binVeiculoDesordenado);
        fecharArquivoBin(&binVeiculoOrdenado);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho do arquivo de veiculos desordenados
    CABECALHOV cabVeiculos;

    if (!lerCabecalhoBINVeiculo(binVeiculoDesordenado, &cabVeiculos))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculoDesordenado);
        fecharArquivoBin(&binVeiculoOrdenado);
        return;
    }

    int totalRegistrosVeiculos = cabVeiculos.nroRegistros + cabVeiculos.nroRegRemovidos;

    // Criar um vetor de veiculos que serão alocados na RAM para que a ordenação possa ser executada
    VEICULO *veiculos = (VEICULO *)calloc(sizeof(VEICULO), (cabVeiculos.nroRegistros));

    // Inserir os veiculos do arquivo desordenado e ordenar os registros
    ordenarVeiculos(binVeiculoDesordenado, veiculos, totalRegistrosVeiculos);

    // Depois de reordenar, escrever de volta no arquivo ordenado
    cabVeiculos.nroRegRemovidos = 0;
    escreverCabecalhoBINVeiculo(binVeiculoOrdenado, &cabVeiculos);

    for (int i = 0; i < cabVeiculos.nroRegistros; i++)
    {
        escreverBINVeiculo(binVeiculoOrdenado, &veiculos[i]);
    }

    atualizaCabecalhoVeiculo(binVeiculoOrdenado, &cabVeiculos);

    // liberando a memoria alocada pelo vetor de VEICULO
    for (int i = 0; i < cabVeiculos.nroRegistros; i++)
    {
        freeCamposDinamicos(&veiculos[i].modelo, &veiculos[i].categoria);
    }
    free(veiculos);
    veiculos = NULL;

    // Fechando arquivos binários
    fecharArquivoBin(&binVeiculoDesordenado);
    fecharArquivoBin(&binVeiculoOrdenado);

    binarioNaTela(arqOrdenadoBIN);
    return;
}

/**
 * @brief Ordene o arquivo de dados linha.bin de acordo com um campo de
 * ordenação codLinha 
 *
 * @param arqDesordenadoBIN 
 * @param arqOrdenadoBIN 
 * @param campoOrdenacao 
 */
void funcionalidade18(char *arqDesordenadoBIN, char *arqOrdenadoBIN, char *campoOrdenacao)
{

    // Abrir arquivos binários para leitura
    FILE *binLinhaDesordenado = abrirArquivoBin(arqDesordenadoBIN, FILE_MODE1);
    FILE *binLinhaOrdenado = abrirArquivoBin(arqOrdenadoBIN, FILE_MODE3);
    if (binLinhaDesordenado == NULL || binLinhaOrdenado == NULL)
    {
        fecharArquivoBin(&binLinhaDesordenado);
        fecharArquivoBin(&binLinhaOrdenado);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho do arquivo de veiculos desordenados
    CABECALHOL cabLinhas;

    if (!lerCabecalhoBINLinha(binLinhaDesordenado, &cabLinhas))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binLinhaDesordenado);
        fecharArquivoBin(&binLinhaOrdenado);
        return;
    }

    int totalRegistrosLinhas = cabLinhas.nroRegistros + cabLinhas.nroRegRemovidos;

    // Criar um vetor de linhas que serão alocados na RAM para que a ordenação possa ser executada
    LINHA *linhas = (LINHA *)calloc(sizeof(LINHA), (cabLinhas.nroRegistros));

    // Inserir os veiculos do arquivo desordenado e ordenar os registros
    ordenarLinhas(binLinhaDesordenado, linhas, totalRegistrosLinhas);

    // Depois de reordenar, escrever de volta no arquivo ordenado
    cabLinhas.nroRegRemovidos = 0;
    escreveCabecalhoBINLinhas(binLinhaOrdenado, &cabLinhas);

    for (int i = 0; i < cabLinhas.nroRegistros; i++)
    {
        escreverBINLinha(binLinhaOrdenado, &linhas[i]);
    }

    atualizaCabecalhoLinha(binLinhaOrdenado, &cabLinhas);

    // liberando a memoria alocada pelo vetor de LINHA
    for (int i = 0; i < cabLinhas.nroRegistros; i++)
    {
        freeCamposDinamicos(&linhas[i].nomeLinha, &linhas[i].corLinha);
    }
    free(linhas);
    linhas = NULL;

    // Fechando arquivos binários
    fecharArquivoBin(&binLinhaDesordenado);
    fecharArquivoBin(&binLinhaOrdenado);

    binarioNaTela(arqOrdenadoBIN);
    return;
}

/**
 * @brief Permite a recuperação dos dados de todos os registros armazenados no arquivo de
 * dados linha.bin, juntando-os de forma apropriada com os dados de veiculo.bin
 *
 * @param arqVeiculoBIN 
 * @param arqLinhaBIN 
 * @param nomeCampoVeiculo 
 * @param nomeCampoLinha 
 */
void funcionalidade19(char *arqVeiculoBIN, char *arqLinhaBIN, char *nomeCampoVeiculo, char *nomeCampoLinha)
{
    // arquivos ordenados de linhas e veiculos
    FILE *binVeiculoDesordenado = abrirArquivoBin(arqVeiculoBIN, FILE_MODE1);
    FILE *binLinhaDesordenado = abrirArquivoBin(arqLinhaBIN, FILE_MODE1);
    if (binVeiculoDesordenado == NULL || binLinhaDesordenado == NULL)
    {
        fecharArquivoBin(&binVeiculoDesordenado);
        fecharArquivoBin(&binLinhaDesordenado);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler o cabeçalho dos arquivos de veiculo e linhas a serem ordenados
    CABECALHOV cabVeiculos;
    CABECALHOL cabLinhas;

    if (!lerCabecalhoBINVeiculo(binVeiculoDesordenado, &cabVeiculos) || !lerCabecalhoBINLinha(binLinhaDesordenado, &cabLinhas))
    {
        printf("Falha no processamento do arquivo.\n");
        fecharArquivoBin(&binVeiculoDesordenado);
        fecharArquivoBin(&binLinhaDesordenado);
        return;
    }

    int totalRegistrosVeiculos = cabVeiculos.nroRegistros + cabVeiculos.nroRegRemovidos;
    int totalRegistrosLinhas = cabLinhas.nroRegistros + cabLinhas.nroRegRemovidos;

    // Criar um vetor de veiculos e linhas que serão alocados na RAM para que a ordenação possa ser executada
    VEICULO *veiculos = (VEICULO *)calloc(sizeof(VEICULO), (cabVeiculos.nroRegistros));
    LINHA *linhas = (LINHA *)calloc(sizeof(LINHA), (cabLinhas.nroRegistros));

    // Inserir os veiculos e linhas dos arquivos desordenados e ordenar os registros
    ordenarVeiculos(binVeiculoDesordenado, veiculos, totalRegistrosVeiculos);
    ordenarLinhas(binLinhaDesordenado, linhas, totalRegistrosLinhas);

    // Realizar o merge dos arquivos de veiculo e linha ordenados
    int codLinhaVeiculo = -1;
    int codLinhaLinha = -1;
    int linhaAtual = 0;
    int encontrados = 0;

    codLinhaLinha = linhas[linhaAtual++].codLinha;

    for (int i = 0; i < cabVeiculos.nroRegistros; i++)
    {
        codLinhaVeiculo = veiculos[i].codLinha;
        // enquanto podemos realizar a juncao de linhas de onibus aos onibus
        while (codLinhaLinha < codLinhaVeiculo && linhaAtual < cabLinhas.nroRegistros)
        {
            codLinhaLinha = linhas[linhaAtual++].codLinha;
        }

        // se deu match, exibir ambos
        if (codLinhaLinha == codLinhaVeiculo)
        {
            encontrados++;
            exibirRegistrosVeiculo(&cabVeiculos, &veiculos[i]);
            exibirRegistrosLinha(&cabLinhas, &linhas[linhaAtual - 1]);
            printf("\n");
        }
    }

    // Se não foi gerado nenhum registro na junção dos dois arquivos
    if (encontrados == 0)
    {
        printf("Registro inexistente.\n");
    }

    // liberando a memoria alocada pelo vetor de VEICULO
    for (int i = 0; i < cabVeiculos.nroRegistros; i++)
    {
        freeCamposDinamicos(&veiculos[i].modelo, &veiculos[i].categoria);
    }
    free(veiculos);

    // liberando a memoria alocada pelo vetor de LINHA
    for (int i = 0; i < cabLinhas.nroRegistros; i++)
    {
        freeCamposDinamicos(&linhas[i].corLinha, &linhas[i].nomeLinha);
    }
    free(linhas);

    // Fechando arquivos binários
    fecharArquivoBin(&binVeiculoDesordenado);
    fecharArquivoBin(&binLinhaDesordenado);

    return;
}

int main()
{
    int funcionalidade = 0; // codigo da funcionalidade executada
    int N = 0;              // Utilizado nas funcionalidades 7 e 8, por exemplo, para inserção de novos dados nos arquivos binários de veiculos e linhas
    char **args = NULL;     // os args são auxiliares que recebem os argumentos de cada funcionalidade do programa

    // Leitura do input
    scanf("%d ", &funcionalidade);
    args = (char **)malloc(NUM_ARGS * sizeof(char *));
    for (int i = 0; i < NUM_ARGS; i++)
        args[i] = (char *)malloc(BUFFER);

    // todas as funcionalidades do programa
    switch (funcionalidade)
    {
        // =============================== PRIMEIRO Trabalho Prático ===============================
        case 1:
            scanf("%s %s", args[0], args[1]);
            funcionalidade1(args[0], args[1]);
            break;
        case 2:
            scanf("%s %s", args[0], args[1]);
            funcionalidade2(args[0], args[1]);
            break;
        case 3:
            scanf("%s", args[0]);
            funcionalidade3(args[0]);
            break;
        case 4:
            scanf("%s", args[0]);
            funcionalidade4(args[0]);
            break;
        case 5:
            scanf("%s %s", args[0], args[1]);
            scan_quote_string(args[2]);
            funcionalidade5(args[0], args[1], args[2]);
            break;
        case 6:
            scanf("%s %s", args[0], args[1]);
            scan_quote_string(args[2]);
            funcionalidade6(args[0], args[1], args[2]);
            break;
        case 7:
            scanf("%s %d", args[0], &N);
            funcionalidade7(args[0], N);
            break;
        case 8:
            scanf("%s %d", args[0], &N);
            funcionalidade8(args[0], N);
            break;
        // =============================== SEGUNDO Trabalho Prático ===============================
        case 9:
            scanf("%s %s", args[0], args[1]);
            funcionalidade9(args[0], args[1]);
            break;
        case 10:
            scanf("%s %s", args[0], args[1]);
            funcionalidade10(args[0], args[1]);
            break;
        case 11:
            scanf("%s %s %s", args[0], args[1], args[2]);
            scan_quote_string(args[3]);
            funcionalidade11(args[0], args[1], convertePrefixo(args[3]));
            break;
        case 12:
            scanf("%s %s %s %d", args[0], args[1], args[2], &N);
            funcionalidade12(args[0], args[1], N);
            break;
        case 13:
            scanf("%s %s %d", args[0], args[1], &N);
            funcionalidade13(args[0], args[1], N);
            break;
        case 14:
            scanf("%s %s %d", args[0], args[1], &N);
            funcionalidade14(args[0], args[1], N);
            break;
        // =============================== TERCEIRO Trabalho Prático ===============================
        case 15:
            scanf("%s %s %s %s", args[0], args[1], args[2], args[3]);
            funcionalidade15(args[0], args[1], args[2], args[3]);
            break;
        case 16:
            scanf("%s %s %s %s %s", args[0], args[1], args[2], args[3], args[4]);
            funcionalidade16(args[0], args[1], args[2], args[3], args[4]);
            break;
        case 17:
            scanf("%s %s %s", args[0], args[1], args[2]);
            funcionalidade17(args[0], args[1], args[2]);
            break;
        case 18:
            scanf("%s %s %s", args[0], args[1], args[2]);
            funcionalidade18(args[0], args[1], args[2]);
            break;
        case 19:
            scanf("%s %s %s %s", args[0], args[1], args[2], args[3]);
            funcionalidade19(args[0], args[1], args[2], args[3]);
            break;
    }

    // Liberando memoria heap dos argumentos de cada funcionalidade
    for (int i = 0; i < NUM_ARGS; i++)
        free(args[i]);
    free(args);

    return EXIT_SUCCESS;
}
