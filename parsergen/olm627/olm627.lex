%{
#pragma once

 /* This part goes to the h file */

class Olm627Lex : public Scanner {
public:
  int getNextLexeme();
};
%}

%{

 /* This part goes to the first part of the cpp file */
#include "stdafx.h"
#include "Olm627Symbol.h"
#include "Olm627Lex.h"

%}

let     [_a-zA-Z]              /* Letter                                */
alnum   [_a-zA-Z0-9]           /* Alphanumeric character                */
white   [\x00-\x09\x0b\s]      /* White space: all control chars but \n */

%%

"a"			return a;
"b"			return b;
"c"			return c;
"d"			return d;
"f"			return f;
\n			;
\r			;
{white}+    ;
%%

  /* This part goes to the last part of the cpp file */
