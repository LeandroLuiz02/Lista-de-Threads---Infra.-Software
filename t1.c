// QUESTÃO 2
#include <stdio.h>
#include <pthread.h>

pthread_t trens[10]; // cada trem é representado por uma thread
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[5];
int cruzamentos[5]; //recurso compartilhado
int limite = 2;

void delay(int number_of_seconds) // de geeksforgeeks
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

void * cross(void *threadid) {
    int cont = 0; // indica qual cruzamento será acessado
    while(1) {
        
        while(cruzamentos[cont] >= 2) { // cruzamento cheio
            pthread_cond_wait(&cond[cont], &mutex);
        }
        cruzamentos[cont] ++;
        printf("entrando no cruzamento n: %d\n", cont + 1);
        delay(5000);
        printf("saindo do cruzamento n: %d\n", cont + 1);
        cruzamentos[cont] --;
        pthread_cond_signal(&cond[cont]);
        pthread_mutex_unlock(&mutex);

        cont++;
        if (cont == 5) cont = 0;
    }
}

int main() {
    for (int i = 0; i < 5; i++) {
        pthread_cond_init(&cond[i], NULL); // inicia condição de cada cruzamento
        cruzamentos[i] = 0;
    }

    //criação das threads 
    for (int i = 0; i < 10; i++) pthread_create(&trens[i], NULL, cross, NULL);
    for (int i = 0; i < 10; i++) pthread_join(trens[i], NULL);
    //threads são infinitas, logo não é necessário dar join
}