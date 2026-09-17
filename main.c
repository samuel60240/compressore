#include <stdio.h>
#include <stdlib.h>

// Prototipo della funzione di compressione
void* k_lettura_file(int k);

int main(void) {
    int k = 4; // Numero di blocchi in cui dividere il file (es. 4)

    printf("Inizio processo di compressione...\n");

    // Chiamata alla funzione principale
    k_lettura_file(k);

    printf("Compressione completata! Controlla la cartella 'risultato'.\n");

    return 0;
}
