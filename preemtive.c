#include<stdio.h>
#include<stdlib.h>

void priority_prem(int [], int[], int[], int[], int);
void rr(int [], int[], int[], int[], int);
void srtf(int [], int[], int[], int[], int);

int main() {
	FILE *fp;
    char c;
	int i = 0, process[32], arrival[32], burst[32], priority[32], num, cnt = 0, choice;
	fp = fopen("input.txt", "r");
	while((c = getc(fp)) != '\n');

	//printf("\n");

	 while(fscanf(fp, "%d", &num) > 0) {
		if(cnt % 4 == 0)
        	process[i] = num;
        else if(cnt % 4 == 1)
        	arrival[i] = num;
        else if(cnt % 4 == 2)
        	burst[i] = num;
        else if(cnt % 4 == 3) {
        	priority[i] = num;
        	i++;
        }

        cnt++;
    }

	printf("Select algorithm : \n");
	printf("1.PRIORITY PREMPTIVE\n");
	printf("2.ROUND ROBIN\n");
	printf("3.SRTF\n\nEnter Choice: ");
	scanf("%d", &choice);
	switch(choice) {
		case 1 :
			priority_prem(process, arrival, burst, priority, i);
			break;

		case 2 :
			rr(process, arrival, burst, priority, i);
			break;
		case 3:
			srtf(process, arrival, burst, priority, i);
			break;
	}

	fclose(fp);
	return 0;
}

void priority_prem(int process[32], int arrival[32], int burst[32], int priority[32], int cnt) {
	FILE *fo = fopen("./ganttChartScripts/dat/PRIORITYforGanttChart.dat", "w");
	int i, temp, j, total_wait = 0, final = cnt, flag = 0;
	float avg_turn = 0, avg_wait = 0;

	for(i = 0; i < cnt; i++) {
		for(j = i + 1; j < cnt; j++) {
			if(priority[i] > priority[j]) {
				temp = arrival[i];
				arrival[i] = arrival[j];
				arrival[j] = temp;

				temp = process[i];
				process[i] = process[j];
				process[j] = temp;

				temp = burst[i];
				burst[i] = burst[j];
				burst[j] = temp;

				temp = priority[i];
				priority[i] = priority[j];
				priority[j] = temp;
			}
		}
	}

	printf("Process Start   End     Turn   Waiting\n");
	printf("--------------------------------------\n");
	i = 0;
	while(final != 0) {
		flag = 0;
		if(burst[i] != 0 && arrival[i] <= total_wait) {
			for(j = 0; j < i; j++) {
				if(burst[j] == 0)
					flag++;
			}

			if(flag == i) {
				printf("%d\t%d\t%d\t%d\t%d\n", process[i], total_wait, total_wait + burst[i], total_wait + burst[i] - arrival[i], total_wait - arrival[i]);
			    fprintf(fo, "%d\t%d\t%d\t%c\n", process[i], total_wait, total_wait + 1, '-');
				avg_turn = avg_turn + total_wait + burst[i] - arrival[i];
				avg_wait = avg_wait + total_wait - arrival[i];
				final--;
				total_wait = total_wait + burst[i];
				burst[i] = 0;
				i++;
			}
			else {
				printf("%d\t%d\t%d\t%d\t%d\n", process[i], total_wait, total_wait + 1, total_wait + 1 - arrival[i], total_wait - arrival[i]);
			    fprintf(fo, "%d\t%d\t%d\t%c\n", process[i], total_wait, total_wait + 1, '-');
				avg_turn = avg_turn + total_wait + 1 - arrival[i];
				avg_wait = avg_wait + total_wait - arrival[i];
				burst[i]--;
				total_wait++;
				i = 0;
			}
		}
		else {
			if((i + 1) == cnt)
				i = 0;
			else
				i++;
		}
	}

	printf("--------------------------------------\n");
	avg_turn = avg_turn / cnt;
	avg_wait = avg_wait / cnt;
	printf("Average turnaround time : %f\nAverage waiting time : %f\n", avg_turn, avg_wait);
	printf("--------------------------------------\n");
    fclose(fo);
}

