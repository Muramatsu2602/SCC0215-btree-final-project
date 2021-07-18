/**
 * @file merge.h
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 *
 * @version 0.1
 * @date 2021-07-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef MERGE_H
#define MERGE_H

    #include "../utils/fileManager.h"
    #include "../registry/linha.h"
    #include "../registry/veiculo.h"
    #include "../index/index.h"

    void freeCamposDinamicos(char ** campo1, char ** campo2);
    int comparadorVeiculo(const void *p, const void *q);
    int comparadorLinha(const void *p, const void *q);
    void copiarCamposVeiculo(VEICULO *veiculoOrigem, VEICULO *veiculoDestino);
    void copiarCamposLinha(LINHA *linhaOrigem, LINHA *linhaDestino);
    void ordenarVeiculos(FILE *binVeiculoOrdenado, VEICULO *veiculos, int totalRegistroVeiculos);
    void ordenarLinhas(FILE *binLinhaOrdenado, LINHA *linhas, int totalRegistrosLinhas);
    void juncoesLoop(FILE *binVeiculo, FILE *binLinha, FILE *binIndex, CABECALHOV *cabVeiculos, CABECALHOL *cabLinhas, CABECALHOI *cabIndex, int flag);

#endif