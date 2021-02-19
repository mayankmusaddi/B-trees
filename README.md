# B-trees

## Introduction
A B+ tree is an m-ary tree with a variable but often large number of children per node. A B+ tree consists of a root, internal nodes and leaves.The root may be either a leaf or a node with two or more children.

A B+ tree can be viewed as a B-tree in which each node contains only keys (not key–value pairs), and to which an additional level is added at the bottom with linked leaves.

The primary value of a B+ tree is in storing data for efficient retrieval in a block-oriented storage context — in particular, filesystems. This is primarily because unlike binary search trees, B+ trees have very high fanout (number of pointers to child nodes in a node, typically on the order of 100 or more), which reduces the number of I/O operations required to find an element in the tree. 

## Implementation Details
This is a simple implementation of the B+ Trees with the following functionalities:
 - INSERT X - insert X into the B+ tree
 - FIND X - print YES if X is already inserted, else NO
 - COUNT X - print number of occurrences of X in B+tree
 - RANGE X Y - print number of elements in range x to y (both x and y included)

The order of the tree is taken by default to be 3.

### Execution Instructions
To Execute use the following commands:
```
g++ main.cpp
./a.out <input_file>
```