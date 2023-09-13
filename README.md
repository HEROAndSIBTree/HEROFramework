# HEROFramework
This repository conatins the code for paper "HERO: A Hierarchical Set Partitioning and Join Framework for Speeding up the Set Intersection Over Graphs".

We provide a makefile to compile the executable programm. By typing ```make``` in the ```src``` dictionary, it will generate five executables, that is, ```tc```, ```mc```, ```sl```, ```pairtest``` and ```reorder```. Among these executables, the ```reorder``` is used to get the HBGP order (proposed in the above paper) of graphs, while the others refer to the downstream tasks which are conducted in the experiments section of the above paper.

To conduct the global intersection or local intersection, run:
```
pairtest -opt
```
where ```-opt``` refer to "global" or "local".

To conduct the triangle counting (tc), maximal clique enumeration (mc) and subgraph listing (sl), just run the corresponding executable as follows:
```
Task -graphfile
```
where Task is one of tc, mc or sl, and graphfile is the name of graph datasets (please refer to lines 34-45 in the graph.hpp).
