DotWriter
=========
DotWriter is a simple C++ API for creating DOT files for use in programs such as GraphViz. Express a graph in terms of nodes, edges, and subgraphs, and then DotWriter can generate a DOT file for you.

Currently, DotWriter is NOT safe for multithreaded code. If there is demand, I could change it so it is...

Status
------
DotWriter does not work yet. I am still hashing out the API and features. :)

Supported DOT Language Features
-------------------------------

### Graphs
    graph   :   [ strict ] (graph | digraph) [ ID ] '{' stmt_list '}'

* Graph
* Digraph
* Customizable graph IDs.
    * Unquoted and HTML IDs are not supported.

### Statements

    stmt_list   :   [ stmt [ ';' ] [ stmt_list ] ]
    stmt    :   node_stmt
        |   edge_stmt
        |   attr_stmt
        |   ID '=' ID
        |   subgraph


#### Attribute Statements
    attr_stmt   :   (graph | node | edge) attr_list
    attr_list   :   '[' [ a_list ] ']' [ attr_list ]
    a_list  :   ID [ '=' ID ] [ ',' ] [ a_list ]

#### Edge Statement
    edge_stmt   :   (node_id | subgraph) edgeRHS [ attr_list ]
    edgeRHS     :   edgeop (node_id | subgraph) [ edgeRHS ]

#### Node Statements
    node_stmt   :   node_id [ attr_list ]
    node_id     :   ID [ port ]

##### Ports
    port    :   ':' ID [ ':' compass_pt ]
        |   ':' compass_pt
    compass_pt  :   (n | ne | e | se | s | sw | w | nw | c | _)

### Subgraphs
    subgraph    :   [ subgraph [ ID ] ] '{' stmt_list '}'