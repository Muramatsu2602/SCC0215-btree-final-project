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
    
    // Criar a struct para armazenamento temporário dos dados da linha de onibus
    LINHA linha;

    // Ler linha a linha do arquivo csv e inserir no arquivo binário
    // Lembrando que a struct LINHA conterá temporariamente os dados da linha do arquivo CSV lida
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;

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

void funcionalidade3(char *nomeBIN)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivo(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHOV cabVeiculos;
    // Ler o cabeçalho de veiculos do arquivo binário
    lerCabecalhoBINVeiculo(bin, &cabVeiculos);
    
    // Chegar se não há registros no arquivo
    if(feof(bin) || cabVeiculos.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
    }    

    // Agora ler os registros e exibir na tela
    VEICULO veiculo;
    veiculo.modelo = NULL;
    veiculo.categoria = NULL;

    int totalRegistros = cabVeiculos.nroRegistros + cabVeiculos.nroRegRemovidos;

    for(int i=0; i<totalRegistros; i++)
    {
        // Ler o registro
        lerBINVeiculo(bin, &veiculo, FALSE, NULL, NULL);

        // Só exibir os veiculos que não estão marcados logicamente como excluidos
        if(veiculo.removido == '1')
            exibirRegistrosVeiculo(&cabVeiculos, &veiculo);

        // liberando memoria dos campos dinamicos
        free(veiculo.modelo);
        free(veiculo.categoria);
        veiculo.modelo = NULL;
        veiculo.categoria = NULL;
    }

    // Fechando arquivo binário
    fclose(bin);

    return;
}

void funcionalidade4(char *nomeBIN)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivo(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHOL cabLinhas;
    // Ler o cabeçalho de veiculos do arquivo binário
    lerCabecalhoBINLinha(bin, &cabLinhas);

    // Chegar se não há registros no arquivo
    if(feof(bin) || cabLinhas.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
    }    

    // Agora ler os registros e exibir na tela
    LINHA linha;
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;

    int totalRegistros = cabLinhas.nroRegistros + cabLinhas.nroRegRemovidos;

    for(int i=0; i<totalRegistros; i++)
    {
        // Ler o registro
        lerBINLinha(bin, &linha, FALSE, NULL, NULL);

        // Só exibir os veiculos que não estão marcados logicamente como excluidos
        if(linha.removido == '1')
            exibirRegistrosLinha(&cabLinhas, &linha);

        // liberando memoria os campos dinamicos
        free(linha.nomeLinha);
        free(linha.corLinha);
        linha.nomeLinha = NULL;
        linha.corLinha = NULL;
    }

    // Fechando arquivo binário
    fclose(bin);

    return;
}

void funcionalidade5(char *nomeBIN, char *campo, char *valor)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivo(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHOV cabVeiculos;
    // Ler o cabeçalho de veiculos do arquivo binário
    lerCabecalhoBINVeiculo(bin, &cabVeiculos);
    
    // Chegar se não há registros no arquivo
    if(feof(bin) || cabVeiculos.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
    }    

    // Agora ler os registros e exibir na tela
    VEICULO veiculo;
    veiculo.modelo = NULL;
    veiculo.categoria = NULL;

    int totalRegistros = cabVeiculos.nroRegistros + cabVeiculos.nroRegRemovidos;

    for(int i=0; i<totalRegistros; i++)
    {
        // Ler o registro
        if(lerBINVeiculo(bin, &veiculo, TRUE, campo, valor))
        {
            exibirRegistrosVeiculo(&cabVeiculos, &veiculo);
            // liberando memoria dos campos dinamicos
            free(veiculo.modelo);
            free(veiculo.categoria);
            veiculo.modelo = NULL;
            veiculo.categoria = NULL;
        }

        /*// Só exibir os veiculos que não estão marcados logicamente como excluidos e cujo valor do campo escolhido seja igual
        if(veiculo.removido == '1')
        {
            if(strcmp(campo, "prefixo") == 0)
            {
                if(strcmp(veiculo.prefixo, valor) == 0)
                    exibirRegistrosVeiculo(&cabVeiculos, &veiculo);
            }
            else if(strcmp(campo, "data") == 0)
            {
                if(strcmp(veiculo.data, valor) == 0)
                    exibirRegistrosVeiculo(&cabVeiculos, &veiculo);
            }
            else if(strcmp(campo, "quantidadeLugares") == 0)
            {
                if(veiculo.quantidadeLugares == atoi(valor))
                    exibirRegistrosVeiculo(&cabVeiculos, &veiculo);
            }
            else if(strcmp(campo, "modelo") == 0)
            {
                if(strcmp(veiculo.modelo, valor) == 0)
                    exibirRegistrosVeiculo(&cabVeiculos, &veiculo);
            }
            else if(strcmp(campo, "categoria") == 0)
            {
                if(strcmp(veiculo.categoria, valor) == 0)
                    exibirRegistrosVeiculo(&cabVeiculos, &veiculo);
            }
        }*/
    }

    // Fechando arquivo binário
    fclose(bin);

    return;
}

