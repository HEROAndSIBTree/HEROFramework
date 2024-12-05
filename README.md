# HEROFramework
This repository conatins the code for paper "HERO: A Hierarchical Set Partitioning and Join Framework for Speeding up the Set Intersection Over Graphs".

We provide a CMakeLists.txt to compile the executable programms. Please do as follows.
```
mkdir -p build && cd build
cmake ..
make -j
```
Five executables will be generated, that is, ```tc```, ```mc```, ```sl```, ```pt``` and ```reorder```. Among these executables, the ```reorder``` is used to get the HBGP order (proposed in the above paper) of graphs, while the others refer to the downstream tasks which are conducted in the experiments section of the above paper.

To conduct the global intersection or local intersection, run:
```
./pt -opt
```
where ```-opt``` should be replaced by "global" or "local".

To conduct the triangle counting (tc) and maximal clique enumeration (mc), just run the corresponding executable. Take ```tc``` as example:
```
./tc -graphfile
```
where  ```-graphfile``` should be replaced by the name of graph datasets (please refer to lines 34-45 in the graph.hpp).

To conduct the subgraph listing (sl), please run the ```sl``` as follows:
```
./sl -graphfile -pattern
```
where  ```-graphfile``` should be replaced by the name of graph datasets (please refer to lines 34-45 in the graph.hpp), and ```-pattern``` should be selected from ```4-cycle```, ```4-dimond``` and ```4-clique```.

We also provide scripts to conduct the above experiments in the folder ```scripts```.  
```
export HERO_ROOT="/path/of/your/HERO/repository"
cd scripts
bash run_pt.sh global
bash run_pt.sh local
bash run_tc.sh
bash run_mc.sh
bash run_sl.sh 4-cycle
bash run_sl.sh 4-dimond
bash run_sl.sh 4-clique
```
All the experiment logs will be output in the created folder ```exp```.

Due to the storage limitation, the dataset is shared through the Google Drive. Please get access from the following linking: https://drive.google.com/open?id=1-08dDoWy-s6qJu38QsbokdnZgzgtUrkM&usp=drive_fs.

We feel sorry that only graphfiles with origin ID are contained in the above link currently, since we are troubled by the poor uploading speed. Our proposed HBGP ordering can be get by running the excutable '''./reorder'''. To get the competitor orderings, please turn to the following github repository (the corresponding paper has already be cited in our submission.): https://github.com/pkumod/GraphSetIntersection.  
