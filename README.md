# compressore
Implementazione di un algoritmo di compressione basato codifica Huffman.
Ricerca della divione ottima per la compressione del file.
test vari.
# teorizzazione del problema
Sia F un file di dimensione D, espresso in numero di byte di cui è formato.
Definiamo la complessina di compressione di un file in termini di ricostruzione dell'
albero della codifica Huffman. Limitando la ricostruzione a un numero k.

Capire per quale valore di k in funzione delle dimensioni è meglio analizzare scomporre il file per comprimerlo evitando 
cosi un overhead dovuto alla ricostruzione della codifica.

Definia X come il numero di parti distinte in cui verra scomposto il file e da cui si otterranno le diverse codifiche.

X = D^(1/k) 

Definisco le diverse parti di X come  (X_1, X_2,..,X_k), codifiche di F


Ogni soluzione X_i è un albero della codifica Huffaman della forma (X_i1, X_i2, ..., x_iq), la seguente sequenza di X_ij 
è una coda priorità dove gli elementi sono ordinati per frequenza non crescente.

Definisco il costo di ogni codifica di F come (Y_1,Y_2,..Y_k) 

Ogni Y_i è della forma (x_i1-x_j1+ x_i1-x_(j+1)1...x_i1-x_k1)+(X_i2-X_j2....)+(X_iq-X_jq + ...) con i < j , 1 <= i <= k , 1 <= j <= q ;
 ovvero la somma delle differenze con l'elemento x_i e ogni nuova soluzine successiva 

Y_i = sigma(i=1 -> k (sigma l=1 -> q (sigma j = i + 1 -> k (x_il) -X_jl ))) 

Sia I un istanza dicodifica di F con divisione in k parti, il costo di codifica 
sara S(I) = signam i=1 -> k (Y_i)

Minore sarà la differenza differenza tra le soluzioni più grande sara il valore per ogni Y_i per cui la migliore
configurazione essendo quella che evita gli sprechi è quella con la differenza maggiore tra le soluzioni, perche
preannuncia una significativa variazione dei bit ricorrenti e del testo del file di conseguenza.
Considero il valore assoluto dei risultati trovati per determinare la soluzione migliore.
Continuo la ricerca finche non vi è una divisione del file con valore inferiore alla precedente.
Applico la divisione a file F di dimensione D > m , dove m sara un valore che verra determinato in base al valore 
medio della grandezza dei file e limitato in funzione di quest'ultima per evitare un sovraprocessamento del file.


# Applicazione dell'algoritmo base

Il file bersaglio dovra essere insierito un un'apposita cartella designata come punto di recupero dei file da comprimere,
il nome della cartella è "Cestino_compressore".
Il file compresso si trovera successivamente collocato in una cartella designata col nome "risultato".
Nell'analogo modo il file da decomrprimere dovra essere inserito nell'apposita cartella denominata "Cestino_decompressore".


