# ifndef INSERTION_SORT_H
# define INSERTION_SORT_H

void insertionSort(int *chaves, int n) 
{ 
    int aux;
    for(int i = 1; i < n; i++)
    {
    	// Selecionar o primeiro elemento não ordenado
    	aux = chaves[i];

    	// Selecionar o elemento anterior
    	int j = i - 1;

    	// Enviar o primeiro não ordenado para trás até que ele esteja na posição correta
    	while(j >= 0 && chaves[j] > aux)
    	{
    		chaves[j + 1] = chaves[j];
    		j--;
    	}
    	chaves[j + 1] = aux;
    }
}

# endif