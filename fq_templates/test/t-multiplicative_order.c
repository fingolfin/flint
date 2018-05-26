/*
    Copyright (C) 2018 Luca De Feo

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifdef T

#include "templates.h"

int
main(void)
{
    int i, result;
    FLINT_TEST_INIT(state);

    flint_printf("multiplicative_order... ");
    fflush(stdout);

    for (i = 0; i < flint_test_multiplier(); i++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, t) a, tmp;
        fmpz_t ord;

        TEMPLATE(T, ctx_randtest)(ctx, state);

        TEMPLATE(T, init)(a, ctx);
        TEMPLATE(T, init)(tmp, ctx);
        TEMPLATE(T, randtest)(a, state, ctx);

        fmpz_init(ord);
        result = TEMPLATE(T, multiplicative_order)(ord, a, ctx);
        TEMPLATE(T, pow)(tmp, a, ord, ctx);

        if (result && !TEMPLATE(T, is_one)(tmp, ctx))
        {
            flint_printf("FAIL:\n\n");
            flint_printf("a = "), TEMPLATE(T, print_pretty)(a, ctx), flint_printf("\n");
            flint_printf("ord = "), fmpz_print(ord), flint_printf("\n");
            TEMPLATE(T, ctx_print)(ctx);
            abort();
        }

        fmpz_clear(ord);
        TEMPLATE(T, clear)(a, ctx);
        TEMPLATE(T, clear)(tmp, ctx);

        TEMPLATE(T, ctx_clear)(ctx);
    }

    FLINT_TEST_CLEANUP(state);
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}



#endif
