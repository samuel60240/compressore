#include <stdio.h>
#include <stdlib.h>
#include "strutture.h"
#include <dirent.h>
#include <string.h>
char path[255];if(list->prev==NULL && v_caratteristico[l] > 0){
  list->occorrenze=v_caratteristico[l];
  list->value = l;




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

//comprime il file in k versioni di HUFFMAN trees inizio: (numero di k)(lunghezza file)(coda)(file codificato)(...)(coda)(file)
char* k_lettura_file(int k,FILE* file,long long d){


    unsigned char v_caratteristico[d/k];
    unsigned char c;


      int l = 0;
      CODA_PRIORITA *list = malloc(sizeof(CODA_PRIORITA));
      list->next=NULL;
      list->prev=NULL;

      while (l <= 255) {
        //caso iniziale
        if  (list->prev==NULL && v_caratteristico[l] > 0 && list->next==NULL) {
          list->occorrenze=v_caratteristico[l];
          list->value = l++;
        }
        //ciclo in avanti
        while (v_caratteristico[l]>0 && v_caratteristico[l] < list->occorrenze) {
            if(list->next==NULL){
              CODA_PRIORITA* aus = malloc(sizeof(CODA_PRIORITA));
              aus->occorrenze = v_caratteristico[l];
              aus->value = l;
              aus->next = NULL;
              aus->prev = list;
              list->next = aus;

            }else{
              list=list->next;

            }
        }
        //ciclo all'indietro'
        while (v_caratteristico[l]>0 && v_caratteristico[l] > list->occorrenze) {
          if(list->prev==NULL){
            CODA_PRIORITA* aus = malloc(sizeof(CODA_PRIORITA));
            aus->occorrenze = v_caratteristico[l];
            aus->value = l;
            aus->next = list;
            aus->prev = NULL;
            list->prev = aus;
          }else{
            list=list->prev;

          }
        }
        l++;

      }
      while (list->next!=NULL) list=list->next;//torna a capo
      //inserimento in ordine(lista)
      NODO* nodo=malloc(sizeof(NODO));

      while(list->prec!=NULL){
        nodo->left = list;
        list->bit = 0;
        list=list->prev;
        nodo->right = list;
        list = list -> prev;

      }
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
