#include <stdio.h>  // printf
#include <string.h> // strlen

#include "hw01.h"
#include "hw01_tests.h"

/*
 * This file uses a minimalistic testing framework. You are not required to
 * understand it perfectly and you will see a much better one
 * with more features later in the course.
 *
 * For now, you can write simple tests below lines marked with TODO.
 * All you need are two functions(*) SHOULD_PASS and SHOULD_FAIL.
 *
 * For instance,
 *
 *      TEST_FUNCTION(noTwoConsecutiveSpaces);
 *
 * says that the function being tested is "noTwoConsecutiveSpaces".
 * Then,
 *
 *      SHOULD_PASS("Hello, World");
 *
 * requires that the function returns true on "Hello, World".
 * Otherwise the test fails and you need to fix your function.
 * Similartly,
 *
 *      SHOULD_FAIL("Hello,  world");
 *
 * asserts that noTwoConsecutiveSpaces returns false on given input,
 * otherwise the test fails. You can call as many SHOULD_PASS or SHOULD_FAIL
 * as you like, with different parameters.
 *
 * (*) Those are not really functions, but sssh!
 */

TEST(noTwoConsecutiveSpaces) {
    TEST_FUNCTION(noTwoConsecutiveSpaces);

    SHOULD_PASS("Hello, world");
    SHOULD_FAIL("Hello,  world");

    // You can write your own tests below this line.

    // TODO

    // ... but ABOVE this one.
    TEST_DONE;
}

TEST(sentenceStartsWithUppercase) {
    TEST_FUNCTION(sentenceStartsWithUppercase);

    SHOULD_PASS("Ahoj konicci.");
    SHOULD_FAIL("ahoj svete.");

    // TODO Your tests go here.

    TEST_DONE;
}

TEST(allSentencesAreClosed) {
    TEST_FUNCTION(allSentencesAreClosed);

    // TODO Your tests go here.

    TEST_DONE;
}

TEST(noSpaceBeforeSeparators) {
    TEST_FUNCTION(noSpaceBeforeSeparators);

    // TODO Your tests go here.

    TEST_DONE;
}

TEST(spaceAfterSeparators) {
    TEST_FUNCTION(spaceAfterSeparators);

    // TODO Your tests go here.

    TEST_DONE;
}

TEST(properParenthesesPairing) {
    TEST_FUNCTION(properParenthesesPairing);

    // TODO Your tests go here.

    TEST_DONE;
}

TEST(noParenthesesSentenceCrossing) {
    TEST_FUNCTION(noParenthesesSentenceCrossing);

    // TODO Your tests go here.

    TEST_DONE;
}

TEST(noSpaceBeforeClosingOrAfterOpeningApostrophe) {
    TEST_FUNCTION(noSpaceBeforeClosingOrAfterOpeningApostrophe);

    // TODO Your tests go here.

    TEST_DONE;
}

/****  YOU SHOULD NOT EDIT ANYTHING BELOW THIS LINE  *************************/

int main(void)
{
    TEST_RUN(sentenceStartsWithUppercase);
    TEST_RUN(noSpaceBeforeSeparators);
    TEST_RUN(noTwoConsecutiveSpaces);
    TEST_RUN(spaceAfterSeparators);
    TEST_RUN(properParenthesesPairing);
    TEST_RUN(noParenthesesSentenceCrossing);
    TEST_RUN(allSentencesAreClosed);
    TEST_RUN(noSpaceBeforeClosingOrAfterOpeningApostrophe);
    return 0;
}
