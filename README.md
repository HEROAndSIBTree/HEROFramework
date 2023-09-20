# HEROFramework
This repository conatins the code for paper "HERO: A Hierarchical Set Partitioning and Join Framework for Speeding up the Set Intersection Over Graphs".

We provide a makefile to compile the executable programms. By typing ```make``` in the ```src``` dictionary, it will generate five executables, that is, ```tc```, ```mc```, ```sl```, ```pairtest``` and ```reorder```. Among these executables, the ```reorder``` is used to get the HBGP order (proposed in the above paper) of graphs, while the others refer to the downstream tasks which are conducted in the experiments section of the above paper.

To conduct the global intersection or local intersection, run:
```
./pairtest -opt
```
where ```-opt``` should be replaced by "global" or "local".

To conduct the triangle counting (tc), maximal clique enumeration (mc) and subgraph listing (sl), just run the corresponding executable. Take ```tc``` as example:
```
./tc -graphfile
```
where  ```-graphfile``` should be replaced by the name of graph datasets (please refer to lines 34-45 in the graph.hpp).

Due to the storage limitation, the dataset is shared through the Google Drive. Please get access from the following linking: https://drive.google.com/open?id=1mhDTHIO-OxhW1jSd18LwloOtX4msSn-F&usp=drive_fs.

We feel sorry that only graphfiles with origin ID are contained in the above link currently, since we are troubled by the poor uploading speed. Our proposed HBGP ordering can be get by running the excutable '''./reorder'''. To get the competitor orderings, please turn to the following github repository (the corresponding paper has already be cited in our submission.): https://github.com/pkumod/GraphSetIntersection.  
