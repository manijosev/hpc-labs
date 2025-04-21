#include<stdio.h>
#include<omp.h>
int main(void){
    int nthreads,tid;
    #pragma omp parallel private(tid),shared(nthreads)
    {
        nthreads = omp_get_num_threads();
        tid = omp_get_thread_num();
        printf("Hello from thread %d out of %d\n", tid,nthreads);
    }

}