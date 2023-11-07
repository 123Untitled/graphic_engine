#include <metal_stdlib>
/*
 * Loki Random Number Generator
 * Copyright (c) 2017 Youssef Victor All rights reserved.
 *
 *      Function                        Result
 *      ------------------------------------------------------------------
 *
 *      TausStep                        Combined Tausworthe Generator or
 *                                      Linear Feedback Shift Register (LFSR)
 *                                      random number generator. This is a
 *                                      helper method for rng, which uses
 *                                      a hybrid approach combining LFSR with
 *                                      a Linear Congruential Generator (LCG)
 *                                      in order to produce random numbers with
 *                                      periods of well over 2^121
 *
 *      rand                            A pseudo-random number based on the
 *                                      method outlined in "Efficient
 *                                      pseudo-random number generation
 *                                      for monte-carlo simulations using
 *                                      graphic processors" by Siddhant
 *                                      Mohanty et al 2012.
 *
 */

#include <metal_stdlib>
using namespace metal;

#ifndef LOKI
#define LOKI


class Loki {
private:
	thread float seed;

	unsigned TausStep(const unsigned z, const int s1, const int s2, const int s3, const unsigned M) {

		unsigned b=(((z << s1) ^ z) >> s2);
		return (((z & M) << s3) ^ b);
	}

public:

	thread Loki(const unsigned seed1, const unsigned seed2 = 1, const unsigned seed3 = 1) {
		unsigned seed = seed1 * 1099087573UL;
		unsigned seedb = seed2 * 1099087573UL;
		unsigned seedc = seed3 * 1099087573UL;

		// Round 1: Randomise seed
		unsigned z1 = TausStep(seed,13,19,12,429496729UL);
		unsigned z2 = TausStep(seed,2,25,4,4294967288UL);
		unsigned z3 = TausStep(seed,3,11,17,429496280UL);
		unsigned z4 = (1664525*seed + 1013904223UL);

		// Round 2: Randomise seed again using second seed
		unsigned r1 = (z1^z2^z3^z4^seedb);

		z1 = TausStep(r1,13,19,12,429496729UL);
		z2 = TausStep(r1,2,25,4,4294967288UL);
		z3 = TausStep(r1,3,11,17,429496280UL);
		z4 = (1664525*r1 + 1013904223UL);

		// Round 3: Randomise seed again using third seed
		r1 = (z1^z2^z3^z4^seedc);

		z1 = TausStep(r1,13,19,12,429496729UL);
		z2 = TausStep(r1,2,25,4,4294967288UL);
		z3 = TausStep(r1,3,11,17,429496280UL);
		z4 = (1664525*r1 + 1013904223UL);

		this->seed = (z1^z2^z3^z4) * 2.3283064365387e-10;
	}

	thread float rand() {
		unsigned hashed_seed = this->seed * 1099087573UL;

		unsigned z1 = TausStep(hashed_seed,13,19,12,429496729UL);
		unsigned z2 = TausStep(hashed_seed,2,25,4,4294967288UL);
		unsigned z3 = TausStep(hashed_seed,3,11,17,429496280UL);
		unsigned z4 = (1664525*hashed_seed + 1013904223UL);

		thread float old_seed = this->seed;

		this->seed = (z1^z2^z3^z4) * 2.3283064365387e-10;

		return old_seed;
	}
};




#endif
