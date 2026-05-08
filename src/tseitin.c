#include "tseitin.h"

#include <stdio.h>

#include "err.h"
#include "util.h"

/**
 * Inserts a clause with one literal into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    a literal
 */
void addUnaryClause(VarTable* vt, CNF* cnf, Literal a) {
    Clause* clause = mkTernaryClause(vt, a, 0, 0);
    addClauseToCNF(cnf, clause);
}

/**
 * Inserts a clause with two literals into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    the first literal
 * @param b    the second literal
 */
void addBinaryClause(VarTable* vt, CNF* cnf, Literal a, Literal b) {
    Clause* clause = mkTernaryClause(vt, a, b, 0);
    addClauseToCNF(cnf, clause);
}

/**
 * Inserts a clause with three literals into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    the first literal
 * @param b    the second literal
 * @param c    the third literal
 */
void addTernaryClause(VarTable* vt, CNF* cnf, Literal a, Literal b, Literal c) {
    Clause* clause = mkTernaryClause(vt, a, b, c);
    addClauseToCNF(cnf, clause);
}

/**
 * Adds clauses for a propositional formula to a CNF.
 *
 * For a propositional formula pf, clauses that are added that are equivalent to
 *
 *     x <=> pf
 *
 * where x is usually a fresh variable. This variable is also returned.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param pf   a propositional formula
 * @return     the variable x, as described above
 */

VarIndex addClauses(VarTable* vt, CNF* cnf, const PropFormula* pf) {
    // if (pf->kind == VAR) {
    //     // addUnaryClause(vt, cnf, pf->data.var);
    //     return pf->data.var;
    // }

    VarIndex freshVar;

    switch (pf->kind) {
        case VAR: {
            return pf->data.var;
        }
        case NOT: {
            VarIndex operandVar = addClauses(vt, cnf, pf->data.single_op);
            freshVar = mkFreshVariable(vt);
            addBinaryClause(
                vt, cnf, -freshVar,
                -operandVar);  // NOT cases according to the project description
            addBinaryClause(vt, cnf, operandVar, freshVar);
            break;
        }
        case AND: {
            VarIndex operand_1 = addClauses(
                vt, cnf, pf->data.operands[0]);  // as it is a binary operator ,
                                                 // so we need two operands
            VarIndex operand_2 = addClauses(vt, cnf, pf->data.operands[1]);
            freshVar = mkFreshVariable(vt);
            addBinaryClause(
                vt, cnf, operand_1,
                -freshVar);  // AND cases according to the project description
            addBinaryClause(vt, cnf, operand_2, -freshVar);
            addTernaryClause(vt, cnf, freshVar, -operand_1, -operand_2);
            break;
        }
        case OR: {
            VarIndex operand_1 = addClauses(vt, cnf, pf->data.operands[0]);
            VarIndex operand_2 = addClauses(vt, cnf, pf->data.operands[1]);
            freshVar = mkFreshVariable(vt);
            addBinaryClause(
                vt, cnf, -operand_1,
                freshVar);  // OR cases according to the project description
            addBinaryClause(vt, cnf, -operand_2, freshVar);
            addTernaryClause(vt, cnf, -freshVar, operand_1, operand_2);
            break;
        }
        case IMPLIES: {
            VarIndex operand_1 = addClauses(vt, cnf, pf->data.operands[0]);
            VarIndex operand_2 = addClauses(vt, cnf, pf->data.operands[1]);
            freshVar = mkFreshVariable(vt);
            addBinaryClause(vt, cnf, operand_1, freshVar);
            addTernaryClause(vt, cnf, -freshVar, -operand_1,
                             operand_2);  // IMPLIES cases according to the
                                          // project description
            addBinaryClause(vt, cnf, -operand_2, freshVar);
            break;
        }
        case EQUIV: {
            VarIndex operand_1 = addClauses(vt, cnf, pf->data.operands[0]);
            VarIndex operand_2 = addClauses(vt, cnf, pf->data.operands[1]);
            freshVar = mkFreshVariable(vt);
            addTernaryClause(vt, cnf, -freshVar, -operand_1, operand_2);
            addTernaryClause(vt, cnf, -freshVar, operand_1, -operand_2);
            addTernaryClause(vt, cnf, freshVar, -operand_1, -operand_2);
            addTernaryClause(
                vt, cnf, freshVar, operand_1,
                operand_2);  // EQUIV cases according to the project description
            // return freshVar;
            break;
        }
    }

    return freshVar;
}

CNF* getCNF(VarTable* vt, const PropFormula* f) {
    CNF* res = mkCNF();

    VarIndex x = addClauses(vt, res, f);

    addUnaryClause(vt, res, x);

    return res;
}
