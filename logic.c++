#include <stdio.h>
#include <time.h>

bool circuit(bool* in){
        return (in[0] || in[1]) && (!in[1] || !in[3]) && (in[2] || in[3]) &&
               (!in[3] || !in[4]) && (in[4] || !in[5]) && (in[5] || in[6]) &&
               (in[5] || !in[6]) && (in[7] || !in[8]) && (in[8] || in[9]) &&
               (in[8] || !in[9]) && (!in[9] || !in[10]) && (in[10] || in[11]) &&
               (in[11] || in[9]) && (in[12] || in[13]) && (!in[7] || !in[13]) &&
               (in[13] || !in[14]) && (in[14] || in[15]) && (!in[15] || in[6]);

}

void to_bool(int num, bool* v){
        int mask=0b01;
        int count=0;
        while(count !=16){
                v[count] = ((num & mask) !=0);
                count++;
                mask <<=1;
        }
}

int main(){
        clock_t start=clock();

        bool v[16];
        int limit = 1 << 16;
        for (int t=0 ; t<100; t++){
                int count=0;
                for(int i=0;i<limit;i++){
                        to_bool(i,v);
                        if(circuit(v)){
                                count++;
                        }
                }
                printf("%d\n", count);
                break;
        }
        

        clock_t end=clock();
        double time = ((double)(end-start)/CLOCKS_PER_SEC);
        printf("Running time: %lf\n",time);

}