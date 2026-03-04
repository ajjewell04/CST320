#pragma once
//**************************************
// parse.h
//
// Declares parse routines for top-down recursive-descent parser
// Use in CST 320 Lab2
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

// The following functions search for the named non-terminal
// They return true if found, false otherwise.
// As a side effect, the function will output an error message on failure.
bool FindPROG();
bool FindSTMTS();
bool FindSTMT();
bool FindEXPR();
bool FindEXPR_P();
bool FindPLUSOP();
bool FindTERM();
bool FindTERM_P();
bool FindTIMESOP();
