#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

struct node{
    int memsize;
    int allocp;
    int pos;
    int allocSize;
    int comp_t;
}m[200];

void merge(struct node arr[], int l, int m, int r);
void mergeSort(struct node arr[], int l, int r);
int largest(int arr[][3], int n);
void display(int nm, int np, int iF, int t, int globalFlag, int P[][3], char statP[]);
void wait( int seconds );


int main(){
    int nm,np, i, j, p[200][3], iF, MAT;
    for(int i=0; i<200; i++){
        m[i].allocp = -1;
    }
    printf("\nEnter number of blocks: ");
    scanf("%d", &nm);
    printf("\n");
    for(i=0;i<nm;i++){
        while ((getchar()) != '\n');
        printf("Enter block size of MB%d : ", (i+1));
        scanf("%d", &m[i].memsize);
        m[i].pos=i;
    }
    printf("\n\nEnter number of processes: ");
    scanf("%d",&np);
    printf("\n");
    for(i=0;i<np;i++){
        while ((getchar()) != '\n');
        printf("Enter process size of P%d: ", (i+1));
        scanf("%d", &p[i][0]);
        printf("Enter Arrival Time of P%d: ", (i+1));
        scanf("%d", &p[i][1]);
        printf("Enter Burst Time of P%d: ", (i+1));
        scanf("%d", &p[i][2]);
        printf("\n");
    }

    int total_t = 0;
    total_t = largest(p, np);
    while ((getchar()) != '\n');
    printf("\nEnter \n\t1 for first fit \n\t2 for best fit \n\t3 for worst fit\n: ");
    scanf("%d",&MAT);
    printf("\n\n");
    int globalFlag=0;
    struct node sortedArr[nm];
    for(i=0; i<nm; i++){
        sortedArr[i] = m[i];
    }
    mergeSort(sortedArr, 0, nm-1);
    char statP[np];  // W0=Waiting; U1=UnAllocated; R2=Running; C3=Completed;
    for(i=0; i<np; i++){
        statP[i]='W';
    }
    if(MAT==2){
        for(int t=0; t<total_t; t++){
            for(i=0; i<np; i++){
                for(j=0; j<nm; j++){
                    if(p[i][0]<=sortedArr[j].memsize && sortedArr[j].allocp==-1 && p[i][1]==t){
                        sortedArr[j].allocp=i;
                        sortedArr[j].allocSize=p[i][0];
                        sortedArr[j].comp_t= p[i][1]+p[i][2];
                        statP[i]='R';
                        break;
                    }
                }
                for(int k=0; k<nm; k++){
                    if(m[k].pos == sortedArr[j].pos){
                        m[k] = sortedArr[j];
                        break;
                    }
                }
            }
            for(i=0; i<nm; i++){
                if(t==0){
                    break;
                }
                if(t==sortedArr[i].comp_t){
                    sortedArr[i].allocp = -1;
                    sortedArr[i].allocSize=0;
                    sortedArr[i].comp_t=0;
                }
                if(t==m[i].comp_t){
                    statP[m[i].allocp]='C';
                    m[i].allocp = -1;
                    m[i].allocSize=0;
                    m[i].comp_t=0;
                }
            }
            display(nm, np, iF, t, globalFlag, p, statP);
        }
    }
    else if(MAT==3){
        for(int t=0; t<total_t; t++){
            for(i=0; i<np; i++){
                for(j=nm-1; j>=0; j--){
                    if(p[i][0]<=sortedArr[j].memsize && sortedArr[j].allocp==-1 && p[i][1]==t){
                        sortedArr[j].allocp=i;
                        sortedArr[j].allocSize=p[i][0];
                        sortedArr[j].comp_t= p[i][1]+p[i][2];
                        statP[i]='R';
                        break;
                    }
                }
                for(int k=nm-1; k>=0; k--){
                    if(m[k].pos == sortedArr[j].pos){
                        m[k] = sortedArr[j];
                        break;
                    }
                }
            }
            for(i=0; i<nm; i++){
                if(t==0){
                    break;
                }
                if(t==sortedArr[i].comp_t){
                    sortedArr[i].allocp = -1;
                    sortedArr[i].allocSize=0;
                    sortedArr[i].comp_t=0;
                }
                if(t==m[i].comp_t){
                    statP[m[i].allocp]='C';
                    m[i].allocp = -1;
                    m[i].allocSize=0;
                    m[i].comp_t=0;
                }
            }
            display(nm, np, iF, t, globalFlag, p, statP);
        }

    }
    else{
        for(int t=0; t<total_t; t++){
            for(i=0;i<np;i++){
                for(j=0;j<nm;j++){
                    if(p[i][0]<=m[j].memsize && m[j].allocp==-1 && p[i][1]==t){
                        m[j].allocp=i;
                        m[j].allocSize=p[i][0];
                        m[j].comp_t= p[i][1]+p[i][2];
                        statP[i]='R';
                        break;
                    }
                }
            }
            for(i=0; i<nm; i++){
                if(t==0){
                    break;
                }
                if(t==sortedArr[i].comp_t){
                    sortedArr[i].allocp = -1;
                    sortedArr[i].allocSize=0;
                    sortedArr[i].comp_t=0;
                }
                if(t==m[i].comp_t){
                    statP[m[i].allocp]='C';
                    m[i].allocp = -1;
                    m[i].allocSize=0;
                    m[i].comp_t=0;
                }
            }
            display(nm, np, iF, t, globalFlag, p, statP);
        }
    }
    for(i=0; i<nm; i++){         
        if(total_t==m[i].comp_t){
            statP[m[i].allocp]='C';
            m[i].allocp = -1;
            m[i].allocSize=0;
            m[i].comp_t=0;
        }
    }
    for(i=0; i<np; i++){
        if(statP[i]=='W'){
            statP[i]='U';
            globalFlag = 1;
        }
    }
    display(nm, np, iF, total_t, globalFlag, p, statP);
    
    return 0;
}

