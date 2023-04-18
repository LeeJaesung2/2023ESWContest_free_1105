#include <stdio.h>
#include <queue>
#include <iostream>
#include <chrono>

#define NUM_THREADS 2

using namespace std;
using std::chrono::system_clock;

void * consumer(void *arg)
{   
    queue<int>* q = static_cast<queue<int>*>(arg);
    int item;
    auto start = system_clock::now();
    for (int i = 0; i < 1000000; i++) {
        if (q->pop()) { // try to pop item from the queue
            //printf("Popped item %d\n", item);
        } else {
            //printf("Concurrent queue is empty\n");
        }
    }
    auto end = system_clock::now();
    cout << "consumer " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    return 0;
}

void * producer(void *arg)
{
    queue<int>* q = static_cast<queue<int>*>(arg);
    auto start = system_clock::now();
    for (int i = 0; i < 1000000; i++) {
        q->push(i); // push item into the concurrent queue
    }
    auto end = system_clock::now();
    cout << "producer " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    return 0;

}

int main()
{
    void *status;
    int thr_id;

    queue<int> q;
    
    // 각각의 스레드를 생성
    pthread_t threads[NUM_THREADS];

    //첫번째 스레드 생성
    thr_id = pthread_create(&threads[0], NULL, &consumer, (void *)&q);
    if(thr_id < 0){
        perror("failure create thread");
    }

    //두번째 스레드 생성
    thr_id = pthread_create(&threads[1], NULL, &producer, (void *)&q);
    if(thr_id < 0){
        perror("failure create thread");
    }

    
    // 각각의 스레드가 종료될 때까지 대기
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }

    //모든 스레드 종료시 메인스레드 기능
    printf("all of threads are dead");

    return 0;
}