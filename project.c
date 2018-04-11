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

//supose all the process at same time
//then generating arrival time in random
void random_gen_arv_time(int size,int limit)
{
    for(int i=0;i<size;i++)
    {
        /*int temp=(rand()%limit)+1;
        s[i].time_arv=temp;*/

        s[i].time_arv=0;
    }
}
//supose all the process at same time
//then generating random brust time 
void rand_gen_burst_time(int size,int limit)
{
    for(int i=0;i<size;i++)
    {
        int temp=(rand()%limit)+1;
        s[i].burst_time=temp;
    }
}
//lottery function 
void lott()
{

    p=0;
    for (int i = 0; i < size; i++) 
    {
        if ((s[i].burst_time > 0)) 
        {
            lottery[i] = s[i].burst_time / 20;
            if ((lottery[i] == 0) && (s[i].burst_time > 0))
                lottery[i] = 1;
        }       
        else
            lottery[i]=0;

        for (int z = 0; z < lottery[i]; z++) 
        {
            ticket[i][z] = p++;
        }

    }

            
}

int Total_req()
{
    int sum=0;
    for(int i=0;i<size;i++)
        sum+=s[i].burst_time;
    return sum;
}
//remove the node of the function
void Remove_the_node(struct lot *temp)
{
    int i=0;
    while(&s[i]!=temp)
        i++;

    if(i!=size)
        while(i<size-1)
        {   s[i].time_arv=s[i+1].time_arv;
            s[i].burst_time=s[i+1].burst_time;
            s[i].lottery_value=s[i+1].lottery_value;
            i++;
        }
    else
        size--;
}

void *pro(int * loc)
{
    int temp1=*loc;
    printf("Process %d is being executed \n",temp1+1);
    sem_wait(&s_common);
    printf("entering critical section of %d \n",temp1+1);
    
    printf(" %d \t\t %d \t\t %d \n",temp1+1,s[temp1].time_arv,s[temp1].burst_time,lottery[temp1]);

    if ((s[temp1].burst_time > 0))  
    {
        s[temp1].burst_time -= max_quantom_value;
        lottery[temp1]=0;
        if (s[temp1].burst_time < 0) 
        {
            s[temp1].burst_time=0;
            printf(" %d \t\t %d \t\t %d \n",temp1+1,s[temp1].time_arv,s[temp1].burst_time,lottery[temp1]);
            Remove_the_node(&s[temp1]);
            size--;
        }
        else
            printf(" %d \t\t %d \t\t %d \n",temp1+1,s[temp1].time_arv,s[temp1].burst_time,lottery[temp1]);

        printf("exiting critical section of %d \n",temp1+1);
        sem_post(&s_common);
        pthread_exit(NULL);
    }

}
//main function
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
