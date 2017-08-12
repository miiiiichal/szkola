#include <iostream>
#include "functions.h"

int main(){
	int a_val;
	int *a_ptr;
	int &a_ref = a_val;
	std::cout<<"zad 2 : getVal_funcs "<<std::endl;
//	std::cout<<"get int value : ";
//	std::cin>>a_val;
	a_val = get_valByVal();
	print_val(a_val);
	
	get_valByPtr(&a_val);
	print_val(&a_val);
		

//	get_valByPtr(a_ptr);
	get_valByRef(a_ref);
	print_valRef(a_ref);

	print_valRef(a_val);
//	fun1(a_val);
	std::cout<<"-----ptr-------: "<< a_ptr <<std::endl;
	a_ptr = &a_val;
	std::cout<<"-----ptr-------: "<< a_ptr <<std::endl;
	std::cout<<"ptr_value: "<<*a_ptr<<std::endl;


return 0;
}



