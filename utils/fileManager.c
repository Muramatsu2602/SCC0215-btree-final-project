/**
 * @file fileManager.c
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fileManager.h"
#include "../registry/linha.h"
#include "../registry/veiculo.h"

/**
 * @brief Le uma linha ate o \0
 * 
 * @param stream 
 * @return char* 
 */
char *readLine(FILE *stream)
{
    char *str = NULL;
    int position = 0;
    // allocating one by at a time
    do
    {
        str = realloc(str, sizeof(char) * (position + 1));
        // reads a caracter from stream then stores it in str[position]
        str[position] = (char)fgetc(stream);
        // moving forward
        position++;
        // verifies if we got to the end of the line

        if (str[position - 1] == '\r')
            str[position - 1] = '\0';

    } while (str[position - 1] != '\n' && !feof(stream));

    // erasing the included '\n'
    str[position - 1] = '\0';

    return str;
}

/**
 * @brief Abre um arquivo
 * 
 * @param filename 
 * @param mode 
 * @return FILE* 
 */
FILE *abrirArquivo(const char *filename, const char *mode)
{
    FILE *fp = NULL;

    if ((fp = fopen(filename, mode)) != NULL)
        return fp;

    printf("Erro na abertura do arquivo de nome %s!\n", filename);
    return NULL;
}

/**
 * @brief deleta o arquivo permanentemente
 * 
 * @param filename 
 * @return int 
 */
int removerArquivo(const char *filename)
{
    if (!filename)
        return EXIT_FAILURE;

    int res = 0;

    if ((res = remove(filename)) == 0)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

/**
 * @brief preenche caracteres vazios (ou lixo) com @, no formato '/0@@@@@
 * 
 * @param tamOcupado 
 * @param tamMaximo 
 * @param str 
 * @return boolean 
 */
boolean preenchendoLixo(int tamOcupado, int tamMaximo, char *str)
{
    if (!str)
        return FALSE;

    for (int i = tamOcupado; i < tamMaximo; i++)
        str[i] = (char)'@';

    return TRUE;
}

/**
 * @brief Compara arquivos binarios. Usado para correcao 
 * 
 * @param nomeArquivoBinario 
 */
void binarioNaTela(char *nomeArquivoBinario)
{ /* Você não precisa entender o código dessa função. */

    /* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
    *  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb")))
    {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++)
    {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}

/**
 * @brief altera campo status do arquivo para 1 e da fclose(), indicando que este está consistente
 * 
 * @param fp 
 * @return boolean 
 */
boolean fecharArquivo(FILE **fp)
{
    if (!fp)
        return FALSE;

    const boolean status = TRUE;

    fseek(*fp, 1, SEEK_SET);

    if (!fwrite(&status, sizeof(boolean), 1, *fp))
        return FALSE;

    fclose(*fp);

    return TRUE;
}