#include "lexical_analysis.h"
#include "syntax_analysis.h"
#include "generateMcode.h"
#include "generateObjcode.h"

int main() {
	syntax_analyse("testfile.txt", false);
	printMidCode("midCode.txt");
	generateObjcode("mips.txt");
}