/*#######################################################################################################################################################
 CSE522 Real-time Embedded Systems – Assignment 2 by Group 2 (Hena Shah ( ASU ID: 1213348511) & Nidhi Dubey ( ASU ID: 1213031246))

In this code we have programmed the scheduling algorithms for Earliest Deadline First, Rate Monotic and Deadline Monotonic. Many printf has been commented to increase the execution time for the program. Only if thas task-set is schedulable or not is being reported in EDF and loding table is not being printed. Also in RM and DM whether or not each task is schedulable or not is being reported. 
######################################################################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#define MIN(x,y) ((x<y)?x:y)
int i,j,k,h,l,x,y,temp1,cnt,n;
float curr_len,curr,exec=0;
float last_len,busy_per_len;
float temp;
int sched,sched_check;
float task_util, t_sum, sum_util, next_sum,rando,power;
int count_edf[10];
int count_rm[10];
int count_dm[10];
int count_no[10];
// For compution busy period length
float busy_period_calc(int last_len,float array[],int taskno)
{
	float len;
	float total_len=0;


	for (i=0;i<3*taskno;i=i+3)
	{
	len=(float)last_len/(float)array[i+2];
	curr_len=ceil(len);
	total_len=total_len + (curr_len*array[i]);
	}

return total_len;
}
 
/*The below function is of earliest deadline first. Utilization for each task in found and if the total utilization is less than one than the task set is schedulable. If utilization is greater than one we find the loading factor for the busy period. If loading factor becomes greater than 1 for any instance than taskset is not schedulable or else it is schedulable. For the taskset which is schedulable we increment the count_edf for the give utilization of taskset */

void edf_density(float array[],int taskno,int task_utili)
{
	printf("\n\n---------------------------------------------------EDF analysis begins here --------------------------------------------- \n");
	float util=0;
	sched=0;
	int i;
	for(i=0;i<3*taskno;i=i+3)
	{
	if(array[i+1]<=array[i+2])
	util=util+(array[i]/array[i+1]);
	else
	util=util+(array[i]/array[i+2]);
	}

	//printf("\nDensity Utilisation is %0.2f ",util);
	if(util<=1)
	{
	sched=1;	
			switch(task_utili)
				{
				case 5:
					count_edf[0]=count_edf[0]+1;
					break;
				case 15:
					count_edf[1]=count_edf[1]+1;
					break;
				case 25:
					count_edf[2]=count_edf[2]+1;
					break;
				case 35:
					count_edf[3]=count_edf[3]+1;
					break;
				case 45:
					count_edf[4]=count_edf[4]+1;
					break;
				case 55:
					count_edf[5]=count_edf[5]+1;
					break;
				case 65:
					count_edf[6]=count_edf[6]+1;
					break;
				case 75:
					count_edf[7]=count_edf[7]+1;
					break;
				case 85:
					count_edf[8]=count_edf[8]+1;
					break;
				case 95:
					count_edf[9]=count_edf[9]+1;
					break;
				}
	printf("\nAs checked by EDF Task set is schedulable by density analysis");
	}
	else
	printf("\nDensity utilisation is greater than 1. Checking for load factory analysis");
}


