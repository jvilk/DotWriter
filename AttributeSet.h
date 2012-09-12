/**
 * Stores a set of DOT attributes for various types of objects.
 * Handles validating input attributes to ensure they are appropriate
 * for the given object.
 *
 * Attribute descriptions lifted from:
 * http://www.graphviz.org/content/attrs
 *
 * TODO(jvilk): Make errors explicit at runtime rather than silently fix them.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_ATTRIBUTESET_H_
#define DOTWRITER_ATTRIBUTESET_H_

#include <algorithm>
#include <stdexcept>
#include <vector>

#include "Attribute.h"
#include "Enums.h"
#include "Util.h"

using std::runtime_error;

namespace DotWriter {

class AttributeSet {
private:
  std::vector<Attribute*> _attributes;

public:
  AttributeSet() {
  }

  virtual ~AttributeSet() {
    std::vector<Attribute*>::iterator it;
    for (it = _attributes.begin(); it != _attributes.end(); it++) {
      delete *it;
    }
  }

  // TODO: If they add one, how do I expose removing it?
  // Not to mention escaping input...
  void AddCustomAttribute(std::string name, std::string val);

protected:
  void AddAttribute(Attribute* attribute) {
    _attributes.push_back(attribute);
  }

  void RemoveAttribute(Attribute* attribute) {
    std::vector<Attribute*>::iterator it = 
      std::find(_attributes.begin(), _attributes.end(), attribute);

    if (it != _attributes.end()) {
      _attributes.erase(it);
    }
  }

  template<typename T>
  void AddFirstClassAttribute(AttributeType::e type, T val) {
    Attribute* attr = new FirstClassAttribute<T>(type, val);
    AddAttribute(attr);
  }

  template<typename T, typename F>
  void AddEnumAttribute(AttributeType::e type, T val) {
    Attribute* attr = new EnumAttribute<T, F>(type, val);
    AddAttribute(attr);
  }

  template<typename T, typename F>
  void AddEnumListAttribute(AttributeType::e type, const std::vector<T>& val) {
    Attribute* attr = new EnumListAttribute<T, F>(type, val);
    AddAttribute(attr);
  }

  void AddPointAttribute(AttributeType::e type, double x, double y) {
    Attribute* attr = new PointAttribute(type, x, y);
    AddAttribute(attr);
  }
};

class GraphAttributeSet : AttributeSet {
public:
  GraphAttributeSet() { };

  /**
   * Factor damping force motions. On each iteration, a nodes movement is
   * limited to this factor of its potential motion. By being less than 1.0, the 
   * system tends to ``cool'', thereby preventing cycling. 
   */
  void SetDamping(double val) {
    AddFirstClassAttribute<double>(AttributeType::DAMPING, val);
  }

  /**
   * Spring constant used in virtual physical model. It roughly corresponds to
   * an ideal edge length (in inches), in that increasing K tends to increase
   * the distance between nodes. Note that the edge attribute len can be used to
   * override this value for adjacent nodes. 
   */
  void SetK(double val) {
    AddFirstClassAttribute<double>(AttributeType::K, val);
  }

  /**
   * Hyperlinks incorporated into device-dependent output. At present, used in
   * ps2, cmap, i*map and svg formats. For all these formats, URLs can be
   * attached to nodes, edges and clusters. URL attributes can also be attached
   * to the root graph in ps2, cmap and i*map formats. This serves as the base
   * URL for relative URLs in the former, and as the default image map file in
   * the latter.
   * 
   * For svg, cmapx and imap output, the active area for a node is its visible
   * image. For example, an unfilled node with no drawn boundary will only be
   * active on its label. For other output, the active area is its bounding box.
   * The active area for a cluster is its bounding box. For edges, the active
   * areas are small circles where the edge contacts its head and tail nodes. In
   * addition, for svg, cmapx and imap, the active area includes a thin polygon
   * approximating the edge. The circles may overlap the related node, and the
   * edge URL dominates. If the edge has a label, this will also be active.
   * Finally, if the edge has a head or tail label, this will also be active.
   *
   * Note that, for edges, the attributes headURL, tailURL, labelURL and edgeURL
   * allow control of various parts of an edge. Also note that, if active areas
   * of two edges overlap, it is unspecified which area dominates. 
   */
  void SetURL(std::string val) {
    val = SanitizeString(val);
    AddFirstClassAttribute<std::string>(AttributeType::URL, val);
  }

  /**
   * When attached to the root graph, this color is used as the background for
   * entire canvas. When a cluster attribute, it is used as the initial
   * background for the cluster. If a cluster has a filled style, the cluster's
   * fillcolor will overlay the background color.
   *
   * If the value is a colorList, a gradient fill is used. By default, this is a
   * linear fill; setting style=radial will cause a radial fill. At present,
   * only two colors are used. If the second color (after a colon) is missing,
   * the default color is used for it. See also the gradientangle attribute for
   * setting the gradient angle.
   *
   * If no background color is specified for the root graph, no graphics
   * operation are performed on the background. This works fine for PostScript
   * but for bitmap output, all bits are initialized to something. This means
   * that when the bitmap output is included in some other document, all of the
   * bits within the bitmap's bounding box will be set, overwriting whatever
   * color or graphics where already on the page. If this effect is not desired,
   * and you only want to set bits explicitly assigned in drawing the graph, set
   * bgcolor="transparent". 
   */
  void SetBGColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::BGCOLOR, val);
  }
  void SetBGColor(const std::vector<Color::e> vals) {
    AddEnumListAttribute<Color::e, Color>(AttributeType::BGCOLOR, vals);
  }

  /**
   * If true, the drawing is centered in the output canvas. 
   */
  void ShouldCenter(bool val) {
    AddFirstClassAttribute<bool>(AttributeType::CENTER, val);
  }

  /**
   * Specifies the character encoding used when interpreting string input as a
   * text label. The default value is "UTF-8". The other legal value is
   * "iso-8859-1" or, equivalently, "Latin1".
   */
  void SetCharset(Charset::e val) {
    AddEnumAttribute<Charset::e, Charset>(AttributeType::CHARSET, val);
  }

  /**
   * Mode used for handling clusters. If clusterrank is "local", a subgraph
   * whose name begins with "cluster" is given special treatment. The subgraph
   * is laid out separately, and then integrated as a unit into its parent
   * graph, with a bounding rectangle drawn about it. If the cluster has a label
   * parameter, this label is displayed within the rectangle. Note also that
   * there can be clusters within clusters. At present, the modes "global" and
   * "none" appear to be identical, both turning off the special cluster
   * processing. 
   */
  void SetClusterRank(ClusterMode::e val) {
    AddEnumAttribute<ClusterMode::e, ClusterMode>(AttributeType::CLUSTERRANK,
      val);
  }

  /**
   * Comments are inserted into output. Device-dependent 
   */
  void SetComment(std::string val) {
    AddFirstClassAttribute<std::string>(AttributeType::COMMENT, val);
  }

  /**
   * If true, allow edges between clusters. (See lhead and ltail) 
   */
  void SetCompound(bool val) {
    AddFirstClassAttribute<bool>(AttributeType::COMPOUND, val);
  }

  /**
   * If true, use edge concentrators. This merges multiedges into a single edge
   * and causes partially parallel edges to share part of their paths. The
   * latter feature is not yet available outside of dot. 
   */
  void SetConcentrate(bool val) {
    AddFirstClassAttribute<bool>(AttributeType::CONCENTRATE, val);
  }

  /**
   * This specifies the distance between nodes in separate connected components.
   * If set too small, connected components may overlap. Only applicable if
   * pack=false. 
   */
  void SetDefaultDist(double val) {
    AddFirstClassAttribute<double>(AttributeType::DEFAULTDIST, val);
  }

  /**
   * Set the number of dimensions used for the layout. The maximum value allowed
   * is 10. 
   */
  void SetDim(unsigned val) {
    if (val > 10) val = 10;
    AddFirstClassAttribute<unsigned>(AttributeType::DIM, val);
  }

  /**
   * Set the number of dimensions used for rendering. The maximum value allowed
   * is 10. If both dimen and dim are set, the latter specifies the dimension
   * used for layout, and the former for rendering. If only dimen is set, this
   * is used for both layout and rendering dimensions.
   *
   * Note that, at present, all aspects of rendering are 2D. This includes the
   * shape and size of nodes, overlap removal, and edge routing. Thus, for
   * dimen > 2, the only valid information is the pos attribute of the nodes.
   * All other coordinates will be 2D and, at best, will reflect a projection of
   * a higher-dimensional point onto the plane. 
   */
  void SetDimen(int val) {
    if (val > 10) val = 10;
    AddFirstClassAttribute<unsigned>(AttributeType::DIMEN, val);
  }

  /**
   * Only valid when mode="ipsep". If true, constraints are generated for each
   * edge in the largest (heuristic) directed acyclic subgraph such that the
   * edge must point downwards. If "hier", generates level constraints similar
   * to those used with mode="hier". The main difference is that, in the latter
   * case, only these constraints are involved, so a faster solver can be used. 
   */
  void SetDirEdgeConstraints(DirEdgeConstraints::e val) {
    AddEnumAttribute<DirEdgeConstraints::e, DirEdgeConstraints>(
      AttributeType::DIREDGECONSTRAINTS, val);
  }

  /**
   * This specifies the expected number of pixels per inch on a display device.
   * For bitmap output, this guarantees that text rendering will be done more
   * accurately, both in size and in placement. For SVG output, it is used to
   * guarantee that the dimensions in the output correspond to the correct
   * number of points or inches. 
   */
  void SetDPI(double val) {
    AddFirstClassAttribute<double>(AttributeType::DPI, val);
  }

  /**
   * Terminating condition. If the length squared of all energy gradients are <
   * epsilon, the algorithm stops.
   */
  void SetEpsilon(double val) {
    AddFirstClassAttribute<double>(AttributeType::EPSILON, val);
  }

  /**
   * Margin used around polygons for purposes of spline edge routing. The
   * interpretation is the same as given for sep. This should normally be
   * strictly less than sep. 
   */
  void SetEsep(double val) {
    AddFirstClassAttribute<double>(AttributeType::ESEP, val);
  }

  /**
   * Color used for text.
   */  
  void SetFontColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::FONTCOLOR, val);
  }

  /**
   * Font used for text. This very much depends on the output format and, for
   * non-bitmap output such as PostScript or SVG, the availability of the font
   * when the graph is displayed or printed. As such, it is best to rely on font
   * faces that are generally available, such as Times-Roman, Helvetica or
   * Courier.
   *
   * If Graphviz was built using the fontconfig library, the latter library will
   * be used to search for the font. However, if the fontname string contains a
   * slash character "/", it is treated as a pathname for the font file, though
   * font lookup will append the usual font suffixes.
   *
   * If Graphviz does not use fontconfig, fontname will be considered the name
   * of a Type 1 or True Type font file. If you specify fontname=schlbk, the
   * tool will look for a file named schlbk.ttf or schlbk.pfa or schlbk.pfb in
   * one of the directories specified by the fontpath attribute. The lookup does
   * support various aliases for the common fonts. 
   */
  void SetFontName(std::string val) {
    AddFirstClassAttribute<std::string>(AttributeType::FONTNAME, val);
  }

  /**
   * Allows user control of how basic fontnames are represented in SVG output.
   * If fontnames is undefined or "svg", the output will try to use known SVG
   * fontnames. For example, the default font "Times-Roman" will be mapped to
   * the basic SVG font "serif". This can be overridden by setting fontnames to
   * "ps" or "gd". In the former case, known PostScript font names such as
   * "Times-Roman" will be used in the output. In the latter case, the
   * fontconfig font conventions are used. Thus, "Times-Roman" would be treated
   * as "Nimbus Roman No9 L". These last two options are useful with SVG viewers
   * that support these richer fontname spaces. 
   */
  void SetFontNames(std::string val) {
    AddFirstClassAttribute<std::string>(AttributeType::FONTNAMES, val);
  }

  /**
   * Directory list used by libgd to search for bitmap fonts if Graphviz was not
   * built with the fontconfig library. If fontpath is not set, the environment
   * variable DOTFONTPATH is checked. If that is not set, GDFONTPATH is checked.
   * If not set, libgd uses its compiled-in font path. Note that fontpath is an
   * attribute of the root graph. 
   */
  void SetFontPath(std::string val) {
    AddFirstClassAttribute<std::string>(AttributeType::FONTPATH, val);
  }

  /**
   * Font size, in points, used for text. 
   */
  void SetFontSize(double val) {
    AddFirstClassAttribute<double>(AttributeType::FONTSIZE, val);
  }

  /**
   * If true, all xlabel attributes are placed, even if there is some overlap
   * with nodes or other labels. 
   */
  void SetForceLabels(bool val) {
    AddFirstClassAttribute<bool>(AttributeType::FORCELABELS, val);
  }

  /**
   * If a gradient fill is being used, this determines the angle of the fill.
   * For linear fills, the colors transform along a line specified by the angle
   * and the center of the object. For radial fills, a value of zero causes the
   * colors to transform radially from the center; for non-zero values, the
   * colors transform from a point near the object's periphery as specified by
   * the value.
   *
   * If unset, the default angle is 0. 
   */
  void SetGradientAngle(int val) {
    AddFirstClassAttribute<int>(AttributeType::GRADIENTANGLE, val);
  }

  /**
   * Specifies a list of directories in which to look for image files as
   * specified by the image attribute or using the IMG element in HTML-like
   * labels. The string should be a list of (absolute or relative) pathnames,
   * each separated by a semicolon (for Windows) or a colon (all other OS). The
   * first directory in which a file of the given name is found will be used to
   * load the image. If imagepath is not set, relative pathnames for the image
   * file will be interpreted with respect to the current working directory.
   *
   * TODO(jvilk): Make this a list attribute so the user doesn't have to worry
   * about OS-specifiy malarky. 
   */
  void SetImagePath(std::string val) {
    AddFirstClassAttribute<std::string>(AttributeType::IMAGEPATH, val);
  }

  /**
   * The value indicates whether to treat a node whose name has the form
   * |edgelabel|* as a special node representing an edge label. The default (0)
   * produces no effect. If the attribute is set to 1, sfdp uses a penalty-based
   * method to make that kind of node close to the center of its neighbor. With
   * a value of 2, sfdp uses a penalty-based method to make that kind of node
   * close to the old center of its neighbor. Finally, a value of 3 invokes a
   * two-step process of overlap removal and straightening. 
   */
  void SetLabelScheme(unsigned val) {
    if (val > 3) val = 3;
    AddFirstClassAttribute<unsigned>(AttributeType::LABEL_SCHEME, val);
  }

  /**
   * Justification for cluster labels. If "r", the label is right-justified
   * within bounding rectangle; if "l", left-justified; else the label is
   * centered. Note that a subgraph inherits attributes from its parent. Thus,
   * if the root graph sets labeljust to "l", the subgraph inherits this value. 
   */
  void SetLabelJust(Justification::e val) {
    AddEnumAttribute<Justification::e, Justification>(AttributeType::LABELJUST,
      val);
  }

  /**
   * Vertical placement of labels for nodes, root graphs and clusters.
   *
   * For graphs and clusters, only "t" and "b" are allowed, corresponding to
   * placement at the top and bottom, respectively. By default, root graph
   * labels go on the bottom and cluster labels go on the top. Note that a
   * subgraph inherits attributes from its parent. Thus, if the root graph sets
   * labelloc to "b", the subgraph inherits this value.
   *
   * For nodes, this attribute is used only when the height of the node is
   * larger than the height of its label. If labelloc is set to "t", "c", or
   * "b", the label is aligned with the top, centered, or aligned with the
   * bottom of the node, respectively. In the default case, the label is
   * vertically centered. 
   */
  void SetLabelLoc(LabelLoc::e val) {
    if (val != LabelLoc::C) {
      AddEnumAttribute<LabelLoc::e, LabelLoc>(AttributeType::LABELLOC, val);
    }
  }

  /**
   * Specifies the name of the layout algorithm to use, such as "dot" or
   * "neato". Normally, graphs should be kept independent of a type of layout.
   * In some cases, however, it can be convenient to embed the type of layout
   * desired within the graph. For example, a graph containing position
   * information from a layout might want to record what the associated layout
   * algorithm was.
   *
   * This attribute takes precedence over the -K flag or the actual command name
   * used.
   */
  void SetLayout(std::string val) {
    AddFirstClassAttribute<std::string>(AttributeType::LAYOUT, val);
  }

  /**
   * Number of levels allowed in the multilevel scheme.
   */
  void SetLevels(int val) {
    AddFirstClassAttribute<int>(AttributeType::LEVELS, val);
  }

  /**
   * Specifies strictness of level constraints in neato when mode="ipsep" or
   * "hier". Larger positive values mean stricter constraints, which demand more
   * separation between levels. On the other hand, negative values will relax
   * the constraints by allowing some overlap between the levels.
   */
  void SetLevelsGap(double val) {
    AddFirstClassAttribute<double>(AttributeType::LEVELSGAP, val);
  }

  /**
   * Height of graph or cluster label, in inches.
   */
  void SetLHeight(double val) {
    AddFirstClassAttribute<double>(AttributeType::LHEIGHT, val);
  }

  /**
   * Label position, in points. The position indicates the center of the label.
   */
  void SetLP(double x, double y) {
    AddPointAttribute(AttributeType::LP, x, y);
  }

  /**
   * Width of graph or cluster label, in inches.
   */
  void SetLWidth(double val) {
    AddFirstClassAttribute<double>(AttributeType::LWIDTH, val);
  }

  /**
   * For graphs, this sets x and y margins of canvas, in inches. If the margin
   * is a single double, both margins are set equal to the given value.
   * 
   * Note that the margin is not part of the drawing but just empty space left
   * around the drawing. It basically corresponds to a translation of drawing,
   * as would be necessary to center a drawing on a page. Nothing is actually
   * drawn in the margin. To actually extend the background of a drawing, see
   * the pad attribute.
   *
   * For clusters, this specifies the space between the nodes in the cluster and
   * the cluster bounding box. By default, this is 8 points.
   *
   * For nodes, this attribute specifies space left around the node's label. By
   * default, the value is 0.11,0.055.
   */
  void SetMargin(double val) {
    AddFirstClassAttribute<double>(AttributeType::MARGIN, val);
  }
  void SetMargin(double x, double y) {
    AddPointAttribute(AttributeType::MARGIN, x, y);
  }

  /**
   * Sets the number of iterations used.
   */
  void SetMaxIter(int val) {
    AddFirstClassAttribute<int>(AttributeType::MAXITER, val);
  }

  /**
   * Multiplicative scale factor used to alter the MinQuit (default = 8) and
   * MaxIter (default = 24) parameters used during crossing minimization. These
   * correspond to the number of tries without improvement before quitting and
   * the maximum number of iterations in each pass.
   */
  void SetMCLimit(double val) {
    AddFirstClassAttribute<double>(AttributeType::MCLIMIT, val);
  }

  /**
   * Specifies the minimum separation between all nodes.
   */
  void SetMinDist(double val) {
    AddFirstClassAttribute<double>(AttributeType::MINDIST, val);
  }

  /**
   * Technique for optimizing the layout. For neato, if mode is "major", neato
   * uses stress majorization. If mode is "KK", neato uses a version of the
   * gradient descent method. The only advantage to the latter technique is that
   * it is sometimes appreciably faster for small (number of nodes < 100)
   * graphs. A significant disadvantage is that it may cycle.
   * 
   * There are two experimental modes in neato, "hier", which adds a top-down
   * directionality similar to the layout used in dot, and "ipsep", which allows
   * the graph to specify minimum vertical and horizontal distances between
   * nodes. (See the sep attribute.) "len" attribute.
   */
  void SetMode(Mode::e val) {
    AddEnumAttribute<Mode::e, Mode>(AttributeType::MODE, val);
  }

  /**
   * This value specifies how the distance matrix is computed for the input
   * graph. The distance matrix specifies the ideal distance between every pair
   * of nodes. neato attemps to find a layout which best achieves these
   * distances. By default, it uses the length of the shortest path, where the
   * length of each edge is given by its len attribute. If model is "circuit",
   * neato uses the circuit resistance model to compute the distances. This
   * tends to emphasize clusters. If model is "subset", neato uses the subset
   * model. This sets the edge length to be the number of nodes that are
   * neighbors of exactly one of the end points, and then calculates the
   * shortest paths. This helps to separate nodes with high degree.
   *
   * For more control of distances, one can use model=mds. In this case, the len
   * of an edge is used as the ideal distance between its vertices. A shortest
   * path calculation is only used for pairs of nodes not connected by an edge.
   * Thus, by supplying a complete graph, the input can specify all of the
   * relevant distances.
   */
  void SetModel(Model::e val) {
    AddEnumAttribute<Model::e, Model>(AttributeType::MODEL, val);
  }

  /**
   * If Graphviz is built with MOSEK defined, mode=ipsep and mosek=true, the
   * Mosek software (www.mosek.com) is use to solve the ipsep constraints.
   */
  void SetMosek(bool val) {
    AddFirstClassAttribute<bool>(AttributeType::MOSEK, val);
  }

  /**
   * In dot, this specifies the minimum space between two adjacent nodes in the
   * same rank, in inches.
   *
   * For other layouts, this affects the spacing between loops on a single node,
   * or multiedges between a pair of nodes
   */
  void SetNodeSep(double val) {
    AddFirstClassAttribute<double>(AttributeType::NODESEP, val);
  }

  /**
   * By default, the justification of multi-line labels is done within the
   * largest context that makes sense. Thus, in the label of a polygonal node, a
   * left-justified line will align with the left side of the node (shifted by
   * the prescribed margin). In record nodes, left-justified line will line up
   * with the left side of the enclosing column of fields. If nojustify is
   * "true", multi-line labels will be justified in the context of itself. For
   * example, if the attribute is set, the first label line is long, and the
   * second is shorter and left-justified, the second will align with the
   * left-most character in the first line, regardless of how large the node
   * might be.
   */
  void SetNoJustify(bool val) {
    AddFirstClassAttribute<bool>(AttributeType::NOJUSTIFY, val);
  }

  /**
   * If set, normalize coordinates of final layout so that the first point is at
   * the origin, and then rotate the layout so that the first edge is
   * horizontal.
   */
  void SetNormalize(bool val) {
    AddFirstClassAttribute<bool>(AttributeType::NORMALIZE, val);
  }

  /**
   * Used to set number of iterations in network simplex applications. nslimit
   * is used in computing node x coordinates, nslimit1 for ranking nodes. If
   * defined, # iterations = nslimit(1) * # nodes; otherwise, # iterations =
   * MAXINT.
   */
  void SetNsLimit(double val) {
    AddFirstClassAttribute<double>(AttributeType::NSLIMIT, val);
  }
  void SetNsLimit1(double val) {
    AddFirstClassAttribute<double>(AttributeType::NSLIMIT1, val);
  }

  /**
   * If the value of the attribute is "out", then the outedges of a node, that
   * is, edges with the node as its tail node, must appear left-to-right in the
   * same order in which they are defined in the input. If the value of the
   * attribute is "in", then the inedges of a node must appear left-to-right in
   * the same order in which they are defined in the input. If defined as a
   * graph or subgraph attribute, the value is applied to all nodes in the graph
   * or subgraph. Note that the graph attribute takes precedence over the node
   * attribute.
   */
  void SetOrdering(Ordering::e val) {
    AddEnumAttribute<Ordering::e, Ordering>(AttributeType::ORDERING, val);
  }

  /**
   * If "[lL]*", set graph orientation to landscape Used only if rotate is not
   * defined.
   * TODO(jvilk): Huh.
   */
  void SetOrientation(std::string val);

  /**
   * Specify order in which nodes and edges are drawn.
   * TODO(jvilk): What does the value mean...
   */
  void SetOutputOrder(OutputMode::e val) {
    AddEnumAttribute<OutputMode::e, OutputMode>(AttributeType::OUTPUTORDER,
      val);
  }

  /**
   * Determines if and how node overlaps should be removed. Nodes are first
   * enlarged using the sep attribute. If "true" , overlaps are retained. If the
   * value is "scale", overlaps are removed by uniformly scaling in x and y. If
   * the value converts to "false", and it is available, Prism, a proximity
   * graph-based algorithm, is used to remove node overlaps. This can also be
   * invoked explicitly with "overlap=prism". This technique starts with a small
   * scaling up, controlled by the overlap_scaling attribute, which can remove a
   * significant portion of the overlap. The prism option also accepts an
   * optional non-negative integer suffix. This can be used to control the
   * number of attempts made at overlap removal. By default, overlap="prism" is
   * equivalent to overlap="prism1000". Setting overlap="prism0" causes only the
   * scaling phase to be run.
   *
   * If Prism is not available, or the version of Graphviz is earlier than 2.28,
   * "overlap=false" uses a Voronoi-based technique. This can always be invoked
   * explicitly with "overlap=voronoi".
   *
   * If the value is "scalexy", x and y are separately scaled to remove
   * overlaps.
   *
   * If the value is "compress", the layout will be scaled down as much as
   * possible without introducing any overlaps, obviously assuming there are
   * none to begin with.
   *
   * N.B.The remaining allowed values of overlap correspond to algorithms which,
   * at present, can produce bad aspect ratios. In addition, we deprecate the
   * use of the "ortho*" and "portho*".
   *
   * If the value is "vpsc", overlap removal is done as a quadratic optimization
   * to minimize node displacement while removing node overlaps.
   *
   * If the value is "orthoxy" or "orthoyx", overlaps are moved by optimizing
   * two constraint problems, one for the x axis and one for the y. The suffix
   * indicates which axis is processed first. If the value is "ortho", the
   * technique is similar to "orthoxy" except a heuristic is used to reduce the
   * bias between the two passes. If the value is "ortho_yx", the technique is
   * the same as "ortho", except the roles of x and y are reversed. The values
   * "portho", "porthoxy", "porthoxy", and "portho_yx" are similar to the
   * previous four, except only pseudo-orthogonal ordering is enforced.
   *
   * If the layout is done by neato with mode="ipsep", then one can use
   * overlap=ipsep. In this case, the overlap removal constraints are
   * incorporated into the layout algorithm itself. N.B. At present, this only
   * supports one level of clustering.
   *
   * Except for fdp and sfdp, the layouts assume overlap="true" as the default.
   * Fdp first uses a number of passes using a built-in, force-directed
   * technique to try to remove overlaps. Thus, fdp accepts overlap with an
   * integer prefix followed by a colon, specifying the number of tries. If
   * there is no prefix, no initial tries will be performed. If there is nothing
   * following a colon, none of the above methods will be attempted. By default,
   * fdp uses overlap="9:prism". Note that overlap="true", overlap="0:true" and
   * overlap="0:" all turn off all overlap removal.
   *
   * By default, sfdp uses overlap="prism0".
   *
   * Except for the Voronoi and prism methods, all of these transforms preserve
   * the orthogonal ordering of the original layout. That is, if the x
   * coordinates of two nodes are originally the same, they will remain the
   * same, and if the x coordinate of one node is originally less than the x
   * coordinate of another, this relation will still hold in the transformed
   * layout. The similar properties hold for the y coordinates. This is not
   * quite true for the "porth*" cases. For these, orthogonal ordering is only
   * preserved among nodes related by an edge.
   *
   * TODO(jvilk): Try to add validation code to this...
   */
  void SetOverlap(std::string val) {
    AddFirstClassAttribute<std::string>(AttributeType::OVERLAP, val);
  }

  /**
   * When overlap=prism, the layout is scaled by this factor, thereby removing a
   * fair amount of node overlap, and making node overlap removal faster and
   * better able to retain the graph's shape.
   *
   * If overlap_scaling is negative, the layout is scaled by -1*overlap_scaling
   * times the average label size. If overlap_scaling is positive, the layout is
   * scaled by overlap_scaling. If overlap_scaling is zero, no scaling is done.
   */
  void SetOverlapScaling(double val) {
    AddFirstClassAttribute<double>(AttributeType::OVERLAP_SCALING, val);
  }

  /**
   * This is true if the value of pack is "true" (case-insensitive) or a
   * non-negative integer. If true, each connected component of the graph is
   * laid out separately, and then the graphs are packed together. If pack has
   * an integral value, this is used as the size, in points, of a margin around
   * each part; otherwise, a default margin of 8 is used. If pack is interpreted
   * as false, the entire graph is laid out together. The granularity and method
   * of packing is influenced by the packmode attribute.
   *
   * For layouts which always do packing, such as twopi, the pack attribute is
   * just used to set the margin.
   */
  void SetPack(bool val) {
    AddFirstClassAttribute<bool>(AttributeType::PACK, val);
  }
  void SetPack(int val) {
    AddFirstClassAttribute<int>(AttributeType::PACK, val);
  }

  /**
   * This indicates how connected components should be packed. Note that
   * defining packmode will automatically turn on packing as though one had set
   * pack=true.
   *
   * The modes "node", "clust" or "graph" specify that the components should be
   * packed together tightly, using the specified granularity. A value of "node"
   * causes packing at the node and edge level, with no overlapping of these
   * objects. This produces a layout with the least area, but it also allows
   * interleaving, where a node of one component may lie between two nodes in
   * another component. A value of "graph" does a packing using the bounding box
   * of the component. Thus, there will be a rectangular region around a
   * component free of elements of any other component. A value of "clust"
   * guarantees that top-level clusters are kept intact. What effect a value has
   * also depends on the layout algorithm. For example, neato does not support
   * clusters, so a value of "clust" will have the same effect as the default
   * "node" value.
   *
   * The mode "array(_flag)?(%d)?" indicates that the components should be
   * packed at the graph level into an array of graphs. By default, the
   * components are in row-major order, with the number of columns roughly the
   * square root of the number of components. If the optional flags contains
   * 'c', then column-major order is used. Finally, if the optional integer
   * suffix is used, this specifies the number of columns for row-major or the
   * number of rows for column-major. Thus, the mode "array_c4" indicates array
   * packing, with 4 rows, starting in the upper left and going down the first
   * column, then down the second column, etc., until all components are used.
   *
   * If a graph is smaller than the array cell it occupies, it is centered by
   * default. The optional flags may contain 't', 'b', 'l', or 'r', indicating
   * that the graphs should be aligned along the top, bottom, left or right,
   * respectively.
   *
   * If the optional flags contains 'u', this causes the insertion order of
   * elements in the array to be determined by user-supplied values. Each
   * component can specify its sort value by a non-negative integer using the
   * sortv attribute. Components are inserted in order, starting with the one
   * with the smallest sort value. If no sort value is specified, zero is used.
   *
   * TODO(jvilk): Create a custom type for this rather than use a string.
   */
  void SetPackMode(std::string val) {
    AddFirstClassAttribute<std::string>(AttributeType::PACKMODE, val);
  }

  /**
   * The pad attribute specifies how much, in inches, to extend the drawing area
   * around the minimal area needed to draw the graph. If the pad is a single
   * double, both the x and y pad values are set equal to the given value. This
   * area is part of the drawing and will be filled with the background color,
   * if appropriate.
   *
   * Normally, a small pad is used for aesthetic reasons, especially when a
   * background color is used, to avoid having nodes and edges abutting the
   * boundary of the drawn region.
   */
  void SetPad(double x, double y) {
    AddPointAttribute(AttributeType::PAD, x, y);
  }

  /**
   * Width and height of output pages, in inches. If only a single value is
   * given, this is used for both the width and height.
   *
   * If this is set and is smaller than the size of the layout, a rectangular
   * array of pages of the specified page size is overlaid on the layout, with
   * origins aligned in the lower-left corner, thereby partitioning the layout
   * into pages. The pages are then produced one at a time, in pagedir order.
   *
   * At present, this only works for PostScript output. For other types of
   * output, one should use another tool to split the output into multiple
   * output files. Or use the viewport to generate multiple files.
   */
  void SetPage(double x, double y) {
    AddPointAttribute(AttributeType::PAGE, x, y);
  }

  /**
   * If the page attribute is set and applicable, this attribute specifies the
   * order in which the pages are emitted. This is limited to one of the 8 row
   * or column major orders.
   *
   * "BL", "BR", "TL", "TR", "RB", "RT", "LB", "LT". These specify the 8 row or
   * column major orders for traversing a rectangular array, the first character
   * corresponding to the major order and the second to the minor order. Thus,
   * for "BL", the major order is from bottom to top, and the minor order is
   * from left to right. This means the bottom row is traversed first, from left
   * to right, then the next row up, from left to right, and so on, until the 
   * topmost row is traversed.
   */
  void SetPageDir(PageDir::e val);
  void SetQuadTree(QuadType::e val);
  void SetQuadTree(bool val);
  void SetQuantum(double val);
  void SetRankDir(RankDir::e val);
  void SetRankSep(double val);
  void SetRankSep(const vector<double>& val);
  void SetRatio(double val);
  void SetRatio(std::string val);
  void SetReminCross(bool val);
  void SetRepulsiveForce(double val);
  void SetResolution(double val);
  void SetRoot(std::string val);
  void SetRoot(bool val);
  void SetRotate(int val);
  void SetRotation(double val);
  void SetScale(double val);
  void SetScale(double x, double y);
  void SetSearchSize(int val);
  void SetSearchSize(double val);
  void SetSep(double val);
  void SetSep(double x, double y);
  void SetShowBoses(int val);
  void SetSize(double val);
  void SetSize(double x, double y);
  void SetSmoothing(SmoothType::e val);
  void SetSortV(int val);
  void SetSplines(bool val);
  void SetSplines(std::string val);
  void SetStart(StartType::e val);
  void SetStyle(Style::e val);
  void SetStyleSheet(std::string val);
  void SetTarget(std::string val);
  void SetTrueColor(bool val);
  //void SetViewPort(ViewPort val);
  void SetVoroMargin(double val);
  void SetVoroMargin(int val);

};

class SubgraphAttributeSet {
private:
  std::vector<Attribute*> _attributes;

public:
  SubgraphAttributeSet() { };
};

class NodeAttributeSet {
private:
  std::vector<Attribute*> _attributes;

public:
  NodeAttributeSet() { };
};

class EdgeAttributeSet {
private:
  std::vector<Attribute*> _attributes;

public:
  EdgeAttributeSet() { };
};

}  //namespace DotWriter

#endif