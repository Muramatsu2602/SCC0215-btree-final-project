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
    #include <math.h>

    #define TRUE 1
    #define FALSE 0
    #define boolean int
    #define FILE_MODE1 "rb"
    #define FILE_MODE2 "r"

    char *readLine(FILE *stream);
    void binarioNaTela(char *nomeArquivoBinario);
    
#endif