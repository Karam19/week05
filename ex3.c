#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4
int buf[N];
int c = 0;

int* prod_lock;
int* cons_lock;

int* create_lock() {
  int* ret = malloc(sizeof(int));
  *ret = 0;
  return ret;
}

void sleep(int* cond) {
  *cond = 1;
  while (*cond) {
    pthread_yield();
  }
}

void wakeup(int* cond) { *cond = 0; }

void* producer(void* a) {
  int i;
  for (i = 0; 1; ++i) {
    if (c >= N) {
      sleep(prod_lock);
    }

    buf[c] = i;
    ++c;
    if (c >= 1) {
      wakeup(cons_lock);
    }
  }

  return 0;
}

void* comsumer(void* a) {
  for (;;) {
    if (c < N - 1) wakeup(prod_lock);
    if (c == 0) sleep(cons_lock);

    int item = buf[c - 1];
    --c;

    printf("Item: %d; c: %d\n", item, c);
  }

  return 0;
}

int main(int argc, char const* argv[]) {
  prod_lock = create_lock();
  cons_lock = create_lock();

  pthread_t prod_t;
  pthread_t cons_t;
  pthread_create(&cons_t, 0, comsumer, 0);
  pthread_create(&prod_t, 0, producer, 0);

  pthread_join(prod_t, 0);
  pthread_join(cons_t, 0);
  
  // the code starts running normally but after some time a race condition happens and force the code to runs into deadlock ecause there are no synchronization primitives
  // eventually the consumer and calls sleep() (because c = 0)
  // the producer fills the buffer and sleeps as well, but the consumer haven't called sleep() yet 
  // after that both processes wait for each other (deadlock)
  return 0;
}