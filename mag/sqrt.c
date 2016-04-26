/*
    Copyright (C) 2014 Fredrik Johansson

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "mag.h"

void
mag_sqrt(mag_t y, const mag_t x)
{
    if (mag_is_special(x))
    {
        mag_set(y, x);
    }
    else
    {
        double t;
        fmpz e;

        t = MAG_MAN(x) * ldexp(1.0, -MAG_BITS);
        e = MAG_EXP(x);

        if (!COEFF_IS_MPZ(e))
        {
            if (e % 2 != 0)
            {
                e = (e - 1) >> 1;
                t *= 2.0;
            }
            else
            {
                e >>= 1;
            }
            t = sqrt(t) * (1 + 1e-13);
            mag_set_d_2exp_fmpz(y, t, &e);
        }
        else
        {
            if (fmpz_is_odd(MAG_EXPREF(x)))
                t *= 2.0;
            fmpz_fdiv_q_2exp(MAG_EXPREF(y), MAG_EXPREF(x), 1);
            t = sqrt(t) * (1 + 1e-13);
            mag_set_d_2exp_fmpz(y, t, MAG_EXPREF(y));
        }
    }
}
