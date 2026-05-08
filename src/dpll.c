#include "dpll.h"

#include <stdbool.h>

#include "err.h"
#include "list.h"
#include "util.h"
#include "variables.h"

typedef enum Reason { CHOSEN, IMPLIED } Reason;

/**
 * Struct to represent an entry in the assignment stack. Should only be created
 * and freed by pushAssignment and popAssignment.
 */
typedef struct Assignment {
    VarIndex var;
    Reason reason;
} Assignment;

/**
 * Adds a new assignment to the assignment stack.
 *
 * @param stack  an assignment stack
 * @param var    the variable to assign
 * @param r      the reason for the assignment
 */
void pushAssignment(List* stack, VarIndex var, Reason r) {
    Assignment* a = (Assignment*)malloc(sizeof(Assignment));
    a->var = var;
    a->reason = r;
    push(stack, a);
}

/**
 * Removes the head element of an assignment stack and frees it.
 *
 * @param stack  an assignment stack
 */
void popAssignment(List* stack) {
    Assignment* a = (Assignment*)peek(stack);
    free(a);
    pop(stack);
}

/**
 * Führt eine Iteration des DPLL Algorithmus aus.
 *
 * @param vt       die zugrunde liegende Variablentabelle
 * @param stack    der Zuweisungsstack
 * @param cnf      die zu prüfende Formel
 * @return         1 falls der Algorithmus mit SAT terminieren sollte,
 *                 0 falls der Algorithmus weiterlaufen sollte,
 *                 -1 falls der Algorithmus mit UNSAT terminieren sollte
 */
/**
 * Performs one iteration of the DPLL algorithm.
 *
 * @param vt       the underlying variable table
 * @param stack    an assignment stack
 * @param cnf      the formula to check
 * @return         1 if the algorithm should terminate with SAT,
 *                 0 if the algorithm should continue,
 *                -1 if the algorithm should terminate with UNSAT
 */

int iterate(VarTable* vt, List* stack, CNF* cnf) {
    int eval = evalCNF(cnf);
    if (eval == TRUE) {  // this was the final one
        return 1;
    }

    ListIterator item = mkIterator(&(cnf->clauses));

    while (isValid(&item)) {
        Clause* clause_i = (Clause*)getCurr(&item);

        if (clause_i->val == FALSE) {
            Assignment* Topelement = (Assignment*)peek(stack);
            while (Topelement != NULL && Topelement->reason != CHOSEN) {
                updateVariableValue(vt, Topelement->var, UNDEFINED);
                pop(stack);
                Topelement = (Assignment*)peek(stack);
            }

            if (Topelement != NULL) {
                TruthValue Alternate_value =
                    (getVariableValue(vt, Topelement->var) == TRUE) ? FALSE

                                                                    : TRUE;
                Topelement->reason = IMPLIED;
                updateVariableValue(vt, Topelement->var, Alternate_value);

                return 0;
            } else {
                return -1;
            }
        }

        Literal lodu = getUnitLiteral(vt, clause_i);
        if (lodu != 0) {
            VarIndex var = abs(lodu);
            TruthValue val = (lodu > 0) ? TRUE : FALSE;
            pushAssignment(stack, var, IMPLIED);
            updateVariableValue(vt, var, val);
            return 0;
        }

        next(&item);
    }

    VarIndex Newvariable = getNextUndefinedVariable(vt);
    if (Newvariable != 0) {
        pushAssignment(stack, Newvariable, CHOSEN);
        updateVariableValue(vt, Newvariable, TRUE);
        return 0;
    }
    return -1;
}

//  IN THE ABOVE ITERATE FUNTION , THE STRUCTURE IS INSPIRED BY THE PSEAUDO CODE
//  FROM PROJECT DESCRIPTION , THE USAGE OF TERNARY OPERATOR IS DONE WITH HELP
//  OF GPT AND THE PRINT STATMENTS WERE ONLY ADDING WHILE DEBUGGING ,AS INITIALY
//  MY CODE RAN INTO INFINITE LOOP

char isSatisfiable(VarTable* vt, CNF* cnf) {
    List stack = mkList();

    int res;
    do {
        res = iterate(vt, &stack, cnf);
    } while (res == 0);

    while (!isEmpty(&stack)) {
        popAssignment(&stack);
    }

    return (res < 0) ? 0 : 1;
}
