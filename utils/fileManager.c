/**
 * @file fileManager.c
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @brief funcoes para manipulacao de arquivos binarios e outra funcoes auxiliares
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

    return NULL;
}

/**
 * @brief igual ao abrirArquivo mas muda campo status para '0' enquanto o arq binario esta aberto
 * 
 * @param filename 
 * @param mode 
 * @return FILE* 
 */
FILE *abrirArquivoBin(const char *filename, const char *mode)
{
    FILE *fp = NULL;
    char status = 0;

    if ((fp = fopen(filename, mode)) != NULL)
    {
        // Verificar se o arquivo foi fechado anteriormente com sucesso
        fread(&status, sizeof(char), 1, fp);
        
        if(status == '0')
        {
            // O arquivo não foi fechado anteriormente com sucesso
            fclose(fp);
            return NULL;
        }
        
        // Se foi fechado anteriormente com sucesso, mudar o campo status para 0 e prosseguir

        status = '0';

        fseek(fp, 0, SEEK_SET);
        fwrite(&status, sizeof(char), 1, fp);

        // voltando para o inicio do arquivo
        fseek(fp, 0, SEEK_SET);

        return fp;
    }

    // printf("Erro na abertura do arquivo de nome %s!\n", filename);
    return NULL;
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
    if (str == NULL)
        return FALSE;

    // preenche com @ a quantidade de caracteres da diferenca entre tamMaximo e tamOcupado
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

    int64 i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb")))
    {
        // fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
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
        cs += (int64)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}

/**
 * @brief altera campo status do arquivo para 1 e da, indicando que este está consistente
 * 
 * @param fp 
 * @return boolean 
 */
boolean fecharArquivoBin(FILE **fp)
{
    if (!*fp)
        return FALSE;

    const char status = '1';

    // indo ao inicio do arquivo binario
    fseek(*fp, 0, SEEK_SET);

    // atualizar status pra 1 no arquivo antes de fecha-lo
    if (!fwrite(&status, sizeof(char), 1, *fp))
        return FALSE;

    fclose(*fp);

    return TRUE;
}

/**
 * @brief 
 *		Use essa função para ler um campo string delimitado entre aspas (").
 *		Chame ela na hora que for ler tal campo. Por exemplo:
 * 	
 *		A entrada está da seguinte forma:
 *			nomeDoCampo "MARIA DA SILVA"
 *	
 *		Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
 *			scanf("%s", str1); // Vai salvar nomeDoCampo em str1
 *			scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
 * 
 * @param str 
 */
void scan_quote_string(char *str)
{

    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n')
    { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    }
    else if (R == '\"')
    {
        if (scanf("%[^\"]", str) != 1)
        { // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar(); // ignorar aspas fechando
    }
    else if (R != EOF)
    { // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    }
    else
    { // EOF
        strcpy(str, "");
    }
}