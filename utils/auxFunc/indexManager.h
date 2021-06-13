# ifndef INDEX_MANAGER_H
# define INDEX_MANAGER_H

	#define EXIT_FAIL -1

	/**
	 * @brief 
	 * 
	 * @param chaves 
	 * @param n 
	 */
	void insertionSort(int * chaves, int n) 
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

	/**
	 * @brief 
	 * 
	 * @param vec 
	 * @param ini 
	 * @param fim 
	 * @param key 
	 * @return int 
	 */
	int recBinSearch(int *vec, int ini, int fim, int key)
	{
		// CASOS TRIVIAIS
		// se o vetor esta vazio
		if (ini > fim)        // c -->
			return EXIT_FAIL; // r

		int c = (int)(ini + (fim - ini) / 2.0); // 4a

		// se a chave foi encontrada
		if (vec[c] == key) // p + c
			return c;      // r (no caso base)

		// CASO RECURSIVO
		if (vec[c] > key)
			// Ocorre chamada de funcao para metade do vetor + op aritm:
			recBinSearch(vec, ini, c - 1, key); // R(n/2) + a
		else
			// Ocorre chamada de funcao para metade do vetor + op aritm:
			recBinSearch(vec, c + 1, fim, key); // R(n/2) + a
	}


# endif