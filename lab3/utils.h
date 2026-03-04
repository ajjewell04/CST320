#pragma once
#include <string>
//**********************************************
// Various utilities that might be useful for a top-down recursive-descent 
// parser
//
// Author: Phil Howard
//
// NOTE:
//    You can use either PeekToken()/AdvanceToken() or GetToken()/UngetToken() 
//    but you can not use both sets of functions in the same program.

// used to print error messages in a standard format.
// expecting is the string representation of the token the parser was expecting.
void Error(std::string expecting);

// Peek a token witout removing it from the input stream
int PeekToken();

// Remove a token from the input stream, and return it.
// Note: the return value is the token after the advance.
int AdvanceToken();

// Get the next token from the input stream
int GetToken();

// Place a token back into the input stream.
// NOTE: you can only replace a single token. Multiple calls to
// UngetToken without intervening calls to GetToken is erroneous.
int UngetToken(int token);