void funcionalidade6(char *nomeBIN, char *campo, char *valor)
{
    // Abrir arquivo binário para leitura
    FILE *bin = abrirArquivo(nomeBIN, FILE_MODE1);
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHOL cabLinhas;
    // Ler o cabeçalho de veiculos do arquivo binário
    lerCabecalhoBINLinha(bin, &cabLinhas);

    // Chegar se não há registros no arquivo
    if(feof(bin) || cabLinhas.nroRegistros == 0)
    {
        printf("Registro inexistente.\n");
    }    

    // Agora ler os registros e exibir na tela
    LINHA linha;
    linha.nomeLinha = NULL;
    linha.corLinha = NULL;

    int totalRegistros = cabLinhas.nroRegistros + cabLinhas.nroRegRemovidos;

    for(int i=0; i<totalRegistros; i++)
    {
        // Ler o registro
        if(lerBINLinha(bin, &linha, TRUE, campo, valor))
        {
            exibirRegistrosLinha(&cabLinhas, &linha);
            // liberando memoria os campos dinamicos
            free(linha.nomeLinha);
            free(linha.corLinha);
            linha.nomeLinha = NULL;
            linha.corLinha = NULL;
        }

        /*// Só exibir os veiculos que não estão marcados logicamente como excluidos e cujo valor do campo escolhido seja igual
        if(linha.removido == '1')
        {
            if(strcmp(campo, "codLinha") == 0)
            {
                if(linha.codLinha == atoi(valor))
                    exibirRegistrosLinha(&cabLinhas, &linha);
            }
            else if(strcmp(campo, "aceitaCartao") == 0)
            {
                if(linha.aceitaCartao == valor[0])
                    exibirRegistrosLinha(&cabLinhas, &linha);
            }
            else if(strcmp(campo, "nomeLinha") == 0)
            {
                if(strcmp(linha.nomeLinha, valor) == 0)
                    exibirRegistrosLinha(&cabLinhas, &linha);
            }
            else if(strcmp(campo, "corLinha") == 0)
            {
                if(strcmp(linha.corLinha, valor) == 0)
                    exibirRegistrosLinha(&cabLinhas, &linha);
            }
        }*/
    }

    // Fechando arquivo binário
    fclose(bin);

    return;
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
        binarioNaTela(arg2);
        break;
    case 2: // Lê o arquivo .csv para linhas e cria o arquivo binário de linhas
        // Recebe o nome do arquivo .csv e o nome do arquivo .bin a ser criado
        // Lembrar da manipulação do campo STATUS no cabeçalho do arquivo
        scanf("%s %s", arg1, arg2);
        funcionalidade2(arg1, arg2);
        binarioNaTela(arg2);
        break;
    case 3: // Abre o arquivo .bin de veiculos e exibe todos os dados nele contidos
        // Cada dado deve ser exibido em uma linha diferente e cada <veiculo> será separado por uma linha em branco
        // Para valores nulos exibir: "campo com valor nulo"
        // Recebe o nome do arquivo .bin de veiculos
        scanf("%s", arg1);
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
        scanf("%s", arg1);
        funcionalidade4(arg1);
        break;
    case 5: // Abrir o arquivo .bin de veiculos e exibir na tela os veículos que atendem aos critérios de busca enviados
        // Recebe o nome do arquivo .bin de veiculos
        // Recebe o nome do campo a ser buscado
        // Recebe o valor do campo a ser buscado
        scanf("%s %s", arg1, arg2);
        scan_quote_string(arg3);
        funcionalidade5(arg1, arg2, arg3);
        break;
    case 6: // Abrir o arquivo .bin de linhas e exibir na tela as linhas de onibus que atendem aos critérios de busca enviados
        // Recebe o nome do arquivo .bin de linhas
        // Recebe o nome do campo a ser buscado
        // Recebe o valor do campo a ser buscado
        scanf("%s %s", arg1, arg2);
        scan_quote_string(arg3);
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
