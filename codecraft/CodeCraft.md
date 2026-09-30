# Smart Pointer
RAII 是 Resource Acquisition Is Initialization 的缩写  
中文通常翻译为： 资源获取即初始化  
把资源的生命周期绑定到对象的生命周期上。   
```
void GoldCraft() override{
    std::unique_ptr<int[]> arr;
    arr = std::make_unique<int[]>(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    std::cout<<arr[0]<<","<<arr[1]<<","<<arr[2]<<std::endl;
}
```
小知识：  
如果一个函数负责创建/分配动态数组，并且希望数组生命周期由调用者继续管理，  
那么用 std::unique_ptr 传入通常比裸指针更安全、更清晰。  
```
#include <memory>
void createArray(std::unique_ptr<int[]>& arr){
    arr = std::make_unique<int[]>(10);

    arr[0] = 100;
    arr[1] = 200;
}
int main(){
    std::unique_ptr<int[]> arr;
    createArray(arr);

    // arr 现在拥有这块数组
    // 可以继续使用
    // arr[0] == 100

} // main结束，arr自动释放数组
```
unique_pointer作为返回值：因为 unique_ptr 不能拷贝，只能移动，函数返回时编译器会自动进行 move/返回值优化。 
```
#include <memory>
std::unique_ptr<int[]> createArray(){
    auto arr = std::make_unique<int[]>(10);

    arr[0] = 100;
    arr[1] = 200;

    return arr;
}
int main(){
    auto arr = createArray();

    // 使用
    std::cout << arr[0];

} // arr 析构，数组自动释放
```


