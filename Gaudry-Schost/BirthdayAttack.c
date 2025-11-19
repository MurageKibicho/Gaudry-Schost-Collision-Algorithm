#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <assert.h>
#include <mpfr.h>
//Run: clear && gcc BirthdayAttack.c -lgmp -lm -lmpfr -o g.o && ./g.o
//Complete walkthrough: https://leetarxiv.substack.com/p/birthday-attack

double MPZ_Log(mpz_t x, double logarithmBase)
{
	assert(logarithmBase != 0);
	if(mpz_cmp_ui(x, 0) == 0){return 0;}
	signed long int ex;
	const double di = mpz_get_d_2exp(&ex, x);
	return ((log(di) + log(logarithmBase) * (double) ex) / log(logarithmBase));
}

double MPZ_LogFactorial(mpz_t n, double logarithmBase)
{
	if(mpz_cmp_ui(n, 0) == 0 || mpz_cmp_ui(n, 1) == 0){return 0.0;}
	mpfr_prec_t prec = 256;
	mpfr_rnd_t rnd = MPFR_RNDN;
	mpfr_t nf, np1, lngamma_n, logbase_mpf;
	mpfr_inits2(prec, nf, np1, lngamma_n, logbase_mpf, (mpfr_ptr) 0);
	mpfr_set_z(nf, n, rnd);
	mpfr_add_ui(np1, nf, 1, rnd);

	mpfr_lngamma(lngamma_n, np1, rnd);
	mpfr_set_d(logbase_mpf, logarithmBase, rnd);
	mpfr_log(logbase_mpf, logbase_mpf, rnd);  // log(logBase)
	mpfr_div(lngamma_n, lngamma_n, logbase_mpf, rnd);
	double result = mpfr_get_d(lngamma_n, rnd);
	mpfr_clears(nf, np1, lngamma_n, logbase_mpf, (mpfr_ptr) 0);

	return result;
}

double MPZ_LogProductRange(mpz_t start, mpz_t stop, double logarithmBase) 
{
	if(mpz_cmp(start, stop) > 0){return 0;}
	double log_b_factorial = MPZ_LogFactorial(stop, logarithmBase);
	mpz_sub_ui(start, start, 1);

	double log_a_minus_one_factorial = 0;
	if(mpz_cmp_ui(start, 0) >= 0)
	{
		log_a_minus_one_factorial = MPZ_LogFactorial(start, logarithmBase);
	}
	mpz_add_ui(start, start, 1);
	return log_b_factorial - log_a_minus_one_factorial;
}

// Birthday problem probability calculation
double BirthdayProblem(mpz_t days, mpz_t people, double logarithmBase) 
{
	mpz_t rangeStart, rangeEnd;
	mpz_init(rangeStart);
	mpz_sub(rangeStart, days, people);
	mpz_add_ui(rangeStart, rangeStart, 1); // start = days - people + 1
	double collisionProbability = 1.0; 
	double log_product = MPZ_LogProductRange(rangeStart, days, logarithmBase);
	double log_denominator = mpz_get_d(people) * MPZ_Log(days, logarithmBase);

	double log_probabilityNoCollision = log_product - log_denominator;
	double probabilityNoCollision = pow(logarithmBase, log_probabilityNoCollision);
	collisionProbability = collisionProbability - probabilityNoCollision;
	mpz_clear(rangeStart);
	return collisionProbability;
}

int main()
{
	mpz_t days, people;
	mpz_inits(days, people, NULL);
	char *testString0 = "8923272916";
	char *testString1 = "104463";
	mpz_set_str(days, testString0, 10);
	mpz_set_str(people, testString1, 10);
	double logarithmBase = 2;
	double result = BirthdayProblem(days, people, logarithmBase);
	gmp_printf("%Zd with %Zd people: %.6f\n", days, people, result);
	mpz_clears(days, people, NULL);
	return 0;
}
