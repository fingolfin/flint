/*
    Copyright (C) 2008, 2009 William Hart
    Copyright (C) 2011 Sebastian Pancratz
    Copyright (C) 2012 Fredrik Johansson

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "arb_poly.h"

void
_arb_poly_mullow_classical(arb_ptr res,
    arb_srcptr poly1, slong len1,
    arb_srcptr poly2, slong len2, slong n, slong prec)
{
    len1 = FLINT_MIN(len1, n);
    len2 = FLINT_MIN(len2, n);

    if (n == 1)
    {
        arb_mul(res, poly1, poly2, prec);
    }
    else if (poly1 == poly2 && len1 == len2)
    {
        slong i;

        _arb_vec_scalar_mul(res, poly1, FLINT_MIN(len1, n), poly1, prec);
        _arb_vec_scalar_mul(res + len1, poly1 + 1, n - len1, poly1 + len1 - 1, prec);

        for (i = 1; i < len1 - 1; i++)
            _arb_vec_scalar_addmul(res + i + 1, poly1 + 1,
                FLINT_MIN(i - 1, n - (i + 1)), poly1 + i, prec);

        for (i = 1; i < FLINT_MIN(2 * len1 - 2, n); i++)
            arb_mul_2exp_si(res + i, res + i, 1);

        for (i = 1; i < FLINT_MIN(len1 - 1, (n + 1) / 2); i++)
            arb_addmul(res + 2 * i, poly1 + i, poly1 + i, prec);
    }
    else
    {
        slong i;

        _arb_vec_scalar_mul(res, poly1, FLINT_MIN(len1, n), poly2, prec);

        if (n > len1)
            _arb_vec_scalar_mul(res + len1, poly2 + 1, n - len1,
                                      poly1 + len1 - 1, prec);

        for (i = 0; i < FLINT_MIN(len1, n) - 1; i++)
            _arb_vec_scalar_addmul(res + i + 1, poly2 + 1,
                                         FLINT_MIN(len2, n - i) - 1,
                                         poly1 + i, prec);
    }
}

void
arb_poly_mullow_classical(arb_poly_t res, const arb_poly_t poly1,
                                            const arb_poly_t poly2,
                                                slong n, slong prec)
{
    slong len_out;

    if (poly1->length == 0 || poly2->length == 0 || n == 0)
    {
        arb_poly_zero(res);
        return;
    }

    len_out = poly1->length + poly2->length - 1;
    if (n > len_out)
        n = len_out;

    if (res == poly1 || res == poly2)
    {
        arb_poly_t t;
        arb_poly_init2(t, n);
        _arb_poly_mullow_classical(t->coeffs, poly1->coeffs, poly1->length,
                                    poly2->coeffs, poly2->length, n, prec);
        arb_poly_swap(res, t);
        arb_poly_clear(t);
    }
    else
    {
        arb_poly_fit_length(res, n);
        _arb_poly_mullow_classical(res->coeffs, poly1->coeffs, poly1->length,
                                    poly2->coeffs, poly2->length, n, prec);
    }

    _arb_poly_set_length(res, n);
    _arb_poly_normalise(res);
}
