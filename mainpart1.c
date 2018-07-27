/*#######################################################################################################################################################
 CSE522 Real-time Embedded Systems – Assignment 2 by Group 2 (Hena Shah ( ASU ID: 1213348511) & Nidhi Dubey ( ASU ID: 1213031246))

In this code we have programmed the scheduling algorithms for Earliest Deadline First, Rate Monotic and Deadline Monotonic. We have shown complete loading factor and Real-time Analysis if the taskset doesn't meet the sufficient condition.
######################################################################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define MIN(x,y) ((x<y)?x:y) // to determine minimum of two elements
int i,j,k,h,l,x,y,temp1,cnt;
float curr_len,curr,exec=0;
float last_len,busy_per_len;
float temp;
int sched,sched_check;

// EDF analysis for Di=pi
void edf(float array[],int taskno)
{
	printf("\n\n---------------------------------------------------EDF analysis begins here --------------------------------------------- \n");
	float util=0;
	int i;
	
	// compute utilisation ei/pi
	for(i=0;i<3*taskno;i=i+3)
	{
	if(array[i+1]<=array[i+2])
	util=util+(array[i]/array[i+1]);
	else
	util=util+(array[i]/array[i+2]);
	}

	printf("\nUtilisation is %f ",util);
	// schedulable only if utilisation<=1
	if(util<=1)
	{
	printf("\nAs checked by EDF Task set is schedulable by Utilization analysis");
	}
	else
	printf("\nUtilisation is greater than 1. NOT Schedulable in EDF");


}

// RM and DM analysis for Di=Pi
void rm(float array[],int taskno)
{
printf("\n ---------------------------------------------------RM and DM analysis begins here --------------------------------------------- \n");
	//to set all task with increasing priority with respect to period/deadline
	float temp;
	int z,p,m;
	float utilization=0,power,cond,task,execution=0,deadline;
	int i;
	float execution2=0;
	int sched_check=0;
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
		
		sched_check=1;
		utilization=array[0]/array[2];
		// compute utilisation ei/pi
		printf("\n\nUpper Bound for task 1 = 1.0000");
		printf("\nUtilisation for task 1 = %f",utilization);

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
		utilization=array[z]/array[z+2];

		for(z=0;z<3*(i);z=z+3)
		{
			utilization=utilization+ array[z]/array[z+2];

		}
		// calculate upper bound
		task=(float)1/(i+1);
		power=pow(2,task);
		cond=(i+1)*(power-1);
		printf("\n\nUpper Bound for task %d = %0.2f",i+1,cond);
		printf("\nUtilisation for task %d = %f",i+1,utilization);
		// schedulable only if utilisation<=1
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

		// RT anaylysis begins here 
		execution=0;	
		printf("\nNOT Schedulable in RM for %d task, going for Real time analysis",i);
		for(z=0;z<3*(i+1);z=z+3)
		{
			execution= execution + array[z];
			deadline=array[z+1];
		}
			
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

		}

		if(sched_check==1)
		{
			printf("\nResponse time for task %d is %0.2f and task deadline is %0.2f",i+1, execution2,deadline);
			printf("\nAs examined by Real-Time analysis, task %d is SCHEDULABLE.",i+1);

		}

	}
	}
	}

}

// To compute busy period length for EDF for Di<Pi
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

// To compute density utilisation for EDF for Di<Pi
void edf_density(float array[],int taskno)
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

	printf("\nDensity Utilisation is %f ",util);
	if(util<=1)
	{
	sched=1;
	printf("\nAs checked by EDF Task set is schedulable by density analysis");
	}
	else
	printf("\nDensity utilisation is greater than 1. Checking for load factory analysis");
}

/*The below function is of earliest deadline first for Di<Pi. Utilization for each task in found and if the total utilization is less than one than the task set is schedulable. If utilization is greater than one we find the loading factor for the busy period. If loading factor becomes greater than 1 for any instance than taskset is not schedulable or else it is schedulable.*/

void feasibility(float array[],int taskno)
{
	int ch=0;
	float temp1;
	float temp2;

	edf_density(array,taskno);

	if(sched==0)
	{

	last_len=0;
	for(i=0;i<taskno*3;i=i+3)
	{
		last_len=last_len+array[i];
	}

	curr_len=busy_period_calc(last_len,array,taskno);

	// stop when busy period converges
	while(curr_len!=last_len)
	{
		last_len=curr_len;
		curr_len=busy_period_calc(last_len,array,taskno);
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
	printf("\nDeadline matrix\n");
	for (i=0;i<r;i++)
	{
	for(j=0;j<c;j++)
	{
		if(*(arr + i*c + j)!=0)
		{
		printf("%0.2f \t",*(arr + i*c + j));
		count++;
		}
	}
	printf("\n");

	}

	k=0;
	float mat[count][2];
	float load[count];
	// to create a deadline matrix
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

	// to sort deadlines
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


	printf("\nFinal loading factor matrix:\n");
	printf("\nDeadline\t  h\t  Loading factor\n");
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
				printf("%0.2f\t\t%0.2f\t\t%0.2f\n",mat[i][0],exec,load[i]);
				if(load[i]>1)
				{
				ch=1;
				}
				
			}
	// if all tasks have loading factor less than 0 at deadline then only taskset is schedulable
	if(ch==0)
	printf("\nTaskset is schedulable in EDF");
	else
	printf("\nTaskset is not schedulable in EDF");





//printf("\n ---------------------------------------------------EDF analysis ends here --------------------------------------------- \n");
}
}


