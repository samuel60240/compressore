#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "strutture.h"

// Prototipi
void *buffer_converter(CODA_PRIORITA *albero, int d, FILE *in, FILE *out);
int dimensione_file(FILE* file);
void* k_lettura_file(int k);
unsigned char esplorazione(CODA_PRIORITA* nodo, unsigned char codice, int *profondita);


int dimensione_file(FILE* file) {
  fseek(file, 0, SEEK_END);
  int v_dimensione_file = (int) ftell(file);
  rewind(file);
  return v_dimensione_file;
}


void* k_lettura_file(int k) {
  // NOTA: "Cestino_compressore/*.*" non è supportato da fopen ANSI C standard.
  // Specifica il nome esatto del file da aprire.
  FILE* file_in = fopen("Cestino_compressore/input.bin", "rb");

  if (file_in == NULL) {
    perror("Errore d'apertura file input");
    return NULL;
  }

  FILE* file_out = fopen("risultato/file_compresoo.txt", "wb");
  if (file_out == NULL) {
    perror("Errore d'apertura file output!");
    fclose(file_in);
    return NULL;
  }

  int d_file = dimensione_file(file_in);
  int repeat = 0;

  while (repeat < k) {
    // Dimensione corretta a 256 per coprire tutti i valori unsigned char (0-255)
    unsigned char v_caratteristico[256] = {0};

    int l = 0;

    CODA_PRIORITA *list = malloc(sizeof(CODA_PRIORITA));
    if (list == NULL) {
      perror("Errore allocazione memoria");
      break;
    }
    list->next = NULL;
    list->prev = NULL;
    CODA_PRIORITA *radice = NULL;

    while (l <= 255) {
      if (list->prev == NULL && v_caratteristico[l] > 0 && list->next == NULL) {
        list->occorrenze = v_caratteristico[l];
        list->value = l;
      }

      // Ciclo in avanti
      while (v_caratteristico[l] > 0 && v_caratteristico[l] < list->occorrenze) {
        if (list->next == NULL) {
          CODA_PRIORITA* aus = malloc(sizeof(CODA_PRIORITA));
          aus->occorrenze = v_caratteristico[l];
          aus->value = l;
          aus->next = NULL;
          aus->prev = list;
          list->next = aus;
        } else {
          list = list->next;
        }
      }

      // Ciclo all'indietro
      while (v_caratteristico[l] > 0 && v_caratteristico[l] > list->occorrenze) {
        if (list->prev == NULL) {
          CODA_PRIORITA* aus = malloc(sizeof(CODA_PRIORITA));
          aus->occorrenze = v_caratteristico[l];
          aus->value = l;
          aus->next = list;
          aus->prev = NULL;
          list->prev = aus;
        } else {
          list = list->prev;
        }
      }
      l++;

      while (list->next != NULL) list = list->next; // Torna alla fine

      // Inserimento in ordine
      CODA_PRIORITA* nodo = malloc(sizeof(CODA_PRIORITA));
      if (list != NULL) {
        nodo->left = list;
        list->leaf = true;
        list->bit = false;

        if (list->prev != NULL) {
          list = list->prev;
          nodo->right = list;
          list->leaf = true;
          list->bit = true;
          nodo->leaf = false;
          nodo->value = 0;
          nodo->occorrenze = nodo->left->occorrenze + nodo->right->occorrenze;
        }
      }

      // Implementazione Huffman
      while (list->prev != NULL) {
        list = list->prev;
        if (list->occorrenze > nodo->occorrenze) {
          CODA_PRIORITA* aus = malloc(sizeof(CODA_PRIORITA));
          aus->right = list;
          list->leaf = true;
          list->bit = true;
          aus->left = nodo;
          nodo->bit = false;
          aus->occorrenze = nodo->occorrenze + list->occorrenze;
          aus->leaf = false;
          nodo = aus;
        } else {
          CODA_PRIORITA* aus = malloc(sizeof(CODA_PRIORITA));
          aus->left = list;
          list->leaf = true;
          list->bit = false;
          aus->right = nodo;
          nodo->bit = true;
          aus->occorrenze = nodo->occorrenze + list->occorrenze;
          aus->leaf = false;
          nodo = aus;
        }
      }
      radice = nodo;
    }

    // Conversione e scrittura del blocco
    buffer_converter(radice, (d_file + k - 1) / k, file_in, file_out);
    repeat++;
  }

  fclose(file_in);
  fclose(file_out);
  return NULL;
}


unsigned char esplorazione(CODA_PRIORITA* nodo, unsigned char codice, int *profondita) {
  unsigned char exit = 0;
  *profondita = 0;

  while (nodo->right->value != codice && nodo->left->value != codice) {
    if (nodo->right->leaf == true) {
      exit = (exit << 1) | nodo->left->bit;
    } else {
      exit = (exit << 1) | nodo->right->bit;
    }
    (*profondita)++;
  }
  exit = (exit << (7 - *profondita));
  return exit;
}


void *buffer_converter(CODA_PRIORITA *albero, int d, FILE *in, FILE *out) {
  CODA_PRIORITA* copia = albero;
  unsigned char bytes[d];
  unsigned char byte_out[d]; // Dimensione esatta d (senza +1 per '\0')
  size_t bytes_letti = 0;

  int j = 0;
  int bit_liberi = 8;

  // Azzera il primo byte di output
  if (d > 0) byte_out[0] = 0;

  while ((bytes_letti = fread(bytes, 1, d, in)) > 0) {
    for (int i = 0; i < bytes_letti; i++) {
      int m = 0;
      unsigned char exit = esplorazione(copia, bytes[i], &m);

      while (m > 0) {
        byte_out[j] |= (exit >> (8 - bit_liberi));

        if (m >= bit_liberi) {
          exit <<= bit_liberi;
          m -= bit_liberi;
          j++;
          bit_liberi = 8;

          if (j == d) {
            fwrite(byte_out, 1, d, out);
            j = 0;
          }
          byte_out[j] = 0; // Azzera la nuova cella del buffer
        } else {
          bit_liberi -= m;
          m = 0;
        }
      }
    }
  }

  // Scrittura finale dei byte rimanenti
  if (j > 0 || bit_liberi < 8) {
    if (bit_liberi < 8) {
      unsigned char maschera = (unsigned char)(0xFF << bit_liberi);
      byte_out[j] &= maschera;
      j++; // Include l'ultimo byte parziale nel totale da scrivere
    }

    fwrite(byte_out, 1, j, out);
  }

  return NULL;
}
