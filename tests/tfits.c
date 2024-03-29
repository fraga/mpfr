/* Test file for:
 mpfr_fits_sint_p, mpfr_fits_slong_p, mpfr_fits_sshort_p,
 mpfr_fits_uint_p, mpfr_fits_ulong_p, mpfr_fits_ushort_p

Copyright 2004-2015 Free Software Foundation, Inc.
Contributed by the AriC and Caramel projects, INRIA.

This file is part of the GNU MPFR Library.

The GNU MPFR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MPFR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MPFR Library; see the file COPYING.LESSER.  If not, see
http://www.gnu.org/licenses/ or write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifdef HAVE_CONFIG_H
# include "config.h"       /* for a build within gmp */
#endif

#include "mpfr-intmax.h"
#include "mpfr-test.h"

#define FTEST(N,NOT,FCT)                                        \
  do                                                            \
    {                                                           \
      __gmpfr_flags = ex_flags;                                 \
      if (NOT FCT (x, (mpfr_rnd_t) r))                          \
        {                                                       \
          printf ("Error %d for %s, rnd = %s and x = ",         \
                  N, #FCT,                                      \
                  mpfr_print_rnd_mode ((mpfr_rnd_t) r));        \
          mpfr_dump (x);                                        \
          exit (1);                                             \
        }                                                       \
      if (__gmpfr_flags != ex_flags)                            \
        {                                                       \
          mpfr_flags_t flags = __gmpfr_flags;                   \
          printf ("Flags error %d for %s, rnd = %s and x = ",   \
                  N, #FCT,                                      \
                  mpfr_print_rnd_mode ((mpfr_rnd_t) r));        \
          mpfr_dump(x);                                         \
          printf ("Expected flags:");                           \
          flags_out (ex_flags);                                 \
          printf ("Got flags:     ");                           \
          flags_out (flags);                                    \
          exit (1);                                             \
        }                                                       \
    }                                                           \
  while (0)

#define CHECK_ALL(N,NOT)                                        \
  do                                                            \
    {                                                           \
      FTEST (N, NOT, mpfr_fits_ulong_p);                        \
      FTEST (N, NOT, mpfr_fits_slong_p);                        \
      FTEST (N, NOT, mpfr_fits_uint_p);                         \
      FTEST (N, NOT, mpfr_fits_sint_p);                         \
      FTEST (N, NOT, mpfr_fits_ushort_p);                       \
      FTEST (N, NOT, mpfr_fits_sshort_p);                       \
    }                                                           \
  while (0)

#define CHECK_MAX(N,NOT)                                        \
  do                                                            \
    {                                                           \
      FTEST (N, NOT, mpfr_fits_uintmax_p);                      \
      FTEST (N, NOT, mpfr_fits_intmax_p);                       \
    }                                                           \
  while (0)

