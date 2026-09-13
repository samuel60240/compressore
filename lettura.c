#include <stdio.h>
#include <stdlib.h>
#include "strutture.h"
#include <dirent.h>

int dimensione_file(FILE* file){

  if(file == NULL){
	
	perror("Errore d'apertura file");
	return 1;

  }

  fseek(file,0, SEEK_END);
  int v_dimensione_file = (int) ftell(file);
  rewind(file);
  return v_dimensione_file;
}

char* k_lettura_file(int k, FILE* g,long long d){

    unsigned char v_caratteristico[d/k];
    unsigned char c;
    FILE* out = fopen("risultato/compresso/*.*","rb");

    for(int j = 0; j < k; j++){
      for (unsigned int i = 0; i < d/k;i ++){
        v_caratteristico[c]++;

      }
      //inserimento in ordine(lista)

      //NODO* radice = codifica_HUFFMAN(CODA_PRIORITA* lista);

      //codifica HUFFman   O(n)
      //scrivo codifica
      //scrivo file BInario
      //azzero vettore
      //libero albero
    }

}
CODA_PRIORITA* lista_ordinata(char* vettore_caratteristico){

  CODA_PRIORITA* coda = malloc(sizeof(CODA_PRIORITA));

  coda->value = 0;
  coda->occorrenze = vettore_caratteristico[0];
  coda->next = NULL;

  int k = 1;
  while(k < 256){

    CODA_PRIORITA* aus = coda;

    while(aus->occorrenze > vettore_caratteristico[k] && aus->next == NULL ){

    }
  }
}




NODO* codifica_HUFFMAN(FILE* file){
  NODO* nodo = malloc(sizeof(NODO));

}
