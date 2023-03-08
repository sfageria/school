/* ------------------------
SHUBH FAGERIA
ASSIGNMENT 3
CS3305
251005738
------------------------- */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

//global variables for all threads to access including 
//      count of all primes accross threads
//      share of maxNum that each thread has
volatile int numPrimes = 0;
pthread_mutex_t mutex;
unsigned long long eachThread;
unsigned long long remain;
int numThreads;

//function to determine if argument n is a prime number
int isPrime(unsigned long long n) {

    //return false when n is 0,1,4
    if (n == 0 || n == 1 || n == 4) {
        return 0;
    }

    //return true when n is 5
    if (n == 5) {
        return 1;
    }

    //for the rest of the numbers
    //loop through all integers from n to n/2, dividing n by said num
    for (unsigned long long i = 2; i < n/2; i++) {

        //if there is no remainder in the division, the number is not prime and false is returned
        if (n % i == 0) {
            //printf("\n number %d is not prime as %d / %d is whole", n,n,i);
            return 0;
        }
    }

    //otherwise, return true
    return 1;
}

//function for each thread to calculate their share of prime sum and counts
//takes in thread number as parameter
void * threadFunc(void * arg) {

    
    //calculating start and end numbers for this thread based on the thread number
    //and the global variable 'eachThread' holding the partitioned share
    int threadNum = *(int *) arg;
    unsigned long long startNum = threadNum * eachThread;
    unsigned long long endNum = startNum + eachThread ;
    //printf("each thread: %d\n", eachThread);

    //adding on the remainder (0 or 1) to the last thread
    if (threadNum == numThreads) {
        endNum += remain;
    }

    unsigned long long int threadSum = 0;
    unsigned long long threadCount = 0;

    printf("Thread # %d is finding primes from low = %llu to high = %llu\n", threadNum, startNum, endNum - 1);

    //for loop iterating between the threads start and end numbers
    for (unsigned long long i = startNum; i < endNum; i++) {

        //check if num is prime
        if (isPrime(i) == 1) {
            //add to global prime sum and count
            //addToSum(i);

            //implement mutex lock so thread can modify global variable prime num count without interference
            pthread_mutex_lock(&mutex);
            numPrimes += 1;
            pthread_mutex_unlock(&mutex);
            
            //add to local prime sum and count (personal to each thread)
            threadSum += i;
            threadCount += 1;
        }
    }

    printf("Sum of thread %d is %llu, Count is %llu\n", threadNum, threadSum, threadCount);
    *(unsigned long long int *)arg = threadSum;

    return arg;
}

int main( int argc, char* argv[])
{
    
    int maxNum;
    pthread_mutex_init(&mutex,NULL);

    //exit if proper arguments are not supplied
    if (argc != 3) {
        printf("Please enter the proper command line arguments\n");
        return 0;
    }

    //retrieve command line arguments
    numThreads = atoi(argv[1]);
    maxNum = atoi(argv[2]);

    //calculate the share per thread based on the number of threads to split into 
    eachThread = trunc(maxNum / numThreads); 
    remain = maxNum - eachThread;

    //init threads
    pthread_t worker_thread[numThreads];
    //int threadNumber[numThreads];

    //create number of threads supplied in command line arguments 
    for (int i = 0; i < numThreads; i++) {
        int * start = malloc(sizeof(int));
        * start = i;
        pthread_create(&worker_thread[i], NULL, &threadFunc, start);

    }

    //wait for threads finish
    unsigned long long int totalSum = 0;
    for (int i = 0; i < numThreads; i++) {
        unsigned long long int * primeSum;
        pthread_join(worker_thread[i], (void**)&primeSum);
        totalSum += *primeSum;
        free(primeSum);

    }

    printf("\n        GRAND SUM IS %llu, COUNT IS %d\n", totalSum, numPrimes);

    pthread_mutex_destroy(&mutex);
    return 0;
}

