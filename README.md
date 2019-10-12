# Graph Mapping
There are two directed graphs G and G’. The graphs do not have self-edges. We find a
one-one mapping M from nodes in G to nodes in G’ such that there is an edge from v1 to v2 in G if and only if there is an edge from M(v1) to M(v2) in G'. 

## Usage

To generate a larger graph with nodes n1, edges e2 and smaller graph with nodes n2, edges e2 

```bash
python generate.py n1 e1 n2 e2
```

Similar to generate.py but generates only non_isolated nodes

```bash
python generate_non_isolated.py n1 e1 n2 e2
```
Execute the program:
```bash
./compile.sh #For compiling the program
./run1.sh [testname] #for generating [testname].satinput, the input to minisat
./minisat [testname].satinput [testname].satoutput #Generates [testname].satoutput
./run2.sh [testname] #Generates [testname].mapping, the mapping for the problem

```
To check whether the mapping is true or false:
```bash
python check.py [testname].graphs [testname].mapping
```

## Input Format

Nodes are represented by positive integers starting from 1. Each line represents an edge from the first node to the second. Both graphs are presented in the single file, the larger first. The line with "0 0" is the boundary between the two.

1 2

1 3

1 4

2 4

3 4

0 0

1 2

3 2




## Output Format

The mapping will map each node of G into a node id for G’. The first numbers on each line represent a node as numbered in the smaller graph G, and the second number represents the node of the larger graph G’ to which it is mapped. The output of the same problem is

1 2

2 4

3 3

If the problem is unsatisfiable output a 0.







## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update the tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
