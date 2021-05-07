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

int main(int agrc, char* argv[])
{
    int funcionalidade = 0;
    scanf("%d",&funcionalidade);
    
    char *arg1 = (char *) malloc (BUFFER);
    char *arg2 = (char *) malloc (BUFFER);
    char *arg3 = (char *) malloc (BUFFER);
    int N; // Utilizado nas funcionalidades 7 e 8 para inserção de novos dados nos arquivos binários de veiculos e linhas

    switch(funcionalidade)
    {
        case 1: // Lê o arquivo .csv para veiculos e cria o arquivo binário de veiculos
            // Recebe o nome do arquivo .csv e o nome do arquivo .bin a ser criado
            // Lembrar da manipulação do campo STATUS no cabeçalho do arquivo

            break;
        case 2: // Lê o arquivo .csv para linhas e cria o arquivo binário de linhas
            // Recebe o nome do arquivo .csv e o nome do arquivo .bin a ser criado
            // Lembrar da manipulação do campo STATUS no cabeçalho do arquivo

            break;
        case 3: // Abre o arquivo .bin de veiculos e exibe todos os dados nele contidos
                // Cada dado deve ser exibido em uma linha diferente e cada <veiculo> será separado por uma linha em branco
                // Para valores nulos exibir: "campo com valor nulo"
            // Recebe o nome do arquivo .bin de veiculos

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

            break;
        case 5: // Abrir o arquivo .bin de veiculos e exibir na tela os veículos que atendem aos critérios de busca enviados
            // Recebe o nome do arquivo .bin de veiculos
            // Recebe o nome do campo a ser buscado
            // Recebe o valor do campo a ser buscado

            break;
        case 6: // Abrir o arquivo .bin de linhas e exibir na tela as linhas de onibus que atendem aos critérios de busca enviados
            // Recebe o nome do arquivo .bin de linhas
            // Recebe o nome do campo a ser buscado
            // Recebe o valor do campo a ser buscado

            break;
        case 7: // Inserção de novos registros no arquivo de entrada .bin de veiculos
            // Recebe o nome do arquivo .bin de veiculos
            // Recebe o número de novos registros a serem inseridos

            break;
        case 8: // Inserção de novos registros no arquivo de entrada .bin de linhas
            // Recebe o nome do arquivo .bin de linhas
            // Recebe o número de novos registros a serem inseridos

            break;
    }

    return EXIT_SUCCESS;
}
