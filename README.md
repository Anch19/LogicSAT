# LogicSAT

A SAT Solver implementation for solving Boolean Satisfiability (SAT) problems using classical SAT-solving techniques.

This project parses Boolean formulas in CNF (Conjunctive Normal Form) and determines whether the formula is satisfiable or unsatisfiable. The solver is designed as an educational and algorithmic implementation of core SAT-solving concepts used in constraint solving, verification systems, AI, and optimization problems.

---

## Features

- DIMACS CNF file parsing
- Boolean satisfiability checking
- Efficient clause evaluation
- Recursive backtracking search
- Unit propagation
- Pure literal elimination
- SAT / UNSAT result generation
- Clean modular implementation

---

## Algorithms Used

Depending on your implementation, keep only the relevant ones:

- DPLL (Davis–Putnam–Logemann–Loveland)
- Backtracking Search
- Unit Propagation
- Pure Literal Elimination
- Clause Learning (if implemented)
- Conflict Analysis (if implemented)

---

## Project Structure

```text
.
├── src/                # Source code
├── examples/           # Example CNF test files
├── tests/              # Test cases
├── Makefile / build    # Build configuration
└── README.md
```

---

## Input Format

The solver accepts formulas in standard DIMACS CNF format.

Example:

```txt
c Example CNF Formula
p cnf 3 2
1 -3 0
2 3 -1 0
```

---

## Build Instructions

### Compile

```bash
make
```

or

```bash
g++ *.cpp -o sat_solver
```

---

## Running the Solver

```bash
./sat_solver input.cnf
```

Example:

```bash
./sat_solver examples/test.cnf
```

---

## Example Output

```txt
SAT

1 = TRUE
2 = FALSE
3 = TRUE
```

or

```txt
UNSAT
```

---

## Applications of SAT Solvers

SAT solvers are widely used in:

- Artificial Intelligence
- Hardware Verification
- Software Testing
- Scheduling Problems
- Constraint Solving
- Automated Planning
- Formal Verification
- Optimization Problems

---

## Learning Objectives

This project was built to explore:

- Computational complexity
- NP-complete problems
- Search algorithms
- Logic and satisfiability
- Efficient recursive solving techniques

---

## Future Improvements

- CDCL optimization
- Watched literals
- Heuristic branching
- Performance benchmarking
- Parallel SAT solving
- GUI visualization

---

## Technologies Used

- C++
- STL
- Makefile
- DIMACS CNF standard

---

## Author

Antriksh Chaudhary

