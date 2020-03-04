/*
 Program: template.h. Describes how a header file is defined.
 Created on: 24 Jan 2020
 Edited on: 24 Jan 2020
 Created by: Yourself
*/

// The preprocessor statements here are referred to as 'include guards'
// They prevent a file from being included too many times
#ifndef TEMPLATE_H  //Preprocessor conditional statement
#define TEMPLATE_H  

#include <stdio.h>

// Functions in header files do not require a {body}. It will be in the source files instead.
void firstFunction();

int secondFunction(int param1, int param2);


// Ending preprocessor statement for first condition if
#endif