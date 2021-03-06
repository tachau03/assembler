#include "assembler.h"

/*
 * The functions in this file print pseudo-binary output for:
 *    - integer values, e.g., 23
 *    - integer values represented as strings, e.g., "23"
 *    - registers, e.g., $zero ==> "00000"
 *    - jump targets
 *    - branch offsets
 * The pseudo-binary output actually consists of character 0's and 1's,
 * with the length of the binary output specified by the `numBits`
 * parameter.  The `lineNum` parameter is used when printing error
 * messages.
 *
 * Author(s): Chau Ta, Alyce Brady
 * Date: 02/28/22
 * 
 * Modification Log:
 *    - AB, 3/25/2020 - implement printIntInString, stub printInt,
 *                      provide skeletons for others
 *    - AB, 3/1/2022 -  replace previous printIntInString with
 *                      printSignedIntInString and printUnsignedIntInString.
 *                      New internal printIntInString does range checking.
 *    - CT, 3/1/2022 - complete printInt, printReg, printJumpTarget,
 *                      printBranchOffSet
 */

/* Print integer value in pseudo-binary (made up of character '0's and '1's).
 *      @param value   value to print in pseudo-binary
 *      @param length  length of binary code needed, in bits
 *      @pre           value can be represented in `length` number of bits
 *      @pre           value >= 0
 *   (You can decide whether or not to require that `value` be non-negative.)
 */
void printInt(int value, int length)
{
    /* Print the value passed as a parameter in "character binary" format.
     */

    /* This is NOT the correct final behavior of this function, but may
     * be useful during development because the output is easier to read
     * than binary (or pseudo-binary) output would be.
     * When all testing has been completed, the printf statement
     * should be removed, commented out, or turned into a printDebug.
     */

    printDebug(" (%d)", value);

    /* Print unsigned int as binary 
    int powOf2;
    powOf2 = pow(2, length - 1);
    int i;
    for (i = 0; i < length; i++)
    {
        if ( value - powOf2 >= 0)    
        {
            value -= powOf2;
            printf("%d", 1);
        }
        else
            printf("%d", 0);
    
        powOf2 /= 2;  
    }
    */

    /* Print signed (unsigned) int as binary */
    int powOf2;
    powOf2 = 1;
    powOf2 <<= (length - 1); /* start with the left most bit */

    while ( powOf2 > 0 )
    {
        if ( (value & powOf2) == 0 )
            printf("0");
        else    
            printf("1");
        powOf2 >>= 1; /* shift right 1 bit */
    }
}


/* Print register in pseudo-binary (made up of character '0's and '1's).
 *      @param regName   name of register to print in pseudo-binary
 *      @param lineNum   line number (for error messages)
 * If the register name passed as a parameter is an invalid register name,
 * this function prints "<invalid reg>" instead of the register number,
 * allowing the rest of the instruction to be parsed and printed.
 */
void printReg(char * regName, int lineNum)
{
    /* If the string contains a valid register name, print the register number.
     * Otherwise print "<invalid reg>" to stdout and an error message to
     * stderr.
     */

    static char * regArray[] =
        {
                "$zero",
                "$at",
                "$v0","$v1",
                "$a0","$a1","$a2","$a3",
                "$t0","$t1","$t2","$t3", "$t4","$t5","$t6","$t7",
                "$s0","$s1","$s2","$s3", "$s4","$s5","$s6","$s7",
                "$t8","$t9",
                "$k0","$k1",
                "$gp","$sp","$fp","$ra",
        };
    
    /* Loop through register array. If match, print register number */
    int i;
    for (i = 0; i < 32; i++)
    {
        if ( strcmp(regName, regArray[i]) == SAME )
        {
            printInt(i, 5);
            return;
        }
    }

    /* if can not find the register, print an error message */
    fprintf( stderr, "Line %d: trying to print <%s> as invalid register.\n"
            , lineNum, regName );
}

/* Declare internal printIntInString used by both printSignedIntInString
 * and printUnsignedIntInString.
 */
static void printIntInString(char * intInString, int numBits,
        int lowerBound, int upperBound, int lineNum);

/* Print signed value in `intInString` as pseudo-binary (made up of character
 * '0's and '1's).  Prints and error if the token in the string is not a
 * valid integer or if it is not in the range
 * -2^(numBits-1) <= integer < 2^(numBits-1).
 *      @param intInString   string containing integer, e.g., "23"
 *      @param length  length of binary code needed, in bits
 *      @param lineNum line number (for error messages)
 */
