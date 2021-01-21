#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "hw01.h"

/****  UTILITIES  ************************************************************/

/*
 * Returns true if and only if 'c' is one of the characters of 'str'.
 *
 * DO NOT EDIT THIS FUNCTION, it is not part of your homework.
 * You can, however, use it to make your life easier, like this:
 *
 *      if (isOneOf(c, str)) {
 *          // this will execute if c is contained in str
 *      } else {
 *          // this will execute otherwise
 *      }
 *
 * For instance,
 *      isOneOf(' ', "Hello, World!")   -> true
 *      isOneOf(' ', "Pocahontas")      -> false
 *      isOneOf('.', "Hello, World!")   -> false
 *      isOneOf('!', "Hello, World!")   -> true
 */
bool isOneOf(char c, char str[])
{
    return strchr(str, c) != NULL;
}

/****  HERE BEGINS YOUR HOMEWORK  ********************************************/

/*
 * There shall be no two consecutive spaces in the string.
 * You are to FIND and FIX an error in this code.
 *
 * Examples:
 *  Wrong: "Hello  world."
 *  Wrong: "Hello.  World."
 *  Right: "Hello world."
 *  Right: "Hello. Wonderful. World."
 */
bool noTwoConsecutiveSpaces(char str[], size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (isspace(str[i]) && isspace(str[i - 1]))
            return false;
    }
    return true;
}

/**
 * The string represents a list of sentences. A sentence ends with with a
 * '.', '?', or a '!' sign, and starts with the first nonspace character
 * in the string or after the end of the previous sentence.
 *
 * Hint: see 'isupper' function
 */
bool sentenceStartsWithUppercase(char str[], size_t len)
{
    return true;
}

/*
 * Every sentence in the string ends with '.', '?' or a '!'.
 */
bool allSentencesAreClosed(char str[], size_t len)
{
    return true;
}

/**
 * There is no separator with a space before it.
 *
 *  Wrong: "Hello, World !"
 *  Wrong: "Hello , World!"
 *  Right: "Hello, World!"
 */
bool noSpaceBeforeSeparators(char str[], size_t len)
{
    return true;
}

/**
 * Every seprator (i.e. '.', ',', '!' or '?') is followed by a space,
 * with the exception of the separator is the last character in the string.
 *
 *  Wrong: "Hello,world."
 *  Right: "Hello, world."
 */
bool spaceAfterSeparators(char str[], size_t len)
{
    return true;
}

/**
 * Every opening bracket '(' is properly closed with ')' and there are no
 * nested parentheses.
 *
 *  Wrong: ":("
 *  Right: "Hello world (again)!"
 *  Wrong: "Hello, (tiny (puny)) world!"
 */
bool properParenthesesPairing(char str[], size_t len)
{
    return true;
}

/****  BONUS  ****************************************************************/

/*
 * You are not required to implement the following functions, but you
 * can get up to 3 bonus points if you do it correctly.
 */

/**
 * Parentheses should not cross sencentes. That is, if a bracket is opened
 * outside of a sentence, it must be closed outside as well.
 * On the other hand, a bracket opened inside a sentence has to be
 * closed before the sentence ends.
 *
 *  Right: "(First sentence. Second sentence.) Third sentence."
 *  Right: "First (inside) sentence."
 *  Wrong: "First (sentence. Second) sentence."
 *  Wrong: "(First sentence. Second) sentence."
 */
bool noParenthesesSentenceCrossing(char str[], size_t len)
{
    return true;
}

/**
 * There can be double quotes inside the text '"', but always in pair.
 * Opening quote must not be followed by a space and similarly
 * there can be no space before the closing quote.
 */
bool noSpaceBeforeClosingOrAfterOpeningApostrophe(char str[], size_t len)
{
    return true;
}