int
main (void)
{
  mpfr_t x, y;
  mpfr_flags_t flags[2] = { 0, MPFR_FLAGS_ALL }, ex_flags;
  int i, r, fi;

  tests_start_mpfr ();

  mpfr_init2 (x, 256);
  mpfr_init2 (y, 8);

  RND_LOOP (r)
    for (fi = 0; fi < numberof (flags); fi++)
      {
        ex_flags = flags[fi];

        /* Check NaN */
        mpfr_set_nan (x);
        CHECK_ALL (1, !!);

        /* Check +Inf */
        mpfr_set_inf (x, 1);
        CHECK_ALL (2, !!);

        /* Check -Inf */
        mpfr_set_inf (x, -1);
        CHECK_ALL (3, !!);

        /* Check +0 */
        mpfr_set_zero (x, 1);
        CHECK_ALL (4, !);

        /* Check -0 */
        mpfr_set_zero (x, -1);
        CHECK_ALL (5, !);

        /* Check small positive op */
        mpfr_set_str1 (x, "1@-1");
        CHECK_ALL (6, !);

        /* Check 17 */
        mpfr_set_ui (x, 17, MPFR_RNDN);
        CHECK_ALL (7, !);

        /* Check large values (no fit) */
        mpfr_set_ui (x, ULONG_MAX, MPFR_RNDN);
        mpfr_mul_2exp (x, x, 1, MPFR_RNDN);
        CHECK_ALL (8, !!);
        mpfr_mul_2exp (x, x, 40, MPFR_RNDN);
        CHECK_ALL (9, !!);

        /* Check the limits of the types (except 0 for unsigned types) */
        mpfr_set_ui (x, ULONG_MAX, MPFR_RNDN);
        FTEST (10, !, mpfr_fits_ulong_p);
        mpfr_add_ui (x, x, 1, MPFR_RNDN);
        FTEST (11, !!, mpfr_fits_ulong_p);
        mpfr_set_si (x, LONG_MAX, MPFR_RNDN);
        FTEST (12, !, mpfr_fits_slong_p);
        mpfr_add_ui (x, x, 1, MPFR_RNDN);
        FTEST (13, !!, mpfr_fits_slong_p);
        mpfr_set_si (x, LONG_MIN, MPFR_RNDN);
        FTEST (14, !, mpfr_fits_slong_p);
        mpfr_sub_ui (x, x, 1, MPFR_RNDN);
        FTEST (15, !!, mpfr_fits_slong_p);
        mpfr_set_ui (x, UINT_MAX, MPFR_RNDN);
        FTEST (16, !, mpfr_fits_uint_p);
        mpfr_add_ui (x, x, 1, MPFR_RNDN);
        FTEST (17, !!, mpfr_fits_uint_p);
        mpfr_set_si (x, INT_MAX, MPFR_RNDN);
        FTEST (18, !, mpfr_fits_sint_p);
        mpfr_add_ui (x, x, 1, MPFR_RNDN);
        FTEST (19, !!, mpfr_fits_sint_p);
        mpfr_set_si (x, INT_MIN, MPFR_RNDN);
        FTEST (20, !, mpfr_fits_sint_p);
        mpfr_sub_ui (x, x, 1, MPFR_RNDN);
        FTEST (21, !!, mpfr_fits_sint_p);
        mpfr_set_ui (x, USHRT_MAX, MPFR_RNDN);
        FTEST (22, !, mpfr_fits_ushort_p);
        mpfr_add_ui (x, x, 1, MPFR_RNDN);
        FTEST (23, !!, mpfr_fits_ushort_p);
        mpfr_set_si (x, SHRT_MAX, MPFR_RNDN);
        FTEST (24, !, mpfr_fits_sshort_p);
        mpfr_add_ui (x, x, 1, MPFR_RNDN);
        FTEST (25, !!, mpfr_fits_sshort_p);
        mpfr_set_si (x, SHRT_MIN, MPFR_RNDN);
        FTEST (26, !, mpfr_fits_sshort_p);
        mpfr_sub_ui (x, x, 1, MPFR_RNDN);
        FTEST (27, !!, mpfr_fits_sshort_p);

        /* Check negative op */
        for (i = 1; i <= 4; i++)
          {
            int inv;

            mpfr_set_si_2exp (x, -i, -2, MPFR_RNDN);
            mpfr_rint (y, x, (mpfr_rnd_t) r);
            inv = MPFR_NOTZERO (y);
            FTEST (40, inv ^ !, mpfr_fits_ulong_p);
            FTEST (40,       !, mpfr_fits_slong_p);
            FTEST (40, inv ^ !, mpfr_fits_uint_p);
            FTEST (40,       !, mpfr_fits_sint_p);
            FTEST (40, inv ^ !, mpfr_fits_ushort_p);
            FTEST (40,       !, mpfr_fits_sshort_p);
          }
      }

#ifdef _MPFR_H_HAVE_INTMAX_T

  mpfr_set_prec (x, sizeof (uintmax_t) * CHAR_BIT);

  RND_LOOP (r)
    {
      /* Check NaN */
      mpfr_set_nan (x);
      CHECK_MAX (1, !!);

      /* Check +Inf */
      mpfr_set_inf (x, 1);
      CHECK_MAX (2, !!);

      /* Check -Inf */
      mpfr_set_inf (x, -1);
      CHECK_MAX (3, !!);

      /* Check +0 */
      mpfr_set_zero (x, 1);
      CHECK_MAX (4, !);

      /* Check -0 */
      mpfr_set_zero (x, -1);
      CHECK_MAX (5, !);

      /* Check small positive op */
      mpfr_set_str1 (x, "1@-1");
      CHECK_MAX (6, !);

      /* Check 17 */
      mpfr_set_ui (x, 17, MPFR_RNDN);
      CHECK_MAX (7, !);

      /* Check hugest */
      mpfr_set_ui_2exp (x, 42, sizeof (uintmax_t) * 32, MPFR_RNDN);
      CHECK_MAX (8, !!);

      /* Check the limits of the types (except 0 for uintmax_t) */
      mpfr_set_uj (x, MPFR_UINTMAX_MAX, MPFR_RNDN);
      FTEST (10, !, mpfr_fits_uintmax_p);
      mpfr_add_ui (x, x, 1, MPFR_RNDN);
      FTEST (11, !!, mpfr_fits_uintmax_p);
      mpfr_set_sj (x, MPFR_INTMAX_MAX, MPFR_RNDN);
      FTEST (12, !, mpfr_fits_intmax_p);
      mpfr_add_ui (x, x, 1, MPFR_RNDN);
      FTEST (13, !!, mpfr_fits_intmax_p);
      mpfr_set_sj (x, MPFR_INTMAX_MIN, MPFR_RNDN);
      FTEST (14, !, mpfr_fits_intmax_p);
      mpfr_sub_ui (x, x, 1, MPFR_RNDN);
      FTEST (15, !!, mpfr_fits_intmax_p);

      /* Check negative op */
      for (i = 1; i <= 4; i++)
        {
          int inv;

          mpfr_set_si_2exp (x, -i, -2, MPFR_RNDN);
          mpfr_rint (y, x, (mpfr_rnd_t) r);
          inv = MPFR_NOTZERO (y);
          FTEST (40, inv ^ !, mpfr_fits_uintmax_p);
          FTEST (40,       !, mpfr_fits_intmax_p);
        }
    }

#endif  /* _MPFR_H_HAVE_INTMAX_T */

  mpfr_clear (x);
  mpfr_clear (y);

  tests_end_mpfr ();
  return 0;
}
