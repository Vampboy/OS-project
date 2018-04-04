//lets startthe game :)
//NEW Commit
#include<stdio.h>
#include<semaphore.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

struct tm * timeinfo;
int stud_size=0,tech_size=0;

struct que
{
    int priority;
    int bt_time;
    int r_time;
    int ar_time;
    char person_name[20];
    int turn;
};

struct que stud_que[10],tech_que[10];

void gettime()
{
    time_t rawtime;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

}
int check=0;
void check_time()
{
    gettime();
    if(timeinfo->tm_hour>=10 && timeinfo->tm_hour<12)
    {
        check=1;
    }
}



void print_data(struct que a[],int size)
{

for(int i=0;i<size;i++)
    {
        printf("%d %d %d %s\n",a[i].priority,a[i].ar_time,a[i].bt_time,a[i].person_name);
    }


}

int min_av_student=1000,stud_loc,min_turn=100;
void pro_min_student()
{min_av_student=1000;
    min_turn=1;
    for(int i=0;i<stud_size;i++)
    {
        if(min_av_student>stud_que[i].ar_time && min_turn>=stud_que[i].turn)
        {
                min_av_student=stud_que[i].ar_time;
                stud_loc=i;
                min_turn=stud_que[i].turn;
        }

    }

   // cout<<stud_que[stud_loc].person_name<<endl;
}

int min_av_teacher=1000,tech_loc;
void pro_min_teacher()
{
    min_av_teacher=1000;
    min_turn=1;
    for(int i=0;i<tech_size;i++)
    {
        if(min_av_teacher>tech_que[i].ar_time && min_turn>=stud_que[i].turn)
        {
                min_av_teacher=tech_que[i].ar_time;
                tech_loc=i;
                min_turn=tech_que[i].turn;
                printf("yoo\n");
        }

    }
    //cout<<tech_que[tech_loc].person_name<<endl;
}
int quantom=20,size;

void remove_element(struct que * temp)
{
    int i=0;
    if(temp->priority==2)
    {   
        while(&stud_que[i]!=temp)
            i++;

        if(i!=stud_size)
            while(i<stud_size-1)
            {   stud_que[i].ar_time=stud_que[i+1].ar_time;
                stud_que[i].bt_time=stud_que[i+1].bt_time;
                i++;
            }
        //else
            stud_size--;
    }
    if(temp->priority==1)
    {   
        while(&tech_que[i]!=temp)
            i++;

        if(i!=tech_size)
            while(i<tech_size-1)
            {   tech_que[i].ar_time=tech_que[i+1].ar_time;
                tech_que[i].bt_time=tech_que[i+1].bt_time;
                i++;
            }
        //else
            tech_size--;
    }

}

void *pro(struct que *temp)
{
    printf("%s your turn is here \n",temp->person_name);    
    

        sleep(2);
    if ((temp->bt_time > 0))  
    {
        temp->bt_time -= quantom;
        if (temp->bt_time < 0) 
        {
            temp->bt_time=0;
            printf("%s quere is completly executed :\n",temp->person_name);
            remove_element(temp);
            //size--;
        }
        else
        {
            printf("%d",temp->bt_time);
            printf("%s quere is to big Wait for Your next turn\n",temp->person_name);
            temp->turn++;
        
        }
        sleep(2);
       
    }
     pthread_exit(NULL);
}

int main()
{
    pthread_t p1;

    //check_time();
    check=1;
    if(check==1)
    {
        int flag1=1;
        printf("logging into quere system:\n");
        while(flag1)
        {
            char name[20],position[20];
            int arival_time,burst_time;

            printf("Enter the details in the quere form:\n");
            printf("Enter your name: ");
            scanf("%s",name);
            printf("Enter your position(student/teacher): ");
            scanf("%s",position);
            printf("Enter your arrival time: ");
            scanf("%d",&arival_time);
            printf("Enter your quere time needed: ");
            scanf("%d",&burst_time);

            struct que *temp;

            if(strcmp(position,"student")==0||strcmp(position,"STUDENT")==0)
            {
                temp=&stud_que[stud_size];
                printf("helo\n");
                stud_size++;
                temp->priority=2;
            }
            else if(strcmp(position,"teacher")==0||strcmp(position,"TEACHER")==0)
            {
                temp=&tech_que[tech_size];
                printf("yoo\n");
                tech_size++;
                temp->priority=1;
            }

            temp->ar_time=arival_time;
            temp->bt_time=burst_time;
            temp->r_time=burst_time;
            strcpy(temp->person_name,name);
            temp->turn=0;


            printf("add another form(Y/N)\n");
            char ch;
            scanf("%c",&ch);
            scanf("%c",&ch);
            if(ch=='y'||ch=='Y')
            flag1=1;
            else
            flag1=0;            
                     

        }
        print_data(stud_que,stud_size);
        print_data(tech_que,tech_size);
        printf("%d\n",stud_size);
        printf("%d\n",tech_size);
        

        printf("Wait till we call your name: \n");

        pro_min_student();
        pro_min_teacher();

        
        while(tech_size!=0||stud_size!=0)
        {
            printf("%d %d \n",min_av_student,min_av_teacher);

            if(min_av_student<min_av_teacher)
            {
                pthread_create(&p1,NULL,pro,(void *)&stud_que[stud_loc]);
                pthread_join(p1,NULL);
                //stud_size--;
            }
            else 
            {
                pthread_create(&p1,NULL,pro,(void *)&tech_que[tech_loc]);
                pthread_join(p1,NULL);
               // tech_size--;

            }

             print_data(stud_que,stud_size);
            print_data(tech_que,tech_size);
            printf("%d\n",stud_size);
            printf("%d\n",tech_size);
            pro_min_student();
            pro_min_teacher();

        }
    }
    else
        printf("Cannt login during this time of day\n ");

}