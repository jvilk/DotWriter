DotWriter
=========
DotWriter is a simple C++ API for creating DOT files for use in programs such as GraphViz. Express a graph in terms of nodes, edges, and subgraphs, and then DotWriter can generate a DOT file for you.

Currently, DotWriter is NOT safe for multithreaded code. If there is demand, I could change it so it is...

Features
--------
DotWriter allows you to specify arbitrarily complex graphs that feature:

* Subgraphs
* Clusters
* Nodes
* Edges
* Attributes on all of the above!

It supports most of the attributes listed on the GraphViz webpage:
http://www.graphviz.org/content/attrs

Building / Installing
---------------------
```shell
./autogen.sh
./configure
make
sudo make install
```

Todo
----
There are still a number of things that need to be done.
* Support for layers.
* Making custom types for various complex attributes. Currently I punt on this by making them strings.
* Ensure all output is properly sanitized.
* Allow attributes to be changed / read / etc. Currently, you can set things once.
