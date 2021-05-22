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

#include "utils/fileManager.h"
#include "registry/linha.h"
#include "registry/veiculo.h"

/**
 * @brief Realiza a leitura do arquivo CSV e cria e insere um arquivo binário contendo o cabeçalho e os veiculos lidos.
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

    return;
}

/**
 * @brief Realiza a leitura do arquivo CSV e cria e insere um arquivo binário contendo o cabeçalho e as linhas de ônibus lidas.
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

    return;
}

/**
 * @brief Realiza a recuperação dos dados de todos os registros armazenados no arquivo BINÁRIO de veiculos e os exibe de maneira organizada.
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
 * @brief Realiza a recuperação dos dados de todos os registros armazenados no arquivo BINÁRIO de linhas de ônibus e os exibe de maneira organizada.
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
 * @brief Faz a recuperação de dados de todos os registros do arquivo BINÁRIO que satisfaçam um critério de busca determinado pelo usuário.
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
 * @brief Faz a recuperação de dados de todos os registros do arquivo BINÁRIO que satisfaçam um critério de busca determinado pelo usuário.
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
 * @brief Permite a inserção de N novos registros no arquivo binário de veiculos. Utiliza da função scan_quote_string para a leitura dos campos.
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

        if(veiculo.modelo != NULL) 
        {
            free(veiculo.modelo);
            veiculo.modelo = NULL;
        }   
        if(veiculo.categoria != NULL)
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

        if(linha.nomeLinha != NULL)
        {
            free(linha.nomeLinha);
            linha.nomeLinha = NULL;
        }
        if(linha.corLinha != NULL)
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

int main(int agrc, char *argv[])
{
    int funcionalidade = 0;
    scanf("%d ", &funcionalidade);

    // arg1, arg2 e arg3 são auxiliares que recebem os argumentos de cada funcionalidade do programa
    char *arg1 = (char *)malloc(BUFFER);
    char *arg2 = (char *)malloc(BUFFER);
    char *arg3 = (char *)malloc(BUFFER);
    int N = 0; // Utilizado nas funcionalidades 7 e 8 para inserção de novos dados nos arquivos binários de veiculos e linhas

    switch (funcionalidade)
    {
    case 1:
        scanf("%s %s", arg1, arg2);
        funcionalidade1(arg1, arg2);
        binarioNaTela(arg2);
        break;
    case 2:
        scanf("%s %s", arg1, arg2);
        funcionalidade2(arg1, arg2);
        binarioNaTela(arg2);
        break;
    case 3:
        scanf("%s", arg1);
        funcionalidade3(arg1);
        break;
    case 4:
        scanf("%s", arg1);
        funcionalidade4(arg1);
        break;
    case 5:
        scanf("%s %s", arg1, arg2);
        scan_quote_string(arg3);
        funcionalidade5(arg1, arg2, arg3);
        break;
    case 6:
        scanf("%s %s", arg1, arg2);
        scan_quote_string(arg3);
        funcionalidade6(arg1, arg2, arg3);
        break;
    case 7:
        scanf("%s %d", arg1, &N);
        funcionalidade7(arg1, N);
        break;
    case 8:
        scanf("%s %d", arg1, &N);
        funcionalidade8(arg1, N);
        break;
    }

    // Liberando memoria heap dos argumentos de cada funcionalidade
    free(arg1);
    free(arg2);
    free(arg3);

    return EXIT_SUCCESS;
}
