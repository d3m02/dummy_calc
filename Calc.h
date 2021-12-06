#pragma once

#include <iostream>
#include <string>
#include <conio.h>
#include <map>
#include <algorithm>
#include <vector>
#include <math.h>

// Clean screen. Not so elegant way, temporary solution
#ifdef _MSVC
#include <Windows.h>
#define cls() system("cls")
#else
#define cls() system("clear");
#endif

// Sweet red colored output for our error
#define error_mess(x) "\033[0;31m"<<x<<"\033[0m"<<std::endl
#define trace(x) std::cout<<"\n\t\033[0;33m"<<x<<"\033[0m"<<std::endl
#define warn_symbol(x) error_mess(x)<<"\033[0;33m Unexpected symbol. You can delete it with Backspace\033[0m"

#define HeaderMess() std::cout << "\033[0;32m"\
		<< "\t\t Hi there! \n"\
		"So, it's very dummy calc. Yeah, give it a try.\n"\
		"Write your query and when you hit \"=\" - you receive solution"\
		"Current available operations:\n"\
		"\t[*]\t[/ or \\]\t[-]\t[+]\n"\
		"\t[sqrt]\t[^]\t[()]"\
		<< "\033[0m" << std::endl <<std::endl<< "< "


class Calc
{
private:
	enum error_code {
		NoError = 0,
		DivByZero = -1,
		NegativeSqrt = -2,
		ValidationError = -3
	};

	std::string m_query;
	error_code m_status;
	bool m_trace;


	/**
	*	Find operation with higher priority.
	*	Priority:
	*		- brackets
	*		- sqrt
	*		- power (^)
	*		- multiplication (*) and division (/)
	*		- addition (+) and substitute (-)
	**/
	int FindPrioOp(std::string* query);

	/**
	* Why there another function? Well, I though, it can be good idea, to separate finding operation and performing actual operation.
	* First - more clear code. Merge both function - you get possibly huge block of code, which scanning inputted line for operation sign, trim values,
	*         make calculation and repeat it.
	* Second - FindPropOp - tells is there any possible operation and where is this location.
	*		   PerformOp - just make calculation, when operands and operation char are known, we need just make right calculation with.
	* Third - it's sounds a little bit weird: GetAnswer->solve->*another solve*? That’s why all parse\priority stuff in Calc::solve function
	**/
	void PerformOp(std::string* op, std::string* l_value, std::string* r_value, std::string* res);

	void solve(std::string* query);
	/**
	* During calculation, we need to make some parts unique.
	* Division can be with "\" sign and "/" sign, space-symbols unneeded, in numbers with floating point - someone use ',', but c++ float require '.'
	* So, we give user some free movements during query inputting, but under the hood - make all thing like WE want >:-)
	**/
	void format(std::string* query);

	/**
	* -- "But wait a minute, above, in third point you tell something about weird stuff. And here - GetAnswer->solve"
	* Yeah, strange. But Calc::solve function is calling recursively. And somewhere I need to place final solution, and important thing - error messaging.
	* For not repeating same error handling output - I decide to merge it with Calc::solve output.
	**/
	void GetAnswer();

public:
	void clear();

	Calc();

	void validate();
	void input();
	void input(std::string query);

	/*
	* Show calculation steps
	*/
	void trace_res(bool val);
};
