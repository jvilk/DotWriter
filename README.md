DotWriter
=========
DotWriter is a simple C++ API for creating DOT files for use in programs such as GraphViz. Express a graph in terms of nodes, edges, and subgraphs, and then DotWriter can generate a DOT file for you.

Currently, DotWriter is NOT safe for multithreaded code. If there is demand, I could change it so it is...

Status
------
DotWriter does not work yet. I am still hashing out the API and features. :)

DOT Attribute Coverage
----------------------
(Table adapted from http://www.graphviz.org/content/attrs)
<table>
<tr><th>Name</th><th>Used By</th><th>Type</th><th>Default</th><th>Minimum</th><th>Notes</th></tr>
 <tr><td>Damping
</td><td>G</td><td>double</td><TD ALIGN="CENTER">0.99</td><td>0.0</td><td>neato only</td> </tr>
 <tr><td>K
</td><td>GC</td><td>double</td><TD ALIGN="CENTER">0.3</td><td>0</td><td>sfdp, fdp only</td> </tr>
 <tr><td>URL
</td><td>ENGC</td><td>escString
</td><TD ALIGN="CENTER">&#60;none&#62;</td><td></td><td>svg, postscript, map only</td> </tr>
 <tr><td>area
</td><td>NC</td><td>double</td><TD ALIGN="CENTER">1.0</td><td>&#62;0</td><td>patchwork only</td> </tr>
 <tr><td>arrowhead
</td><td>E</td><td>arrowType
</td><TD ALIGN="CENTER">normal</td><td></td><td></td> </tr>
 <tr><td>arrowsize
</td><td>E</td><td>double</td><TD ALIGN="CENTER">1.0</td><td>0.0</td><td></td> </tr>
 <tr><td>arrowtail
</td><td>E</td><td>arrowType
</td><TD ALIGN="CENTER">normal</td><td></td><td></td> </tr>
 <tr><td>aspect
</td><td>G</td><td>aspectType
</td><TD ALIGN="CENTER"></td><td></td><td>dot only</td> </tr>
 <tr><td>bb
</td><td>G</td><td>rect
</td><TD ALIGN="CENTER"></td><td></td><td>write only</td> </tr>
 <tr><td>bgcolor
</td><td>GC</td><td>color
<BR />colorList
</td><TD ALIGN="CENTER">&#60;none&#62;</td><td></td><td></td> </tr>
 <tr><td>center
</td><td>G</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td></td> </tr>
 <tr><td>charset
</td><td>G</td><td>string</td><TD ALIGN="CENTER">"UTF-8"</td><td></td><td></td> </tr>
 <tr><td>clusterrank
</td><td>G</td><td>clusterMode
</td><TD ALIGN="CENTER">local</td><td></td><td>dot only</td> </tr>
 <tr><td>color
</td><td>ENC</td><td>color
<BR />colorList
</td><TD ALIGN="CENTER">black</td><td></td><td></td> </tr>
 <tr><td>colorscheme
</td><td>ENCG</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>comment
</td><td>ENG</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>compound
</td><td>G</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td>dot only</td> </tr>
 <tr><td>concentrate
</td><td>G</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td></td> </tr>
 <tr><td>constraint
</td><td>E</td><td>bool
</td><TD ALIGN="CENTER">true</td><td></td><td>dot only</td> </tr>
 <tr><td>decorate
</td><td>E</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td></td> </tr>
 <tr><td>defaultdist
</td><td>G</td><td>double</td><TD ALIGN="CENTER">1+(avg. len)*sqrt(|V|)</td><td>epsilon</td><td>neato only</td> </tr>
 <tr><td>dim
</td><td>G</td><td>int</td><TD ALIGN="CENTER">2</td><td>2</td><td>sfdp, fdp, neato only</td> </tr>
 <tr><td>dimen
</td><td>G</td><td>int</td><TD ALIGN="CENTER">2</td><td>2</td><td>sfdp, fdp, neato only</td> </tr>
 <tr><td>dir
</td><td>E</td><td>dirType
</td><TD ALIGN="CENTER">forward(directed)<BR />none(undirected)</td><td></td><td></td> </tr>
 <tr><td>diredgeconstraints
</td><td>G</td><td>string<BR />bool
</td><TD ALIGN="CENTER">false</td><td></td><td>neato only</td> </tr>
 <tr><td>distortion
</td><td>N</td><td>double</td><TD ALIGN="CENTER">0.0</td><td>-100.0</td><td></td> </tr>
 <tr><td>dpi
</td><td>G</td><td>double</td><TD ALIGN="CENTER">96.0<BR />0.0</td><td></td><td>svg, bitmap output only</td> </tr>
 <tr><td>edgeURL
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, map only</td> </tr>
 <tr><td>edgehref
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, map only</td> </tr>
 <tr><td>edgetarget
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">&#60;none&#62;</td><td></td><td>svg, map only</td> </tr>
 <tr><td>edgetooltip
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, cmap only</td> </tr>
 <tr><td>epsilon
</td><td>G</td><td>double</td><TD ALIGN="CENTER">.0001 * # nodes(mode == KK)<BR />.0001(mode == major)</td><td></td><td>neato only</td> </tr>
 <tr><td>esep
</td><td>G</td><td>addDouble
<BR />addPoint
</td><TD ALIGN="CENTER">+3</td><td></td><td>not dot</td> </tr>
 <tr><td>fillcolor
</td><td>NEC</td><td>color
<BR />colorList
</td><TD ALIGN="CENTER">lightgrey(nodes)<BR />black(clusters)</td><td></td><td></td> </tr>
 <tr><td>fixedsize
</td><td>N</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td></td> </tr>
 <tr><td>fontcolor
</td><td>ENGC</td><td>color
</td><TD ALIGN="CENTER">black</td><td></td><td></td> </tr>
 <tr><td>fontname
</td><td>ENGC</td><td>string</td><TD ALIGN="CENTER">"Times-Roman"</td><td></td><td></td> </tr>
 <tr><td>fontnames
</td><td>G</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td>svg only</td> </tr>
 <tr><td>fontpath
</td><td>G</td><td>string</td><TD ALIGN="CENTER">system-dependent</td><td></td><td></td> </tr>
 <tr><td>fontsize
</td><td>ENGC</td><td>double</td><TD ALIGN="CENTER">14.0</td><td>1.0</td><td></td> </tr>
 <tr><td>forcelabels
</td><td>G</td><td>bool
</td><TD ALIGN="CENTER">true</td><td></td><td></td> </tr>
 <tr><td>gradientangle
</td><td>NCG</td><td>int</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>group
</td><td>N</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td>dot only</td> </tr>
 <tr><td>headURL
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, map only</td> </tr>
 <tr><td>headclip
</td><td>E</td><td>bool
</td><TD ALIGN="CENTER">true</td><td></td><td></td> </tr>
 <tr><td>headhref
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, map only</td> </tr>
 <tr><td>headlabel
</td><td>E</td><td>lblString
</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>headport
</td><td>E</td><td>portPos
</td><TD ALIGN="CENTER">center</td><td></td><td></td> </tr>
 <tr><td>headtarget
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">&#60;none&#62;</td><td></td><td>svg, map only</td> </tr>
 <tr><td>headtooltip
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, cmap only</td> </tr>
 <tr><td>height
</td><td>N</td><td>double</td><TD ALIGN="CENTER">0.5</td><td>0.02</td><td></td> </tr>
 <tr><td>href
</td><td>GCNE</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, postscript, map only</td> </tr>
 <tr><td>id
</td><td>GNE</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, postscript, map only</td> </tr>
 <tr><td>image
</td><td>N</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>imagepath
</td><td>G</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>imagescale
</td><td>N</td><td>bool
<BR />string</td><TD ALIGN="CENTER">false</td><td></td><td></td> </tr>
 <tr><td>label
</td><td>ENGC</td><td>lblString
</td><TD ALIGN="CENTER">"'N" (nodes)<BR />"" (otherwise)</td><td></td><td></td> </tr>
 <tr><td>labelURL
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, map only</td> </tr>
 <tr><td>label_scheme
</td><td>G</td><td>int</td><TD ALIGN="CENTER">0</td><td>0</td><td>sfdp only</td> </tr>
 <tr><td>labelangle
</td><td>E</td><td>double</td><TD ALIGN="CENTER">-25.0</td><td>-180.0</td><td></td> </tr>
 <tr><td>labeldistance
</td><td>E</td><td>double</td><TD ALIGN="CENTER">1.0</td><td>0.0</td><td></td> </tr>
 <tr><td>labelfloat
</td><td>E</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td></td> </tr>
 <tr><td>labelfontcolor
</td><td>E</td><td>color
</td><TD ALIGN="CENTER">black</td><td></td><td></td> </tr>
 <tr><td>labelfontname
</td><td>E</td><td>string</td><TD ALIGN="CENTER">"Times-Roman"</td><td></td><td></td> </tr>
 <tr><td>labelfontsize
</td><td>E</td><td>double</td><TD ALIGN="CENTER">14.0</td><td>1.0</td><td></td> </tr>
 <tr><td>labelhref
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, map only</td> </tr>
 <tr><td>labeljust
</td><td>GC</td><td>string</td><TD ALIGN="CENTER">"c"</td><td></td><td></td> </tr>
 <tr><td>labelloc
</td><td>NGC</td><td>string</td><TD ALIGN="CENTER">"t"(clusters)<BR />"b"(root graphs)<BR />"c"(nodes)</td><td></td><td></td> </tr>
 <tr><td>labeltarget
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">&#60;none&#62;</td><td></td><td>svg, map only</td> </tr>
 <tr><td>labeltooltip
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, cmap only</td> </tr>
 <tr><td>landscape
</td><td>G</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td></td> </tr>
 <tr><td>layer
</td><td>ENC</td><td>layerRange
</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>layers
</td><td>G</td><td>layerList
</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>layerselect
</td><td>G</td><td>layerRange
</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>layersep
</td><td>G</td><td>string</td><TD ALIGN="CENTER">" :'t"</td><td></td><td></td> </tr>
 <tr><td>layout
</td><td>G</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>len
</td><td>E</td><td>double</td><TD ALIGN="CENTER">1.0(neato)<BR />0.3(fdp)</td><td></td><td>fdp, neato only</td> </tr>
 <tr><td>levels
</td><td>G</td><td>int</td><TD ALIGN="CENTER">MAXINT</td><td>0.0</td><td>sfdp only</td> </tr>
 <tr><td>levelsgap
</td><td>G</td><td>double</td><TD ALIGN="CENTER">0.0</td><td></td><td>neato only</td> </tr>
 <tr><td>lhead
</td><td>E</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td>dot only</td> </tr>
 <tr><td>lheight
</td><td>GC</td><td>double</td><TD ALIGN="CENTER"></td><td></td><td>write only</td> </tr>
 <tr><td>lp
</td><td>EGC</td><td>point
</td><TD ALIGN="CENTER"></td><td></td><td>write only</td> </tr>
 <tr><td>ltail
</td><td>E</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td>dot only</td> </tr>
 <tr><td>lwidth
</td><td>GC</td><td>double</td><TD ALIGN="CENTER"></td><td></td><td>write only</td> </tr>
 <tr><td>margin
</td><td>NCG</td><td>double<BR />point
</td><TD ALIGN="CENTER">&#60;device-dependent&#62;</td><td></td><td></td> </tr>
 <tr><td>maxiter
</td><td>G</td><td>int</td><TD ALIGN="CENTER">100 * # nodes(mode == KK)<BR />200(mode == major)<BR />600(fdp)</td><td></td><td>fdp, neato only</td> </tr>
 <tr><td>mclimit
</td><td>G</td><td>double</td><TD ALIGN="CENTER">1.0</td><td></td><td>dot only</td> </tr>
 <tr><td>mindist
</td><td>G</td><td>double</td><TD ALIGN="CENTER">1.0</td><td>0.0</td><td>circo only</td> </tr>
 <tr><td>minlen
</td><td>E</td><td>int</td><TD ALIGN="CENTER">1</td><td>0</td><td>dot only</td> </tr>
 <tr><td>mode
</td><td>G</td><td>string</td><TD ALIGN="CENTER">major</td><td></td><td>neato only</td> </tr>
 <tr><td>model
</td><td>G</td><td>string</td><TD ALIGN="CENTER">shortpath</td><td></td><td>neato only</td> </tr>
 <tr><td>mosek
</td><td>G</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td>neato only</td> </tr>
 <tr><td>nodesep
</td><td>G</td><td>double</td><TD ALIGN="CENTER">0.25</td><td>0.02</td><td></td> </tr>
 <tr><td>nojustify
</td><td>GCNE</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td></td> </tr>
 <tr><td>normalize
</td><td>G</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td>not dot</td> </tr>
 <tr><td>nslimit
<BR />nslimit1
</td><td>G</td><td>double</td><TD ALIGN="CENTER"></td><td></td><td>dot only</td> </tr>
 <tr><td>ordering
</td><td>GN</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td>dot only</td> </tr>
 <tr><td>orientation
</td><td>N</td><td>double</td><TD ALIGN="CENTER">0.0</td><td>360.0</td><td></td> </tr>
 <tr><td>orientation
</td><td>G</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>outputorder
</td><td>G</td><td>outputMode
</td><TD ALIGN="CENTER">breadthfirst</td><td></td><td></td> </tr>
 <tr><td>overlap
</td><td>G</td><td>string<BR />bool
</td><TD ALIGN="CENTER">true</td><td></td><td>not dot</td> </tr>
 <tr><td>overlap_scaling
</td><td>G</td><td>double</td><TD ALIGN="CENTER">-4</td><td>-1.0e10</td><td>prism only</td> </tr>
 <tr><td>pack
</td><td>G</td><td>bool
<BR />int</td><TD ALIGN="CENTER">false</td><td></td><td>not dot</td> </tr>
 <tr><td>packmode
</td><td>G</td><td>packMode
</td><TD ALIGN="CENTER">node</td><td></td><td>not dot</td> </tr>
 <tr><td>pad
</td><td>G</td><td>double<BR />point
</td><TD ALIGN="CENTER">0.0555 (4 points)</td><td></td><td></td> </tr>
 <tr><td>page
</td><td>G</td><td>double<BR />point
</td><TD ALIGN="CENTER"></td><td></td><td></td> </tr>
 <tr><td>pagedir
</td><td>G</td><td>pagedir
</td><TD ALIGN="CENTER">BL</td><td></td><td></td> </tr>
 <tr><td>pencolor
</td><td>C</td><td>color
</td><TD ALIGN="CENTER">black</td><td></td><td></td> </tr>
 <tr><td>penwidth
</td><td>CNE</td><td>double</td><TD ALIGN="CENTER">1.0</td><td>0.0</td><td></td> </tr>
 <tr><td>peripheries
</td><td>NC</td><td>int</td><TD ALIGN="CENTER">shape default(nodes)<BR />1(clusters)</td><td>0</td><td></td> </tr>
 <tr><td>pin
</td><td>N</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td>fdp, neato only</td> </tr>
 <tr><td>pos
</td><td>EN</td><td>point
<BR />splineType
</td><TD ALIGN="CENTER"></td><td></td><td></td> </tr>
 <tr><td>quadtree
</td><td>G</td><td>quadType
<BR />bool
</td><TD ALIGN="CENTER">normal</td><td></td><td>sfdp only</td> </tr>
 <tr><td>quantum
</td><td>G</td><td>double</td><TD ALIGN="CENTER">0.0</td><td>0.0</td><td></td> </tr>
 <tr><td>rank
</td><td>S</td><td>rankType
</td><TD ALIGN="CENTER"></td><td></td><td>dot only</td> </tr>
 <tr><td>rankdir
</td><td>G</td><td>rankdir
</td><TD ALIGN="CENTER">TB</td><td></td><td>dot only</td> </tr>
 <tr><td>ranksep
</td><td>G</td><td>double<BR />doubleList
</td><TD ALIGN="CENTER">0.5(dot)<BR />1.0(twopi)</td><td>0.02</td><td>twopi, dot only</td> </tr>
 <tr><td>ratio
</td><td>G</td><td>double<BR />string</td><TD ALIGN="CENTER"></td><td></td><td></td> </tr>
 <tr><td>rects
</td><td>N</td><td>rect
</td><TD ALIGN="CENTER"></td><td></td><td>write only</td> </tr>
 <tr><td>regular
</td><td>N</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td></td> </tr>
 <tr><td>remincross
</td><td>G</td><td>bool
</td><TD ALIGN="CENTER">false</td><td></td><td>dot only</td> </tr>
 <tr><td>repulsiveforce
</td><td>G</td><td>double</td><TD ALIGN="CENTER">1.0</td><td>0.0</td><td>sfdp only</td> </tr>
 <tr><td>resolution
</td><td>G</td><td>double</td><TD ALIGN="CENTER">96.0<BR />0.0</td><td></td><td>svg, bitmap output only</td> </tr>
 <tr><td>root
</td><td>GN</td><td>string<BR />bool
</td><TD ALIGN="CENTER">&#60;none&#62;(graphs)<BR />false(nodes)</td><td></td><td>circo, twopi only</td> </tr>
 <tr><td>rotate
</td><td>G</td><td>int</td><TD ALIGN="CENTER">0</td><td></td><td></td> </tr>
 <tr><td>rotation
</td><td>G</td><td>double</td><TD ALIGN="CENTER">0</td><td></td><td>sfdp only</td> </tr>
 <tr><td>samehead
</td><td>E</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td>dot only</td> </tr>
 <tr><td>sametail
</td><td>E</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td>dot only</td> </tr>
 <tr><td>samplepoints
</td><td>N</td><td>int</td><TD ALIGN="CENTER">8(output)<BR />20(overlap and image maps)</td><td></td><td></td> </tr>
 <tr><td>scale
</td><td>G</td><td>double<BR />point
</td><TD ALIGN="CENTER"></td><td></td><td>twopi only</td> </tr>
 <tr><td>searchsize
</td><td>G</td><td>int</td><TD ALIGN="CENTER">30</td><td></td><td>dot only</td> </tr>
 <tr><td>sep
</td><td>G</td><td>addDouble
<BR />addPoint
</td><TD ALIGN="CENTER">+4</td><td></td><td>not dot</td> </tr>
 <tr><td>shape
</td><td>N</td><td>shape
</td><TD ALIGN="CENTER">ellipse</td><td></td><td></td> </tr>
 <tr><td>showboxes
</td><td>ENG</td><td>int</td><TD ALIGN="CENTER">0</td><td>0</td><td>dot only</td> </tr>
 <tr><td>sides
</td><td>N</td><td>int</td><TD ALIGN="CENTER">4</td><td>0</td><td></td> </tr>
 <tr><td>size
</td><td>G</td><td>double<BR />point
</td><TD ALIGN="CENTER"></td><td></td><td></td> </tr>
 <tr><td>skew
</td><td>N</td><td>double</td><TD ALIGN="CENTER">0.0</td><td>-100.0</td><td></td> </tr>
 <tr><td>smoothing
</td><td>G</td><td>smoothType
</td><TD ALIGN="CENTER">"none"</td><td></td><td>sfdp only</td> </tr>
 <tr><td>sortv
</td><td>GCN</td><td>int</td><TD ALIGN="CENTER">0</td><td>0</td><td></td> </tr>
 <tr><td>splines
</td><td>G</td><td>bool
<BR />string</td><TD ALIGN="CENTER"></td><td></td><td></td> </tr>
 <tr><td>start
</td><td>G</td><td>startType
</td><TD ALIGN="CENTER">""</td><td></td><td>fdp, neato only</td> </tr>
 <tr><td>style
</td><td>ENCG</td><td>style
</td><TD ALIGN="CENTER"></td><td></td><td></td> </tr>
 <tr><td>stylesheet
</td><td>G</td><td>string</td><TD ALIGN="CENTER">""</td><td></td><td>svg only</td> </tr>
 <tr><td>tailURL
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, map only</td> </tr>
 <tr><td>tailclip
</td><td>E</td><td>bool
</td><TD ALIGN="CENTER">true</td><td></td><td></td> </tr>
 <tr><td>tailhref
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, map only</td> </tr>
 <tr><td>taillabel
</td><td>E</td><td>lblString
</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>tailport
</td><td>E</td><td>portPos
</td><TD ALIGN="CENTER">center</td><td></td><td></td> </tr>
 <tr><td>tailtarget
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">&#60;none&#62;</td><td></td><td>svg, map only</td> </tr>
 <tr><td>tailtooltip
</td><td>E</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, cmap only</td> </tr>
 <tr><td>target
</td><td>ENGC</td><td>escString
<BR />string</td><TD ALIGN="CENTER">&#60;none&#62;</td><td></td><td>svg, map only</td> </tr>
 <tr><td>tooltip
</td><td>NEC</td><td>escString
</td><TD ALIGN="CENTER">""</td><td></td><td>svg, cmap only</td> </tr>
 <tr><td>truecolor
</td><td>G</td><td>bool
</td><TD ALIGN="CENTER"></td><td></td><td>bitmap output only</td> </tr>
 <tr><td>vertices
</td><td>N</td><td>pointList
</td><TD ALIGN="CENTER"></td><td></td><td>write only</td> </tr>
 <tr><td>viewport
</td><td>G</td><td>viewPort
</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
 <tr><td>voro_margin
</td><td>G</td><td>double</td><TD ALIGN="CENTER">0.05</td><td>0.0</td><td>not dot</td> </tr>
 <tr><td>weight
</td><td>E</td><td>int<BR />double</td><TD ALIGN="CENTER">1</td><td>0(dot)<BR />1(neato,fdp)</td><td></td> </tr>
 <tr><td>width
</td><td>N</td><td>double</td><TD ALIGN="CENTER">0.75</td><td>0.01</td><td></td> </tr>
 <tr><td>xlabel
</td><td>EN</td><td>lblString
</td><TD ALIGN="CENTER">""</td><td></td><td></td> </tr>
</table>

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