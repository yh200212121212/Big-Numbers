/* DO NOT EDIT THIS FILE - it is machine generated */
#pragma once

#include <LRparser.h>

const extern ParserTables *Java5Tables;

#line 5 "C:\\mytools2015\\ParserGen\\java\\Java5.y"
#pragma once

#include <LRparser.h>
#include "Java5Symbol.h"
#include "Java5Lex.h"

const extern ParserTables *Java5Tables;

class Java5Parser : public LRparser {
private:
  int reduceAction(unsigned int prod);

  void userStackInit() {                     // Called before the first parsecycle
  }

  void userStackShiftSymbol(unsigned int symbol) {    // Called when LRparser shift in inputtoken
  }

  void userStackPopSymbols(unsigned int count) {       // Pop count symbols from userstack
  }

  void userStackShiftDollarDollar() {        // Push($$) to userstack. called at the end of each reduction
  }

  void defaultReduce(unsigned int prod) {             // $$ = $1
  }

  int *getStackTop(int fromtop) {
    return NULL;
  }
  String m_fileName;
public:
  Java5Parser(const TCHAR *fileName, Scanner *lex=NULL, unsigned int stacksize = 1000) : LRparser(*Java5Tables,lex,stacksize) {
    m_fileName = fileName;
  }
  ~Java5Parser() {
  }
};


#line 18 "C:\\mytools2015\\parsergen\\lib\\parsergencpp.par"

