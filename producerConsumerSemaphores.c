#include <pthread.h> //pthread_create(), pthread_exit(),pthread_join()
#include <stdio.h>   //printf(), sprintf(), perror()
#include <stdlib.h> //srand(), exit(), EXIT_FAILURE,EXIT_SUCCESS
#include <semaphore.h> //sem_wait(), sem_post()...

#define BUFFER_SIZE 50
int numberOfProducers;
int numberOfComsumers;
int buffer[BUFFER_SIZE];
int next_in,next_out,currentSize;

sem_t mutex;
sem_t filled;
sem_t empty;

void insert(int item){
  //founte:geeksForGeeks producer-consumer problem using semaphores
//  while(currentSize>=BUFFER_SIZE);
  sem_wait(&empty);
  sem_wait(&mutex);
 // if(currentSize<BUFFER_SIZE){
  buffer[next_in]= item;
  currentSize++;
//  printf("Insert: element to insert:%d\n",item);
  next_in = (next_in+1)%BUFFER_SIZE;
 // printf("DESPUES DE INSERT:next_in = %d\n",next_in);
//}
  sem_post(&mutex);
  sem_post(&filled);

}

int remove_item(){
 // while(currentSize<=0);
  sem_wait(&filled);
  sem_wait(&mutex);
 int item;
// if(currentSize>0){
 item = buffer[next_out];
//printf("Element to remove: %d",item);
 next_out = (next_out+1)%BUFFER_SIZE;
 //printf("DESPUES DE REMOVE:next_out = %d\n",next_out);
//}
 sem_post(&mutex);
 sem_post(&empty);
 return item;
}
void * producer(void * param){
   printf("VIVO producer\n");
   int item;
   while(1){
      item = rand() % BUFFER_SIZE ;
      insert(item);
      printf("inserted: %d\n", item);
   }
}
void * consumer(void * param){
   printf("inside the consumer method\n");
   int item;
   while(1){
   item = remove_item();
   printf("removed: %d\n", item);
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
    int i;
    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);
    pthread_t tids_producers[producers];//array to create the same ammount of threads as consumers and producers
    pthread_t tids_consumers[consumers];
    next_in = 0;
    next_out = 0;
    //initialization of semaphores
    sem_init(&mutex,1,1);
    sem_init(&empty,1,BUFFER_SIZE);
    sem_init(&filled, 1, 0);
    for(i=0;i<producers;i++){
	pthread_attr_t attr;
        pthread_attr_init(&attr);
   	pthread_create(&tids_producers[i],&attr,producer,NULL);
    }
    for (i=0;i<consumers;i++){
   	 pthread_attr_t attr;
   	 pthread_attr_init(&attr);
   	 pthread_create(&tids_consumers[i],&attr,consumer,NULL);
    }

   for (i =0; i<producers; i++){
   	 pthread_join(tids_producers[i],NULL);
    }
  for(i = 0;i<consumers;i++){
    	pthread_join(tids_consumers[i],NULL);
    }
}


