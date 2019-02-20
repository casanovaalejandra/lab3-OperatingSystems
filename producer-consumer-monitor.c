#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE  50



    int p = 0;
    int c = 0;

    int nfull = 0;
    pthread_mutex_t has_full_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t has_empty_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t has_empty_cond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t has_full_cond = PTHREAD_COND_INITIALIZER;
    int bffer[BUFFER_SIZE];
    
    


    







int remove_item(){
        if(nfull < BUFFER_SIZE/3)
           sleep(6);
       
    while(nfull <= 0)
        pthread_cond_wait(&has_full_cond, &has_full_mutex);

    
    int consumed = bffer[c];
    c = (c + 1) % BUFFER_SIZE;
    nfull--;
    printf("removed: %d\n", nfull);
    pthread_cond_signal(&has_empty_cond);
    return consumed;
}


void insert(int item){
    if(nfull > BUFFER_SIZE/3)
        sleep(6);

    while(nfull >= BUFFER_SIZE)
        pthread_cond_wait(&has_empty_cond, &has_empty_mutex);


    bffer[p] = item;
    p = (p + 1) % BUFFER_SIZE;
    nfull++;
    printf("inserted %d\n", nfull);
    pthread_cond_signal(&has_full_cond);
}

void * producer(void * param){
   int item;
   while(1){
      item = rand() % BUFFER_SIZE ;
      insert(item);
     // printf("inserted: %d\n", nfull);
   }
}
void * consumer(void * param){
   int item;
   while(1){
   item = remove_item();
   //printf("removed: %d\n", nfull);
   }
}
int main(int argc, char * argv[])
{


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




    pthread_t *prods = malloc(sizeof(pthread_t)* producers);
    pthread_t *cons = malloc(sizeof(pthread_t)*consumers);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int i=0;
    for(i=0;i<producers;i++)
        pthread_create(&prods[i], &attr, producer, NULL);
    for(i=0;i<consumers;i++)
        pthread_create(&cons[i], &attr, consumer, NULL);
    sleep(150);
    exit(0);


    //define threads..mutexes..
    //launch producers
    //launch consumers
}
