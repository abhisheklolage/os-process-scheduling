#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#define MAX 16

typedef struct{
    char pid[4];
    int burst_time;
    int arrival_time;
    int priority;
    int waiting_time;
    int turnaround_time;
}process;

void fcfs(process p[], int c);
void sjf(process p[], int c);
//void strf(process p[], int c);
//void pp(process p[], int c);
void pnp(process p[], int c);
//void robin(process p[], int c);
void hrrn(process p[], int c);

void print_table(process p[], int order[],int fin);
void print_gantt_chart(process p[], int n, int order[], int ft[], int st[]);

int count = 0, i = 0, j = 0;
char h1[MAX];
char h2[MAX];
char h3[MAX];
char h4[MAX];

int tot_wt = 0, tot_tat = 0;
double avg_wt = 0, avg_tat = 0;
int algorithm = 0;

void fcfs(process p[count], int c) {
	algorithm = 1;
    int st[c], ft[c], order[c];
	int min = p[0].arrival_time;
	int total_time = 0;
	int outcast, target;
	int discard[c];
	int k;
	int wtf = 0;

	for(k = 0; k < c; k++)
		discard[k] = -1;

	for(i = 0;i < c; i++) {
		if(i == 0) {
			for(j = 0; j < c; j++)
				if(p[j].arrival_time <= min) {
					min = p[j].arrival_time;
					target = j;
				}
		}
		else {
			min = 99999999;
			for(j = 0; j < c; j++) {
				wtf = 0;
				if(p[j].arrival_time <= min && p[outcast].arrival_time <= p[j].arrival_time ) {
					for(k = 0; k < c; k++)
						if(discard[k] == j) {
							wtf = 1;
							break;
						}
					if(wtf == 0) {
						min = p[j].arrival_time;
						target = j;
					}
				}
			}
		}

		outcast = target;
		discard[i] = target;

		if(i == 0) {
			st[target] = p[target].arrival_time;
			ft[target] = p[target].arrival_time + p[target].burst_time;
		}
		else {
			if(p[target].arrival_time >= total_time)
				st[target] = p[target].arrival_time;
			else
				st[target] = total_time;

			ft[target] = st[target] + p[target].burst_time;
		}

		p[target].waiting_time = st[target] - p[target].arrival_time;
		p[target].turnaround_time = ft[target] - p[target].arrival_time;

		total_time = ft[target];

		tot_wt = tot_wt + p[target].waiting_time;
		tot_tat = tot_tat + p[target].turnaround_time;

		order[i] = target + 1;
	}

	avg_wt = (double)tot_wt / c;
	avg_tat = (double)tot_tat / c;
	print_table(p, order,total_time);
	print_gantt_chart(p, c, order,ft,st);
}

void sjf(process p[count], int c) {
	algorithm = 2;
    int st[c], ft[c], order[c];
	int min;
	int min_burst;
	int total_time = 0;
	int outcast, target;
	int discard[c];
	int next[c];
	int k;

	for(k = 0; k < c; k++)
		discard[k] = -1;

	for(i = 0; i < c; i++) {
		min = 9999999;
		min_burst = 9999999;
		k = 0;

		if(i == 0) {
			for(j = 0; j < c; j++)
				if(p[j].arrival_time <= min)
					min = p[j].arrival_time;

			for(j = 0; j < c; j++) {
				if(p[j].arrival_time <= min) {
					next[k] = j;
					k++;
				}
			}
		}
		else {
			for(j = 0; j < c; j++) {
				if(p[j].arrival_time <= ft[target] && discard[j] == -1) {
					next[k] = j;
					k++;
				}
			}
			if(k == 0) {
				for(j = 0; j < c; j++) {
					if(p[j].arrival_time > ft[target]){
						next[k] = j;
						k++;
					}
				}

			}

		}

		for(j = 0; j < k; j++) {
			if(p[next[j]].burst_time <= min_burst) {
				min_burst = p[next[j]].burst_time;
				target = next[j];
			}
		}

		outcast = target;
		discard[target] = target;
		if(i == 0) {
			st[target] = p[target].arrival_time;

			ft[target] = p[target].arrival_time + p[target].burst_time;
		}
		else {
			if(p[target].arrival_time >= total_time)
				st[target] = p[target].arrival_time;
			else
				st[target] = total_time;

			ft[target] = st[target] + p[target].burst_time;
		}

		p[target].waiting_time = st[target] - p[target].arrival_time;
		p[target].turnaround_time = ft[target] - p[target].arrival_time;

		total_time = ft[target];

		tot_wt = tot_wt + p[target].waiting_time;
		tot_tat = tot_tat + p[target].turnaround_time;

		order[i] = target + 1;

	}

	avg_wt = (double)tot_wt / c;
	avg_tat = (double)tot_tat / c;
	print_table(p, order,total_time);
	print_gantt_chart(p, c, order,ft,st);
}

