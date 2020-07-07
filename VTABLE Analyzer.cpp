//Demonstration of internal working of Function  Overriding in Run Time Polymorphism

#include<iostream>
using namespace std;

class Base
{
	int a;
	public:
		virtual void fun()
		{
			cout<<"inside Base fun"<<endl;
		}
		
		virtual void gun()
		{
			cout<<"inside Base gun"<<endl;
			
		}
		void sun()
		{
			cout<<"inside Base sun"<<endl;
		}
		
	
};

class Derived : public Base
{
	int x,y;
	public:
		void fun()
		{
			cout<<"inside Derived fun"<<endl;
		}
		virtual void run()
		{
			cout<<"inside Derived run"<<endl;
		}
	
};

int main()
{
	Derived dobj;
	cout<<"\nsize of  Derived object : "<<sizeof(dobj)<<endl;
	cout<<"Base adress of Derived object : "<<&dobj<<endl;
	
	//typecasting Derived* to long int*
	long int *dp=(long int*)(&dobj);

	
	long int *vptr=(long int *)(*dp);
	cout<<"Adress of VPTR 	: "<<dp<<endl;
	cout<<"Adress of VTABLE : "<<vptr<<endl;
	
	
	//fp is function pointer
	void (*fp)();
	
	//here the base class fun is overrided 
	fp=(void(*)())(*vptr);
	fp();
	
	//second function of VTABLE i.e base class gun 
	fp=(void(*)())(*(vptr+1));
	fp();
	
	//third function of VTABLE i.e Derived class run
	fp=(void(*)())(*(vptr+2));
	fp();
	
	
	return 0;
	
}
