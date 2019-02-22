#include <pthread.h> //pthread_create(), pthread_exit(),pthread_join()
#include <stdio.h>   //printf(), sprintf(), perror()
#include <stdlib.h> //srand(), exit(), EXIT_FAILURE,EXIT_SUCCESS

//define variables
#define BUFFER_SIZE 50
int numberOfProducers;
int numberOfComsumers;
int buffer[BUFFER_SIZE];
int next_in,next_out,currentSize;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER; //initialization of the mutex variab$

void insert(int item){
//  pthread_mutex_lock(&mymutex); //when inserting to the buffer we lock
//  while(currentSize>=BUFFER_SIZE);
  pthread_mutex_lock(&mymutex); //when inserting to the buffer we lock
  if(currentSize<BUFFER_SIZE && ((next_in+1)%BUFFER_SIZE)!=next_out){
  buffer[next_in]= item;
  next_in = (next_in+1)%BUFFER_SIZE;
  currentSize++;
  }pthread_mutex_unlock(&mymutex); //unlock the mutex
}

int remove_item(){

// pthread_mutex_lock(&mymutex);
 int item;
 //while(currentSize<=0);
 pthread_mutex_lock(&mymutex);
 if(currentSize>0 && next_out!=next_in){
 item = buffer[next_out];
 next_out = (next_out+1)%BUFFER_SIZE;
 currentSize--;
 }
 pthread_mutex_unlock(&mymutex);
 return item;
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
