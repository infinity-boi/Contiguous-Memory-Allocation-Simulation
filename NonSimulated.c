// Done by Gourav Singh (K21ZNB61)

#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

struct node{
    int memsize;
    int allocp;
    int pos;
    int allocSize;
}m[200];

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

int main(){
    int nm,np, i, j, p[200], iF, MAT;
    for(int i=0; i<200; i++){
        m[i].allocp = -1;
    }

    printf("\n\tEnter number of blocks: ");
    scanf("\t%d", &nm);
    printf("\n\tEnter block size\n");
    for(i=0;i<nm;i++){
        scanf("\t%d", &m[i].memsize);
        m[i].pos=i;
    }
    printf("\n\n\tEnter number of processes: ");
    scanf("%d",&np);
    printf("\n\tEnter process size: \n");
    for(i=0;i<np;i++){
        scanf("\t%d", &p[i]);
    }
    printf("\n\n\tEnter \n\t\t1 for first fit \n\t\t2 for best fit \n\t\t3 for worst fit\n: ");
    scanf("%d",&MAT);
    printf("\n\n");
    int globalFlag=0;
    struct node sortedArr[nm];
    for(i=0; i<nm; i++){
        sortedArr[i] = m[i];
    }
    mergeSort(sortedArr, 0, nm-1);
    if(MAT==2){
        for(i=0; i<np; i++){
            int flag=0;
            j=0;
            while(m[j].allocp!=-1){
                j++;
            }
            for(j=0; j<nm; j++){
                if(p[i]<=sortedArr[j].memsize && sortedArr[j].allocp==-1){
                    sortedArr[j].allocp=i;
                    sortedArr[j].allocSize=p[i];
                    break;
                }
            }
            for(int k=0; k<nm; k++){
                if(m[k].pos == sortedArr[j].pos && m[k].allocp==-1){
                    m[k].allocp=i;
                    m[k].allocSize=p[i];
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                printf("\tUnallocated Process P%d \n", (i+1));
                globalFlag=1;    
            }
        }
    }
    else if(MAT==3){
        for(i=0; i<np; i++){
            int flag=0;
            j=0;
            while(m[j].allocp!=-1){
                j++;
            }
            for(j=nm-1; j>=0; j--){
                if(p[i]<=sortedArr[j].memsize && sortedArr[j].allocp==-1){
                    sortedArr[j].allocp=i;
                    sortedArr[j].allocSize=p[i];
                    break;
                }
            }
            for(int k=nm-1; k>=0; k--){
                if(m[k].pos == sortedArr[j].pos && m[k].allocp==-1){
                    m[k].allocp=i;
                    m[k].allocSize=p[i];
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                printf("\tUnallocated Process P%d \n", (i+1));
                globalFlag=1;    
            }
        }


    }
    else{
        for(i=0;i<np;i++){
            int flag=0;
            for(j=0;j<nm;j++){
                if(p[i]<=m[j].memsize && m[j].allocp==-1){
                    m[j].allocp=i;
                    m[j].allocSize=p[i];
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                printf("\tUnallocated Process P%d \n", (i+1));
                globalFlag=1;    
            }
        }
    }
    printf("\n");
    int intFrag=0,extFrag=0;
    printf("\tMemory\t\t");
    for(i=0;i<nm;i++){
        printf("%d\t", m[i].memsize);
    }
    printf("\n");
    printf("\tP. Alloc.\t");
    for(i=0;i<nm;i++){
        if(m[i].allocp!=-1){
            printf("P%d\t",(m[i].allocp+1));
        }
        else{
            printf("Empty\t");
        }
    }
    printf("\n");
    printf("\tInt. Frag.\t");
    for(i=0;i<nm;i++){
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
        printf("\tWastage  of  Memory:\t\t %d\n", (extFrag+intFrag));
    return 0;
}
