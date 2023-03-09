/*
    Copyright (C) 2009, 2013 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    FLINT_TEST_INIT(state);

    flint_printf("divrem_preinv....");
    fflush(stdout);

    

    /* Check q*b + r = a, no aliasing */
    for (i = 0; i < 200 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, b, b_inv, q, r, prod;

        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(b_inv);
        fmpz_poly_init(q);
        fmpz_poly_init(r);
        fmpz_poly_init(prod);
        fmpz_poly_randtest(a, state, n_randint(state, 100), 100);
        fmpz_poly_randtest_not_zero(b, state, n_randint(state, 100) + 1, 100);
        fmpz_set_ui(b->coeffs + b->length - 1, 1); /* b must be monic */

        fmpz_poly_preinvert(b_inv, b);
        fmpz_poly_divrem_preinv(q, r, a, b, b_inv);
        fmpz_poly_mul(prod, q, b);
        fmpz_poly_add(prod, prod, r);

        result = (fmpz_poly_equal(a, prod));
        if (!result)
        {
            flint_printf("FAIL:\n");
            fmpz_poly_print(a), flint_printf("\n\n");
            fmpz_poly_print(prod), flint_printf("\n\n");
            fmpz_poly_print(q), flint_printf("\n\n");
            fmpz_poly_print(r), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(b_inv);
        fmpz_poly_clear(q);
        fmpz_poly_clear(r);
        fmpz_poly_clear(prod);
    }
    
    /* Check r and a alias */
    for (i = 0; i < 100 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, b, b_inv, q, r;

        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(b_inv);
        fmpz_poly_init(q);
        fmpz_poly_init(r);
        fmpz_poly_randtest(a, state, n_randint(state, 100), 100);
        fmpz_poly_randtest_not_zero(b, state, n_randint(state, 100) + 1, 100);
        fmpz_set_ui(b->coeffs + b->length - 1, 1); /* b must be monic */

        fmpz_poly_divrem_basecase(q, r, a, b);
        fmpz_poly_preinvert(b_inv, b);
        fmpz_poly_divrem_preinv(q, a, a, b, b_inv);

        result = (fmpz_poly_equal(a, r));
        if (!result)
        {
            flint_printf("FAIL:\n");
            fmpz_poly_print(a), flint_printf("\n\n");
            fmpz_poly_print(q), flint_printf("\n\n");
            fmpz_poly_print(r), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(b_inv);
        fmpz_poly_clear(q);
        fmpz_poly_clear(r);
    }

    /* Check r and b alias */
    for (i = 0; i < 100 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, b, b_inv, q, r;

        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(b_inv);
        fmpz_poly_init(q);
        fmpz_poly_init(r);
        fmpz_poly_randtest(a, state, n_randint(state, 100), 100);
        fmpz_poly_randtest_not_zero(b, state, n_randint(state, 100) + 1, 100);
        fmpz_set_ui(b->coeffs + b->length - 1, 1); /* b must be monic */

        fmpz_poly_divrem_basecase(q, r, a, b);
        fmpz_poly_preinvert(b_inv, b);
        fmpz_poly_divrem_preinv(q, b, a, b, b_inv);

        result = (fmpz_poly_equal(b, r));
        if (!result)
        {
            flint_printf("FAIL:\n");
            fmpz_poly_print(a), flint_printf("\n\n");
            fmpz_poly_print(q), flint_printf("\n\n");
            fmpz_poly_print(r), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(b_inv);
        fmpz_poly_clear(q);
        fmpz_poly_clear(r);
    }

    /* Check q and a alias */
    for (i = 0; i < 100 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, b, b_inv, q, r;

        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(b_inv);
        fmpz_poly_init(q);
        fmpz_poly_init(r);
        fmpz_poly_randtest(a, state, n_randint(state, 100), 100);
        fmpz_poly_randtest_not_zero(b, state, n_randint(state, 100) + 1, 100);
        fmpz_set_ui(b->coeffs + b->length - 1, 1); /* b must be monic */

        fmpz_poly_divrem_basecase(q, r, a, b);
        fmpz_poly_preinvert(b_inv, b);
        fmpz_poly_divrem_preinv(a, r, a, b, b_inv);

        result = (fmpz_poly_equal(a, q));
        if (!result)
        {
            flint_printf("FAIL:\n");
            fmpz_poly_print(a), flint_printf("\n\n");
            fmpz_poly_print(q), flint_printf("\n\n");
            fmpz_poly_print(r), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(b_inv);
        fmpz_poly_clear(q);
        fmpz_poly_clear(r);
    }

    /* Check q and b alias */
    for (i = 0; i < 100 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, b, b_inv, q, r;

        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(b_inv);
        fmpz_poly_init(q);
        fmpz_poly_init(r);
        fmpz_poly_randtest(a, state, n_randint(state, 100), 100);
        fmpz_poly_randtest_not_zero(b, state, n_randint(state, 100) + 1, 100);
        fmpz_set_ui(b->coeffs + b->length - 1, 1); /* b must be monic */

        fmpz_poly_divrem_basecase(q, r, a, b);
        fmpz_poly_preinvert(b_inv, b);
        fmpz_poly_divrem_preinv(b, r, a, b, b_inv);

        result = (fmpz_poly_equal(b, q));
        if (!result)
        {
            flint_printf("FAIL:\n");
            fmpz_poly_print(a), flint_printf("\n\n");
            fmpz_poly_print(q), flint_printf("\n\n");
            fmpz_poly_print(r), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(b_inv);
        fmpz_poly_clear(q);
        fmpz_poly_clear(r);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