void printSignedIntInString(char * intInString, int numBits, int lineNum)
{
    /* The decimal value should be in the asymmetric range:
     *    -2^(numBits-1) <= integer < 2^(numBits-1).
     */
    int upperBound = 1 << (numBits - 1);
    int lowerBound = -1 * upperBound;
    printIntInString(intInString, numBits, lowerBound, upperBound, lineNum);
}


/* Print unsigned value in `intInString` as pseudo-binary (made up of character
 * '0's and '1's).  Prints and error if the token in the string is not a
 * valid integer or if it is not in the range 0 <= integer < 2^(numBits).
 *      @param intInString   string containing integer, e.g., "23"
 *      @param length  length of binary code needed, in bits
 *      @param lineNum line number (for error messages)
 */
void printUnsignedIntInString(char * intInString, int numBits, int lineNum)
{
    int upperBound = 1 << numBits;
    int lowerBound = 0;
    printIntInString(intInString, numBits, lowerBound, upperBound, lineNum);
}


/* Print value in `intInString` as pseudo-binary (made up of character
 * '0's and '1's).  Prints an error if the token in the string is not a
 * valid integer or if it is not in the asymmetric range given by
 *  lowerBound <= value < upperBound
 *      @param intInString   string containing integer, e.g., "23"
 *      @param numBits length of binary code needed, in bits
 *      @param lowerBound lower bound; value should be >= lowerBound
 *      @param upperBound upper bound; value should be < upperBound
 *      @param lineNum line number (for error messages)
 */
static void printIntInString(char * intInString, int numBits,
        int lowerBound, int upperBound, int lineNum)
{
    char * endPtr;

    /* Convert string to decimal (base 10) value. */
    int decimal = (int) strtol(intInString, &endPtr, 10);

    /* If the string did not contain a valid int, print an error message. */
    if ( *intInString == '\0' || *endPtr != '\0' )
    {
        printError("Line %d: trying to print %s as an int (%s).\n",
                lineNum, intInString, "not a valid integer");
        return;
    }

    /* If the decimal value is in range, print it.  Otherwise, print an
     * error message.  The value should be in the asymmetric range:
     *      lowerBound <= decimal < upperBound.
     */
    if ( decimal >= lowerBound && decimal < upperBound )
    {
        /* In range!  Print the value. */
        printInt(decimal, numBits);
    }
    else
    {
        printError("Line %d: %s is not in range %d <= %s < %d.\n",
                lineNum, intInString, lowerBound, intInString, upperBound);
    }
}


/* Print address to jump to.
 *      @param targetLabel   label being branched to
 *      @param table         label table
 *      @param lineNum       line number (for error messages)
 */
void printJumpTarget(char * targetLabel, LabelTableArrayList * table,
                     int lineNum)
{
    /* This function should call findLabel to get the address associated
     * with the target label in the label table, calculate the portion of
     * the address to store in the instruction, and then print that value
     * with the appropriate number of bits.
     */

    int labelAddr;
    labelAddr = findLabelAddr (table, targetLabel); 

    if ( labelAddr == -1 )
        printError("Line %d: trying to jump to %s as an invalid target label.\n",
                lineNum, targetLabel);
    else
        printInt(labelAddr / 4, 26);    /* Divide by 4 to calculate the 
                                        portion of the address */

}


/* Print branch offset to branch to the target label.
 *      @param targetLabel   label being branched to
 *      @param table         label table
 *      @param PC            Program Counter (could use lineNum instead)
 *      @param lineNum       line number (for error messages)
 */
void printBranchOffset(char * targetLabel, LabelTableArrayList * table,
                       int PC, int lineNum)
{
    /* This function should call findLabel to get the address associated
     * with the target label in the label table, calculate the distance
     * from the PC to that address and the portion of that to store in the
     * instruction, and then print that value with the appropriate number
     * of bits.
     */

    int labelAddr;
    labelAddr = findLabelAddr (table, targetLabel); 

    if ( labelAddr == -1 )
        printError("Line %d: trying to branch to %s as an invalid target label.\n",
                lineNum, targetLabel);
    else
        printInt( (labelAddr - PC) / 4, 16);    /* Calculate the 
                                                portion of the address */

}
