#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
struct lot
{
    int time_arv;
    int burst_time;
    int lottery_value;

}s[10];

int ticket[10][10],lottery[10];
int win;
int p = 0;
int size=10;
int max_quantom_value=20;

sem_t s_common;
void random_gen_arv_time(int size,int limit);

//supose all the process at same time
//then generating random brust time 
void rand_gen_burst_time(int size,int limit);

//lottery function 
void lott();

int Total_req();
void Remove_the_node(struct lot *temp);
void *pro(int * loc);
int main()
{
    pthread_t v[10];
    sem_init(&s_common,0,1);
    srand(getpid());

    random_gen_arv_time(10,30);
    rand_gen_burst_time(10,200);
    while(Total_req())
    {
        lott();
        printf("################################## Data ###############################################");
        printf("#######################################################################################");
        printf("  Process Number \t Arrival Time\t Brst Time\t Number Lottery\t Lottery Tickets\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        for(int i=0;i<size;i++)
        {
            printf(" %d \t\t %d \t\t %d \t\t %d \t\t %d to %d\n",i+1,s[i].time_arv,s[i].burst_time,lottery[i],ticket[i][0],ticket[i][0]+lottery[i]);
        }


        win=rand()%p;

        printf("\nLucky winner is  the process number. %d\n\n",win+1); //winner process number 

        int temp1;
        for(int i =0;i<size;i++)
            for(int z=0;z<lottery[i];z++)
                if(ticket[i][z]==win)
                    temp1=i;



        pthread_create(&v[temp1],NULL,pro,(void*) &temp1);
        pthread_join(v[temp1],NULL);




    }
    printf("    **********************--------------------------************************\nterminating the Lottery ( back ) \n****************************--------------*****************************");

}
