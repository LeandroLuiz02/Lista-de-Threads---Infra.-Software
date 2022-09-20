#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

#define LIMITE_ALGARISMOS 20
#define N 4
#define T 3
#define C 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t vote_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex do array de votos (necessário??)
pthread_t threads[T];

// RECURSOS COMPARTILHADOS
FILE *arquivos[N];
int disponivel[N]; // 0 para arqivo disponivel para leitura e 1 para já aberto
int votos[C+1]; // array contendo o total de votos que cada candidato recebeu
int filesLeft = 1; // recebe 0 se todos os arquivos forem abertos 

int stringToInt(char *string) {
    int size = strlen(string);
    int total = 0;

    for (int i = 0; i < size; i++) {
        total += (string[i] - 48) * pow(10, size - i - 1);
    }

    return total;
}

void *count() {
    while(filesLeft) { // talvez isso seja perigoso
        int file; // identificador do arquivo a ser aberto
        while(pthread_mutex_trylock(&mutex) != 0){;}
        for (int i = 0; i < N; i++) {
            if (disponivel[i] == 0) {
                disponivel[i] = 1;
                file = i;
                if (i == N - 1) filesLeft = 0;
                break;
            }
        }
        pthread_mutex_unlock(&mutex);

        // Abrindo o arquivo
        char fileName[LIMITE_ALGARISMOS]; 
        sprintf(fileName, "%d.txt", file+1);
        arquivos[file] = fopen(fileName, "r");

        // contando votos
        while(pthread_mutex_trylock(&vote_mutex) != 0) {;}
        int voto;
        while(fscanf(arquivos[file], "%d", &voto) > 0) {votos[voto]++;}
        pthread_mutex_unlock(&vote_mutex);
    }
}

int main() {
    for (int i = 0; i < C + 1; i++) votos[i] = 0;

    for (int i = 0; i < T; i++) pthread_create(&threads[i], NULL, count, NULL);
    for (int i = 0; i < T; i++) pthread_join(threads[i], NULL);
    for (int i = 1; i < C + 1; i++) {
        printf("candidato %d, votos: %d\n", i, votos[i]);
    }

    pthread_exit(NULL);
}