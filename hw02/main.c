#include <stdio.h>
#include <stdlib.h>

#define oppHandPossibilities 1980

const char suitArr[4] = { 'h', 'd', 's', 'c' };
const char valueArr[13] = { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };

void sortCards(int *cards, int len, int (*sortF)(int));
int getValById(int id);
int getSuitById(int id);

int notInArr(int *a, int len, int value)
{
    for (int i = 0; i < len; i++) {
        if (a[i] == value) {
            return 0;
        };
    }
    return 1;
}

/**
 * @brief ccatArrays Concatenate array (can be used to make copy with one NULL array)
 * @param target - here are copied both arrays (must have length of lA + lB when initialized)
 * @param a, b - arrays to concatenate
 * @param lA, lB - thir length
 */
void ccatArrays(int *target, int *a, int lA, int *b, int lB)
{
    int i, j;
    for (i = 0; i < lA; i++) {
        target[i] = a[i];
    }
    for (j = 0; j < lB; j++) {
        target[i + j] = b[j];
    }
}

int getValue(char c)
{
    if (0x32 <= c && c <= 0x39) {
        return c - 0x32;
    }
    switch (c) {
    case 'T':
        return 8;
    case 'J':
        return 9;
    case 'Q':
        return 10;
    case 'K':
        return 11;
    case 'A':
        return 12;
    default:
        return 0;
    }
}

int getSuit(char c)
{
    switch (c) {
    case 'h':
        return 0;
    case 'd':
        return 1;
    case 's':
        return 2;
    case 'c':
        return 3;
    default:
        return 0;
    }
}

void getCards(int *cards, int len)
{
    for (int i = 0; i < len; i++) {
        char c = fgetc(stdin);
        char card[2];

        for (int j = 0; j < 2; j++) {
            card[j] = c;
            c = fgetc(stdin);
        }
        cards[i] = getValue(card[0]) + 13 * getSuit(card[1]);
    }
}

int getValById(int id)
{
    return id % 13;
}

int getNegativeValById(int id)
{
    return -id % 13;
}

int getSuitById(int id)
{
    return id / 13;
}

int getNegativeSuitById(int id)
{
    return -id / 13;
}

int getValFromDictById(int a, int (*dictF)(int), int *keys, int *amount)
{
    int i;
    for (i = 0; i < 7; i++) {
        if (keys[i] == dictF(a)) {
            return amount[i];
        }
    }
    return 0;
}

/**
 * @brief freqAnalyse - from analyzed array fill given arrays with {key : amount} on corresponding index
 * @param cards - array to analyse
 * @param keys - here are saved keys of occured cards (their value)
 * @param amount - here are saved amount of single cards
 */
int freqAnalyse(int *cards, int *keys, int *amount, int (*freqF)(int))
{
    sortCards(cards, 7, freqF);
    int position = -1;

    for (int i = 0; i < 7; i++) {
        int value = freqF(cards[i]);

        if (position >= 0 && keys[position] == value) {
            amount[position]++;
        } else {
            position++;
            keys[position] = value;
            amount[position] = 1;
        }
    }
    int max = amount[0];
    for (int i = 1; i < 7; i++) {
        if (amount[i] > max) {
            max = amount[i];
        }
    }
    return max;
}

/**
 * @brief sortCards Insertion sort - sorts by given function
 * @param cards - array of card IDs
 */
void sortCards(int *cards, int len, int (*sortF)(int))
{
    int i, j;
    for (i = 1; i < len; i++) {
        int id = cards[i];
        j = i - 1;

        while (j >= 0 && sortF(cards[j]) < sortF(id)) {
            cards[j + 1] = cards[j];
            j--;
        }
        cards[j + 1] = id;
    }
}

int sortCardsByDict(int *cards, int len, int (*freqF)(int))
{
    int keys[7] = { 0 }, amount[7] = { 0 };
    int length = freqAnalyse(cards, keys, amount, freqF);

    int i, j;
    for (i = 1; i < len; i++) {
        int selected = cards[i];
        int selectedValue = getValFromDictById(selected, freqF, keys, amount);
        j = i - 1;

        while (j >= 0 && getValFromDictById(cards[j], freqF, keys, amount) < selectedValue) {
            cards[j + 1] = cards[j];
            j--;
        }
        cards[j + 1] = selected;
    }
    return length;
}

