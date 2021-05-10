/**
 * @file fileManager.h
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef FILEMANAGER_H
    #define FILEMANAGER_H

    #include <stdio.h>
    #include <ctype.h>

    #define TRUE 1
    #define FALSE 0
    #define boolean int

    #define FILE_MODE1 "rb+"
    #define FILE_MODE2 "r+"
    #define FILE_MODE3 "wb+"

    #define BUFFER 50

    // Manipulando Arquivos
    char *readLine(FILE *stream);
    FILE *abrirArquivo(const char *filename, const char *mode);
    int removerArquivo(const char *filename);
    boolean preenchendoLixo(int tamOcupado, int tamMaximo, char *str);
    boolean fecharArquivo(FILE **fp);

    // Debugging
    void binarioNaTela(char *nomeArquivoBinario);

#endif