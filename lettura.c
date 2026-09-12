#include <stdio.h>

long long dimensione_file(FILE* file){

  if(file == NULL){
	
	perror("Errore d'apertura file");
	reutrn 1;

  }

  fseek(file,0, SEEK_END);
  dimensione_file = ftell(file);
 
  return rewind(file);
}

