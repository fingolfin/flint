/*
    Copyright (C) 2020 Fredrik Johansson

    This file is part of Calcium.

    Calcium is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "utils_flint.h"

static int
within_limits(const fmpz_mpoly_t poly, slong poly_len_limit, slong poly_bits_limit, const fmpz_mpoly_ctx_t ctx)
{
    slong bits;

    if (fmpz_mpoly_length(poly, ctx) > poly_len_limit)
        return 0;

    bits = fmpz_mpoly_max_bits(poly);
    bits = FLINT_ABS(bits);

    if (bits > poly_bits_limit)
        return 0;

    return 1;
}

int
fmpz_mpoly_buchberger_naive_with_limits(fmpz_mpoly_vec_t G, const fmpz_mpoly_vec_t F,
    slong ideal_len_limit, slong poly_len_limit, slong poly_bits_limit, const fmpz_mpoly_ctx_t ctx)
{
    pairs_t B;
    fmpz_mpoly_t h;
    slong i, j, index_h;
    pair_t pair;
    int success;

    fmpz_mpoly_vec_set_primitive_unique(G, F, ctx);

    if (G->length <= 1)
        return 1;

    if (G->length >= ideal_len_limit)
        return 0;

    for (i = 0; i < G->length; i++)
        if (!within_limits(fmpz_mpoly_vec_entry(G, i), poly_len_limit, poly_bits_limit, ctx))
            return 0;

    pairs_init(B);
    fmpz_mpoly_init(h, ctx);

    for (i = 0; i < G->length; i++)
        for (j = i + 1; j < G->length; j++)
            pairs_append(B, i, j);

    success = 1;
    while (B->length != 0)
    {
        pair = fmpz_mpoly_select_pop_pair(B, G, ctx);

        fmpz_mpoly_spoly(h, fmpz_mpoly_vec_entry(G, pair.a), fmpz_mpoly_vec_entry(G, pair.b), ctx);
        fmpz_mpoly_reduction_primitive_part(h, h, G, ctx);

        if (!fmpz_mpoly_is_zero(h, ctx))
        {
            /* printf("h stats %ld, %ld, %ld\n", h->length, h->bits, G->length); */

            if (G->length >= ideal_len_limit || !within_limits(h, poly_len_limit, poly_bits_limit, ctx))
            {
                success = 0;
                break;
            }

            index_h = G->length;
            fmpz_mpoly_vec_append(G, h, ctx);

            for (i = 0; i < index_h; i++)
                pairs_append(B, i, index_h);
        }
    }

    fmpz_mpoly_clear(h, ctx);
    pairs_clear(B);

    return success;
}

void
fmpz_mpoly_buchberger_naive(fmpz_mpoly_vec_t G, const fmpz_mpoly_vec_t F, const fmpz_mpoly_ctx_t ctx)
{
    pairs_t B;
    fmpz_mpoly_t h;
    slong i, j, index_h;
    pair_t pair;

    fmpz_mpoly_vec_set_primitive_unique(G, F, ctx);

    if (G->length <= 1)
        return;

    pairs_init(B);
    fmpz_mpoly_init(h, ctx);

    for (i = 0; i < G->length; i++)
        for (j = i + 1; j < G->length; j++)
            pairs_append(B, i, j);

    while (B->length != 0)
    {
        pair = fmpz_mpoly_select_pop_pair(B, G, ctx);
        fmpz_mpoly_spoly(h, fmpz_mpoly_vec_entry(G, pair.a), fmpz_mpoly_vec_entry(G, pair.b), ctx);
        fmpz_mpoly_reduction_primitive_part(h, h, G, ctx);

        if (!fmpz_mpoly_is_zero(h, ctx))
        {
            index_h = G->length;
            fmpz_mpoly_vec_append(G, h, ctx);

            for (i = 0; i < index_h; i++)
                pairs_append(B, i, index_h);
        }
    }

    fmpz_mpoly_clear(h, ctx);
    pairs_clear(B);
}