int hasSuccesor(int cardId, int nextId)
{
    return (13 + getValById(cardId) - getValById(nextId)) % 13 == 1;
}

void bubbleFromTo(int *cards, int start, int end)
{
    int selected = cards[start];
    for (int j = start; j < end - 1; j++) {
        cards[j] = cards[j + 1];
    }
    cards[end - 1] = selected;
}

void sortCardsByConsecutive(int *cards, int range)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4 && range >= 5; j++) {
            int card = cards[j % range];
            int next = cards[(j + 1) % range];
            int previous = cards[(range + j - 1) % range];

            int hasSucc = hasSuccesor(card, next);
            int isAceTwo = getValById(card) == 12 && getValById(previous) == 0;

            if (getValById(card) == getValById(next) || (!hasSucc && !isAceTwo)) {
                bubbleFromTo(cards, j, 7);
                range--;
                j--;
            } else if (!hasSucc && isAceTwo) {
                bubbleFromTo(cards, j, range);
                j--;
            }
        }
    }
}

int checkFlush(int *cards)
{
    sortCards(cards, 7, &getValById);
    sortCardsByDict(cards, 7, &getSuitById);
    int suit = getSuitById(cards[0]);

    for (int i = 1; i < 5; i++) {
        int actSuit = getSuitById(cards[i]);

        /* If precedent cards has not same suit */
        if (actSuit != suit) {
            return 0;
        }
    }
    return 5;
}

int checkStraight(int *cards)
{
    sortCardsByConsecutive(cards, 7);

    if (getValById(cards[3]) > getValById(cards[0])) {
        return 0;
    }

    for (int i = 0; i < 4; i++) {
        int card = cards[i];
        int next = cards[i + 1];

        if (!hasSuccesor(card, next)) {
            return 0;
        }
    }
    return 4;
}

int checkStraightFlush(int *cards)
{
    sortCards(cards, 7, &getValById);
    int suitLength = sortCardsByDict(cards, 7, &getSuitById);
    sortCardsByConsecutive(cards, suitLength);

    if (getValById(cards[3]) > getValById(cards[0])) {
        return 0;
    }

    for (int i = 0; i < 4; i++) {
        int card = cards[i];
        int next = cards[i + 1];

        if (!hasSuccesor(card, next) || getSuitById(card) != getSuitById(next)) {
            return 0;
        }
    }
    return 8;
}

int checkTuples(int *cards)
{
    sortCards(cards, 7, &getValById);
    sortCardsByDict(cards, 7, &getValById);
    int maxTuples = 2;
    int i = 0;
    int selected = getValById(cards[0]);
    int value = 2;

    /* a-tuple followed by b-tuple has evaluation of  2^(a+b-2) - 2^(b-2)*/
    while (maxTuples > 0) {
        i++;
        int actVal = getValById(cards[i]);

        if (actVal == selected && i < 5) {
            value *= 2;
        } else {
            maxTuples--;
            selected = actVal;
        }
        value--;
    }
    sortCards(cards + i, 7 - i, &getValById);

    return value;
}

char compareHands(int *myCards, int *oppCards, int val)
{
    char result = 'D';
    if (val == 4 || val == 5) {
        sortCards(myCards, 7, &getValById);
        sortCards(oppCards, 7, &getValById);
    }
    if (val == 4) {
        sortCardsByConsecutive(myCards, 7);
        sortCardsByConsecutive(oppCards, 7);
    } else if (val == 5) {
        sortCardsByDict(myCards, 7, &getSuitById);
        sortCardsByDict(oppCards, 7, &getSuitById);
    }
    for (int i = 0; i < 5; i++) {
        if (getValById(myCards[i]) != getValById(oppCards[i])) {
            result = getValById(myCards[i]) > getValById(oppCards[i]) ? 'W' : 'L';
            break;
        }
    }
    return result;
}

int evalCards(int *cards)
{
    int straight = 0, flush = 0, tuple = 0;
    int maxValue = 0;

    flush = checkFlush(cards);
    if (flush > maxValue) {
        maxValue = flush;
    }

    sortCards(cards, 7, &getValById);
    straight = checkStraight(cards);
    if (straight > maxValue) {
        maxValue = straight;
    }
    if (flush && straight && checkStraightFlush(cards)) {
        return 8;
    }

    tuple = checkTuples(cards);
    if (tuple > maxValue) {
        maxValue = tuple;
    }

    return maxValue;
}