void hrrn(process p[count], int c) {
	algorithm = 4;
	int st[c], ft[c], order[c];
	int min;
	int temp_wait[c];
	int max_rr;
	int hrr[c];
	int total_time = 0;
	int outcast, target;
	int discard[c];
	int next[c];
	int k;

	for(k = 0; k < c; k++) {
		discard[k] = -1;
		temp_wait[k] = 0;
	}

	for(i = 0; i < c; i++) {
		min = 9999999;
		max_rr = 0;
		k = 0;

		if(i == 0) {
			for(j = 0; j < c; j++)
				if(p[j].arrival_time < min)
					min = p[j].arrival_time;

			for(j = 0; j < c; j++) {
				if(p[j].arrival_time == min) {
					next[k] = j;
					hrr[k] = 1  + temp_wait[j] / p[j].burst_time;
					k++;
				}
			}
		}
		else {
			for(j = 0; j < c; j++) {
				if(p[j].arrival_time <= ft[target] && discard[j] == -1) {
					next[k] = j;
					hrr[k] = 1  + temp_wait[j] / p[j].burst_time;
					k++;
				}
			}
			if(k == 0) {
			for(j = 0; j < c; j++){
				if(p[j].arrival_time > ft[target]){
					next[k] = j;
					hrr[k] = 1  + temp_wait[j] / p[j].burst_time;
					k++;
				}
			}
			}
		}

		for(j = 0; j < k; j++) {
			if(hrr[j] > max_rr) {
				max_rr = hrr[j];
				target = next[j];
			}
		}

		outcast = target;
		discard[target] = target;

		if(i == 0) {
			st[target] = p[target].arrival_time;
			ft[target] = p[target].arrival_time + p[target].burst_time;
		}
		else {
			if(p[target].arrival_time >= total_time)
				st[target] = p[target].arrival_time;
			else
				st[target] = total_time;

			ft[target] = st[target] + p[target].burst_time;
		}

		p[target].waiting_time = st[target] - p[target].arrival_time;
		p[target].turnaround_time = ft[target] - p[target].arrival_time;

		total_time = ft[target];

		tot_wt = tot_wt + p[target].waiting_time;
		tot_tat = tot_tat + p[target].turnaround_time;

		order[i] = target + 1;

		for(j = 0 ;j < k; j++)
			temp_wait[next[j]] = temp_wait[next[j]] + p[target].burst_time;
	}

	avg_wt = (double)tot_wt / c;
	avg_tat = (double)tot_tat / c;
	print_table(p, order,total_time);
	print_gantt_chart(p, c, order,ft,st);
}