/*The below function is of rate monotonic. Over here first the array each arranged in increasing period, in other words the task with smallest period is given highest priority.After rearranging we compare it with the array that would be formed when array of arranged in increasing order of density. If order of task for both the array is same we go for normal RM analysis or else we go for non RM analysis. In the RM analysis the utilization for each task is found by taking the density in consideration. For the task for which utilization is greater than the upper bound Real Time analysis is done. If the analysis converges at time less than the deadline for the particular task, the task is schedulable. This procedure is done for the whole taskset, and if the taskset is schedulable then we increment the count_rm for the utilization assigned for the taskset. 
In case of non RM the utilization for the task set is compared with the deadline of the task under test. If period is smaller we simply with utilization for that task and if period is greated than deadline is find utilization by dividing the execution time for the task by minimun of deadline and period for task under test. If utilization is greater than the upper bound we do Real Time analysis for the task under test.If the analysis converges at time less than the deadline for the particular task, the task is schedulable. */


//Rate Monotonic response time analysis
void compute_rm(float array[],int taskno)
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
	// sort the array based on period 
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

	// to check if density order matches with RM order
	for(z=0;z<3*taskno;z=z+3)
	{
		min1=MIN(array[z+1],array[z+2]);

		for(p=z+3;p<3*taskno;p=p+3)
		{
			min2=MIN(array[p+1],array[p+2]);
			if(min1>min2)
			{
					// If it doesn't match then go for NON RM analysis
					printf("\nDensity order doesn't match going for NON RM");
					flag_rm=1;
					break;
			}

		}
	}

	
	// If NON RM case
	if(flag_rm==1)
	{
		sched_check=1;
		//compute effective utilisation for all tasks
		utilization=array[0]/MIN(array[1],array[2]);
		printf("\n\nUpper Bound for task 1 = 1.00");
		printf("\nUtilisation for task 1 = %0.2f",utilization);
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
		//deadline and period of current task
		deadline=array[z+1]; 
		period=array[z+2];
		utilization=array[z]/MIN(array[z+1],array[z+2]);

		for(z=0;z<3*(i);z=z+3)
		{
			if(array[z+2]<=deadline)
			{
				// increment number of tasks if task period is less than Di
				utilization=utilization+ array[z]/MIN(array[z+1],array[z+2]);
				countno++;
			}

			else
			{
				utilization=utilization + array[z]/MIN(period,deadline);
			}
		}
		
		// computer upper bound
		task=(float)1/(countno+1);
		power=pow(2,task);
		cond=(countno+1)*(power-1);
		printf("\n\nUpper Bound for task %d = %0.2f",i+1,cond);
		printf("\nUtilisation for task %d = %f",i+1,utilization);
		countno=1;
		// schedulable only if utilisation<=1
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
		
			execution2=execution;
			execution=0;

			// computer response time until it converges or exceeds deadline
			while(execution2!=execution)
			{
			execution=execution2;
			execution2=0;
			for(z=0;z<=3*(i);z=z+3)
			{

			execution2= execution2 + ceil(execution/array[z+2])*array[z];
			}
			
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
		}
		}
		
		if(flag_rm==0)
		{
		
		sched_check=1;
		//compute effective utilisation for all tasks
		utilization=array[0]/MIN(array[1],array[2]);
		printf("\n\nUpper Bound for task 1 = 1.0000");
		printf("\nUtilisation for task 1 = %f",utilization);

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
		// computer upper bound
		task=(float)1/(i+1);
		power=pow(2,task);
		cond=(i+1)*(power-1);
		printf("\n\nUpper Bound for task %d = %0.2f",i+1,cond);
		printf("\nUtilisation for task %d = %f",i+1,utilization);
		// check for suffiecient condition
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
		// if utilisation is greater than bound but less than 1
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
		// computer response time until it converges or exceeds deadline
		while(execution2!=execution)
		{
			execution=execution2;
			execution2=0;
			for(z=0;z<3*(i+1);z=z+3)
			{
				execution2= execution2 + ceil(execution/array[z+2])*array[z];
			}
			printf(" \nexec 2 %f", execution2);
			if(execution2	> deadline)
			{

				sched_check=0;
				break;
			}
		}
		// if exceeds deadline then task is not schedulable
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
	}}
//printf("\n ---------------------------------------------------RM analysis ends here --------------------------------------\n");
	return;

}

/*The below function is of deadline monotonic. Over here first the array each arranged in increasing deadline, in other words the task with smallest deadline is given highest priority.After rearranging the array the utilization for each task is set by taking the density in consideration. For the task for which utilization is greater than the upper bound Real Time analysis is done. If the analysis converges at time less than the deadline for the particular task, the task is schedulable.  */