// Feasibility analysis for EDF
void feasibility(float array[],int taskno,int task_utili)
{
	int ch=0;
	float temp1;
	float temp2;

	edf_density(array,taskno,task_utili);

	if(sched==0)
	{

	last_len=0;
	for(i=0;i<taskno*3;i=i+3)
	{
		last_len=last_len+array[i];
	}
	//printf("\n last length %f",last_len);
	curr_len=busy_period_calc(last_len,array,taskno);

	while(curr_len!=last_len)
	{
		last_len=curr_len;
		curr_len=busy_period_calc(last_len,array,taskno);
		//printf("\n curr length %f",curr_len);
		//printf("\n last length %f",last_len);
	}

	busy_per_len=curr_len;
	printf("\nBusy period length %0.2f",busy_per_len);

	int r=taskno,c=busy_per_len;
	float *arr = (float *)malloc(r * c * sizeof(float));

	printf("\n");


	// computing deadline for all the tasks
	for (i=0;i<r;i++)
	{	if(array[(i*3)+1]<=busy_per_len)
		*(arr + i*c + 0)=array[(i*3)+1];

	}

	for (i=0;i<r;i++)
	{
	for(j=1;j<c;j++)
	{
		temp1=*(arr + i*c + (j-1))+array[(i*3)+2];
		if(temp1<=busy_per_len)
		{
		*(arr + i*c + j)=temp1;
		}
		else
		break;
	}
	}
  	int count=0;
	// print taskset deadlines
	//printf("\nDeadline matrix\n");
	for (i=0;i<r;i++)
	{
	for(j=0;j<c;j++)
	{
		if(*(arr + i*c + j)!=0)
		{
		//printf("%0.2f \t",*(arr + i*c + j));
		count++;
		}
	}
	//printf("\n");

	}
	//printf("\ncount %d",count);

	k=0;
	float mat[count][2];
	float load[count];
	while(k<count)
	{
	for (i=0;i<r;i++)
	{
	for(j=0;j<c;j++)
	{
		if(*(arr + i*c + j)!=0)
		{
			mat[k][0]=*(arr + i*c + j);
			 mat[k][1]=array[(i*3)];
			k++;
		}
	}
	}
	}


        for (i = 0; i < count;i++)
        {
            for (j = i + 1; j < count;j++)
            {
                if (mat[i][0] > mat[j][0])
                {
                     temp1=  mat[i][0];
		     temp2=mat[i][1];

                    mat[i][0] =mat[j][0];
		    mat[i][1] =mat[j][1];
                    mat[j][0] =temp1;
                    mat[j][1] =temp2;

                }
	    }
	}


	//printf("\nFinal loading factor matrix:\n");
	//printf("\nDeadline\t  h\t  Loading factor\n");
	// To computer loading factor
	
			for(i=0;i<count;i++)
			{

				exec=0;
				curr= mat[i][0];
				// To check for all tasksets in the time limit t2-t1
				for (j=0;j<=i;j++)
					{
					if(curr>=mat[j][0])
						{
							exec=exec+mat[j][1];
						}
					}

				load[i]=(float)exec/(float)curr; // calc loading factor
				if(load[i]>1)
				{
				ch=1;
				break;
				}
				//printf("%0.2f\t\t%0.2f\t\t%0.2f\n",mat[i][0],exec,load[i]);
			}


	if(ch==0)
	{
				switch(task_utili)
				{
				case 5:
					count_edf[0]=count_edf[0]+1;
					break;
				case 15:
					count_edf[1]=count_edf[1]+1;
					break;
				case 25:
					count_edf[2]=count_edf[2]+1;
					break;
				case 35:
					count_edf[3]=count_edf[3]+1;
					break;
				case 45:
					count_edf[4]=count_edf[4]+1;
					break;
				case 55:
					count_edf[5]=count_edf[5]+1;
					break;
				case 65:
					count_edf[6]=count_edf[6]+1;
					break;
				case 75:
					count_edf[7]=count_edf[7]+1;
					break;
				case 85:
					count_edf[8]=count_edf[8]+1;
					break;
				case 95:
					count_edf[9]=count_edf[9]+1;
					break;
				}
	printf("\nTaskset is schedulable in EDF");
	}
	else
	{
	printf("\nTaskset is not schedulable in EDF");
	printf("\nDeadline\t  h\t  Loading factor\n");
	printf("%0.2f\t\t%0.2f\t\t%0.2f\n",mat[i][0],exec,load[i]);
	}

//printf("\n ---------------------------------------------------EDF analysis ends here --------------------------------------------- \n");
}
}

