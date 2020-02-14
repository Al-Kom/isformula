// Laboratory work 1 on the discipline LOIS
// Completed by student of group 721702 BSUIR Komar Alexander Nazarovich
// H-file of library of propositional logic language formula verifier
// Version 3. Using PCRE. Moving from main-file to separated library.
//
// www.pcre.org
// https://www.opennet.ru/base/dev/pcre_cpp.txt.html
// https://eax.me/libpcre/
//

#ifndef PLLFV_H
#define PLLFV_H

// String of pattern of propositional language language formula,
// that defined by a grammar:
//  <constant>		::=1|0
//  <atom>		    ::=A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z
//  <negation>		::=!
//  <conjunction>		::=&
//  <disjunction>		::=|
//  <implication>		::=->
//	<equivalent>		::=~
//	<opening bracket>	::=(
//	<closing bracket>	::=)
//	<binary operation>	::=<conjunction>|<disjunction>|<implication>|<equivalent>
//	<unary complex formula>::=<opening bracket><negation><formula><closing bracket>
//	<binary complex formula>::=<opening bracket><formula><binary operation><formula><closing bracket>
//	<formula>		::=<constant>|<atom>|<unary complex formula>|<binary complex formula>
const static std::string DEFAULT_FORMULA_PATTERN = "(?(DEFINE)"
                                    "(?'atom'[A-Z])"
                                    "(?'const'0|1)"
                                    "(?'binop'[&|~]|->)"
                                    "(?'unf'\\(!(?P>formula)\\))"
                                    "(?'binf'\\((?P>formula)(?P>binop)(?P>formula)\\))"
                                    "(?'formula'(?P>atom)|(?P>const)|(?P>unf)|(?P>binf)))"
                                    "^(?P>formula)$";

// compileRegexp()
// Compile string of pattern, written by PCRE.
// Return a pointer to a private data structure that contains the compiled pattern,
// or NULL if an error was detected (by www.pcre.org)
pcre *compileRegexp(const char *pattern);

// matchRegexp()
// Verify that string completely match regular expression.
// Return true if it's match, and false if not (or match not incompletely).
bool matchRegexp(const pcre *regular, const char *str, int len);

#endif //PLLFV_H
