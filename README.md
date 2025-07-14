# gurobi-notes
## Notes from Dr. Joel Sokol's course [Introduction to Optimization Through the Lens of Data Science](https://www.gurobi.com/resources/intro-to-optimization-through-the-lens-of-data-science/) in partnership with Gurobi 

Optimization is the mathematical methodology that takes the information output from data science models and uses it to suggest the best decisions and actions to take.
Sometimes optimization by itself answers questions, and sometimes it sits on top of data science questions.
Gurobi is not a good software for building models; instead, we automate the model-building process through Python/C++/etc., and then solve the model using Gurobi.
This course is about building optimization models.

The mathematical optimization model typically doesn't change with new data since it's typically modeling a decision process, and is not a data-fitting process.
We need to separate data from the model.

## The Components of an Optimization Model
- **Variables**: Decisions to be made
- **Constraints**: Restrictions and trade-offs
- **Objective**: Solution quality

  Terminology:
- Solution: a value for each variable.
- Feasible Solution: a solution satisfying all constraints.
- Optimal Solution: a feasible Solution with the best objective value

## Classic Optimization Problems
- (Stigler's) [diet problem](https://en.wikipedia.org/wiki/Stigler_diet)
- Call center scheduling: meet forecasted demand for each day, each worker should get 2 days off after 5 days of working.
  - Be creative with variables. We can model it using $X_i$: the number of workers who start working on day $i$
- Using different setups of data, we can easily analyze sensitivity and what-if scenarios.

## Types of Optimization Models based on Objective Functions
Real-valued Variables:
- Linear program -- Fast, even for large instances. e.g. > 10e6 instances is doable in Gurobi.
- **Convex** Quadratic program -- Fast
- **Non-convex** Quadratic program -- takes a looot longer
- **Convex** Quadratically-constrained program -- Fast
- **Non-Convex** Quadratically-constrained program -- takes a lot longer

Not-necessarily-real-valued variables (having integer variables), also called _integer programs_:
- Mixed-integer Linear program
- Pure-integer Linear program
- Binary-integer Linear program
 
Gurobi is good at solving all these types of integer Linear programs. 
When the optimal solution is unknown, Gurobi can give us a _very good_ solution in a _reasonable_ amount of time, and guarantee the solution's quality. For example: _No worse that 2% worse than the optimal, even though the optimal solution is unknown..._.
Even very small non-convex optimization problems can be hard to solve. 
## More Gurobi
### Variable Types in Gurobi
- Continuous
- Integer
- Binary
### Expressions
We can use expressions built up from variables, squared variables, or bilinear variables added or multiplied by constant values.
We can also use 
- division,
- three variables multiplied,
- variable to a power other than zero or one (possibly not even an integer, e.g., $x^{1.5}$),
- Trigonometric functions of a variable.
These could be modeled using the expressions above and hence are acceptable.
But would be better avoided, since they slow down the solution time.
### Constraint Type
What can be used:
- (non-strict) inequality, e.g, <=
- equality

What cannot be used:
- strict inequality, e.g, <
- inequality
### Infeasibility Error
- Actual infeasibility
- Incorrect Model 
## Gurobi Assumptions
Unless indicated otherwise, Gurobi assumes
- nonnegativity of variables, and
- continuity of variables.
  
## Tips and Tricks
- Make sure to actually include the constraints that link the variables together.
  - If you get answers that don't make sense, you could be missing such constraints.
- When modeling your real-world problem into an optimization model, check your constraints to see if both sides of your inequality (or equality) have the same unit.
  - This could be an easy way to find bugs in your modeling.
- Recursive-looking constraints are okay!
- You might need to try to **discover** the accurate specification of the problem (possibly from different sources).
   - Building other models might be needed in order to get the accurate specification, i.e., the input for the optimization model.
## Best Practices
### Variable Substitution
- Are models with fewer variables better?
  - Old answer: yes, fewer variables, better speed
  - New answer: no! Substitutions are done internally in Gurobi. Keep your model explainable, and have as many variables as you want! 
- When you do substitute, be mindful of new nonnegativity constraints: they could have been hidden before.
- When dealing with quadratics of sums, it helps to create auxiliary variables.
- Expressions built up from the following are better avoided, since they slow down the solution time:
  - division of variables,
  - three variables multiplied,
  - variable to a power other than zero or one (possibly not even an integer, e.g., $x^{1.5}$), and
  - Trigonometric functions of a variable.
### Use data files, not hard-coded data.
OBVIOUSLY! re-usability reasons.

## How do optimization algorithms work anyway?
Main Steps:
1. Initialization
2. Iteration: Find an improving direction, and move towards it by a step.
3. Stop when:
  - the solution doesn't change much
  - time runs out
  - the solution is optimal or close enough to the optimal one

This gives us a guarantee to get the optimal solution if the problem is convex.

## More Resources
[Gurobi C++ API Documentations](https://docs.gurobi.com/projects/optimizer/en/current/reference/cpp.html)