void pnp(process p[count], int c) {
	algorithm = 3;
	int st[c], ft[c], order[c];
	int min;
	int max_prio;
	int total_time = 0;
	int outcast, target;
	int discard[c];
	int next[c];
	int k;

	for(k = 0; k < c; k++)
		discard[k] = -1;

	for(i = 0; i < c; i++) {
		min = 9999999;
		max_prio = 0;
		k = 0;

		if(i == 0) {
			for(j = 0; j < c; j++)
				if(p[j].arrival_time <= min)
					min = p[j].arrival_time;

			for(j = 0; j < c; j++) {
				if(p[j].arrival_time <= min) {
					next[k] = j;
					k++;
				}
			}
		}
		else {
			for(j = 0; j < c; j++) {
				if(p[j].arrival_time <= ft[target] && discard[j] == -1) {
					next[k] = j;
					k++;
				}
			}
			if(k == 0) {
				for(j = 0; j < c; j++) {
				if(p[j].arrival_time > ft[target]){
					next[k] = j;
					k++;
				}
			}
			}
		}

		for(j = 0; j < k; j++) {
			if(p[next[j]].priority >= max_prio) {
				max_prio = p[next[j]].priority;
				target = next[j];
			}
		}

		outcast = target;
		discard[target] = target;

		if(i == 0) {
			st[target] = p[target].arrival_time;
			ft[target] = p[target].arrival_time + p[target].burst_time;
		}
		else {
			if(p[target].arrival_time >= total_time)
				st[target] = p[target].arrival_time;
			else
				st[target] = total_time;

			ft[target] = st[target] + p[target].burst_time;
		}

		p[target].waiting_time = st[target] - p[target].arrival_time;
		p[target].turnaround_time = ft[target] - p[target].arrival_time;

		total_time = ft[target];

		tot_wt = tot_wt + p[target].waiting_time;
		tot_tat = tot_tat + p[target].turnaround_time;

		order[i] = target + 1;
	}

	avg_wt = (double)tot_wt / c;
	avg_tat = (double)tot_tat / c;
	print_table(p, order,total_time);
	print_gantt_chart(p, c, order,ft,st);
}

void print_table(process p[count],int order[count],int fin)
{
	printf("+-----+---------------+------------+----------+--------------+-----------------+\n");
    printf("| PID | Arrival Time  | Burst Time | Priority | Waiting Time | Turnaround Time |\n");
    printf("+-----+---------------+------------+----------+--------------+-----------------+\n");

    for(i = 0; i < count; i++) {
    	printf("| P%-2d |     %2d        |     %2d     |   %2d     |      %2d      |        %2d       |\n"
        , i+1, p[i].arrival_time, p[i].burst_time, p[i].priority, p[i].waiting_time, p[i].turnaround_time );
    printf("+-----+---------------+------------+----------+--------------+-----------------+\n");
    }

    printf("\nTotal Waiting Time      : %-2d\n", tot_wt);
    printf("Average Waiting Time    : %-2.2lf\n", avg_wt);
    printf("Total Turnaround Time   : %-2d\n", tot_tat);
    printf("Average Turnaround Time : %-2.2lf\n", avg_tat);
    printf("Throughput              : %-2.2lf\n", (double)count/fin);
}

