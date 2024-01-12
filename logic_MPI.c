#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>
#include <time.h>
#define EXTRACT_BIT(n,i) ( ((n)&(1<<(i)))?1:0 )
int circuitvalue(short v[]){

        return( (v[0]||v[1]) && (!v[1]||!v[3]) && (v[2]||v[3]) &&
                (!v[3]||!v[4]) && (v[4]||!v[5])&& (v[5]||v[6]) && 
                (v[5]||!v[6]) && (v[7]||!v[8]) && (v[8]||v[9]) && 
                (v[8]||!v[9])&& (!v[9]||!v[10]) && (v[10]||v[11]) && 
                (v[11]||v[9]) && (v[12]||v[13]) && (!v[7]||!v[13])&& 
                (v[13]||!v[14]) && (v[14]||v[15]) && (!v[15]||v[6]) ) ;
}

int check_circuit(int proc_id , int inputval) ;

int main(int argc , char* argv[]){
        struct timeval start, end;
        double elapsed_time;
        int i ;
        int id ;
        int p ;
        int subtotal ;
        int grand_total ;
        gettimeofday(&start, NULL);

        MPI_Init(&argc , &argv) ;
        MPI_Comm_rank(MPI_COMM_WORLD , &id) ;
        MPI_Comm_size(MPI_COMM_WORLD , &p) ;

        subtotal = 0 ;
        for(i=id ; i<65536 ; i+=p){
                subtotal += check_circuit(id,i) ;
                printf("Process %d is processing value %d\n", id, i);
        }

        MPI_Reduce(&subtotal , &grand_total , 1 , MPI_INT,
                                MPI_SUM , 0 , MPI_COMM_WORLD) ;

        
        gettimeofday(&end, NULL);
        elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0;      // 秒到毫秒
        elapsed_time += (end.tv_usec - start.tv_usec) / 1000.0;   // 微秒到毫秒

        MPI_Finalize() ;
        
        if( 0==id ){
                printf("%d\n" , grand_total) ;
                printf("Execution Time: %f milliseconds\n", elapsed_time);
        }

        return 0 ;

}

int check_circuit(int proc_id , int inputval){

        short v[16] ;
        int i ;

        for(i=0 ; i<16 ; i++){
                v[i] = EXTRACT_BIT(inputval,i) ;
        }

        if( circuitvalue(v) ){
                return 1 ;
        }
        else{
                return 0 ;
        }

}
               
