# BSP-Pattern
Bulk Synchronous Parallel (BSP) - parallel pattern implementation

## USAGE
### Define body thread for computation and communication phase
```c++
ComputationFunction comp_body = [catch_args](std::vector<T> input)
{ 
	//do computation with input
}

CommunicationFunction comm_body = [catch_args](std::vector<T> output, int from, int to)
{ 
	//do communication of output using from and to index
}

```

### Define and init Communication Protocol 
#### (std::vector<std::pair<int,std::vector<int>>>)
```c++
CommunicationProtocol one_to_one;
for(int i=0; i<nw; i++)
{
	std::vector<int> d{i};
	one_to_one.push_back(std::make_pair(i,d));
}
```

### Main function
```c++
SuperStep<int> s1(parallel_degree,data,true);
s1.reset_barrier();
s1.computation(sort_and_separators);
s1.reset_barrier();
s1.communication(void_comm,to_itself);
output=s1.get_results(output);

```
### Compilation and use
Compile:
```
g++ src/*.cpp -lpthread -o main
```

Run:
```
./main n(int) par_degree(int) verbose(0 or 1, optional with 0 default)
```

### Use test.sh script
From BSP main folder:
```
test/test.sh
```
Save results in data/results.csv.
With --verbose flag, print results on standard output without write on csv.
