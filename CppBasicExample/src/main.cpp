#include "../include/common.h"
#include "../include/structTest.h"
#include "../include/functionTest.h"
#include "../include/classTest.h"
#include "../include/templateTest.h"

int main(){
	StructTest();

	FunctionTest();

	ClassTextI_BaseClass_DerivedClass();
	cout<<endl;

	ClassTextII_DestructorIssue();
	cout<<endl;

	ClassTextIII_VirtualDestructor();
	cout<<endl;
	
	ClassTestIV_PureVirtualFunction();

	ClassTextV_Copy();
	cout<<endl;
	
	return 0;
}