void merge(struct node arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    /* create temp arrays */
    struct node L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].memsize <= R[j].memsize) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(struct node arr[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}

int largest(int arr[][3], int n)
{
    int i, maxI=0;
 
    // Initialize maximum element
    int max = arr[0][1]+ arr[0][2];
 
    // Traverse array elements from second and
    // compare every element with current max
    for (i = 1; i < n; i++){
        if (max < (arr[i][1]+arr[i][2])){
            max = arr[i][1]+arr[i][2];
            maxI = i;
        }
    }
        
    return max;
    
}

void wait( int seconds )
{   // Pretty crossplatform, both ALL POSIX compliant systems AND Windows
    #ifdef _WIN32
        Sleep( 1000 * seconds );
    #else
        sleep( seconds );
    #endif
}

void display(int nm, int np, int iF, int t, int globalFlag, int P[][3], char statP[])
{
    wait(1);
    system("clear");
    printf("\n\n");
    printf("\t\t_______________________\n");
    printf("\n\t\t\tTime : %d\n", t);
    printf("\t\t_______________________\n\n\n");
    int intFrag=0,extFrag=0;
    printf("\tMemory\t\t");
    for(int i=0;i<nm;i++){
        printf("%d\t", m[i].memsize);
    }
    printf("\n");
    printf("\tP. Alloc.\t");
    for(int i=0;i<nm;i++){
        if(m[i].allocp!=-1){
            printf("P%d\t",(m[i].allocp+1));
        }
        else{
            printf("Empty\t");
        }
    }
    printf("\n");
    printf("\tP. Size.\t");
    for(int i=0;i<nm;i++){
        if(m[i].allocp!=-1){
            printf("%d\t",(m[i].allocSize));
        }
        else{
            printf("Empty\t");
        }
    }
    printf("\n");
    printf("\tInt. Frag.\t");
    for(int i=0;i<nm;i++){
            if(m[i].allocp!=-1){
                iF = m[i].memsize-m[i].allocSize;
                printf("%d\t",iF);
                intFrag+=m[i].memsize-m[i].allocSize;
            }
            else{
                extFrag+=m[i].memsize;
                printf("Empty\t");
            }
    }
    printf("\n\n");
    if(globalFlag==1)
        printf("\tTotal External Fragmentation:\t %d\n",extFrag);
    else
    {
        printf("\tAvailable Memory: %d\n",extFrag);   
    }
    printf("\tTotal Internal Fragmentation:\t %d\n",intFrag);
    printf("\tWastage  of  Memory:\t\t %d\n\n", (extFrag+intFrag));
    for(int i=0; i<np; i++){
        if(statP[i]=='U') printf("\tUnallocated Process \tP%d (%d)\n", (i+1), P[i][0]);
        else if(statP[i]=='R') printf("\tRunning... Process \tP%d (%d)\n", (i+1), P[i][0]);
        else if(statP[i]=='C') printf("\tCompleted Process \tP%d (%d)\n", (i+1), P[i][0]);
        else printf("\tWaiting... Process \tP%d (%d)\n", (i+1), P[i][0]);
    }
    printf("\n");
}