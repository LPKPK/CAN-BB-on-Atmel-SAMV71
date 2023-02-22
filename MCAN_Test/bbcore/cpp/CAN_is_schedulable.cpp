/** @file CAN_is_schedulable.cpp
 *
 * @brief Tool to determine schedulability of CAN configuration
 *
 * This C version is a translation of CAN_is_schedulable.lua, which works on .csv files.
 * This C version works directly from the header file used by the application code.
 *
 *  Based on:
 *  Controller Area Network (CAN) schedulability analysis: Refuted, revisited and revised
 *  Robert I. Davis · Alan Burns · Reinder J. Bril · Johan J. Lukkien
 *  Published online: 30 January 2007
 *
 *
 * @par
 * @copyright Copyright © 2017-2018 Sunrise Labs, Inc. All rights reserved.
 *
 **/

/* Build and Use Instructions:
** for testing, build with: cc -DCIS_VERBOSE -DCAN_CMD_LINE_BUILD -o cis CAN_is_schedulable.c
** for release, build with: cc -o cis CAN_is_schedulable.c
** to run, e.g.: ./cis > test_500_90.fromc.csv
** to run, e.g.: ./cis -j 750 -d 80 > test_750_80.fromc.csv
*/
#include "CAN_is_schedulable.h"
#include "bb_types.h"

#ifdef CAN_CMD_LINE_BUILD //used by the run_CAN_is_schedulable script to output to csv
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#ifdef CAN_CMD_LINE_BUILD
static void fprintf_hdr (FILE *stream)
{
    fprintf(stream, "%25s, %5s, %s, %6s, %7s, %4s, ", "Ns", "Pm", "Sm", "Tm", "Dm", "Jm");
    fprintf(stream, "%3s, %3s, %5s, %5s, %5s, %2s, ", "Cm", "Bm", "Im", "Wm", "Ym", "Qm");
    fprintf(stream, "%5s, %5s, %5s, %5s", "Um", "Rm", "Nm", "Ng");
    fprintf(stream, "\n");
}

static void fprintf_msg (FILE *stream, msg_v *m)
{
    fprintf(stream, "%25s, 0x%03x, %d, %7d, %7d, %4d, ", m->Ns, m->Pm, m->Sm, m->Tm, m->Dm, m->Jm);
    fprintf(stream, "%3d, %3d, %5d, %5d, %5d, %2d, ", m->Cm, m->Bm, m->Im, m->Wm, m->Ym, m->Qm);
    fprintf(stream, "%5.2f, %5d, %5d, %5s", 100.0 * m->Um, m->Rm, m->Nm, m->Ng ? "#####" : "ok");
    fprintf(stream, "\n");
}

void usage (void)
{
    fprintf(stderr, "usage: cis [-j <jitter_in_microseconds>] [-d <deadline_in_percent_of_period>]\n");
    fprintf(stderr, "        -j <jitter>    queueing jitter in microseconds; default 500\n");
    fprintf(stderr, "        -d <deadline>  message deadline in percent of period; default 90\n");
    exit(EXIT_FAILURE);
}

int main (int argc, char **argv)
{
#if CIS_VERBOSE
    // Nice feedback for human, inconvenient for scripting use of this utility
    fprintf(stderr, "Hello World\n");
#endif
    int i = 1;

    uint32_t deadline_as_percent_of_period = 90u;
    uint32_t jitter_usec = 500u; // default queuing jitter of 500 microseconds

    while (i < argc)
    {
        if (strcmp(argv[i], "-j") == 0)
        {
            i += 1;
            if (i < argc)
            {
                jitter_usec = strtoul(argv[i], NULL, 10);
#if CIS_VERBOSE
                // Nice feedback for human, inconvenient for scripting use of this utility
                fprintf(stderr, "Using jitter of %u microseconds\n", jitter_usec);
#endif
            }
            else
            {
                usage();
            }
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            i += 1;
            if (i < argc)
            {
                deadline_as_percent_of_period = strtoul(argv[i], NULL, 10);
#if CIS_VERBOSE
                // Nice feedback for human, inconvenient for scripting use of this utility
                fprintf(stderr, "Using deadline of %u percent of period\n", deadline_as_percent_of_period);
#endif
            }
            else
            {
                usage();
            }
        }
        else
        {
            usage();
        }
        i += 1;
    }

    msg_v msgs[CAN_ELEM_LAST_MSG]={};
    init_msgs(jitter_usec, deadline_as_percent_of_period, msgs);
#if CIS_VERBOSE
    // Nice feedback for human, inconvenient for scripting use of this utility
    fprintf(stderr, "Msgs Inited\n");
#endif

    calculate_Cm(msgs);
#if CIS_VERBOSE
    // Nice feedback for human, inconvenient for scripting use of this utility
    fprintf(stderr, "Cm\n");
#endif
    calculate_Bm(msgs);
#if CIS_VERBOSE
    // Nice feedback for human, inconvenient for scripting use of this utility
    fprintf(stderr, "Bm\n");
#endif
    calculate_Wm(msgs);
#if CIS_VERBOSE
    // Nice feedback for human, inconvenient for scripting use of this utility
    fprintf(stderr, "Wm\n");
#endif
    calculate_Ym(msgs);

    uint32_t unscheduleable_count = calculate_Rm(msgs);

    fprintf_hdr(stdout);

    for (uint32_t i = 0u; i < CAN_ELEM_LAST_MSG; i++)
    {
        fprintf_msg(stdout, &msgs[i]);
    }

    if (unscheduleable_count > 0u)
    {
        fprintf(stderr, "UNSCHEDULABLE!: %d\n", unscheduleable_count);
        return EXIT_FAILURE;
    }
    else
    {
        fprintf(stderr, "Scheduleable\n");
        return EXIT_SUCCESS;
    }
}
#endif