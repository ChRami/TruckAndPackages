# TruckAndPackages
<h2> Problem Description <h2/>
Loading a truck with packages with weights is a variation of the bin packing problem. Computationally, it is considered to be a combinatorial NP-hard problem. It could be tackled by a Genetic algorithm.

<h2> Objective <h2/>
For a truck and packages of given dimension and value (only for the packages), design GA to fill the truck with packages. The primary aim of the project is to fill the truck with packages, by minimizing the free space.

<h2> Bonus <h2/>

Sometimes, not all the boxes will fit in the truck. The packages can also have some weight/value. Hence, modify the algorithm to fill the truck, by minimizing its free space but maximizing the total number and value of the packages. 

Hint: Look for Pareto frontier optimization and diversifying dominant solutions. 


Fitness Evaluation
Fitness involves 3 measure: <br/>
●	Minimal Free space <br/>
●	Maximum total values in the truck <br/>
●	Maximum number of boxes <br/>

A sample program is provided (fitnessEval.py) which calculates the free space, total values and number of boxes. Use/modify it if needed.

It is a multi-objective problem, hence, make amends as required.
