double f(double x) {
	static int counter = 0;
	++counter;
	return counter * x;
}
