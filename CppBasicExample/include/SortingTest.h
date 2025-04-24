#ifndef __SORTTEST_H__
#define __SORTTEST_H__

#include "common.h"

#define MAX 10 
    
int b[MAX]={13,31,49,1,8,3,2,33,6,12};
int a[MAX];
void seta(int a[]);
void qsort(int *,int,int); 
void insertionSort(int a[]);  
void MergeSort(int a[]); 
void MergeSortRecursion(int SR[], int TR1[], int s, int t);
void Merge(int SR[],int TR[],int i,int m, int n);
void BinarySearch(int *a, int lower, int upper, int target);


void SortingTest(){   
    cout << "=====Sorting Test=====" << endl;

    cout<<"Input:";
    int i,j;int temp;
    for(i=0;i<MAX;i++)cout<<b[i]<<" "; cout<<endl;  
     
    //------------------------------------
    cout<<"Bubble sort, stable, average O(n*n), worst O(n*n), best O(n) "<<endl;
    seta(a);
    for(i=0;i<(MAX-1);i++)
       for(j=0;j<(MAX-1-i);j++)
         if(a[j]>a[j+1]){temp=a[j];a[j]=a[j+1];a[j+1]=temp;}
    cout<<"Bubble sort result:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";  
    cout<<endl; 
    //---------------------------------
    cout<<"Quick sort, divide and conquer, use extra storage, not stable. average O(n*logn), worst O(n*n), best O(n*logn) "<<endl;
    seta(a);
    qsort(a,0,MAX-1); //Recursive
    cout<<"Quick sort result:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";
    cout<<endl;
    //-------------------------------------
    cout<<"Insert sort, simple and stable. average O(n*n), worst O(n*n), best O(n) "<<endl;
    seta(a);
    insertionSort(a);
    cout<<"Insert sort result:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";  
    cout<<endl;  
    //------------------------------------
    //Not implemented: selection sort, simple, all time is O(n*n). 1. compare two elements; 2. swap elements
    //-----------------------------
    //Not implemented: Heap sort, O(n*logn)
    //------------------------- 
    cout<<"Merge sort, all time is O(n*logn), need extra storage O(n), stable"<<endl;
    seta(a);
    MergeSort(a); //Recursive
    cout<<"Merge sort result:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";
    cout<<endl;
    //------------------------------------------------------------------------------
    cout<<endl;
} 



void MergeSort(int a[]){
   MergeSortRecursion(a, a, 0, MAX-1);  
}
void MergeSortRecursion(int input[], int output[], int start, int end){
	if(start == end) {output[start] = input[start]; return; }

    	int tmpOutput[MAX]; //extra storage
	
	int mid = (start + end)/2;
	MergeSortRecursion(input, tmpOutput, start, mid);//sort the first half
	MergeSortRecursion(input, tmpOutput, mid+1, end);//sort the second half

	/*compare the two elements of the two points, choose the smaller one and put into merge space, and move pointer to the next position
	repeat until one of the pointer reaches the end, then put the rest elements of the other part to the end of the merge area*/
	int outputIndex = start;
	const int midPos = mid;
	mid++;
	//loop start to midPos
	//loop mid to end
	//loop outIndex from start to end
    while(start <= midPos && mid <= end){ 
         if(tmpOutput[start] <= tmpOutput[mid]) output[outputIndex++] = tmpOutput[start++];
         else output[outputIndex++] = tmpOutput[mid++];
    }
    while(start <= midPos) output[outputIndex++] = tmpOutput[start++];
    while(mid <= end) output[outputIndex++] = tmpOutput[mid++];
}







void insertionSort(int a[]){
   int j;
   for(int p = 1; p < MAX; p++){
      int tmp = a[p];
      for(j = p; j > 0 && tmp < a[j-1]; j--)  a[j] = a[j-1]; //backward
      a[j] = tmp; 
   } 
} 

/*
template<typename T>
void insertionSort(T a[]){
	int j;
	for (int p = 1; p < MAX; p++){
		T tmp = a[p];
		for (j = p; j >0; j--)
			if(tmp < a[j-1]) a[j] = a[j-1];
			else break;
		a[j] = tmp;
	}
}
*/


void Swap(int &x, int &y){
	int tmp = x;
	x = y;
	y = tmp;
}

void qsort(int a[], int start, int end){ 
    if(start >= end) return;
    int i = start;
	int j = end; //a[start] is main element
	while(true){//after the while loop, the array is divided into two, the first half are smaller than the main element; the second half are larger than the main element
		while(a[i] < a[start] && i!=end)i++; 
		while(a[j] > a[start] && j!=start)j--; 
		if(i >= j) break;
		swap(a[i], a[j]); 
	}
	swap(a[start], a[i]);//insert main element into i(or j) pos
	qsort(a, start, i-1);//then use new i(or j) to divide into two half
	qsort(a, i+1, end);
}  

/*
void qsort(int a[], int start, int end){
	if (start >= end) return;
	int i = start;
	int j = end;
	while (true){
		while (a[i] < a[start] && i != end) i++;
		while (a[j] > a[start] && j != start) j--;
		if (i >= j) break;
		swap(a[i], a[j]);
	}
	swap(a[start], a[i]);
	qsort(a, start, i);
	qsort(a, i+1, end);
}
*/

void seta(int a[]){
    for(int i=0;i<MAX;i++){
            a[i]=b[i];        
    }
}


void BinarySearch(int *a, int lower, int upper, int target){

	//int center, range;

 //   range = upper - lower;
 //   if (range < 0) {
 //       return LIMITS_REVERSED;
 //   } else if( range == 0 && array[lower] != target ){
 //       return NOT_IN_ARRAY;
 //   }

 //   if( array[lower] > array[upper] )
 //       return ARRAY_UNORDERED;

 //   center = ((range)/2) + lower;

 //   if( target == array[center] ){
 //       return center;
 //   } else if( target < array[center] ){
 //       return binarySearch( array, lower, center - 1, target );
 //   } else {
 //       return binarySearch( array, center + 1, upper, target );
 //   }

	//or


    //int center, range;

    //if( lower > upper )
    //    return LIMITS_REVERSED;

    //while( true ){
    //    range = upper - lower;
    //    if( range == 0 && array[lower] != target )
    //        return NOT_IN_ARRAY;

    //    if( array[lower] > array[upper] )
    //        return ARRAY_UNORDERED;

    //    center = ((range)/2) + lower;
    //    if( target == array[center] ){
    //        return center;
    //    } else if( target < array[center] ){
    //        upper = center - 1;
    //    } else {
    //        lower = center + 1;
    //    }
    //}

}


#endif
