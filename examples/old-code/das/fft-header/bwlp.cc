/*
 *                            COPYRIGHT
 *
 *  bwlp - Butterworth lowpass filter coefficient calculator
 *  Copyright (C) 2003, 2004, 2005, 2007 Exstrom Laboratories LLC
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  A copy of the GNU General Public License is available on the internet at:
 *
 *  http://www.gnu.org/copyleft/gpl.html
 *
 *  or you can write to:
 *
 *  The Free Software Foundation, Inc.
 *  675 Mass Ave
 *  Cambridge, MA 02139, USA
 *
 *  You can contact Exstrom Laboratories LLC via Email at:
 *
 *  info(AT)exstrom.com
 *
 *  or you can write to:
 *
 *  Exstrom Laboratories LLC
 *  P.O. Box 7651
 *  Longmont, CO 80501, USA
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "iir.h"
#include <vector>

// n = order of the filter
// fc = filter cutoff frequency as a fraction of Pi [0,1]
int butter_low(int n, double fcf, std::vector<double> &A, std::vector<double> &B)
{
    int sff = 1;  // scale flag: 1 to scale, 0 to not scale ccof
    int i;        // loop variables
    double sf;    // scaling factor
    double *dcof; // d coefficients
    int *ccof;    // c coefficients

    /* calculate the d coefficients */
    dcof = dcof_bwlp(n, fcf);
    if (dcof == NULL)
    {
        perror("Unable to calculate d coefficients");
        return (-1);
    }

    /* calculate the c coefficients */
    ccof = ccof_bwlp(n);
    if (ccof == NULL)
    {
        perror("Unable to calculate c coefficients");
        return (-1);
    }

    sf = sf_bwlp(n, fcf); /* scaling factor for the c coefficients */

    A.resize(n + 1);
    B.resize(n + 1);
    /* Output the c (A) coefficients */
    if (sff == 0)
    {
        for (i = 0; i <= n; ++i)
        {
            A[i] = ccof[i];
        }
    }
    else
    {
        for (i = 0; i <= n; ++i)
        {
            A[i] = (double)ccof[i] * sf;
        }
    }

    /* Output the d(B) coefficients */
    for (i = 0; i <= n; ++i)
    {
        B[i] = dcof[i];
    }

    free(dcof);
    free(ccof);

    return 0;
}
