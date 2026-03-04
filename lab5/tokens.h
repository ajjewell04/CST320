#pragma once

//**************************************
// tokens.h
//
// Defines the values for the tokens returned by yylex
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#define PROGRAM     1000        // program
#define IF          1001        // if
#define ELSE        1002        // else
#define ENDIF       1003        // endif
#define WHILE       1004        // while
#define PRINT       1005        // print
#define CHAR        1006        // char
#define INT         1007        // int
#define LONG        1008        // Wlong
#define FLOAT       1009        // float
#define DOUBLE      1010        // double
#define INT_VAL     1011        // <an integer constant>
#define FLOAT_VAL   1012        // <a float constant>
#define STRUCT      1013        // struct
#define ARRAY       1014        // array
#define RETURN      1015        // return
#define IDENTIFIER  1016        // <a C style identifier>
#define NOT_EQUALS  1017        // !=
#define EQUALS      1018        // ==
#define AND         1019        // &&
#define OR          1020        // ||
#define GE          1021        // >=
#define LE          1022        // <=
#define PRINTS      1023        // prints
#define STRING_LIT  1024        // "stuff..."

#define JUNK_TOKEN  2000