// DM analysis for Di<pi
void compute_dm(float array[],int taskno)
{
printf("\n ---------------------------------------------------DM analysis begins here --------------------------------------------- \n");
	int z,m,p;
	float temp;
	float utilization=0,power,cond,task,execution=0,deadline,execution2=0;
	int i;
	sched_check=1;
	// sort as per deadline
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
		// compute utilisation for each task
		utilization=array[0]/MIN(array[1],array[2]);
		printf("\n\nUpper Bound for task 1 = 1.0000");
		printf("\nUtilisation for task 1 = %f",utilization);
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
		// compute upper bound
		task=(float)1/(i+1);
		power=pow(2,task);
		cond=(i+1)*(power-1);
		printf("\n\nUpper Bound for task %d = %0.2f",i+1,cond);
		printf("\nUtilisation for task %d = %f",i+1,utilization);
		// schedulable only if utilisation less than upper bound
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
		// if utilisation is greater than bound but less than 1
		else
		{

		execution=0;	//going for RT analysis
		printf("\nNOT Schedulable in RM for %d task, going for Real time analysis",i);
		for(z=0;z<3*(i+1);z=z+3)
		{
			execution= execution + array[z];
			deadline=array[z+1];
		}
			
		execution2=execution;
		execution=0;
		// computer response time until it converges or exceeds deadline
		while(execution2!=execution)
		{
			execution=execution2;
			execution2=0;
			for(z=0;z<3*(i+1);z=z+3)
			{
				execution2= execution2 + ceil(execution/array[z+2])*array[z];
			}
			printf(" \nexec 2 %f", execution2);
			if(execution2 > deadline)
			{
				sched_check=0;
				break;
			}
		}
		// if exceeds deadline then task is not schedulable
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
	}

//printf("\n ---------------------------------------------------DM analysis ends here --------------------------------------\n");
	return;
}

/***********************************************	MAIN FUNCTION   ********************************************************************/
int main()
{

	int task_set;
	int check;
   	char c[10];
   	int i=0,j=0,k,l,q,z;				//int k for while loop
	FILE *file = fopen("input.txt", "r");  	        // to open input file. Change if required.
  	char line[1024];

   	fgets(line, sizeof(line), file); 	/* read a line */
	//printf("\n %s",line);
     	while(line[i]!='\0')
	{
	c[i]=line[i];
	i++;
	}
	// check for number for tasksets
	task_set=atoi(c);
	//printf("\n %d",taskset);
	int taskno[task_set];
	float array[task_set][15];
	float taskset[15];
	for(k=0;k<task_set;k++)
	{
		c[j]='\0';
		i=0;
		j=0;
		q=0;
		fgets(line, sizeof(line), file); 	/* read a line */
		//printf("\n %s",line);
     		while(line[i]!='\0')
		{
		c[j]=line[i];
		i++;
		j++;
		}
		taskno[k]=atoi(c);
		for(j=0;j<10;j++)
			{
				c[j]='\0';
			}
		//printf("\n %d \n",taskno[k]);

		for(l=0;l<taskno[k];l++)
		{
			//read each taskset seperately
			i=0;
			j=0;
			for(j=0;j<10;j++)
			{
				c[j]='\0';
			}
			fgets(line, sizeof(line), file);
			while(line[i]!='\0')
			{
				j=0;
				while(line[i]!=' ')
				{
					c[j]=line[i];
					j++;
					i++;
					if(line[i]=='\n')
					{
						break;
					}
				}
			i++;
			array[k][q]=atof(c);
			for(j=0;j<10;j++)
			{
				c[j]='\0';
			}
			//printf(" %f",array[k][q]);
			q++;
			}

		}


	for(z=0;z<3*taskno[k];z++)
	{
		taskset[z]=array[k][z];
	}
// print new taskset 

check=0;
printf("\n\n                         ************************   New Taskset Analysis begins here *********************                              \n");
printf("\nNew Taskset is:");
for(z=0;z<3*taskno[k];z=z+3)
{
	printf("\n%0.2f\t%0.2f\t%0.2f",taskset[z],taskset[z+1],taskset[z+2]);
}
printf("\n");
// Run till you check for all tasksets
	for(i=0;i<3*taskno[k];i=i+3)
	{
	if(taskset[i+1]<taskset[i+2])
	{
		//goto check schedulability for deadline less than period
		check=1;
		break;
	}
	}
	// call specific fuctions for Di<Pi
	if(check==1)
	{
	printf("\nDeadline lesser than period. Going for feasibility and response time analysis");
	feasibility(taskset,taskno[k]);
	compute_rm(taskset,taskno[k]);
	compute_dm(taskset,taskno[k]);
	}
	// if deadline equal to period then check utilisation
	if(check==0)
	{
	printf("\nDeadline equal to period.");
	edf(taskset,taskno[k]);
	rm(taskset,taskno[k]);
	}
	for(z=0;z<3*taskno[k];z++)
	{
		taskset[z]=0;
	}
}

printf("\n");

return 0;
}
//End of Program
