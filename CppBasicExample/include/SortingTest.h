#include<iostream>
#include<vector> 
using namespace std;
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


int main(){   int i,j;int temp;
    for(i=0;i<MAX;i++)cout<<b[i]<<" "; cout<<endl;  
     
    //------------------------------------
    seta(a);//冒泡法，稳定排序，平均时间O(n*n)，最坏时间O(n*n)，最好时间O(n) 
    for(i=0;i<(MAX-1);i++)
       for(j=0;j<(MAX-1-i);j++)
         if(a[j]>a[j+1]){temp=a[j];a[j]=a[j+1];a[j+1]=temp;}
    cout<<"冒泡法:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";  
    cout<<endl; 
    //---------------------------------
    seta(a);//快速排序 这是一个分治算法，注意主元的概念。使用了辅助空间，不稳定算法。
            //平均时间O(n*logn)，最坏时间O(n*n)，最好时间O(n*logn)  
    qsort(a,0,MAX-1); //Recursive
    cout<<"快速排序:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";
    cout<<endl;
    //-------------------------------------
    seta(a);// 插入排序,最简单的稳定排序，平均时间O(n*n)，最坏时间O(n*n)，最好时间O(n)。想想抓牌的过程  
    insertionSort(a);
    cout<<"插入排序:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";  
    cout<<endl;  
    //------------------------------------
    //选择排序: 简单，但是是效率最低的排序。所有时间都是O(n*n)。 1、比较2个关键字大小 2、交换元素 
    //-----------------------------
    //堆排序：很理想的排序，所有时间都是O(n*logn)
    //------------------------- 
    //归并排序。来源于把两个相邻有序记录归并为一个有序记录 
    //很理想的排序，所有时间都是O(n*logn)，缺点是需要辅助空间O(n)，它是稳定的排序
    seta(a);
    MergeSort(a); //Recursive
    cout<<"归并排序:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";
    cout<<endl;
    //------------------------------------------------------------------------------
    getchar(); 
    return 0;
} 



void MergeSort(int a[]){
   MergeSortRecursion(a, a, 0, MAX-1);  
}
void MergeSortRecursion(int input[], int output[], int start, int end){
	if(start == end) {output[start] = input[start]; return; }

    	int tmpOutput[MAX]; //辅助空间 
	
	int mid = (start + end)/2;
	MergeSortRecursion(input, tmpOutput, start, mid);//排好前半部分 
	MergeSortRecursion(input, tmpOutput, mid+1, end);//排好后半部分 

	/*比较两个指针所指向的元素，选择相对小的元素放入到合并空间，并移动指针到下一位置
	重复步骤直到某一指针达到序列尾将另一序列剩下的所有元素直接复制到合并序列尾*/
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
	int j = end; //a[start]是主元 
	while(true){//经过这个while循环,数组被分为两半，前半都比主元后半小，后半都比主元大 
		while(a[i] < a[start] && i!=end)i++; 
		while(a[j] > a[start] && j!=start)j--; 
		if(i >= j) break;
		swap(a[i], a[j]); 
	}
	swap(a[start], a[i]);//把主元start插入到i(或j)的位置 
	qsort(a, start, i-1);//再以新i(或j)为界线分两半
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