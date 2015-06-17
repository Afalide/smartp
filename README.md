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

### Strong or weak?

There are plenty of tutorials on the web which explain how smart pointers works, however here's a very small explanation.

When a smart pointer (SP) holds a raw *pointer, it counts how many other SP have the same raw pointer. SPs count themselves.

This is called a `strong ref`. When a SP is destroyed, it decreases the strong ref count, and when the count reaches 0, the last SP will destroy the internal raw *pointer.

This is very simple and efficient, however there are some drawbacks.

 - You must not play with SPs addresses. SPs are intended to be used as normal primitives variables for the stack mechanism to work properly.
 
 - The SP encapsulates a raw *pointer, this has a natural (but nearly invisible) performance cost, like any "encapsulation pattern" does. 

 - Circular reference will provoke a memory leak.

Example for a circular reference:

For example:

	//a simple struct which holds a smart pointer of another struct

	struct myclass
	{
		smartp<myclass> something;

		public:
		myclass() : something(nullptr) { };	
	};
	
	
	
	int main()
	{
	    smartp<myclass> a(new myclass);
	    smartp<myclass> b(new myclass);
	    smartp<myclass> c(new myclass);
	
	    a->something = b;
	    b->something = c;
	    c->something = a;
	
	    return 0;
	}

Here the circular reference is `a -> b -> c -> a`. This execution results in a memory leak, since the `a` object is not deleted because of an extra strong ref count.

The solution is to use a weak pointer (WP). WPs are created from an existing SP:

	smartp<myclass> sp(new myclass);
	weakp<myclass> wp = sp;

Then you can use the wp the same way:

	wp->do_stuff();

Thus, the myclass should look like this:

	class myclass
	{
		weakp<myclass> something;
	}


WP holds a count wich is called... `weak count`. Unlike the SP, the WP never deletes the internal raw *ptr. This is the SP's job. 
Therefore, the reference cycle is not a problem, there will be no extra strong counts.

However, the WP is considered as unsafe: even if a WP exists, the last SP may be already dead, which means the internal raw *ptr is deleted. Before calling a WP, you should check if it is safe to do so:

	wp->get();

This will return NULL if the internal raw* ptr is deleted.

### How to compile

Only one file must be compiled (the ref_counter class), since the other classes are templates.

Compile the lib:

	g++ -c src/ref_counter.cpp -I include -o ref_counter.o -DSMARTP_BUILDING_LIB
	
On Windows, create a dll and an import library

	g++ -shared -o smartp.dll ref_counter.o -Wl,--out-implib,smartp.dll.a

Brace yourselves, makefile is coming.