void print_gantt_chart(process p[count], int n, int order[count], int ft[count], int st[count])
{
	printf("\n\n          GANTT CHART          \n");
    printf("          ***********          \n");

	FILE *fo = fopen("tmp.txt", "w");
	FILE *fo2 = fopen("tmp2.txt", "w");
    char alg[100];
    if(algorithm == 1){
        strcpy(alg, "./ganttChartScripts/dat/FCFSforGanttChart.dat");
    }
    else if (algorithm == 2){
        strcpy(alg, "./ganttChartScripts/dat/SJFforGanttChart.dat");

    }
    else if(algorithm == 3){
        strcpy(alg, "./ganttChartScripts/dat/PRINONPREEMPforGanttChart.dat");

    }
    else if (algorithm == 4){
        strcpy(alg, "./ganttChartScripts/dat/HRRNforGanttChart.dat");

    }
    FILE *ff = fopen(alg, "w");
	int idle = 0;
	int nn;
	int pos[n];
	int cout = 0;
	int l;

	if(st[order[0] - 1] != 0 ) {
		idle++;
		pos[0] = -1;
	}
	cout = 1;
	for(i = 1; i < n; i++)
		if(ft[order[i - 1] - 1] != st[order[i] - 1]) {
			idle++;
			pos[i+cout] = -1;
			cout++;
		}

	nn = n + idle;
	printf(" ");
    for(i = 0; i < nn; i++) {
        for(j = 0; j < 3; j++)
        	printf("--");

        printf(" ");
	}
    printf("\n|");
	cout = 0;
    for(i = 0; i < nn; i++) {
        for(j = 0; j < 2 ; j++)
        	printf(" ");

        if(pos[i] == -1) {
        	printf("\b");
        	printf("Idle ");
        	cout++;
        	l  = 0;
        }
        else {
        	l = i - cout;
       		printf("%s", p[order[l] - 1].pid);
            fprintf(fo, "%s\t", p[order[l] - 1].pid);
        	if(order[l] > 9)
        		for(j = 0; j < 2; j++)
        			printf(" ");
        	else
        		for(j = 0; j < 3; j++)
        			printf(" ");
       }
        printf("|");
    }
    printf("\n ");

    for(i = 0; i < nn; i++) {
        for(j = 0; j < 3; j++)
        	printf("--");

        printf(" ");
    }
    printf("\n");
    fprintf(fo, "\n");
    cout = 0;
    printf("0");
    //fprintf(fo, "%d\t", 0);
	for(i = 0; i < nn; i++) {
        for(j = 0; j < 3; j++)
        	printf("  ");

		l = i - cout;
        if(pos[i] == -1) {
        	if(st[order[l] - 1] > 9)
        		printf("\b");
          	printf("%d", st[order[l] - 1]);
            fprintf(fo2, "%d\t", st[order[l] - 1]);
        	cout++;
        }

        else {

        	if(ft[order[l] - 1] > 9)
        		printf("\b");
        	printf("%d", ft[order[l] - 1]);
            fprintf(fo2, "%d\t", ft[order[l] - 1]);
        }

	}
    printf("\n\n\n");
    int c;
    FILE *file;
    fclose(fo);
    fclose(fo2);
    /*
     *file = fopen("tmp.txt", "r+");
     *if (file) {
     *        while ((c = getc(file)) != EOF)
     *                    putchar(c);
     *            fclose(file);
     *        printf("+");
     *}
     */

	fo = fopen("tmp.txt", "r");
	fo2 = fopen("tmp2.txt", "r");
    int proc_num = 0;
    int low = 0, high = 0, counter = 0;
    while(fscanf(fo, "%d", &proc_num) != EOF){
        counter++;
    }
    rewind(fo);
    while(counter--){
        fscanf(fo, "%d", &proc_num);
        fscanf(fo2, "%d", &high);
        fprintf(ff, "%d\t%d\t%d\t%c\n", proc_num, low, high, '-');
        low = high;
    }
    fclose(fo);
    fclose(fo2);
    remove("tmp.txt");
    remove("tmp2.txt");
    fclose(ff);
}

int main() {

	system("clear");
	FILE *ptr = fopen("input.txt", "r");
    char ch;
	if(ptr == NULL) {
		printf("Failed: File cannot be opened\n");
		return errno;
	}

	while(!feof(ptr)) {
	  	ch = fgetc(ptr);

  		if(ch == '\n')
    		count++;
	}
	count--;

	fseek(ptr,0,SEEK_SET);

	process p[count];

	int i = 0;

	fscanf(ptr, "%s%s%s%s", h1, h2, h3, h4);
	while(fscanf(ptr, "%s%d%d%d", p[i].pid, &p[i].arrival_time, &p[i].burst_time, &p[i].priority) != EOF)
		i++;

	int choice;

	printf("\n\nSelect Method:\n\n");
	printf("1.FCFS\n");
	printf("2.SJF\n");
	printf("3.Priority Non-Preemptive\n");
	printf("4.HRRN\n");

	printf("Choice: ");
	scanf("%d", &choice);
	system("clear");
	switch (choice) {
		case 1:	fcfs(p, count);
    			break;
		case 2:	sjf(p, count);
    			break;
    	case 3:	pnp(p, count);
    			break;
    	case 4:	hrrn(p, count);
    			break;
	   	default:printf("Error: Wrong Input.Terminate.\n");
   				break;
	}

	return 0;
}
