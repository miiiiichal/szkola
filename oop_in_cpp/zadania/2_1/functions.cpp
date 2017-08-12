#include <iostream>
#include "functions.h"


void fun1(int a){
	std::cout<<"inside fun1: a_val : "<<a <<std::endl;
}

void get_valByRef(int &a){
	std::cout<<"Podaj wartość : ";
	std::cin>>a;
}

int get_valByVal(){
	int a;
	std::cout<<"Podaj wartość : ";
	std::cin >> a;
	return(a);
}

void get_valByPtr(int *ptr){
	
	std::cout<<"Podaj wartość : ";
	std::cin >> *ptr;
}

void print_val(int a){
	
	std::cout<<"print val : "<<std::endl;

	std::cout<<"addr:"<<&a <<"| dec: "<< a  <<std::endl;
}

void print_valRef(int &a){

	std::cout<<"print val from ref : "<<std::endl;

	std::cout<<"addr:"<<&a <<"| dec: "<< a <<std::endl;
}

void print_val(int *a){

	std::cout<<"Print val from ptr: "<<std::endl;
	std::cout<<"addr:"<<a <<"| dec: "<< *a <<std::endl;
}
