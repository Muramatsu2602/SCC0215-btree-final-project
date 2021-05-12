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

    while (lerLinhaCSVVeiculo(csv, &veiculos))
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

    // fechando arquivos
    fclose(csv);
    fclose(bin);
    return;
}

void funcionalidade2(char *nomeCSV, char *nomeBIN)
{
    // Abrir arquivo CSV para leitura
    FILE *csv = abrirArquivo(nomeCSV, FILE_MODE2);
    if (csv == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Escrever o cabeçalho do arquivo binário de linhas
    CABECALHOL cabLinhas;
    if (!lerCabecalhoCSVLinha(csv, &cabLinhas))
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
    if (!escreveCabecalhoBINLinhas(bin, &cabLinhas))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Criar a struct para armazenamento temporário dos dados da linhas onibus
    LINHA linhas;

    // Ler linha a linha do arquivo csv e inserir no arquivo binário
    // Lembrando que a struct LINHA conterá temporariamente os dados da linha do arquivo CSV lida
    linhas.nomeLinha = NULL;
    linhas.corLinha = NULL;

    while (lerLinhaCSVLinha(csv, &linhas))
    {
        if (!escreverBINLinha(bin, &linhas))
        {
            printf("Falha no processamento do arquivo.\n");
            return;
        }
    }

    // limpando a mem heap
    free(linhas.nomeLinha);
    free(linhas.corLinha);

    // fechando arquivos
    fclose(csv);
    fclose(bin);
    return;
}

void funcionalidade3(char *nomeBIN)
{
}

void funcionalidade4(char *nomeBIN)
{
}

void funcionalidade5(char *nomeBIN, char *campo, char *valor)
{
}

void funcionalidade6(char *nomeBIN, char *campo, char *valor)
{
}

void funcionalidade7(char *nomeBIN, int N)
{
}

void funcionalidade8(char *nomeBIN, int N)
{
}

int main(int agrc, char *argv[])
{
    int funcionalidade = 0;
    scanf("%d ", &funcionalidade);

    char *arg1 = (char *)malloc(BUFFER);
    char *arg2 = (char *)malloc(BUFFER);
    char *arg3 = (char *)malloc(BUFFER);
    int N = 0; // Utilizado nas funcionalidades 7 e 8 para inserção de novos dados nos arquivos binários de veiculos e linhas

    switch (funcionalidade)
    {
    case 1: // Lê o arquivo .csv para veiculos e cria o arquivo binário de veiculos
        // Recebe o nome do arquivo .csv e o nome do arquivo .bin a ser criado
        // Lembrar da manipulação do campo STATUS no cabeçalho do arquivo
        scanf("%s %s", arg1, arg2);
        funcionalidade1(arg1, arg2);
        binarioNaTela(arg1);
        break;
    case 2: // Lê o arquivo .csv para linhas e cria o arquivo binário de linhas
        // Recebe o nome do arquivo .csv e o nome do arquivo .bin a ser criado
        // Lembrar da manipulação do campo STATUS no cabeçalho do arquivo
        scanf("%s %s", arg1, arg2);
        funcionalidade2(arg1, arg2);
        // binarioNaTela(arg1);
        break;
    case 3: // Abre o arquivo .bin de veiculos e exibe todos os dados nele contidos
        // Cada dado deve ser exibido em uma linha diferente e cada <veiculo> será separado por uma linha em branco
        // Para valores nulos exibir: "campo com valor nulo"
        // Recebe o nome do arquivo .bin de veiculos

        funcionalidade3(arg1);
        break;
    case 4: // Abre o arquivo .bin de linhas e exibe todos os dados nele contidos
        // Cada dado deve ser exibido em uma linha diferente e cada <linha> será separado por uma linha em branco
        // Para valores nulos exibir: "campo com valor nulo"
        // Recebe o nome do arquivo .bin de linhas
        /*
                Importante!! Com relação ao campo aceitaCartao, mostrar por extenso o tipo:
                    S - PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR
                    N - PAGAMENTO EM CARTAO E DINHEIRO
                    F - PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA
            */

        funcionalidade4(arg1);
        break;
    case 5: // Abrir o arquivo .bin de veiculos e exibir na tela os veículos que atendem aos critérios de busca enviados
        // Recebe o nome do arquivo .bin de veiculos
        // Recebe o nome do campo a ser buscado
        // Recebe o valor do campo a ser buscado

        funcionalidade5(arg1, arg2, arg3);
        break;
    case 6: // Abrir o arquivo .bin de linhas e exibir na tela as linhas de onibus que atendem aos critérios de busca enviados
        // Recebe o nome do arquivo .bin de linhas
        // Recebe o nome do campo a ser buscado
        // Recebe o valor do campo a ser buscado

        funcionalidade6(arg1, arg2, arg3);
        break;
    case 7: // Inserção de novos registros no arquivo de entrada .bin de veiculos
        // Recebe o nome do arquivo .bin de veiculos
        // Recebe o número de novos registros a serem inseridos

        funcionalidade7(arg1, N);
        break;
    case 8: // Inserção de novos registros no arquivo de entrada .bin de linhas
        // Recebe o nome do arquivo .bin de linhas
        // Recebe o número de novos registros a serem inseridos

        funcionalidade8(arg1, N);
        break;
    }
    // Liberando memoria heap dos campos lidos
    free(arg1);
    free(arg2);
    free(arg3);

    return EXIT_SUCCESS;
}
