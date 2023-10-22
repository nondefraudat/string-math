#include "tests.hpp"

using namespace std;

int main() {
	Tests tests;
	tests.add("1", "1", 1.);
	tests.add("1+  2", "1 + 2", 3.);
	tests.add("1+2*3", "1 + 2 * 3", 7.);
	tests.add("(1+2)*3--4", "( 1 + 2 ) * 3 - -4", 13.);
	tests.add("2^-1", "2 ^ -1", .5);
	return tests.getStatus();
}
