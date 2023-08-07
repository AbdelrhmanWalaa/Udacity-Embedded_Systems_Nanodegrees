/* Standard Library */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Library Module */
#include "../Library/standard_types.h"

/* Console Module */
#include "console.h"

/*
 Name: showCursor
 Input: EN_consoleBool True or False
 Output: void
 Description: Static Function to show cursor or hide it according to input.
*/
static void showCursor(EN_cursorState_t state)
{
    /* Check 1: To show the cursor */
    if (state == SHOW)
    {
        printf("\e[?25h");
    }
    /* Check 2: To hide the cursor */
    else if (state == HIDE)
    {
        printf("\e[?25l");
    }
}

/*
 Name: systemPrintOut
 Input: Pointer to string
 Output: EN_consoleError_t Error or No Error
 Description: Function to print string on screen passed as input.
*/
EN_consoleError_t systemPrintOut(uint8_t *string)
{
    /* Define local variable to set the error state, No Error */
    EN_consoleError_t Loc_u8ErrorState = CONSOLE_OK;

    /* Check 1: Pointer is NULL */
    if (string == NULL)
    {
        /* Update error state, Pointer is NULL! */
        Loc_u8ErrorState = NO_STRING;
    }
    /* Check 2: Pointer is not NULL */
    else
    {
        /* Hide cursor */
        showCursor(HIDE);
        /* Clear screen */
        system("cls");
        /* Delay 0.5 sec. */
        sleep(0.5);

        /* Print string */
        printf("\n\n");
        printf("%s", string);

        /* Delay 3 sec. */
        sleep(3);
        /* Clear screen */
        system("cls");
        /* Show cursor */
        showCursor(SHOW);
    }

    return Loc_u8ErrorState;
}

/*
 Name: systemDeletelLine
 Input: Pointer to string
 Output: EN_consoleError_t Error or No Error
 Description: Function to delete current line, then print string on screen passed as input.
*/
EN_consoleError_t systemDeleteLine(uint8_t *string)
{
    /* Define local variable to set the error state, No Error */
    EN_consoleError_t Loc_u8ErrorState = CONSOLE_OK;

    /* Check 1: Pointer is NULL */
    if (string == NULL)
    {
        /* Update error state, Pointer is NULL! */
        Loc_u8ErrorState = NO_STRING;
    }
    /* Check 2: Pointer is not NULL */
    else
    {
        /* Hide cursor */
        showCursor(HIDE);
        /* Clear previous line */
        printf("\033[A");
        /* Clear current line */
        printf("\33[2K\r");

        /* Print string */
        printf("%s", string);

        /* Delay 3 sec. */
        sleep(3);
        /* Clear current line */
        printf("\33[2K\r");
        /* Show cursor */
        showCursor(SHOW);
    }

    return Loc_u8ErrorState;
}