/*The below function is of rate monotonic. Over here first the array each arranged in increasing period, in other words the task with smallest period is given highest priority.After rearranging we compare it with the array that would be formed when array of arranged in increasing order of density. If order of task for both the array is same we go for normal RM analysis or else we go for non RM analysis. In the RM analysis the utilization for each task is found by taking the density in consideration. For the task for which utilization is greater than the upper bound Real Time analysis is done. If the analysis converges at time less than the deadline for the particular task, the task is schedulable. This procedure is done for the whole taskset, and if the taskset is schedulable then we increment the count_rm for the utilization assigned for the taskset. 
In case of non RM the utilization for the task set is compared with the deadline of the task under test. If period is smaller we simply with utilization for that task and if period is greated than deadline is find utilization by dividing the execution time for the task by minimun of deadline and period for task under test. If utilization is greater than the upper bound we do Real Time analysis for the task under test.If the analysis converges at time less than the deadline for the particular task, the task is schedulable. This procedure is done for the whole taskset, and if the taskset is schedulable then we increment the count_rm for the utilization assigned for the taskset.  */

//Rate Monotonic response time analysis
void compute_rm(float array[],int taskno,int task_utili)
{
printf("\n ---------------------------------------------------RM analysis begins here --------------------------------------------- \n");
	//to set all task with increasing priority with respect to period
	float temp,min2,min1;
	int z,p,m,flag_rm;
	flag_rm=0;
	float utilization=0,power,cond,task,execution=0,deadline,period;
	int i;
	float execution2=0;
	int countno=0;
	for(z=0;z<3*taskno;z=z+3)
	{
		for(p=z+3;p<3*taskno;p=p+3)
		{
			if(array[z+2]>array[p+2])
			{
				for(m=0;m<3;m++)
				{
					temp=array[z+m];
					array[z+m]=array[p+m];
					array[p+m]=temp;
				}
			}
		}
	}

	for(z=0;z<3*taskno;z=z+3)
	{
		min1=MIN(array[z+1],array[z+2]);
		if(flag_rm==0)
		{
		for(p=z+3;p<3*taskno;p=p+3)
		{
			min2=MIN(array[p+1],array[p+2]);
			if(min1>min2)
			{

					printf("\nDensity order doesn't match going for NON RM");
					flag_rm=1;
					break;
			}

		}
	}

	}

	if(flag_rm==1)
	{
		sched_check=1;
		utilization=array[0]/MIN(array[1],array[2]);
		//printf("\n\nUpper Bound for task 1 = 1.00");
		//printf("\nUtilisation for task 1 = %0.2f",utilization);
		if(utilization>1)
		{
			printf("\nNOT Schedulable in NON RM for 1 task as Utilization bound is greater than 1");
			sched_check=0;

		}
		if(sched_check==1)
		{

		
		for(i=1;i<taskno;i++)
		{
		countno=0;
		z=3*i;

		deadline=array[z+1];
		period=array[z+2];
		utilization=array[z]/MIN(array[z+1],array[z+2]);

		for(z=0;z<3*(i);z=z+3)
		{
			if(array[z+2]<=deadline)
			{

				utilization=utilization+ array[z]/MIN(array[z+1],array[z+2]);
				countno++;
			}

			else
			{
				utilization=utilization + array[z]/MIN(period,deadline);
			}
		}
		//printf("\ncount %d",countno+1);
		task=(float)1/(countno+1);
		power=pow(2,task);
		cond=(countno+1)*(power-1);
		//printf("\n\nUpper Bound for task %d = %0.2f",i+1,cond);
		//printf("\nUtilisation for task %d = %f",i+1,utilization);
		countno=1;
		if(utilization<cond)
		{
			printf("\nSchedulable in NON RM for task %d",i+1);

		}

		else if(utilization>1)
		{
			printf("\nNOT Schedulable in NON RM for %d task as Utilization bound is greater than 1",i+1);
			sched_check=0;
			break;

		}

		else
		{
			execution=0;	//going for RT analysis
			printf("\nMight be Schedulable in NON RM for task %d, going for RT",i+1);
			for(z=0;z<3*(i+1);z=z+3)
			{
			execution= execution + array[z];
			//deadline=array[z+1];
			}
			//printf("\n exec %f", execution);
			execution2=execution;
			execution=0;

			while(execution2!=execution)
			{
			execution=execution2;
			execution2=0;
			for(z=0;z<=3*(i);z=z+3)
			{

			execution2= execution2 + ceil(execution/array[z+2])*array[z];
				//printf("\nexec 2 %0.2f", execution2);
			}
			//printf("\nexec %0.2f", execution2);
			if(execution2	> deadline)
			{

				sched_check=0;
				break;
			}
			}

			if(sched_check==0)
			{	printf(" \nResponse time for task %d is %0.2f and task deadline is %0.2f",i+1,execution2,deadline);
				printf("\nAs examined by Real-Time analysis, task %d is NOT SCHEDULABLE.",i+1);
				sched_check=0;
			}
			if(sched_check==1)
			{	printf(" \nResponse time for task %d is %0.2f and task deadline is %0.2f",i+1,execution2,deadline);
				printf("\nAs examined by Real-Time analysis, task %d is SCHEDULABLE.",i+1);

			}
		}
		} //for loop ends
	if(sched_check==1)
		{
				switch(task_utili)
				{
				case 5:
					count_edf[0]=count_edf[0]+1;
					break;
				case 15:
					count_edf[1]=count_edf[1]+1;
					break;
				case 25:
					count_edf[2]=count_edf[2]+1;
					break;
				case 35:
					count_edf[3]=count_edf[3]+1;
					break;
				case 45:
					count_edf[4]=count_edf[4]+1;
					break;
				case 55:
					count_edf[5]=count_edf[5]+1;
					break;
				case 65:
					count_edf[6]=count_edf[6]+1;
					break;
				case 75:
					count_edf[7]=count_edf[7]+1;
					break;
				case 85:
					count_edf[8]=count_edf[8]+1;
					break;
				case 95:
					count_edf[9]=count_edf[9]+1;
					break;
				}
			printf("\n Schedulable in RM");
		}
		}

		else
		{
		sched_check=1;
		utilization=array[0]/MIN(array[1],array[2]);
		//printf("\n\nUpper Bound for task 1 = 1.0000");
		//printf("\nUtilisation for task 1 = %f",utilization);

		if(utilization>1)
		{
			printf("\nNOT Schedulable in NON RM for 1 task as Utilization bound is greater than 1");
			sched_check=0;

		}
		if(sched_check==1)
		{
		for(i=1;i<taskno;i++)
		{
		z=3*i;
		utilization=array[z]/MIN(array[z+1],array[z+2]);

		for(z=0;z<3*(i);z=z+3)
		{
			utilization=utilization+ array[z]/MIN(array[z+1],array[z+2]);

		}

		task=(float)1/(i+1);
		power=pow(2,task);
		cond=(i+1)*(power-1);
		//printf("\n\nUpper Bound for task %d = %0.2f",i+1,cond);
		//printf("\nUtilisation for task %d = %f",i+1,utilization);
		if(utilization<cond)
		{
			printf("\nSchedulable in RM for %d task",i+1);

		}

		else if(utilization>1)
		{
			printf("\nNOT Schedulable in RM for %d task as Utilization bound is greater than 1",i+1);
			sched_check=0;
			break;
		}

		else

		{

		execution=0;	//going for RT analysis
		printf("\nNOT Schedulable in RM for %d task, going for Real time analysis",i);
		for(z=0;z<3*(i+1);z=z+3)
		{
			execution= execution + array[z];
			deadline=array[z+1];
		}
			//printf("\nexec %f", execution);
		execution2=execution;
		execution=0;

		while(execution2!=execution)
		{
			execution=execution2;
			execution2=0;
			for(z=0;z<3*(i+1);z=z+3)
			{
				execution2= execution2 + ceil(execution/array[z+2])*array[z];
			}
			//printf(" \nexec 2 %f", execution2);
			if(execution2	> deadline)
			{

				sched_check=0;
				break;
			}
		}

		if(sched_check==0)
		{
			printf("\nResponse time for task %d is %0.2f and task deadline is %0.2f",i+1, execution2,deadline);
			printf("\nAs examined by Real-Time analysis, task %d is NOT SCHEDULABLE.",i+1);

		}

		if(sched_check==1)
		{
			printf("\nResponse time for task %d is %0.2f and task deadline is %0.2f",i+1, execution2,deadline);
			printf("\nAs examined by Real-Time analysis, task %d is SCHEDULABLE.",i+1);

		}

	}
	}

	if(sched_check==1)
	{
			switch(task_utili)
				{
				case 5:
					count_edf[0]=count_edf[0]+1;
					break;
				case 15:
					count_edf[1]=count_edf[1]+1;
					break;
				case 25:
					count_edf[2]=count_edf[2]+1;
					break;
				case 35:
					count_edf[3]=count_edf[3]+1;
					break;
				case 45:
					count_edf[4]=count_edf[4]+1;
					break;
				case 55:
					count_edf[5]=count_edf[5]+1;
					break;
				case 65:
					count_edf[6]=count_edf[6]+1;
					break;
				case 75:
					count_edf[7]=count_edf[7]+1;
					break;
				case 85:
					count_edf[8]=count_edf[8]+1;
					break;
				case 95:
					count_edf[9]=count_edf[9]+1;
					break;
				}
		printf("\n Schedulable in RM");
	}}}}
//printf("\n ---------------------------------------------------RM analysis ends here --------------------------------------\n");
	return;

}


