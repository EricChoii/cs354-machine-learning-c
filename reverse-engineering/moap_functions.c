
// for each of the following functions 
// correct the prototype
// write function body
// replace return with the correct return statement
//
// you may write as many helper functions here as you wish

/**
 * @return r1
 */
int algorithm_1() {
	int a = 8000;		// a: -0xc(%rbp)
	int b = 63;			// b: -0x8(%rbp)
	int r1 = a + b;		// r1: -0x4(%rbp)

   return r1; 
}

/**
  * @param p1	%edi, -0x14(%rbp), 2802
  * @param p2	%esi, -0x18(%rbp), 3
  * @return r2
  */
int algorithm_2(int p1, int p2) {
	int a = p1;		// a: -0x10(%rbp)
	int b = p2;		// b: -0xc(%rbp)
	int r2;

	if (a < b)
		r2 = 3 * a + b;	// r2: -0x4(%rbp)
	else
		r2 = a / b;		// r2: -0x8(%rbp)

	return r2;

}

/**
 * @param p3		%edi
 * @return res	-0x4(%rbp)
 */
int helper_3(int p3) {
	int res = p3 - 1;			// res: -0x4(%rbp)
	return res;
}


/**
 * @param p3	%edi, -0x14(%rbp), 13
 * @param p4	%esi, -0x18(%rbp), 17
 * @return r3
 */
int algorithm_3(int p3, int p4) {
	int a = p3;		// a: -0x4(%rbp)
	int b = p4;		// b: -0xc(%rbp)
	int r3 = 0;		// r3: -0x8(%rbp

	while(b > 0){
		r3 += a;
		b = helper_3(b);
	}

    return r3;
}

/**
 * @param p5	%edi, -0x14(%rbp), 48
 * @param r4	%esi, -0x20(%rbp), pointer
 * @return r4
 */
void algorithm_4(int p5, int *r4) {
	int a = p5;	// a: -0xc(%rbp)
	int *b = r4;	// b: -0x8(%rbp)
	*b = 3 * a;
}



