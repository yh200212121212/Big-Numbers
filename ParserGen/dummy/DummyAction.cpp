/* DO NOT EDIT THIS FILE - it is machine generated */
#line 29 "C:\\mytools2015\\ParserGen\\dummy\\Dummy.y"
#include "stdafx.h"
#include "dummyparser.h"

#line 21 "C:\\mytools2015\\ParserGen\\lib\\parsergencpp.par"

int DummyParser::reduceAction(unsigned int prod) {

  switch(prod) {
  case 1: /* S -> a S */
#line 36 "C:\\mytools2015\\ParserGen\\dummy\\Dummy.y"
    { printf("reduce by S -> a S\n"); }
#line 26 "C:\\mytools2015\\ParserGen\\lib\\parsergencpp.par"
    break;
  case 2: /* S -> S a */
#line 37 "C:\\mytools2015\\ParserGen\\dummy\\Dummy.y"
    { printf("reduce by S -> S a\n"); }
#line 26 "C:\\mytools2015\\ParserGen\\lib\\parsergencpp.par"
    break;
  case 3: /* S -> b */
#line 38 "C:\\mytools2015\\ParserGen\\dummy\\Dummy.y"
    { printf("reduce by S -> b\n");   }
#line 26 "C:\\mytools2015\\ParserGen\\lib\\parsergencpp.par"
    break;
  case 4: /* S -> a */
#line 39 "C:\\mytools2015\\ParserGen\\dummy\\Dummy.y"
    { printf("reduce by S -> a\n");   }
#line 26 "C:\\mytools2015\\ParserGen\\lib\\parsergencpp.par"
    break;
  }
  return 0;
}

#line 44 "C:\\mytools2015\\ParserGen\\dummy\\Dummy.y"

#line 31 "C:\\mytools2015\\ParserGen\\lib\\parsergencpp.par"
