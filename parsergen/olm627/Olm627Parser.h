/* DO NOT EDIT THIS FILE - it is machine generated */
#pragma once

#include <LRparser.h>

const extern ParserTables *Olm627Tables;

#line 5 "C:\\mytools2015\\parsergen\\olm627\\Olm627.y"
#include "olm627symbol.h"

class Olm627Parser : public LRparser {
public:
  Olm627Parser(Scanner *lex=NULL) : LRparser(*Olm627Tables,lex) {}
private:
  int m_dollardollar,*m_stacktop,m_userstack[256];
  int reduceAction(unsigned int prod);
  void userStackInit()         { m_stacktop = m_userstack;                           }
  void userStackShiftSymbol(unsigned int symbol) { m_stacktop++;                     } // push 1 element (garbage) on userstack
  void userStackPopSymbols( unsigned int count)  { m_stacktop     -= count;          } // pop count symbols from userstack
  void userStackShiftDollarDollar()              { *(++m_stacktop) = m_dollardollar; } // push($$) on userstack
  void defaultReduce(       unsigned int p)      { m_dollardollar  = *m_stacktop;    } // $$ = $1
};


#line 18 "C:\\mytools2015\\parsergen\\lib\\parsegencpp.par"
