/*
    Copyright (C) 2012 Fredrik Johansson

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "arb.h"

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("lgamma....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 3000 * arb_test_multiplier(); iter++)
    {
        arb_t a, b, c;
        slong prec1, prec2;

        prec1 = 2 + n_randint(state, 1000);
        prec2 = prec1 + 30;

        arb_init(a);
        arb_init(b);
        arb_init(c);

        arb_randtest_precise(a, state, 1 + n_randint(state, 1000), 3);

        arb_lgamma(b, a, prec1);
        arb_lgamma(c, a, prec2);

        if (!arb_overlaps(b, c))
        {
            flint_printf("FAIL: overlap\n\n");
            flint_printf("a = "); arb_print(a); flint_printf("\n\n");
            flint_printf("b = "); arb_print(b); flint_printf("\n\n");
            flint_printf("c = "); arb_print(c); flint_printf("\n\n");
            abort();
        }

        arb_lgamma(a, a, prec2);

        if (!arb_equal(a, c))
        {
            flint_printf("FAIL: aliasing\n\n");
            abort();
        }

        arb_clear(a);
        arb_clear(b);
        arb_clear(c);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
