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
    seta(a);//ð�ݷ����ȶ�����ƽ��ʱ��O(n*n)���ʱ��O(n*n)�����ʱ��O(n) 
    for(i=0;i<(MAX-1);i++)
       for(j=0;j<(MAX-1-i);j++)
         if(a[j]>a[j+1]){temp=a[j];a[j]=a[j+1];a[j+1]=temp;}
    cout<<"ð�ݷ�:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";  
    cout<<endl; 
    //---------------------------------
    seta(a);//�������� ����һ�������㷨��ע����Ԫ�ĸ��ʹ���˸����ռ䣬���ȶ��㷨��
            //ƽ��ʱ��O(n*logn)���ʱ��O(n*n)�����ʱ��O(n*logn)  
    qsort(a,0,MAX-1); //Recursive
    cout<<"��������:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";
    cout<<endl;
    //-------------------------------------
    seta(a);// ��������,��򵥵��ȶ�����ƽ��ʱ��O(n*n)���ʱ��O(n*n)�����ʱ��O(n)������ץ�ƵĹ���  
    insertionSort(a);
    cout<<"��������:";
    for(i=0;i<MAX;i++)cout<<a[i]<<" ";  
    cout<<endl;  
    //------------------------------------
    //ѡ������: �򵥣�������Ч����͵���������ʱ�䶼��O(n*n)�� 1���Ƚ�2���ؼ��ִ�С 2������Ԫ�� 
    //-----------------------------
    //�����򣺺��������������ʱ�䶼��O(n*logn)
    //------------------------- 
    //�鲢������Դ�ڰ��������������¼�鲢Ϊһ�������¼ 
    //���������������ʱ�䶼��O(n*logn)��ȱ������Ҫ�����ռ�O(n)�������ȶ�������
    seta(a);
    MergeSort(a); //Recursive
    cout<<"�鲢����:";
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

    	int tmpOutput[MAX]; //�����ռ� 
	
	int mid = (start + end)/2;
	MergeSortRecursion(input, tmpOutput, start, mid);//�ź�ǰ�벿�� 
	MergeSortRecursion(input, tmpOutput, mid+1, end);//�źú�벿�� 

	/*�Ƚ�����ָ����ָ���Ԫ�أ�ѡ�����С��Ԫ�ط��뵽�ϲ��ռ䣬���ƶ�ָ�뵽��һλ��
	�ظ�����ֱ��ĳһָ��ﵽ����β����һ����ʣ�µ�����Ԫ��ֱ�Ӹ��Ƶ��ϲ�����β*/
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
	int j = end; //a[start]����Ԫ 
	while(true){//�������whileѭ��,���鱻��Ϊ���룬ǰ�붼����Ԫ���С����붼����Ԫ�� 
		while(a[i] < a[start] && i!=end)i++; 
		while(a[j] > a[start] && j!=start)j--; 
		if(i >= j) break;
		swap(a[i], a[j]); 
	}
	swap(a[start], a[i]);//����Ԫstart���뵽i(��j)��λ�� 
	qsort(a, start, i-1);//������i(��j)Ϊ���߷�����
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