/*The below function is of deadline monotonic. Over here first the array each arranged in increasing deadline, in other words the task with smallest deadline is given highest priority.After rearranging the array the utilization for each task is set by taking the density in consideration. For the task for which utilization is greater than the upper bound Real Time analysis is done. If the analysis converges at time less than the deadline for the particular task, the task is schedulable. This procedure is done for the whole taskset, and if the taskset is schedulable then we increment the count_dm for the utilization assigned for the taskset */

void compute_dm(float array[],int taskno,int task_utili)
{
printf("\n ---------------------------------------------------DM analysis begins here --------------------------------------------- \n");

	int z,m,p;
	float temp;
	float utilization=0,power,cond,task,execution=0,deadline,execution2=0;
	int i;
	sched_check=1;

	for(z=0;z<3*taskno;z=z+3)
	{
		for(p=z+3;p<3*taskno;p=p+3)
		{
			if(array[z+1]>array[p+1])
			{
				for(m=0;m<3;m++)
				{
					temp=array[z+m];
					array[z+m]=array[p+m];
					array[p+m]=temp;
				}
			}
		}
	}

		utilization=array[0]/MIN(array[1],array[2]);
		//printf("\n\nUpper Bound for task 1 = 1.0000");
		//printf("\nUtilisation for task 1 = %f",utilization);
		if(utilization>1)
		{
			printf("\nNOT Schedulable in NON RM for 1 task as Utilization bound is greater than 1");
			sched_check=0;

		}
		if(sched_check==1)
		{
		for(i=1;i<taskno;i++)
		{

		z=3*i;
		utilization=array[z]/MIN(array[z+1],array[z+2]);

		for(z=0;z<3*(i);z=z+3)
		{
			utilization=utilization+ array[z]/MIN(array[z+1],array[z+2]);

		}

		task=(float)1/(i+1);
		power=pow(2,task);
		cond=(i+1)*(power-1);
		//printf("\n\nUpper Bound for task %d = %0.2f",i+1,cond);
		//printf("\nUtilisation for task %d = %f",i+1,utilization);
		if(utilization<cond)
		{
			printf("\nSchedulable in RM for %d task",i+1);

		}

		else if(utilization>1)
		{
			printf("\nNOT Schedulable in RM for %d task as Utilization bound is greater than 1",i+1);
			sched_check=0;
			break;
		}

		else

		{

		execution=0;	//going for RT analysis
		printf("\nNOT Schedulable in RM for %d task, going for Real time analysis",i);
		for(z=0;z<3*(i+1);z=z+3)
		{
			execution= execution + array[z];
			deadline=array[z+1];
		}
			//printf("\nexec %f", execution);
		execution2=execution;
		execution=0;

		while(execution2!=execution)
		{
			execution=execution2;
			execution2=0;
			for(z=0;z<3*(i+1);z=z+3)
			{
				execution2= execution2 + ceil(execution/array[z+2])*array[z];
			}
			//printf(" \nexec 2 %f", execution2);
			if(execution2 > deadline)
			{
				sched_check=0;
				break;
			}
		}

		if(sched_check==0)
		{
			printf("\nResponse time for task %d is %0.2f and task deadline is %0.2f",i+1, execution2,deadline);
			printf("\nAs examined by Real-Time analysis, task %d is NOT SCHEDULABLE.",i+1);
			sched_check=0;

		}

		if(sched_check==1)
		{
			printf("\nResponse time for task %d is %0.2f and task deadline is %0.2f",i+1, execution2,deadline);
			printf("\nAs examined by Real-Time analysis, task %d is SCHEDULABLE.",i+1);

		}

	}
	}

		if(sched_check==1)
		{
				switch(task_utili)
				{
				case 5:
					count_edf[0]=count_edf[0]+1;
					break;
				case 15:
					count_edf[1]=count_edf[1]+1;
					break;
				case 25:
					count_edf[2]=count_edf[2]+1;
					break;
				case 35:
					count_edf[3]=count_edf[3]+1;
					break;
				case 45:
					count_edf[4]=count_edf[4]+1;
					break;
				case 55:
					count_edf[5]=count_edf[5]+1;
					break;
				case 65:
					count_edf[6]=count_edf[6]+1;
					break;
				case 75:
					count_edf[7]=count_edf[7]+1;
					break;
				case 85:
					count_edf[8]=count_edf[8]+1;
					break;
				case 95:
					count_edf[9]=count_edf[9]+1;
					break;
				}
			printf("\n Schedulable in DM");
		}}

//printf("\n ---------------------------------------------------DM analysis ends here --------------------------------------\n");
	return;
}

