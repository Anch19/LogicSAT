# LogicSAT

A Boolean SAT solver written in C, implementing the **DPLL algorithm** for deciding the satisfiability of propositional formulas in CNF.

Given a Boolean formula in DIMACS CNF format, the solver returns either a satisfying variable assignment (`SAT`) or proves no such assignment exists (`UNSAT`).

Built as a coursework project at Universität des Saarlandes to explore search-based decision procedures and the structure of NP-complete problems.

---

## Algorithms

- **DPLL** (Davis–Putnam–Logemann–Loveland) — recursive backtracking search over partial assignments
- **Unit propagation** — forced literals are assigned before branching, pruning the search space
- **Pure literal elimination** — variables appearing in only one polarity are assigned to satisfy their clauses
- **Conflict-driven backtracking** — on conflict, the solver unwinds the most recent decision and tries the opposite assignment

---

## Build

Requires `gcc` and `make`.

```bash
make
```

This produces the `sat_solver` binary.

---

## Usage

```bash
./sat_solver path/to/input.cnf
```

Example:

```bash
./sat_solver test/example.cnf
```

### Output

If satisfiable:

```
SAT
1 = TRUE
2 = FALSE
3 = TRUE
```

If unsatisfiable:

```
UNSAT
```

---

## Input format

Standard [DIMACS CNF](https://www.cs.utexas.edu/~marijn/Sat-Race/2008-rules.html#format). Each clause is a space-separated list of integers terminated by `0`. A positive integer `n` represents variable *xₙ*; a negative integer `-n` represents its negation.

Example — encoding `(x₁ ∨ ¬x₃) ∧ (x₂ ∨ x₃ ∨ ¬x₁)`:

```
c Example CNF formula
p cnf 3 2
1 -3 0
2 3 -1 0
```

The `p cnf` header declares 3 variables and 2 clauses. Lines starting with `c` are comments.

---

## Project structure

```
src/        # solver implementation (parser, DPLL, propagation)
test/       # CNF test inputs
Makefile    # build configuration
```

---

## Possible extensions

Natural next steps if the solver were extended toward modern SAT-solving techniques:

- **CDCL** (Conflict-Driven Clause Learning) — learn clauses from conflicts to prune future search
- **Two-watched-literals** scheme — efficient unit propagation
- **VSIDS** branching heuristic — prioritise variables active in recent conflicts
- **Restarts** and benchmarking against SATLIB instances

---

## Background

SAT was the first problem proven NP-complete (Cook–Levin, 1971). Despite this, modern SAT solvers routinely handle industrial instances with millions of variables. They underpin work in hardware verification, software model checking, automated planning, and formal methods.

---

## Tech

C · STL · Make · DIMACS CNF

## Author

**Antriksh Chaudhary**

