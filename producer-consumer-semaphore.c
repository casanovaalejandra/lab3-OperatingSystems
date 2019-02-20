#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE  25



int p;
int c;
int bffer[BUFFER_SIZE];
int counter;


sem_t *mutex;
sem_t *empty;
sem_t *full ;



void insert(int item){
   // sleep(2);
    sem_wait(empty);
    sem_wait(mutex);
//printf("full %d\n", full->value+1);
    bffer[p] = item;
    p = (p + 1) % BUFFER_SIZE;
    sem_post(mutex);
    sem_post(full);    

}
int remove_item(){
    //sleep(4);
    int next_consumed = BUFFER_SIZE;
    sem_wait(full);
    sem_wait(mutex);
//printf("empty: %d\n", empty->value+1);
    next_consumed = bffer[c];
    c = (c + 1) % BUFFER_SIZE;
    sem_post(mutex); 
    sem_post(empty);
    return next_consumed;
}
void * producer(void * param){
   int item;
   while(1){
      item = rand() % BUFFER_SIZE ;
      insert(item);
      printf("inserted: %d\n", item);
   }
}
void * consumer(void * param){
   int item;
   while(1){
   item = remove_item();
   printf("removed: %d\n", item);
   }
}
int main(int argc, char * argv[])
{

    mutex = malloc(sizeof(sem_t));
    full = malloc(sizeof(sem_t));
    empty = malloc(sizeof(sem_t));
    counter = 0;
    c = 0;
    p = 0;
    if (argc != 3) {
    fprintf(stderr, "usage: a.out <producers> <consumers>\n");
    return -1;
    }
    if (atoi(argv[1]) < 0 || atoi(argv[2]) < 0 ) {
       fprintf(stderr, "Arguments must be non-negative\n ");
       return -1;
    }

    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);

    sem_init(mutex, 1, 1);

    sem_init(empty, 1, BUFFER_SIZE);

    sem_init(full, 1,  0);

    pthread_t *prods = malloc(sizeof(pthread_t)* producers);
    pthread_t *cons = malloc(sizeof(pthread_t)*consumers);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int i=0;
    for(i=0;i<producers;i++)
        pthread_create(&prods[i], &attr, producer, NULL);
    for(i=0;i<consumers;i++)
        pthread_create(&cons[i], &attr, consumer, NULL);

    sleep(1);
    


    //define threads..mutexes..
    //launch producers
    //launch consumers
}
