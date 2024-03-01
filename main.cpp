#define _DEBUG

#include "./interpreter/interpreter.hpp"

int main (int argc, char *argv[]) {
	(void) argc;
	(void)argv;
	Interpreter in;
	in.prompt();
}