#include "pch.h"
#include <Regex.h>

TCHAR *regexDescription[] = {
  _T("Grammar for Regex."                                                                                                    )
 ,_T(""                                                                                                                      )
 ,_T("Pattern            ->  ExprList"                                                                                       )
 ,_T("                    | ^ExprList                      - Match only if at start of line"                                 )
 ,_T("                    |  ExprList$                     - Match only if at end of line"                                   )
 ,_T("                    | ^ExprList$                     - Match only if at both start end end of line"                    )
 ,_T(""                                                                                                                      )
 ,_T("ExprList           -> ExprList Factor"                                                                                 )
 ,_T("                    | Factor"                                                                                          )
 ,_T(""                                                                                                                      )
 ,_T("Factor             -> Term"                                                                                            )
 ,_T("                    | Term?                          - Match 0 or 1 occurrence of Term"                                )
 ,_T("                    | Term+                          - Match 1 or many occurrences of Term"                            )
 ,_T("                    | Term*                          - Match 0 or many occurrences of Term"                            )
 ,_T("                    | Term{m,n}                      - Match m to n (incl.) occurrences of Term. m <= n"               )
 ,_T("                    | Term{m,}                       - Match at least m occurrences of Term"                           )
 ,_T(""                                                                                                                      )
 ,_T("Term               -> NormalCharacter"                                                                                 )
 ,_T("                    | .                              - Match any input character except newline '\n'"                  )
 ,_T("                    | CharacterSet"                                                                                    )
 ,_T("                    | \\(AlternativeList\\)          - Match, if any of the alternatives in the list match"            )
 ,_T("                    | EscapeCommand"                                                                                   )
 ,_T(""                                                                                                                      )
 ,_T("NormalCharacter    -> Any character except *?+{\\.[  - To include these, prefix with \\"                               )
 ,_T("                    ;"                                                                                                 )
 ,_T(""                                                                                                                      )
 ,_T("CharacterSet       -> [SetElementList]               - Match, if CharacterSet contains input character"                )
 ,_T("                    | [^SetElementList]              - Match, if CharacterSet does not contain input character"        )
 ,_T(""                                                                                                                      )
  _T("SetElementList     -> SetElementList SetElement"                                                                       )
 ,_T("                    | SetElement"                                                                                      )
 ,_T(""                                                                                                                      )
 ,_T("SetElement         -> SetCharacter"                                                                                    )
 ,_T("                    | SetCharacter-SetCharacter      - Range of characters is included in CharacterSet"                )
 ,_T(""                                                                                                                      )
 ,_T("SetCharacter       -> Any character except -]        - To include these in the set, prefix with \\"                    )
 ,_T(""                                                                                                                      )
 ,_T("AlternativeList    -> AlternativeList\\|ExprList     - The \\| separates 2 alternatives. Special only inside \\(..\\)" )
 ,_T("                    | ExprList"                                                                                        )
 ,_T(""                                                                                                                      )
 ,_T("EscapeCommand      -> \\SpecialCharacter"                                                                              )
 ,_T(""                                                                                                                      )
 ,_T("SpecialChararacter -> `                              - Back quote.   Match only if at start of buffer"                 )
 ,_T("                    | '                              - Single quote. Match only if at end of buffer"                   )
 ,_T("                    | <                              - Match only if at begining of af word"                           )
 ,_T("                    | >                              - Match only if at end of af word"                                )
 ,_T("                    | b                              - Match only if at word bound"                                    )
 ,_T("                    | B                              - Match only if not at word bound"                                )
 ,_T("                    | w                              - Match any wordcharacter. {0-9,a-z,A-Z}"                         )
 ,_T("                    | W                              - Match any non-wordcharacter"                                    )
 ,_T("                    | 1..9                           - Match if string equals the corresponding register. See below."  )
 ,_T(""                                                                                                                      )
 ,_T(""                                                                                                                      )
 ,_T("Registers."                                                                                                            )
 ,_T("There are 9 registers available when matching a pattern. Every time the pattern compiler reads \\( a register is"      )
 ,_T("allocated, starting from 1. When searching, the first alternative in the list, that matches the inputstring, is saved" )
 ,_T("in the register. The content of one of the 9 registers can be used to match something later in the inputstring by"     )
 ,_T("using \\1..\\9."                                                                                                       )
 ,_T("Note, that the whole string matched by the pattern inside \\(..\\) will be saved in the allocated register."           )
 ,_T("This rule applies recursively if parenthesis are nested. If any of the looping constructs (*+{m,n}) is used"           )
 ,_T("together with \\(..\\), each repetition is concatenated to the registercontent. Ex. \\(ab\\)* will be found at"        )
 ,_T("position 2 in the string \"qqabababc\" and register 1 will contain \"ababab\" after the match."                        )
 ,NULL
};