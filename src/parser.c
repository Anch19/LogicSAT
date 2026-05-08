#include "parser.h"

#include <string.h>

#include "err.h"
#include "lexer.h"
#include "list.h"
#include "util.h"

/**
 * Assigns symbols to strings.
 *
 * Aborts the program with an error message if an invalid input is detected.
 *
 * @param str  a string to translate
 * @return     the resulting symbol
 */

FormulaKind toKind(const char* str) {
    if (strcmp(str, "&&") == 0) {
        return AND;
    } else if (strcmp(str, "||") == 0) {
        return OR;
    } else if (strcmp(str, "=>") == 0) {
        return IMPLIES;
    } else if (strcmp(str, "<=>") == 0) {
        return EQUIV;
    } else if (strcmp(str, "!") == 0) {
        return NOT;
    } else {
        // valid variable?
        int i = 0;
        while (str[i] != '\0') {
            if (!((str[i] >= 'a' && str[i] <= 'z') ||
                  (str[i] >= 'A' && str[i] <= 'Z') ||
                  (str[i] >= '0' && str[i] <= '9'))) {
                err(str);
            }
            i++;
        }
        return VAR;
    }
}

PropFormula* parseFormula(FILE* input, VarTable* vt) {
    List myStack =
        mkList();  // we build a myStack and also intiate myStacksize to
                   // keep the track of number of elemenst in our myStack
                   // which we need in binary and unary operator case
    int myStackSize = 0;
    PropFormula* final_formula =
        NULL;  // this will store the final parsed formula
    char* Mytoken;
    while ((Mytoken = nextToken(input)) !=
           NULL) {  // getting the token form nexttoken funtion
        FormulaKind kind = toKind(Mytoken);

        if (kind == VAR) {
            push(&myStack,
                 mkVarFormula(
                     vt,
                     Mytoken));  // check for everycase of what our token can be
            myStackSize++;
        }

        else if (kind == NOT) {
            if (myStackSize >= 1) {
                PropFormula* operand = (PropFormula*)peek(&myStack);
                pop(&myStack);
                myStackSize--;
                push(&myStack, mkUnaryFormula(kind, operand));
                myStackSize++;
            } else {
                free(Mytoken);
                err("Problem in unary ");
            }
        }

        else if (kind == AND || kind == OR || kind == IMPLIES ||
                 kind == EQUIV) {
            if (myStackSize >= 2) {
                PropFormula* right_op = (PropFormula*)peek(&myStack);
                pop(&myStack);
                myStackSize--;
                PropFormula* left_op = (PropFormula*)peek(&myStack);
                pop(&myStack);
                myStackSize--;
                push(&myStack, mkBinaryFormula(kind, left_op, right_op));
                myStackSize++;
            } else {
                free(Mytoken);
                Mytoken = NULL;
                err("problem in binary cases ");
            }
        } else {
            err(Mytoken);
        }
    }

    if (myStackSize ==
        0) {  // this checks if the parsing happened properly above
              // or not , at the end we should have one element in
              //  in our myStack , which is parsed formula , if
              //  nothing is left that means , error
        err("Nothing took place");
    } else if (myStackSize > 1) {
        err("parsing did not take place properly");
    } else {
        final_formula = peek(&myStack);  // remove the parsed formula
        pop(&myStack);  // i have only remlved the elements from myStack , but
                        // not freed the memory , not sure if it may cause a
                        // memory leak or not
    }
    return final_formula;
}