int findBetterOppHand(int *cards, float max)
{
    int myScore, oppScore, w = 0, d = 0, l = 0;
    float pMax = 0;
    for (int j = 0; j < 52; j++) {
        if (notInArr(cards, 7, j)) {
            cards[7] = j;

            for (int k = j + 1; k < 52; k++) {
                if (notInArr(cards, 8, k)) {
                    cards[8] = k;
                    int myCards[7], oppCards[7];
                    ccatArrays(myCards, cards, 7, NULL, 0);
                    ccatArrays(oppCards, cards + 7, 2, cards + 2, 5);

                    myScore = evalCards(myCards);
                    oppScore = evalCards(oppCards);
                    char compare = 'X';
                    if (myScore == oppScore) {
                        compare = compareHands(myCards, oppCards, myScore);
                    }

                    pMax = oppHandPossibilities - (2*l + d);
                    if (pMax < max) {
                        return 0;
                    }
                    if (myScore > oppScore || compare == 'W') {
                        w++;
                    } else if (compare == 'D') {
                        d++;
                    } else {
                        l++;
                    }
                }
            }
        }
    }
    return 2*w + d;
}

int countBestCard(int *myHand, int *otherCards, int *bestCards, int *wins)
{
    int counter = 0;
    float max = 0;
    int cards[9];
    /* Undefined indexes: 6, 7, 8 */
    ccatArrays(cards, myHand, 2, otherCards, 4);

    for (int i = 0; i < 52; i++) {
        if (notInArr(cards, 6, i)) {
            cards[6] = i;

            float potenMax = findBetterOppHand(cards, max);
            if (potenMax > 0) {
                if (potenMax > max) {
                    bestCards[0] = i;
                    *wins = potenMax + 0.5;
                    counter = 1;
                    max = potenMax;
                } else if (potenMax == max) {
                    bestCards[counter] = i;
                    counter++;
                }
            }
        }
    }
    return counter;
}

int greatCommDiv(int a, int b)
{
    int c = a % b;

    while (c > 0) {
        a = b;
        b = c;
        c = a % b;
    }
    return b;
}

int main(int argc, char *argv[])
{
    int myHand[2], oppHand[2], otherCards[5];
    int myScore, oppScore;
    char c = fgetc(stdin);

    if (argc == 1) {
        while (c != EOF) {
            ungetc(c, stdin);
            int myCards[7], oppCards[7];
            getCards(myHand, 2);
            getCards(oppHand, 2);
            getCards(otherCards, 5);

            ccatArrays(myCards, myHand, 2, otherCards, 5);
            ccatArrays(oppCards, oppHand, 2, otherCards, 5);

            myScore = evalCards(myCards);
            oppScore = evalCards(oppCards);

            char result;
            if (myScore > oppScore) {
                result = 'W';
            } else if (myScore == oppScore) {
                result = compareHands(myCards, oppCards, myScore);
            } else {
                result = 'L';
            }
            printf("%c\n", result);

            c = fgetc(stdin);
        }
    } else if (argv[1][0] == '-' && argv[1][1] == 'r') {
        while (c != EOF) {
            ungetc(c, stdin);
            getCards(myHand, 2);
            getCards(otherCards, 4);

            int bestCards[46] = { 0 }, counter = 0, wins = oppHandPossibilities;
            counter = countBestCard(myHand, otherCards, bestCards, &wins);

            char append = argv[1][2];
            sortCards(bestCards, counter, &getNegativeSuitById);
            sortCards(bestCards, counter, &getNegativeValById);
            for (int i = 0; i < counter; i++) {
                int value = getValById(bestCards[i]);
                int suit = getSuitById(bestCards[i]);
                printf("%c%c", valueArr[value], suitArr[suit]);
                if (i != counter - 1) {
                    putchar(' ');
                }
            }
            printf("\n");
            if (append == '0') {
                int a = 1980, b = wins, c = oppHandPossibilities % wins;
                while (c > 0) {
                    a = b;
                    b = c;
                    c = a % b;
                }
                printf("%i/%i\n", (wins / b), (oppHandPossibilities / b));
            } else if (append != '\0') {
                char format[6] = "%.xf\n";
                format[2] = append;
                printf(format, ((float)wins / oppHandPossibilities));
            }

            c = fgetc(stdin);
        }
    }

    return 0;
}