/* In the main program synthetic task sets of 5000 each for all utilization are being generated. In task set will have 10 or 25 tasks depending on the value entered by the user. Moreover, the deadline distribution also depends on the user choice. For choice 1 deadline is distributed in [Ci,Ti] while for choice two is is distributed in the range of [Ci+(Ti-Ci)/2,Ti] */

int main()
{
int n,i,j;
float task_utili;
int utili;

float a = 1.0;
int m,period;
int choice;
printf(" \n Enter the number of tasts either 10 or 25 in each taskset:");
scanf(" %d",&n);
printf(" Enter Uniform Distribution for deadline- choice 1 and 2:");
scanf(" %d",&choice);
float array[3*n];
float u[n];
//array for taskset utilization
float ut[10];
ut[0]=0.05;
for(i=1;i<10;i++)
ut[i]=ut[i-1]+0.1;

for(i=0;i<10;i++)
printf("\n u[i]= %f",ut[i]);
//two nested for loop for generating 5000 task sets for each utlization
srand((unsigned int)time(NULL));
for(l=0;l<10;l++)
{
for(j=0;j<5000;j++)
{

task_utili=ut[l];
utili=(int)(task_utili*100);
//UUniFast algorithm for finding utilization for each task in a taskset
sum_util=ut[l];
m=n/3;
for(i=1;i<=n-1;i++)
{
rando=(((float)rand()/(float)(RAND_MAX)) *a );	//random generating of float number in the range
power=(float)(1.000/(n-i));
t_sum=pow(rando,(float)power);
next_sum=sum_util*t_sum;
u[i]=sum_util-next_sum; // array of task utilization
sum_util=next_sum;
/*randoming assigning period in 3 ranges (100-1000,1000-10000,10000-100000). N/3 tasks are assigned period in the 3 ranges. After generaring the period, WCET is found for each task which is Ti*Ui. Deadline is uniformly distributed in the range chosen by the user. After getting WCET, Period, Deadline array of these values is formed and EDF, RM and DM is applied on each taskset.*/
if(i<=m)
{
	period= 100 + (rand() % (1000 - 100)); 
	array[3*(i-1)+2] =(float)period;
	array[3*(i-1)]=array[3*(i-1)+2]*u[i];
	if(choice==1)
	{
	array[3*(i-1)+1]=(fmodf((float)rand(),(array[3*(i-1)+2]+1.0- array[3*(i-1)])+array[3*(i-1)]));
	}
	if(choice==2)
	{
array[3*(i-1)+1]=(fmodf((float)rand(),(array[3*(i-1)+2] + 1.0 - (array[3*(i-1)] + ((array[3*(i-1)]-array[3*(i-1)+2])/2))) + (array[3*(i-1)] + ((array[3*(i-1)]-array[3*(i-1)+2])/2))));
	}
//	printf("\n P %f",array[3*(i-1)+2]);
//	printf(" D %f",array[3*(i-1)+1]);
//	printf(" E %f",array[3*(i-1)]);
}
else if(i<=2*m)
{
	period=1000 + (rand() % (10000 - 1000));
	array[3*(i-1)+2] =(float)period;
	array[3*(i-1)]=array[3*(i-1)+2]*u[i];
	if(choice==1)
	{
	array[3*(i-1)+1]=(fmodf((float)rand(),(array[3*(i-1)+2]+1.0- array[3*(i-1)])+array[3*(i-1)]));
	}
	if(choice==2)
	{
	array[3*(i-1)+1]=(fmodf((float)rand(),(array[3*(i-1)+2] + 1.0 - (array[3*(i-1)] + ((array[3*(i-1)]-array[3*(i-1)+2])/2))) + (array[3*(i-1)] + ((array[3*(i-1)]-array[3*(i-1)+2])/2))));
	}
//	printf("\n P %f",array[3*(i-1)+2]);
//	printf(" D %f",array[3*(i-1)+1]);
//	printf(" E %f",array[3*(i-1)]);
}
else
{
	period=	10000 + (rand() % (100000 - 10000));
	array[3*(i-1)+2] =(float)period;
	array[3*(i-1)]=array[3*(i-1)+2]*u[i];
	if(choice==1)
	{
	array[3*(i-1)+1]=(fmodf((float)rand(),(array[3*(i-1)+2]+1.0- array[3*(i-1)])+array[3*(i-1)]));
	}
	if(choice==2)
	{
array[3*(i-1)+1]=(fmodf((float)rand(),(array[3*(i-1)+2] + 1.0 - (array[3*(i-1)] + ((array[3*(i-1)]-array[3*(i-1)+2])/2))) + (array[3*(i-1)] + ((array[3*(i-1)]-array[3*(i-1)+2])/2))));
	}
//	printf("\n P %f",array[3*(i-1)+2]);
//	printf(" D %f",array[3*(i-1)+1]);
//	printf(" E %f",array[3*(i-1)]);
}
//printf(" %f",u[i]);
}
u[n]=sum_util;
period=10000 + (rand() % (100000 - 10000));
array[3*(n-1)+2] =(float)period;
array[3*(n-1)]=array[3*(n-1)+2]*u[i];
	if(choice==1)
	{
	array[3*(n-1)+1]=(fmodf((float)rand(),(array[3*(n-1)+2]+1.0- array[3*(n-1)])+array[3*(n-1)]));
	}
	if(choice==2)
	{
array[3*(n-1)+1]=(fmodf((float)rand(),(array[3*(n-1)+2] + 1.0 - (array[3*(n-1)] + ((array[3*(n-1)]-array[3*(n-1)+2])/2))) + (array[3*(n-1)] + ((array[3*(n-1)]-array[3*(n-1)+2])/2))));
	}
//array[3*(i-1)+1]=(float)deadline;
//printf("\n P %f",array[3*(n-1)+2]);
//printf(" D %f",array[3*(n-1)+1]);
//printf(" E %f",array[3*(n-1)]);
/*for(z=0;z<3*10;z=z+3)
{
	printf("\n% f\t %f\t%f",array[z],array[z+1],array[z+2]);
}*/
//calling functions for EDF,RM and DM for each taskset.
	feasibility(array,n,utili);
	compute_rm(array,n,utili);
	compute_dm(array,n,utili);
}
}

printf(" \t0.05\t0.15\t0.25\t0.35\t0.45\t0.55\t0.65\t0.75\t0.85\t0.95\n");
for(i=0;i<10;i++)
{
	printf(" \t%d",count_no[i]);
}
printf("\n EDF ");
for(i=0;i<10;i++)
{
	printf(" \t%d",count_edf[i]);
}
printf("\n RM ");
for(i=0;i<10;i++)
{
	printf(" \t%d",count_rm[i]);
}
printf("\n DM ");
for(i=0;i<10;i++)
{
	printf(" \t%d",count_dm[i]);
}
printf("\n--------------------------------------------------------\n");


return 0;
}
