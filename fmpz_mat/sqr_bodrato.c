/*=============================================================================

  This file is part of FLINT.

  FLINT is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  FLINT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FLINT; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

  =============================================================================*/
/******************************************************************************

  Copyright (C) 2012 Fredrik Johansson

 ******************************************************************************/

#include "fmpz_mat.h"

#define E fmpz_mat_entry

void
fmpz_mat_sqr_bodrato(fmpz_mat_t B, const fmpz_mat_t A)
{
    slong n = A->r;
    slong i,j;
    if (n == 0)
    {
        return;
    }
    else if (n == 1)
    {
        fmpz_mul(E(B, 0, 0), E(A, 0, 0), E(A, 0, 0));
    }
    else if (n == 2)
    {
        fmpz_t t, u;


        fmpz_init(t);
        fmpz_init(u);

        fmpz_add(t, E(A, 0, 0), E(A, 1, 1));
        fmpz_mul(u, E(A, 0, 1), E(A, 1, 0));

        fmpz_mul(E(B, 0, 0), E(A, 0, 0), E(A, 0, 0));
        fmpz_add(E(B, 0, 0), E(B, 0, 0), u);

        fmpz_mul(E(B, 1, 1), E(A, 1, 1), E(A, 1, 1));
        fmpz_add(E(B, 1, 1), E(B, 1, 1), u);

        fmpz_mul(E(B, 0, 1), E(A, 0, 1), t);
        fmpz_mul(E(B, 1, 0), E(A, 1, 0), t);

        fmpz_clear(t);
        fmpz_clear(u);
    }
    else
    {
        fmpz_mat_t window_A, window11, window12, window21, window22;
        fmpz_mat_t s1, s2, s3, s4;
        fmpz_mat_t p1, p2, p3, p4, p5, p6, p7;
        fmpz_t sum, val;

        slong m = n, x, iseven = 1; 
        
        if (n % 2 == 1)
        {
            m = n - 1;
            iseven = 0;
        }

        fmpz_mat_init(s1, m/2, m/2);
        fmpz_mat_init(s2, m/2, m/2);
        fmpz_mat_init(s3, m/2, m/2);
        fmpz_mat_init(s4, m/2, m/2);
        fmpz_mat_init(p1, m/2, m/2);
        fmpz_mat_init(p2, m/2, m/2);
        fmpz_mat_init(p3, m/2, m/2);
        fmpz_mat_init(p4, m/2, m/2);
        fmpz_mat_init(p5, m/2, m/2);
        fmpz_mat_init(p6, m/2, m/2);
        fmpz_mat_init(p7, m/2, m/2);

        fmpz_mat_window_init(window_A, A, 0, 0, m, m);
        fmpz_mat_window_init(window11, window_A, 0, 0, m/2, m/2);
        fmpz_mat_window_init(window12, window_A, 0, m/2, m/2, m);
        fmpz_mat_window_init(window21, window_A, m/2, 0, m, m/2);
        fmpz_mat_window_init(window22, window_A, m/2, m/2, m, m);

        fmpz_mat_add(s1, window22, window12);
        fmpz_mat_sub(s2, window22, window21);
        fmpz_mat_add(s3, s2, window12);
        fmpz_mat_sub(s4, s3, window11);

        fmpz_mat_sqr(p1, s1);
        fmpz_mat_sqr(p2, s2);
        fmpz_mat_sqr(p3, s3);    
        fmpz_mat_sqr(p4, window11);
        fmpz_mat_mul(p5, window12, window21);
        fmpz_mat_mul(p6, s4, window12);
        fmpz_mat_mul(p7, window21, s4);

        fmpz_mat_zero(s1);
        fmpz_mat_zero(s2);
        fmpz_mat_zero(s3);

        fmpz_mat_add(s1, p3, p5);
        fmpz_mat_sub(s2, p1, s1);
        fmpz_mat_sub(s3, s1, p2);

        fmpz_mat_zero(s1);
        fmpz_mat_zero(p1);
        fmpz_mat_zero(p3);
        fmpz_mat_zero(s4);

        fmpz_mat_add(p1, p4, p5);
        fmpz_mat_sub(s1, s3, p6);
        fmpz_mat_sub(p3, s2, p7);
        fmpz_mat_add(s4, p2, s2);

        if (iseven == 1)
        {
            for (i = 0; i < n/2; ++i)
            {
                for (j = 0; j < n/2; ++j)
                {
                    fmpz_set(fmpz_mat_entry(B, i, j), fmpz_mat_entry(p1, i, j));
                }
            }

            for (i = n/2; i < n; ++i)
            {
                for (j = 0; j < n/2; ++j)
                {
                    fmpz_set(fmpz_mat_entry(B, i, j), fmpz_mat_entry(p3, i - n/2, j));
                }
            }

            for (i = 0; i < n/2; ++i)
            {
                for (j = n/2; j < n; ++j)
                {
                    fmpz_set(fmpz_mat_entry(B, i, j), fmpz_mat_entry(s1, i, j - n/2));
                }
            }

            for (i = n/2; i < n; ++i)
            {
                for (j = n/2; j < n; ++j)
                {
                    fmpz_set(fmpz_mat_entry(B, i, j), fmpz_mat_entry(s4, i - n/2, j - n/2));
                }
            }
        }
        else
        {
            fmpz_mat_t temp_A, cache_A;
            
            fmpz_mat_init(temp_A, n, n);
            fmpz_mat_init(cache_A, n, n);

            fmpz_init(sum);
            fmpz_init(val);
            
            fmpz_mat_set(temp_A, A);


            for (i = 0; i < n; ++i)
            {
                for (j = 0; j < n; ++j)
                {
                    fmpz_mul(fmpz_mat_entry(cache_A, i, j), fmpz_mat_entry(A, i, n - 1), fmpz_mat_entry(A, n - 1, j)); 
                }
            }

            for (i = 0; i < n; ++i)
            {
                fmpz_zero(sum);
                for (x = 0; x < n; ++x)
                {
                    fmpz_mul(val, fmpz_mat_entry(temp_A, n - 1, x), fmpz_mat_entry(temp_A, x, i));
                    fmpz_add(sum, sum, val);
                }
                fmpz_set(fmpz_mat_entry(B, n - 1, i), sum);
            }

            for (i = 0; i < n; ++i)
            {
                fmpz_zero(sum);
                for (x = 0; x < n; ++x)
                {
                    fmpz_mul(val, fmpz_mat_entry(temp_A, x, n - 1), fmpz_mat_entry(temp_A, i, x));
                    fmpz_add(sum, sum, val);
                }
                fmpz_set(fmpz_mat_entry(B, i, n - 1), sum);
            }

            for (i = 0; i < m/2; ++i)
            {
                for (j = 0; j < m/2; ++j)
                {
                    fmpz_add(fmpz_mat_entry(B, i, j), fmpz_mat_entry(p1, i, j), fmpz_mat_entry(cache_A, i, j));
                }
            }
            for (i = m/2; i < m; ++i)
            {
                for (j = 0; j < m/2; ++j)
                {
                    fmpz_add(fmpz_mat_entry(B, i, j), fmpz_mat_entry(p3, i - m/2, j), fmpz_mat_entry(cache_A, i, j));
                }
            }
            for (i = 0; i < m/2; ++i)
            {
                for (j = m/2; j < m; ++j)
                {
                    fmpz_add(fmpz_mat_entry(B, i, j), fmpz_mat_entry(s1, i, j - m/2), fmpz_mat_entry(cache_A, i, j));
                }
            }
            for (i = m/2; i < m; ++i)
            {
                for (j = m/2; j < m; ++j)
                {
                    fmpz_add(fmpz_mat_entry(B, i, j), fmpz_mat_entry(s4, i - m/2, j - m/2), fmpz_mat_entry(cache_A, i, j));
                }
            }

            fmpz_clear(sum);
            fmpz_clear(val);
            
            fmpz_mat_clear(temp_A);
            fmpz_mat_clear(cache_A);

        }

        fmpz_mat_window_clear(window_A);
        fmpz_mat_window_clear(window11);
        fmpz_mat_window_clear(window12);
        fmpz_mat_window_clear(window21);
        fmpz_mat_window_clear(window22);
        fmpz_mat_clear(s1);
        fmpz_mat_clear(s2);
        fmpz_mat_clear(s3);
        fmpz_mat_clear(s4);
        fmpz_mat_clear(p1);
        fmpz_mat_clear(p2);
        fmpz_mat_clear(p3);
        fmpz_mat_clear(p4);
        fmpz_mat_clear(p5);
        fmpz_mat_clear(p6);
        fmpz_mat_clear(p7);
    }
}
