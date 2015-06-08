##Smartp

Smartp is a small and standalone smart pointers library for non 2011 compliant compilers.

I originally wrote these classes to understand the strong/weak reference mechanisms for scholar purposes, but it ended as a reusable lib I use in many projects.

Since C++11 and boost also introduced smart pointers, you should use them if you can.
However, if you don't want to use boost for particular reason, or have an old compiler (like mine), this lib will be useful.

### How to use

The easy way is to create some pointer with the new operator and give it to a smart pointer.

	#include <iostream>
	#include "smartp.hpp"

	int main()
	{
		//create a shared pointer (strong ref)
	    smartp<std::string> sp = new std::string("plop");

		//use the -> operator
		sp->append("hello");

	    return 0;

	} //sp is out of scope, the string is destroyed

When the smart pointer goes out of scope, the created string pointer is deleted.

### How to compile

Only one file must be compiled (the ref_counter class), since the other classes are templates.

Compile the lib:

	g++ -c src/ref_counter.cpp -I include -o ref_counter.o -DSMARTP_BUILDING_LIB
	
On Windows, create a dll and an import library

	g++ -shared -o smartp.dll ref_counter.o -Wl,--out-implib,smartp.dll.a

Brace yourselves, makefile is coming.
