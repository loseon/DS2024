#include<iostream>
using namespace std;
class per
{
    // friend void fun(per *oooo);
    public:
    int name,age=10;
    per(int a,int c){
    name = c;
    age =a;
    cout<<age<<endl;
   }; 
   per(const per &b){
    age =b.age;
    name=b.name;
    cout<<age<<endl;
   };
   void ttttt(per *p){
    cout<<p->height<<endl;
   };

   

   int test(int a){
    height=a;
    return height;
   }
   void prin(){
    cout<<height<<endl;
   }
   private:
   int height = 141;
};
// void fun(per *add){
//     cout<<add->height<<endl;
// }
// void fun2(per p){
//     cout<<p.height<<endl;
// }
void pri(int a){
    cout<<a<<endl;
   }
int main(){
    
    per p1(10, 4);
    per p2(p1);
    cout<<p2.test(11);
    p2.prin();
    int a=2;
    p2.ttttt(&p2);
    pri(p2.age);
    // fun(&p2);
    return 0;
}