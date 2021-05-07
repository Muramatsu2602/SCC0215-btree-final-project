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

    return NULL;
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