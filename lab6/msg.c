#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<string.h>

int main(int argc, char ** argv){
    int rank,ierror,size,tag=0;
    ierror = MPI_Init(&argc,&argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD,&size);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    MPI_Status status;
    char msg[13] = "Hello World!";
    char rec_buf[13];
    if(rank ==0){
        ierror = MPI_Send(msg,13,MPI_CHAR,1,tag,MPI_COMM_WORLD);
    }
    if(rank==1){
        ierror = MPI_Recv(rec_buf,13,MPI_CHAR,0,tag,MPI_COMM_WORLD,&status);
        printf("Process 1 recevied %s \n",rec_buf);
    }
    ierror=MPI_Finalize();
    return ierror;
}