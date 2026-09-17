#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
  printf("funziono1\n");
  FILE* file_in = fopen("Cestino_compressore/proton-recovery-phrase.pdf", "rb");
  if (file_in == NULL) {
    perror("Errore d'apertura file input");
    return NULL;
  }
  printf("funziono2\n");
  FILE* file_out = fopen("risultato/compresso/file_compresoo.txt", "wb");
  if (file_out == NULL) {
    perror("Errore d'apertura file output!");
    fclose(file_in);
    return NULL;
  }

  int d_file = dimensione_file(file_in);
  int dimensione_blocco = d_file / k;
  int repeat = 0;

  while (repeat < k) {
      printf("funziono%d\n",repeat);
    int v_caratteristico[256] = {0};

    long offset_inizio = (long)repeat * dimensione_blocco;
    long limite_blocco = (repeat == k - 1) ? d_file : (offset_inizio + dimensione_blocco);
    int byte_da_leggere = limite_blocco - offset_inizio;

    // 1. Posizionamento e conteggio frequenze nel blocco corrente
    fseek(file_in, offset_inizio, SEEK_SET);
    int ch;
    while (ftell(file_in) < limite_blocco && (ch = fgetc(file_in)) != EOF) {
      v_caratteristico[(unsigned char)ch]++;
    }

    // 2. Creazione della lista ordinata (coda di priorità)
    CODA_PRIORITA *testa = NULL;
    for (int l = 0; l < 256; l++) {
      if (v_caratteristico[l] > 0) {
        CODA_PRIORITA* nuovo = malloc(sizeof(CODA_PRIORITA));
        nuovo->value = (unsigned char)l;
        nuovo->occorrenze = v_caratteristico[l];
        nuovo->leaf = true;
        nuovo->left = NULL;
        nuovo->right = NULL;
        nuovo->next = NULL;
        nuovo->prev = NULL;

        // Inserimento ordinato per occorrenze
        if (testa == NULL || nuovo->occorrenze < testa->occorrenze) {
          nuovo->next = testa;
          if (testa) testa->prev = nuovo;
          testa = nuovo;
        } else {
          CODA_PRIORITA* curr = testa;
          while (curr->next != NULL && curr->next->occorrenze <= nuovo->occorrenze) {
            curr = curr->next;
          }
          nuovo->next = curr->next;
          if (curr->next) curr->next->prev = nuovo;
          curr->next = nuovo;
          nuovo->prev = curr;
        }
      }
    }

    if (testa == NULL) {
      repeat++;
      continue;
    }

    // 3. Costruzione dell'albero di Huffman
    while (testa != NULL && testa->next != NULL) {
      CODA_PRIORITA* primo = testa;
      CODA_PRIORITA* secondo = testa->next;

      testa = secondo->next;
      if (testa) testa->prev = NULL;

      CODA_PRIORITA* padre = malloc(sizeof(CODA_PRIORITA));
      padre->value = 0;
      padre->occorrenze = primo->occorrenze + secondo->occorrenze;
      padre->leaf = false;
      padre->left = primo;
      padre->right = secondo;

      primo->bit = 0;
      secondo->bit = 1;

      // Inserimento del padre nella coda mantenendo l'ordine
      if (testa == NULL || padre->occorrenze < testa->occorrenze) {
        padre->next = testa;
        if (testa) testa->prev = padre;
        testa = padre;
      } else {
        CODA_PRIORITA* curr = testa;
        while (curr->next != NULL && curr->next->occorrenze <= padre->occorrenze) {
          curr = curr->next;
        }
        padre->next = curr->next;
        if (curr->next) curr->next->prev = padre;
        curr->next = padre;
        padre->prev = curr;
      }
    }

    CODA_PRIORITA* radice = testa;

    // 4. Ripristino posizione file all'inizio del blocco per la codifica
    fseek(file_in, offset_inizio, SEEK_SET);

    // 5. Conversione e scrittura del blocco
    buffer_converter(radice, byte_da_leggere, file_in, file_out);

    repeat++;
  }

  fclose(file_in);
  fclose(file_out);
  return NULL;
}


// Esplorazione ricorsiva dell'albero per trovare il codice
static bool trova_codice(CODA_PRIORITA* nodo, unsigned char codice, unsigned char *bit_seq, int *profondita) {
  if (nodo == NULL) return false;

  if (nodo->leaf && nodo->value == codice) {
    return true;
  }

  if (nodo->left) {
    *bit_seq = (*bit_seq << 1) | 0;
    (*profondita)++;
    if (trova_codice(nodo->left, codice, bit_seq, profondita)) return true;
    *bit_seq >>= 1;
    (*profondita)--;
  }

  if (nodo->right) {
    *bit_seq = (*bit_seq << 1) | 1;
    (*profondita)++;
    if (trova_codice(nodo->right, codice, bit_seq, profondita)) return true;
    *bit_seq >>= 1;
    (*profondita)--;
  }

  return false;
}


unsigned char esplorazione(CODA_PRIORITA* nodo, unsigned char codice, int *profondita) {
  unsigned char exit = 0;
  *profondita = 0;
  trova_codice(nodo, codice, &exit, profondita);
  if (*profondita < 8) {
    exit <<= (8 - *profondita);
  }
  return exit;
}


void *buffer_converter(CODA_PRIORITA *albero, int d, FILE *in, FILE *out) {
  if (d <= 0 || albero == NULL) return NULL;

  unsigned char *bytes = malloc(d);
  unsigned char *byte_out = malloc(d);
  if (!bytes || !byte_out) {
    free(bytes);
    free(byte_out);
    return NULL;
  }

  memset(byte_out, 0, d);

  size_t bytes_letti = fread(bytes, 1, d, in);
  int j = 0;
  int bit_liberi = 8;

  for (size_t i = 0; i < bytes_letti; i++) {
    int m = 0;
    unsigned char exit = esplorazione(albero, bytes[i], &m);

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
        if (j < d) byte_out[j] = 0;
      } else {
        bit_liberi -= m;
        m = 0;
      }
    }
  }

  // Scrittura finale dei byte rimanenti
  if (j > 0 || bit_liberi < 8) {
    if (bit_liberi < 8) {
      unsigned char maschera = (unsigned char)(0xFF << bit_liberi);
      byte_out[j] &= maschera;
      j++;
    }
    fwrite(byte_out, 1, j, out);
  }

  free(bytes);
  free(byte_out);
  return NULL;
}