void rr(int process[32], int arrival[32], int burst[32], int priority[32], int cnt) {
	FILE *fo = fopen("./ganttChartScripts/dat/RRforGanttChart.dat", "w");
	int i, temp, j, quantum, final = cnt, total_wait = 0;
	float avg_turn = 0, avg_wait = 0;
	printf("Enter quantum : ");
	scanf("%d", &quantum);

	for(i = 0; i < cnt; i++) {
		for(j = i + 1; j < cnt; j++) {
			if(arrival[i] > arrival[j]) {
				temp = arrival[i];
				arrival[i] = arrival[j];
				arrival[j] = temp;

				temp = process[i];
				process[i] = process[j];
				process[j] = temp;

				temp = burst[i];
				burst[i] = burst[j];
				burst[j] = temp;
			}
		}
	}

	printf("Process Start   End     Turn   Waiting\n");
	printf("--------------------------------------\n");

	i = 0;
	while(final != 0) {
		if(burst[i] != 0 && arrival[i] <= total_wait && burst[i] > quantum) {
			printf("%d\t%d\t%d\t%d\t%d\n", process[i], total_wait, total_wait + quantum, total_wait + quantum - arrival[i], total_wait - arrival[i]);
			fprintf(fo, "%d\t%d\t%d\t%c\n", process[i], total_wait, total_wait + 1, '-');
			avg_turn = avg_turn + total_wait + quantum - arrival[i];
			avg_wait = avg_wait + total_wait - arrival[i];
			total_wait = total_wait + quantum;
			burst[i] = burst[i] - quantum;
			if((i + 1) == cnt)
				i = 0;
			else
				i++;
		}
		else if(burst[i] != 0 && arrival[i] <= total_wait && burst[i] <= quantum) {
			printf("%d\t%d\t%d\t%d\t%d\n", process[i], total_wait, total_wait + burst[i], total_wait + burst[i] - arrival[i], total_wait - arrival[i]);
			fprintf(fo, "%d\t%d\t%d\t%c\n", process[i], total_wait, total_wait + 1, '-');
			avg_turn = avg_turn + total_wait + burst[i] - arrival[i];
			avg_wait = avg_wait + total_wait - arrival[i];
			total_wait = total_wait + burst[i];
			burst[i] = 0;
			final--;
			if((i + 1) == cnt)
				i = 0;
			else
				i++;
		}
		else {
			if((i + 1) == cnt)
				i = 0;
			else
				i++;
		}
	}

	printf("--------------------------------------\n");
	avg_turn = avg_turn / cnt;
	avg_wait = avg_wait / cnt;
	printf("Average turnaround time : %f\nAverage waiting time : %f\n", avg_turn, avg_wait);
	printf("--------------------------------------\n");
    fclose(fo);
}

void srtf(int process[32], int arrival[32], int burst[32], int priority[32], int cnt) {
	int wait[cnt], i, j, temp, total_wait = 0, final = cnt;
	float avg_turn = 0, avg_wait = 0;

	FILE *fo = fopen("./ganttChartScripts/dat/SRTFforGanttChart.dat", "w");
	printf("Process Start   End     Turn   Waiting\n");
	printf("--------------------------------------\n");
	while(final != 0) {
		for(i = 0; i < cnt; i++) {
			for(j = i + 1; j < cnt; j = j + 2) {
				if(burst[i] > burst[j]) {
					temp = arrival[i];
					arrival[i] = arrival[j];
					arrival[j] = temp;

					temp = process[i];
					process[i] = process[j];
					process[j] = temp;

					temp = burst[i];
					burst[i] = burst[j];
					burst[j] = temp;
				}
				else if(burst[i] == burst[j]) {
					if(process[i] > process[j]) {
						temp = arrival[i];
						arrival[i] = arrival[j];
						arrival[j] = temp;

						temp = process[i];
						process[i] = process[j];
						process[j] = temp;

						temp = burst[i];
						burst[i] = burst[j];
						burst[j] = temp;
					}
				}
			}
		}

		for(i = 0; i < cnt; i++) {
			if(burst[i] > 0 && arrival[i] <= total_wait) {
				printf("%d\t%d\t%d\t%d\t%d\n", process[i], total_wait, total_wait + 1, total_wait + 1 - arrival[i], total_wait - arrival[i]);
				fprintf(fo, "%d\t%d\t%d\t%c\n", process[i], total_wait, total_wait + 1, '-');
				avg_turn = avg_turn + total_wait + 1 - arrival[i];
				avg_wait = avg_wait + total_wait - arrival[i];
				burst[i]--;
				total_wait++;
				if(burst[i] == 0)
					final--;
				//i = 0;
				break;
			}
		}
	}

	printf("--------------------------------------\n");
	avg_turn = avg_turn / cnt;
	avg_wait = avg_wait / cnt;
	printf("Average turnaround time : %f\nAverage waiting time : %f\n", avg_turn, avg_wait);
	printf("--------------------------------------\n");
    fclose(fo);
}
