/**
 * Stores a set of DOT attributes for various types of objects.
 * Handles validating input attributes to ensure they are appropriate
 * for the given object.
 *
 * Attribute descriptions lifted from:
 * http://www.graphviz.org/content/attrs
 *
 * TODO(jvilk): Make errors explicit at runtime rather than silently fix them.
 * TODO(jvilk): Allow removal of attributes + redefining values.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_ATTRIBUTESET_H_
#define DOTWRITER_ATTRIBUTESET_H_

#include <algorithm>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "Attribute.h"
#include "Enums.h"
#include "Util.h"

using std::runtime_error;

namespace DotWriter {

class Node;
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

  void AddCustomAttribute(const std::string& name, const std::string& val) {
    AddAttribute(new CustomAttribute(name, val));
  }

  void ToString(std::ostream& out) {
    std::vector<Attribute*>::iterator it;
    for (it = _attributes.begin(); it != _attributes.end(); it++) {
      Attribute* at = *it;
      at->ToString(out);

      if (it+1 != _attributes.end())
        out << ",";
    }
  }

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

    delete attribute;
  }

  void AddBoolAttribute(AttributeType::e type, bool val) {
    Attribute* attr = new BoolAttribute(type, val);
    AddAttribute(attr);
  }

  template<typename T>
  void AddSimpleAttribute(AttributeType::e type, T val) {
    Attribute* attr = new SimpleAttribute<T>(type, val);
    AddAttribute(attr);
  }

  template<typename T>
  void AddSimpleListAttribute(AttributeType::e type,
    const std::vector<T>& vals) {
    Attribute* attr = new SimpleListAttribute<T>(type, vals);
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

  void AddAddDoubleAttribute(AttributeType::e type, double val) {
    Attribute* attr = new AddDoubleAttribute(type, val);
    AddAttribute(attr);
  }

  void AddAddPointAttribute(AttributeType::e type, double x, double y) {
    Attribute* attr = new class AddPointAttribute(type, x, y);
    AddAttribute(attr);
  }
};

class GraphAttributeSet : public AttributeSet {
public:
  GraphAttributeSet() { };

  /**
   * Factor damping force motions. On each iteration, a nodes movement is
   * limited to this factor of its potential motion. By being less than 1.0, the 
   * system tends to ``cool'', thereby preventing cycling. 
   */
  void SetDamping(double val) {
    AddSimpleAttribute<double>(AttributeType::DAMPING, val);
  }

  /**
   * Spring constant used in virtual physical model. It roughly corresponds to
   * an ideal edge length (in inches), in that increasing K tends to increase
   * the distance between nodes. Note that the edge attribute len can be used to
   * override this value for adjacent nodes. 
   */
  void SetK(double val) {
    AddSimpleAttribute<double>(AttributeType::K, val);
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
    AddSimpleAttribute<std::string>(AttributeType::URL, val);
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
  void SetCenter(bool val) {
    AddBoolAttribute(AttributeType::CENTER, val);
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
    AddSimpleAttribute<std::string>(AttributeType::COMMENT, val);
  }

  /**
   * If true, allow edges between clusters. (See lhead and ltail) 
   */
  void SetCompound(bool val) {
    AddBoolAttribute(AttributeType::COMPOUND, val);
  }

  /**
   * If true, use edge concentrators. This merges multiedges into a single edge
   * and causes partially parallel edges to share part of their paths. The
   * latter feature is not yet available outside of dot. 
   */
  void SetConcentrate(bool val) {
    AddBoolAttribute(AttributeType::CONCENTRATE, val);
  }

  /**
   * This specifies the distance between nodes in separate connected components.
   * If set too small, connected components may overlap. Only applicable if
   * pack=false. 
   */
  void SetDefaultDist(double val) {
    AddSimpleAttribute<double>(AttributeType::DEFAULTDIST, val);
  }

  /**
   * Set the number of dimensions used for the layout. The maximum value allowed
   * is 10. 
   */
  void SetDim(unsigned val) {
    if (val > 10) val = 10;
    AddSimpleAttribute<unsigned>(AttributeType::DIM, val);
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
    AddSimpleAttribute<unsigned>(AttributeType::DIMEN, val);
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
    AddSimpleAttribute<double>(AttributeType::DPI, val);
  }

  /**
   * Terminating condition. If the length squared of all energy gradients are <
   * epsilon, the algorithm stops.
   */
  void SetEpsilon(double val) {
    AddSimpleAttribute<double>(AttributeType::EPSILON, val);
  }

  /**
   * Margin used around polygons for purposes of spline edge routing. The
   * interpretation is the same as given for sep. This should normally be
   * strictly less than sep. 
   */
  void SetEsep(double x, double y) {
    AddAddPointAttribute(AttributeType::ESEP, x, y);
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
    AddSimpleAttribute<std::string>(AttributeType::FONTNAME, val);
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
    AddSimpleAttribute<std::string>(AttributeType::FONTNAMES, val);
  }

  /**
   * Directory list used by libgd to search for bitmap fonts if Graphviz was not
   * built with the fontconfig library. If fontpath is not set, the environment
   * variable DOTFONTPATH is checked. If that is not set, GDFONTPATH is checked.
   * If not set, libgd uses its compiled-in font path. Note that fontpath is an
   * attribute of the root graph. 
   */
  void SetFontPath(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::FONTPATH, val);
  }

  /**
   * Font size, in points, used for text. 
   */
  void SetFontSize(double val) {
    AddSimpleAttribute<double>(AttributeType::FONTSIZE, val);
  }

  /**
   * If true, all xlabel attributes are placed, even if there is some overlap
   * with nodes or other labels. 
   */
  void SetForceLabels(bool val) {
    AddBoolAttribute(AttributeType::FORCELABELS, val);
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
    AddSimpleAttribute<int>(AttributeType::GRADIENTANGLE, val);
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
    AddSimpleAttribute<std::string>(AttributeType::IMAGEPATH, val);
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
    AddSimpleAttribute<unsigned>(AttributeType::LABEL_SCHEME, val);
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
    AddSimpleAttribute<std::string>(AttributeType::LAYOUT, val);
  }

  /**
   * Number of levels allowed in the multilevel scheme.
   */
  void SetLevels(int val) {
    AddSimpleAttribute<int>(AttributeType::LEVELS, val);
  }

  /**
   * Specifies strictness of level constraints in neato when mode="ipsep" or
   * "hier". Larger positive values mean stricter constraints, which demand more
   * separation between levels. On the other hand, negative values will relax
   * the constraints by allowing some overlap between the levels.
   */
  void SetLevelsGap(double val) {
    AddSimpleAttribute<double>(AttributeType::LEVELSGAP, val);
  }

  /**
   * Height of graph or cluster label, in inches.
   */
  void SetLHeight(double val) {
    AddSimpleAttribute<double>(AttributeType::LHEIGHT, val);
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
    AddSimpleAttribute<double>(AttributeType::LWIDTH, val);
  }

  /**
   * For graphs, this sets x and y margins of canvas, in inches.
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
  void SetMargin(double x, double y) {
    AddPointAttribute(AttributeType::MARGIN, x, y);
  }

  /**
   * Sets the number of iterations used.
   */
  void SetMaxIter(int val) {
    AddSimpleAttribute<int>(AttributeType::MAXITER, val);
  }

  /**
   * Multiplicative scale factor used to alter the MinQuit (default = 8) and
   * MaxIter (default = 24) parameters used during crossing minimization. These
   * correspond to the number of tries without improvement before quitting and
   * the maximum number of iterations in each pass.
   */
  void SetMCLimit(double val) {
    AddSimpleAttribute<double>(AttributeType::MCLIMIT, val);
  }

  /**
   * Specifies the minimum separation between all nodes.
   */
  void SetMinDist(double val) {
    AddSimpleAttribute<double>(AttributeType::MINDIST, val);
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
    AddBoolAttribute(AttributeType::MOSEK, val);
  }

  /**
   * In dot, this specifies the minimum space between two adjacent nodes in the
   * same rank, in inches.
   *
   * For other layouts, this affects the spacing between loops on a single node,
   * or multiedges between a pair of nodes
   */
  void SetNodeSep(double val) {
    AddSimpleAttribute<double>(AttributeType::NODESEP, val);
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
    AddBoolAttribute(AttributeType::NOJUSTIFY, val);
  }

  /**
   * If set, normalize coordinates of final layout so that the first point is at
   * the origin, and then rotate the layout so that the first edge is
   * horizontal.
   */
  void SetNormalize(bool val) {
    AddBoolAttribute(AttributeType::NORMALIZE, val);
  }

  /**
   * Used to set number of iterations in network simplex applications. nslimit
   * is used in computing node x coordinates, nslimit1 for ranking nodes. If
   * defined, # iterations = nslimit(1) * # nodes; otherwise, # iterations =
   * MAXINT.
   */
  void SetNsLimit(double val) {
    AddSimpleAttribute<double>(AttributeType::NSLIMIT, val);
  }
  void SetNsLimit1(double val) {
    AddSimpleAttribute<double>(AttributeType::NSLIMIT1, val);
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
    AddSimpleAttribute<std::string>(AttributeType::OVERLAP, val);
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
    AddSimpleAttribute<double>(AttributeType::OVERLAP_SCALING, val);
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
    AddBoolAttribute(AttributeType::PACK, val);
  }
  void SetPack(int val) {
    AddSimpleAttribute<int>(AttributeType::PACK, val);
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
    AddSimpleAttribute<std::string>(AttributeType::PACKMODE, val);
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
  void SetPageDir(PageDir::e val) {
    AddEnumAttribute<PageDir::e, PageDir>(AttributeType::PAGEDIR, val);
  }

  /**
   * Quadtree scheme to use.
   */
  void SetQuadTree(QuadType::e val) {
    AddEnumAttribute<QuadType::e, QuadType>(AttributeType::QUADTREE, val);
  }

  /**
   * If quantum > 0.0, node label dimensions will be rounded to integral
   * multiples of the quantum. 
   */
  void SetQuantum(double val) {
    AddSimpleAttribute<double>(AttributeType::QUANTUM, val);
  }

  /**
   * Sets direction of graph layout. For example, if rankdir="LR", and barring
   * cycles, an edge T -> H; will go from left to right. By default, graphs are
   * laid out from top to bottom.
   *
   * This attribute also has a side-effect in determining how record nodes are
   * interpreted. See record shapes. 
   */
  void SetRankDir(RankDir::e val) {
    AddEnumAttribute<RankDir::e, RankDir>(AttributeType::RANKDIR, val);
  }

  /**
   * In dot, this gives the desired rank separation, in inches. This is the
   * minimum vertical distance between the bottom of the nodes in one rank and
   * the tops of nodes in the next. If the value contains "equally", the centers
   * of all ranks are spaced equally apart. Note that both settings are
   * possible, e.g., ranksep = "1.2 equally".
   *
   * In twopi, this attribute specifies the radial separation of concentric
   * circles. For twopi, ranksep can also be a list of doubles. The first double
   * specifies the radius of the inner circle; the second double specifies the
   * increase in radius from the first circle to the second; etc. If there are
   * more circles than numbers, the last number is used as the increment for the
   * remainder. 
   */
  void SetRankSep(double val) {
    AddSimpleAttribute<double>(AttributeType::RANKSEP, val);
  }
  void SetRankSep(const std::vector<double>& val) {
    AddSimpleListAttribute<double>(AttributeType::RANKSEP, val);
  }

  /**
   * Sets the aspect ratio (drawing height/drawing width) for the drawing. Note
   * that this is adjusted before the size attribute constraints are enforced.
   * In addition, the calculations usually ignore the node sizes, so the final
   * drawing size may only approximate what is desired.
   *
   * If ratio is numeric, it is taken as the desired aspect ratio. Then, if the
   * actual aspect ratio is less than the desired ratio, the drawing height is
   * scaled up to achieve the desired ratio; if the actual ratio is greater than
   * that desired ratio, the drawing width is scaled up.
   *
   * If ratio = "fill" and the size attribute is set, node positions are scaled,
   * separately in both x and y, so that the final drawing exactly fills the
   * specified size. If both size values exceed the width and height of the
   * drawing, then both coordinate values of each node are scaled up
   * accordingly. However, if either size dimension is smaller than the
   * corresponding dimension in the drawing, one dimension is scaled up so that
   * the final drawing has the same aspect ratio as specified by size. Then,
   * when rendered, the layout will be scaled down uniformly in both dimensions
   * to fit the given size, which may cause nodes and text to shrink as well.
   * This may not be what the user wants, but it avoids the hard problem of how
   * to reposition the nodes in an acceptable fashion to reduce the drawing
   * size.
   *
   * If ratio = "compress" and the size attribute is set, dot attempts to
   * compress the initial layout to fit in the given size. This achieves a
   * tighter packing of nodes but reduces the balance and symmetry. This feature
   * only works in dot.
   *
   * If ratio = "expand", the size attribute is set, and both the width and the
   * height of the graph are less than the value in size, node positions are
   * scaled uniformly until at least one dimension fits size exactly. Note that
   * this is distinct from using size as the desired size, as here the drawing
   * is expanded before edges are generated and all node and text sizes remain
   * unchanged.
   *
   * If ratio = "auto", the page attribute is set and the graph cannot be drawn
   * on a single page, then size is set to an ``ideal'' value. In particular,
   * the size in a given dimension will be the smallest integral multiple of the
   * page size in that dimension which is at least half the current size. The
   * two dimensions are then scaled independently to the new size. This feature
   * only works in dot. 
   */
  void SetRatio(double val) {
    AddSimpleAttribute<double>(AttributeType::RATIO, val);
  }
  void SetRatio(Ratio::e val) {
    AddEnumAttribute<Ratio::e, Ratio>(AttributeType::RATIO, val);
  }

  /**
   * If true and there are multiple clusters, run cross minimization a second
   * time.
   */
  void SetReMinCross(bool val) {
    AddBoolAttribute(AttributeType::REMINCROSS, val);
  }

  /**
   * The power of the repulsive force used in an extended Fruchterman-Reingold
   * force directed model. Values larger than 1 tend to reduce the warping
   * effect at the expense of less clustering. 
   */
  void SetRepulsiveForce(double val) {
    AddSimpleAttribute<double>(AttributeType::REPULSIVEFORCE, val);
  }

  /**
   * This specifies nodes to be used as the center of the layout and the root of
   * the generated spanning tree. As a graph attribute, this gives the name of
   * the node. As a node attribute (circo only), it specifies that the node
   * should be used as a central node. In twopi, this will actually be the
   * central node. In circo, the block containing the node will be central in
   * the drawing of its connected component. If not defined, twopi will pick a
   * most central node, and circo will pick a random node.
   *
   * If the root attribute is defined as the empty string, twopi will reset it
   * to name of the node picked as the root node. 
   */
  void SetRoot(Node* node);

  /**
   * If 90, set drawing orientation to landscape. 
   */
  void SetRotate(int val) {
    AddSimpleAttribute<int>(AttributeType::ROTATE, val);
  }

  /**
   * Causes the final layout to be rotated counter-clockwise by the specified
   * number of degrees. 
   */
  void SetRotation(double val) {
    AddSimpleAttribute<double>(AttributeType::ROTATION, val);
  }

  /**
   * If set, after the initial layout, twopi will scale the layout by the given
   * factors. The scaling uses the root node of each component as the center of
   * the scaling. If only a single number is given, this is used for both
   * factors. 
   */
  void SetScale(double val) {
    SetScale(val, val);
  }
  void SetScale(double x, double y) {
    AddPointAttribute(AttributeType::SCALE, x, y);
  }

  /**
   * During network simplex, maximum number of edges with negative cut values to
   * search when looking for one with minimum cut value. 
   */
  void SetSearchSize(int val) {
    AddSimpleAttribute<int>(AttributeType::SEARCHSIZE, val);
  }
  void SetSearchSize(double val) {
    AddAddDoubleAttribute(AttributeType::SEARCHSIZE, val);
  }

  /**
   * Specifies margin to leave around nodes when removing node overlap. This
   * guarantees a minimal non-zero distance between nodes.
   *
   * If the attribute begins with a plus sign '+', an additive margin is
   * specified. That is, "+w,h" causes the node's bounding box to be increased
   * by w points on the left and right sides, and by h points on the top and
   * bottom. Without a plus sign, the node is scaled by 1 + w in the x 
   * coordinate and 1 + h in the y coordinate.
   *
   * If only a single number is given, this is used for both dimensions.
   *
   * If unset but esep is defined, the sep values will be set to the esep values
   * divided by 0.8. If esep is unset, the default value is used. 
   */
  void SetSep(double x, double y) {
    AddAddPointAttribute(AttributeType::SEP, x, y);
  }

  /**
   * Print guide boxes in PostScript at the beginning of routesplines if 1, or
   * at the end if 2. (Debugging) 
   */
  void SetShowBoxes(unsigned val) {
    if (val > 2) val = 2;
    AddSimpleAttribute<unsigned>(AttributeType::SHOWBOXES, val);
  }

  /**
   * Maximum width and height of drawing, in inches.
   *
   * If defined and the drawing is larger than the given size, the drawing is
   * uniformly scaled down so that it fits within the given size.
   *
   * If size ends in an exclamation point (!), then it is taken to be the
   * desired size. In this case, if both dimensions of the drawing are less than
   * size, the drawing is scaled up uniformly until at least one dimension
   * equals its dimension in size.
   *
   * Note that there is some interaction between the size and ratio attributes. 
   */
  void SetSize(double x, double y) {
    AddPointAttribute(AttributeType::SIZE, x, y);
  }

  /**
   * Specifies a post-processing step used to smooth out an uneven distribution
   * of nodes. 
   */
  void SetSmoothing(SmoothType::e val) {
    AddEnumAttribute<SmoothType::e, SmoothType>(AttributeType::SMOOTHING, val);
  }

  /**
   * If packmode indicates an array packing, this attribute specifies an
   * insertion order among the components, with smaller values inserted first. 
   */
  void SetSortV(int val) {
    AddSimpleAttribute<int>(AttributeType::SORTV, val);
  }

  /**
   * Controls how, and if, edges are represented. If true, edges are drawn as
   * splines routed around nodes; if false, edges are drawn as line segments. If
   * set to "", no edges are drawn at all.
   *
   * (1 March 2007) The values line and spline can be used as synonyms for false
   * and true, respectively. In addition, the value polyline specifies that
   * edges should be drawn as polylines.
   *
   * (28 Sep 2010) The value ortho specifies edges should be routed as polylines
   * of axis-aligned segments. Currently, the routing does not handle ports or,
   * in dot, edge labels.
   *
   * By default, the attribute is unset. How this is interpreted depends on the
   * layout. For dot, the default is to draw edges as splines. For all other
   * layouts, the default is to draw edges as line segments. Note that for these
   * latter layouts, if splines="true", this requires non-overlapping nodes (cf.
   * overlap). If fdp is used for layout and splines="compound", then the edges
   * are drawn to avoid clusters as well as nodes. 
   */
  void SetSplines(SplineType::e val) {
    AddEnumAttribute<SplineType::e, SplineType>(AttributeType::SPLINES, val);
  }

  /**
   * Parameter used to determine the initial layout of nodes. If unset, the
   * nodes are randomly placed in a unit square with the same seed is always
   * used for the random number generator, so the initial placement is
   * repeatable.
   *
   * The value has the syntax (style)?(seed)?.
   *
   * If style is present, it must be one of the strings "regular", "self", or
   * "random". In the first case, the nodes are placed regularly about a circle.
   * In the second case, an abbreviated version of neato is run to obtain the
   * initial layout. In the last case, the nodes are placed randomly in a unit
   * square.
   *
   * If seed is present, it specifies a seed for the random number generator. If
   * seed is a positive number, this is used as the seed. If it is anything
   * else, the current time, and possibly the process id, is used to pick a
   * seed, thereby making the choice more random. In this case, the seed value
   * is stored in the graph.
   *
   * If the value is just "random", a time-based seed is chosen.
   *
   * Note that input positions, specified by a node's pos attribute, are only
   * used when the style is "random". 
   *
   * TODO(jvilk): Make this an object for validation purposes.
   */
  void SetStart(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::START, val);
  }

  /**
   * Set style information for components of the graph. For cluster subgraphs,
   * if "filled", the cluster box's background is filled.
   *
   * Syntax is styleItem ( ',' styleItem )*
   * where styleItem = name or name'('args')'
   * and args  = name ( ',' name )*
   * and name can be any string of characters not containing a space, a left or
   * right parenthesis, or a comma. Note that whitespace characters are ignored.
   *
   * At present, the recognized style names are "dashed", "dotted", "solid",
   * "invis" and "bold" for nodes and edges, "tapered" for edges only, and
   * "filled", "diagonals" and "rounded" for nodes only. The styles "filled" and
   * "rounded" are recognized for clusters. The style "radial" is recognized for
   * nodes, clusters and graphs, and indicates a radial-style gradient fill if
   * applicable. Additional styles are available in device-dependent form. Style
   * lists are passed to device drivers, which can use this to generate
   * appropriate output.
   *
   * The setlinewidth style value can be used for more control over the width of
   * node borders and edges than is allowed by bold. This style value takes an
   * argument, specifying the width of the line in points. For example,
   * style="bold" is equivalent to style="setlinewidth(2)". The use of
   * setlinewidth is deprecated; one should use the penwidth attribute instead. 
   *
   * TODO(jvilk): Make a custom object for this for validation purposes.
   */
  void SetStyle(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::STYLE, val);
  }

  /**
   * A URL or pathname specifying an XML style sheet, used in SVG output. 
   */
  void SetStyleSheet(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::STYLESHEET, val);
  }

  /**
   * If the object has a URL, this attribute determines which window of the
   * browser is used for the URL. See W3C documentation. 
   */
  void SetTarget(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::STYLESHEET, val);
  }

  /**
   * If set explicitly to true or false, the value determines whether or not
   * internal bitmap rendering relies on a truecolor color model or uses a color
   * palette. If the attribute is unset, truecolor is not used unless there is a
   * shapefile property for some node in the graph. The output model will use
   * the input model when possible.
   *
   * Use of color palettes results in less memory usage during creation of the
   * bitmaps and smaller output files.
   *
   * Usually, the only time it is necessary to specify the truecolor model is if
   * the graph uses more than 256 colors. However, if one uses
   * bgcolor=transparent with a color palette, font antialiasing can show up as
   * a fuzzy white area around characters. Using truecolor=true avoids this
   * problem. 
   */
  void SetTrueColor(bool val) {
    AddBoolAttribute(AttributeType::TRUECOLOR, val);
  }

  // TODO(jvilk): Implement this type.
  //void SetViewPort(ViewPort val);

  /**
   * Factor to scale up drawing to allow margin for expansion in Voronoi
   * technique. dim' = (1+2*margin)*dim. 
   */
  void SetVoroMargin(double val) {
    AddSimpleAttribute<double>(AttributeType::VORO_MARGIN,val);
  }

};

class SubgraphAttributeSet : public AttributeSet {
public:
  SubgraphAttributeSet() { };

  void SetRank(RankType::e val) {
    AddEnumAttribute<RankType::e, RankType>(AttributeType::RANK, val);
  }
};

class ClusterAttributeSet : public AttributeSet {
public:
  ClusterAttributeSet() { };

  /**
   * Spring constant used in virtual physical model. It roughly corresponds to
   * an ideal edge length (in inches), in that increasing K tends to increase
   * the distance between nodes. Note that the edge attribute len can be used to
   * override this value for adjacent nodes. 
   */
  void SetK(double val) {
    AddSimpleAttribute<double>(AttributeType::K, val);
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
    AddSimpleAttribute<std::string>(AttributeType::URL, val);
  }

  /**
   * Indicates the preferred area for a node or empty cluster when laid out by
   * patchwork. 
   */
  void SetArea(double val) {
    AddSimpleAttribute<double>(AttributeType::AREA, val);
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
   * Basic drawing color for graphics, not text. For the latter, use the
   * fontcolor attribute.
   *
   * For edges, the value can either be a single color or a colorList. In the
   * latter case, the edge is drawn using parallel splines or lines, one for
   * each color in the list, in the order given. The head arrow, if any, is
   * drawn using the first color in the list, and the tail arrow, if any, the
   * second color. This supports the common case of drawing opposing edges, but
   * using parallel splines instead of separately routed multiedges.
   * See http://www.graphviz.org/content/attrs#dcolor for more information.
   */
  void SetColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::COLOR, val);
  }
  
  /**
   * Color used to fill the background of a node or cluster assuming
   * style=filled, or a filled arrowhead. If fillcolor is not defined, color is
   * used. (For clusters, if color is not defined, bgcolor is used.) If this is
   * not defined, the default is used, except for shape=point or when the output
   * format is MIF, which use black by default.
   *
   * If the value is a colorList, a gradient fill is used. By default, this is a
   * linear fill; setting style=radial will cause a radial fill. At present,
   * only two colors are used. If the second color (after a colon) is missing,
   * the default color is used for it. See also the gradientangle attribute for
   * setting the gradient angle.
   *
   * Note that a cluster inherits the root graph's attributes if defined. Thus,
   * if the root graph has defined a fillcolor, this will override a color or
   * bgcolor attribute set for the cluster. 
   */
  void SetFillColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::FILLCOLOR, val);
  }
  void SetFillColor(const std::vector<Color::e> vals) {
    AddEnumListAttribute<Color::e, Color>(AttributeType::FILLCOLOR, vals);
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
    AddSimpleAttribute<std::string>(AttributeType::FONTNAME, val);
  }

  /**
   * Font size, in points, used for text. 
   */
  void SetFontSize(double val) {
    AddSimpleAttribute<double>(AttributeType::FONTSIZE, val);
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
    AddSimpleAttribute<int>(AttributeType::GRADIENTANGLE, val);
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
   * Height of graph or cluster label, in inches.
   */
  void SetLHeight(double val) {
    AddSimpleAttribute<double>(AttributeType::LHEIGHT, val);
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
    AddSimpleAttribute<double>(AttributeType::LWIDTH, val);
  }

  /**
   * For graphs, this sets x and y margins of canvas, in inches.
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
  void SetMargin(double x, double y) {
    AddPointAttribute(AttributeType::MARGIN, x, y);
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
    AddBoolAttribute(AttributeType::NOJUSTIFY, val);
  }

  /**
   * Color used to draw the bounding box around a cluster. If pencolor is not
   * defined, color is used. If this is not defined, bgcolor is used. If this is
   * not defined, the default is used.
   *
   * Note that a cluster inherits the root graph's attributes if defined. Thus,
   * if the root graph has defined a pencolor, this will override a color or
   * bgcolor attribute set for the cluster. 
   */
  void SetPenColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::PENCOLOR, val);
  }

  /**
   * Specifies the width of the pen, in points, used to draw lines and curves,
   * including the boundaries of edges and clusters. The value is inherited by
   * subclusters. It has no effect on text.
   *
   * Previous to 31 January 2008, the effect of penwidth=W was achieved by
   * including setlinewidth(W) as part of a style specification. If both are
   * used, penwidth will be used. 
   */
  void SetPenWidth(double val) {
    AddSimpleAttribute<double>(AttributeType::PENWIDTH, val);
  }

  /**
   * Set number of peripheries used in polygonal shapes and cluster boundaries.
   * Note that user-defined shapes are treated as a form of box shape, so the
   * default peripheries value is 1 and the user-defined shape will be drawn in
   * a bounding rectangle. Setting peripheries=0 will turn this off. Also, 1 is
   * the maximum peripheries value for clusters. 
   */
  void SetPeripheries(int val) {
    if (val > 1) val = 1;
    AddSimpleAttribute<int>(AttributeType::PERIPHERIES, val);
  }

  /**
   * If packmode indicates an array packing, this attribute specifies an
   * insertion order among the components, with smaller values inserted first. 
   */
  void SetSortV(int val) {
    AddSimpleAttribute<int>(AttributeType::SORTV, val);
  }

  /**
   * Set style information for components of the graph. For cluster subgraphs,
   * if "filled", the cluster box's background is filled.
   *
   * Syntax is styleItem ( ',' styleItem )*
   * where styleItem = name or name'('args')'
   * and args  = name ( ',' name )*
   * and name can be any string of characters not containing a space, a left or
   * right parenthesis, or a comma. Note that whitespace characters are ignored.
   *
   * At present, the recognized style names are "dashed", "dotted", "solid",
   * "invis" and "bold" for nodes and edges, "tapered" for edges only, and
   * "filled", "diagonals" and "rounded" for nodes only. The styles "filled" and
   * "rounded" are recognized for clusters. The style "radial" is recognized for
   * nodes, clusters and graphs, and indicates a radial-style gradient fill if
   * applicable. Additional styles are available in device-dependent form. Style
   * lists are passed to device drivers, which can use this to generate
   * appropriate output.
   *
   * The setlinewidth style value can be used for more control over the width of
   * node borders and edges than is allowed by bold. This style value takes an
   * argument, specifying the width of the line in points. For example,
   * style="bold" is equivalent to style="setlinewidth(2)". The use of
   * setlinewidth is deprecated; one should use the penwidth attribute instead. 
   *
   * TODO(jvilk): Make a custom object for this for validation purposes.
   */
  void SetStyle(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::STYLE, val);
  }

  /**
   * If the object has a URL, this attribute determines which window of the
   * browser is used for the URL. See W3C documentation. 
   */
  void SetTarget(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::STYLESHEET, val);
  }

  /**
   * Tooltip annotation attached to the node or edge. If unset, Graphviz will
   * use the object's label if defined. Note that if the label is a record
   * specification or an HTML-like label, the resulting tooltip may be
   * unhelpful. In this case, if tooltips will be generated, the user should set
   * a tooltip attribute explicitly. 
   */
  void SetTooltip(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::TOOLTIP, val);
  }
};

class NodeAttributeSet : public AttributeSet {
private:
  std::vector<Attribute*> _attributes;

public:
  NodeAttributeSet() { };

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
    AddSimpleAttribute<std::string>(AttributeType::URL, val);
  }

  /**
   * Indicates the preferred area for a node or empty cluster when laid out by
   * patchwork. 
   */
  void SetArea(double val) {
    AddSimpleAttribute<double>(AttributeType::AREA, val);
  }

  /**
   * Basic drawing color for graphics, not text. For the latter, use the
   * fontcolor attribute.
   *
   * For edges, the value can either be a single color or a colorList. In the
   * latter case, the edge is drawn using parallel splines or lines, one for
   * each color in the list, in the order given. The head arrow, if any, is
   * drawn using the first color in the list, and the tail arrow, if any, the
   * second color. This supports the common case of drawing opposing edges, but
   * using parallel splines instead of separately routed multiedges.
   * See http://www.graphviz.org/content/attrs#dcolor for more information.
   */
  void SetColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::COLOR, val);
  }

  /**
   * Comments are inserted into output. Device-dependent 
   */
  void SetComment(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::COMMENT, val);
  }

  /**
   * Distortion factor for shape=polygon. Positive values cause top part to be
   * larger than bottom; negative values do the opposite. 
   */
  void SetDistortion(double val) {
    AddSimpleAttribute<double>(AttributeType::DISTORTION, val);
  }

  /**
   * Color used to fill the background of a node or cluster assuming
   * style=filled, or a filled arrowhead. If fillcolor is not defined, color is
   * used. (For clusters, if color is not defined, bgcolor is used.) If this is
   * not defined, the default is used, except for shape=point or when the output
   * format is MIF, which use black by default.
   *
   * If the value is a colorList, a gradient fill is used. By default, this is a
   * linear fill; setting style=radial will cause a radial fill. At present,
   * only two colors are used. If the second color (after a colon) is missing,
   * the default color is used for it. See also the gradientangle attribute for
   * setting the gradient angle.
   *
   * Note that a cluster inherits the root graph's attributes if defined. Thus,
   * if the root graph has defined a fillcolor, this will override a color or
   * bgcolor attribute set for the cluster. 
   */
  void SetFillColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::FILLCOLOR, val);
  }
  void SetFillColor(const std::vector<Color::e> vals) {
    AddEnumListAttribute<Color::e, Color>(AttributeType::FILLCOLOR, vals);
  }

  /**
   * If true, the node size is specified by the values of the width and height
   * attributes only and is not expanded to contain the text label. 
   */
  void SetFixedSize(bool val) {
    AddBoolAttribute(AttributeType::FIXEDSIZE, val);
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
    AddSimpleAttribute<std::string>(AttributeType::FONTNAME, val);
  }

  /**
   * Font size, in points, used for text. 
   */
  void SetFontSize(double val) {
    AddSimpleAttribute<double>(AttributeType::FONTSIZE, val);
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
    AddSimpleAttribute<int>(AttributeType::GRADIENTANGLE, val);
  }

  /**
   * If the end points of an edge belong to the same group, i.e., have the same
   * group attribute, parameters are set to avoid crossings and keep the edges
   * straight.
   *
   * TODO(jvilk): Perhaps expose nicely with an object?
   */
  void SetGroup(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::GROUP, val);
  }

  /**
   * Height of node, in inches. This is taken as the initial, minimum height of
   * the node. If fixedsize is true, this will be the final height of the node.
   * Otherwise, if the node label requires more height to fit, the node's height
   * will be increased to contain the label. Note also that, if the output
   * format is dot, the value given to height will be the final value.
   *
   * If the node shape is regular, the width and height are made identical. In
   * this case, if either the width or the height is set explicitly, that value
   * is used. In this case, if both the width or the height are set explicitly,
   * the maximum of the two values is used. If neither is set explicitly, the
   * minimum of the two default values is used. 
   */
  void SetHeight(double val) {
    AddSimpleAttribute<double>(AttributeType::HEIGHT, val);
  }

  /**
   * Gives the name of a file containing an image to be displayed inside a node.
   * The image file must be in one of the recognized formats, typically JPEG,
   * PNG, GIF, BMP, SVG or Postscript, and be able to be converted into the
   * desired output format.
   *
   * The file must contain the image size information. This is usually trivially
   * true for the bitmap formats. For PostScript, the file must contain a line
   * starting with %%BoundingBox: followed by four integers specifying the lower
   * left x and y coordinates and the upper right x and y coordinates of the
   * bounding box for the image, the coordinates being in points. An SVG image
   * file must contain width and height attributes, typically as part of the svg
   * element. The values for these should have the form of a floating point
   * number, followed by optional units, e.g., width="76pt". Recognized units
   * are in, px, pc, pt, cm and mm for inches, pixels, picas, points,
   * centimeters and millimeters, respectively. The default unit is points.
   *
   * Unlike with the shapefile attribute, the image is treated as node content
   * rather than the entire node. In particular, an image can be contained in a
   * node of any shape, not just a rectangle. 
   */
  void SetImage(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::IMAGE, val);
  }

  /**
   * Attribute controlling how an image fills its containing node. In general,
   * the image is given its natural size, (cf. dpi), and the node size is made
   * large enough to contain its image, its label, its margin, and its
   * peripheries. Its width and height will also be at least as large as its
   * minimum width and height. If, however, fixedsize=true, the width and height
   * attributes specify the exact size of the node.
   *
   * During rendering, in the default case (imagescale=false), the image retains
   * its natural size. If imagescale=true, the image is uniformly scaled (i.e.,
   * its aspect ratio is preserved) to fit inside the node. At least one
   * dimension of the image will be as large as possible given the size of the
   * node. When imagescale=width, the width of the image is scaled to fill the
   * node width. The corresponding property holds when imagescale=height. When
   * imagescale=both, both the height and the width are scaled separately to
   * fill the node.
   *
   * In all cases, if a dimension of the image is larger than the corresponding
   * dimension of the node, that dimension of the image is scaled down to fit
   * the node. As with the case of expansion, if imagescale=true, width and
   * height are scaled uniformly. 
   */
  void SetImageScale(ImageScaleType::e val) {
    AddEnumAttribute<ImageScaleType::e, ImageScaleType>(
      AttributeType::IMAGESCALE, val);
  }
  void SetImageScale(bool val) {
    AddBoolAttribute(AttributeType::IMAGESCALE, val);
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
    AddEnumAttribute<LabelLoc::e, LabelLoc>(AttributeType::LABELLOC, val);
  }

  /**
   * For graphs, this sets x and y margins of canvas, in inches.
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
  void SetMargin(double x, double y) {
    AddPointAttribute(AttributeType::MARGIN, x, y);
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
    AddBoolAttribute(AttributeType::NOJUSTIFY, val);
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
   * Angle, in degrees, used to rotate polygon node shapes. For any number of
   * polygon sides, 0 degrees rotation results in a flat base. 
   */
  void SetOrientation(double val) {
    AddSimpleAttribute<double>(AttributeType::ORIENTATION, val);
  }

  /**
   * Specifies the width of the pen, in points, used to draw lines and curves,
   * including the boundaries of edges and clusters. The value is inherited by
   * subclusters. It has no effect on text.
   *
   * Previous to 31 January 2008, the effect of penwidth=W was achieved by
   * including setlinewidth(W) as part of a style specification. If both are
   * used, penwidth will be used. 
   */
  void SetPenWidth(double val) {
    AddSimpleAttribute<double>(AttributeType::PENWIDTH, val);
  }

  /**
   * Set number of peripheries used in polygonal shapes and cluster boundaries.
   * Note that user-defined shapes are treated as a form of box shape, so the
   * default peripheries value is 1 and the user-defined shape will be drawn in
   * a bounding rectangle. Setting peripheries=0 will turn this off. Also, 1 is
   * the maximum peripheries value for clusters. 
   */
  void SetPeripheries(int val) {
    AddSimpleAttribute<int>(AttributeType::PERIPHERIES, val);
  }

  /**
   * If true and the node has a pos attribute on input, neato or fdp prevents
   * the node from moving from the input position. This property can also be
   * specified in the pos attribute itself (cf. the point type).
   *
   * Note: Due to an artifact of the implementation, final coordinates are
   * translated to the origin. Thus, if you look at the output coordinates given
   * in the (x)dot or plain format, pinned nodes will not have the same output
   * coordinates as were given on input. If this is important, a simple
   * workaround is to maintain the coordinates of a pinned node. The vector
   * difference between the old and new coordinates will give the translation,
   * which can then be subtracted from all of the appropriate coordinates. 
   */
  void SetPin(bool val) {
    AddBoolAttribute(AttributeType::PIN, val);
  }
  
  /**
   * Position of node, or spline control points. For nodes, the position
   * indicates the center of the node. On output, the coordinates are in points.
   *
   * In neato and fdp, pos can be used to set the initial position of a node. By
   * default, the coordinates are assumed to be in inches. However, the -s
   * command line flag can be used to specify different units. As the output
   * coordinates are in points, feeding the output of a graph laid out by a
   * Graphviz program into neato or fdp will almost always require the -s flag.
   *
   * When the -n command line flag is used with neato, it is assumed the
   * positions have been set by one of the layout programs, and are therefore in
   * points. Thus, neato -n can accept input correctly without requiring a -s
   * flag and, in fact, ignores any such flag. 
   */
  void SetPos(double x, double y) {
    AddPointAttribute(AttributeType::POS, x, y);
  }
  void SetPos(SplineType::e val) {
    AddEnumAttribute<SplineType::e, SplineType>(AttributeType::POS, val);
  }
  void SetPos(QuadType::e val) {
    AddEnumAttribute<QuadType::e, QuadType>(AttributeType::POS, val);
  }

  // TODO(jvilk): Implement.
  // void SetRect

  /**
   * If true, force polygon to be regular, i.e., the vertices of the polygon
   * will lie on a circle whose center is the center of the node. 
   */
  void SetRegular(bool val) {
    AddBoolAttribute(AttributeType::REGULAR, val);
  }

  /**
   * This specifies nodes to be used as the center of the layout and the root of
   * the generated spanning tree. As a graph attribute, this gives the name of
   * the node. As a node attribute (circo only), it specifies that the node
   * should be used as a central node. In twopi, this will actually be the
   * central node. In circo, the block containing the node will be central in
   * the drawing of its connected component. If not defined, twopi will pick a
   * most central node, and circo will pick a random node.
   *
   * If the root attribute is defined as the empty string, twopi will reset it
   * to name of the node picked as the root node. 
   */
  void SetRoot(bool val) {
    //TODO(jvilk): If node is deleted, this is not cleaned up...
    AddBoolAttribute(AttributeType::ROOT, val);
  }

  /**
   * If the input graph defines the vertices attribute, and output is dot or
   * xdot, this gives the number of points used for a node whose shape is a
   * circle or ellipse. It plays the same role in neato, when adjusting the
   * layout to avoid overlapping nodes, and in image maps. 
   */
  void SetSamplePoints(unsigned val) {
    AddSimpleAttribute<unsigned>(AttributeType::SAMPLEPOINTS, val);
  }

  /**
   * Set the shape of a node. 
   */
  void SetShape(NodeShape::e val) {
    AddEnumAttribute<NodeShape::e, NodeShape>(AttributeType::SHAPE, val);
  }

  /**
   * Print guide boxes in PostScript at the beginning of routesplines if 1, or
   * at the end if 2. (Debugging) 
   */
  void SetShowBoxes(unsigned val) {
    if (val > 2) val = 2;
    AddSimpleAttribute<unsigned>(AttributeType::SHOWBOXES, val);
  }

  /**
   * Number of sides if shape=polygon.
   */
  void SetSides(unsigned val) {
    AddSimpleAttribute<unsigned>(AttributeType::SIDES, val);
  }

  /**
   * Skew factor for shape=polygon. Positive values skew top of polygon to
   * right; negative to left. 
   */
  void SetSkew(double val) {
    AddSimpleAttribute<double>(AttributeType::SKEW, val);
  }

  /**
   * If packmode indicates an array packing, this attribute specifies an
   * insertion order among the components, with smaller values inserted first. 
   */
  void SetSortV(int val) {
    AddSimpleAttribute<int>(AttributeType::SORTV, val);
  }

  /**
   * Set style information for components of the graph. For cluster subgraphs,
   * if "filled", the cluster box's background is filled.
   *
   * Syntax is styleItem ( ',' styleItem )*
   * where styleItem = name or name'('args')'
   * and args  = name ( ',' name )*
   * and name can be any string of characters not containing a space, a left or
   * right parenthesis, or a comma. Note that whitespace characters are ignored.
   *
   * At present, the recognized style names are "dashed", "dotted", "solid",
   * "invis" and "bold" for nodes and edges, "tapered" for edges only, and
   * "filled", "diagonals" and "rounded" for nodes only. The styles "filled" and
   * "rounded" are recognized for clusters. The style "radial" is recognized for
   * nodes, clusters and graphs, and indicates a radial-style gradient fill if
   * applicable. Additional styles are available in device-dependent form. Style
   * lists are passed to device drivers, which can use this to generate
   * appropriate output.
   *
   * The setlinewidth style value can be used for more control over the width of
   * node borders and edges than is allowed by bold. This style value takes an
   * argument, specifying the width of the line in points. For example,
   * style="bold" is equivalent to style="setlinewidth(2)". The use of
   * setlinewidth is deprecated; one should use the penwidth attribute instead. 
   *
   * TODO(jvilk): Make a custom object for this for validation purposes.
   */
  void SetStyle(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::STYLE, val);
  }

  /**
   * If the object has a URL, this attribute determines which window of the
   * browser is used for the URL. See W3C documentation. 
   */
  void SetTarget(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::STYLESHEET, val);
  }

  /**
   * Tooltip annotation attached to the node or edge. If unset, Graphviz will
   * use the object's label if defined. Note that if the label is a record
   * specification or an HTML-like label, the resulting tooltip may be
   * unhelpful. In this case, if tooltips will be generated, the user should set
   * a tooltip attribute explicitly. 
   */
  void SetTooltip(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::TOOLTIP, val);
  }

  // TODO(jvilk): Implement this.
  //SetVertices

  /**
   * Width of node, in inches. This is taken as the initial, minimum width of
   * the node. If fixedsize is true, this will be the final width of the node.
   * Otherwise, if the node label requires more width to fit, the node's width
   * will be increased to contain the label. Note also that, if the output
   * format is dot, the value given to width will be the final value.
   *
   * If the node shape is regular, the width and height are made identical. In
   * this case, if either the width or the height is set explicitly, that value
   * is used. In this case, if both the width or the height are set explicitly,
   * the maximum of the two values is used. If neither is set explicitly, the
   * minimum of the two default values is used. 
   */
  void SetWidth(double val) {
    AddSimpleAttribute<double>(AttributeType::WIDTH, val);
  }

  /**
   * External label for a node or edge. For nodes, the label will be placed
   * outside of the node but near it. For edges, the label will be placed near
   * the center of the edge. This can be useful in dot to avoid the occasional
   * problem when the use of edge labels distorts the layout. For other layouts,
   * the xlabel attribute can be viewed as a synonym for the label attribute.
   *
   * These labels are added after all nodes and edges have been placed. The
   * labels will be placed so that they do not overlap any node or label. This
   * means it may not be possible to place all of them. To force placing all of
   * them, use the forcelabels attribute. 
   */
  void SetXLabel(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::XLABEL, val);
  }
};

class EdgeAttributeSet : public AttributeSet {
public:
  EdgeAttributeSet() { };

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
    AddSimpleAttribute<std::string>(AttributeType::URL, val);
  }

  /**
   * Style of arrowhead on the head node of an edge. This will only appear if
   * the dir attribute is "forward" or "both".
   */
  void SetArrowHead(EdgeArrowTypeName::e val) {
    AddEnumAttribute<EdgeArrowTypeName::e, EdgeArrowTypeName>(
      AttributeType::ARROWHEAD, val);
  }

  /**
   * Multiplicative scale factor for arrowheads. 
   */
  void SetArrowSize(double val) {
    AddSimpleAttribute<double>(AttributeType::ARROWSIZE, val);
  }

  /**
   * Style of arrowhead on the tail node of an edge. This will only appear if
   * the dir attribute is "back" or "both". 
   */
  void SetArrowTail(EdgeArrowTypeName::e val) {
    AddEnumAttribute<EdgeArrowTypeName::e, EdgeArrowTypeName>(
      AttributeType::ARROWTAIL, val);
  }

  /**
   * Basic drawing color for graphics, not text. For the latter, use the
   * fontcolor attribute.
   *
   * For edges, the value can either be a single color or a colorList. In the
   * latter case, the edge is drawn using parallel splines or lines, one for
   * each color in the list, in the order given. The head arrow, if any, is
   * drawn using the first color in the list, and the tail arrow, if any, the
   * second color. This supports the common case of drawing opposing edges, but
   * using parallel splines instead of separately routed multiedges.
   * See http://www.graphviz.org/content/attrs#dcolor for more information.
   */
  void SetColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::COLOR, val);
  }
  void SetColor(const std::vector<Color::e>& vals) {
    AddEnumListAttribute<Color::e, Color>(AttributeType::COLOR, vals);
  }

  /**
   * Comments are inserted into output. Device-dependent 
   */
  void SetComment(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::COMMENT, val);
  }

  /**
   * If false, the edge is not used in ranking the nodes.
   */
  void SetConstraint(bool val) {
    AddBoolAttribute(AttributeType::CONSTRAINT, val);
  }

  /**
   * If true, attach edge label to edge by a 2-segment polyline, underlining the
   * label, then going to the closest point of spline. 
   */
  void SetDecorate(bool val) {
    AddBoolAttribute(AttributeType::DECORATE, val);
  }

  /**
   * Set edge type for drawing arrowheads. This indicates which ends of the edge
   * should be decorated with an arrowhead. The actual style of the arrowhead
   * can be specified using the arrowhead and arrowtail attributes.
   */
  void SetDir(DirType::e val) {
    AddEnumAttribute<DirType::e, DirType>(AttributeType::DIR, val);
  }

  /**
   * If edgeURL is defined, this is the link used for the non-label parts of an
   * edge. This value overrides any URL defined for the edge. Also, this value
   * is used near the head or tail node unless overridden by a headURL or
   * tailURL value, respectively.
   */
  void SetEdgeURL(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::EDGEURL, val);
  }

  /**
   * If the edge has a URL or edgeURL attribute, this attribute determines which
   * window of the browser is used for the URL attached to the non-label part of
   * the edge. Setting it to "_graphviz" will open a new window if it doesn't
   * already exist, or reuse it if it does. If undefined, the value of the
   * target is used. 
   */
  void SetEdgeTarget(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::EDGETARGET, val);
  }

  /**
   * Tooltip annotation attached to the non-label part of an edge. This is used
   * only if the edge has a URL or edgeURL attribute. 
   */
  void SetEdgeTooltip(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::EDGETOOLTIP, val);
  }

  /**
   * Color used to fill the background of a node or cluster assuming
   * style=filled, or a filled arrowhead. If fillcolor is not defined, color is
   * used. (For clusters, if color is not defined, bgcolor is used.) If this is
   * not defined, the default is used, except for shape=point or when the output
   * format is MIF, which use black by default.
   *
   * If the value is a colorList, a gradient fill is used. By default, this is a
   * linear fill; setting style=radial will cause a radial fill. At present,
   * only two colors are used. If the second color (after a colon) is missing,
   * the default color is used for it. See also the gradientangle attribute for
   * setting the gradient angle.
   *
   * Note that a cluster inherits the root graph's attributes if defined. Thus,
   * if the root graph has defined a fillcolor, this will override a color or
   * bgcolor attribute set for the cluster. 
   */
  void SetFillColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::FILLCOLOR, val);
  }
  void SetFillColor(const std::vector<Color::e> vals) {
    AddEnumListAttribute<Color::e, Color>(AttributeType::FILLCOLOR, vals);
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
    AddSimpleAttribute<std::string>(AttributeType::FONTNAME, val);
  }

  /**
   * Font size, in points, used for text. 
   */
  void SetFontSize(double val) {
    AddSimpleAttribute<double>(AttributeType::FONTSIZE, val);
  }

  /**
   * If headURL is defined, it is output as part of the head label of the edge.
   * Also, this value is used near the head node, overriding any URL value. 
   */
  void SetHeadURL(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::HEADURL, val);
  }

  /**
   * If true, the head of an edge is clipped to the boundary of the head node;
   * otherwise, the end of the edge goes to the center of the node, or the
   * center of a port, if applicable. 
   */
  void SetHeadClip(bool val) {
    AddBoolAttribute(AttributeType::HEADCLIP, val);
  }

  /**
   * Text label to be placed near head of edge.
   */
  void SetHeadLabel(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::HEADLABEL, val);
  }

  /**
   * Indicates where on the head node to attach the head of the edge. In the
   * default case, the edge is aimed towards the center of the node, and then
   * clipped at the node boundary. 
   */
  void SetHeadPort(CompassPoint::e val) {
    AddEnumAttribute<CompassPoint::e, CompassPoint>(AttributeType::HEADPORT,
      val);
  }

  /**
   * If the edge has a headURL, this attribute determines which window of the
   * browser is used for the URL. Setting it to "_graphviz" will open a new
   * window if it doesn't already exist, or reuse it if it does. If undefined,
   * the value of the target is used. 
   */
  void SetHeadTarget(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::HEADTARGET, val);
  }

  /**
   * Tooltip annotation attached to the head of an edge. This is used only if
   * the edge has a headURL attribute. 
   */
  void SetHeadTooltip(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::HEADTOOLTIP, val);
  }

  /**
   * If labelURL is defined, this is the link used for the label of an edge.
   * This value overrides any URL defined for the edge. 
   */
  void SetLabelURL(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::LABELURL, val);
  }

  /**
   * This, along with labeldistance, determine where the headlabel (taillabel)
   * are placed with respect to the head (tail) in polar coordinates. The origin
   * in the coordinate system is the point where the edge touches the node. The
   * ray of 0 degrees goes from the origin back along the edge, parallel to the
   * edge at the origin.
   *
   * The angle, in degrees, specifies the rotation from the 0 degree ray, with
   * positive angles moving counterclockwise and negative angles moving
   * clockwise. 
   */
  void SetLabelAngle(double val) {
    AddSimpleAttribute<double>(AttributeType::LABELANGLE, val);
  }

  /**
   * Multiplicative scaling factor adjusting the distance that the
   * headlabel(taillabel) is from the head(tail) node. The default distance is
   * 10 points. See labelangle for more details. 
   */
  void SetLabelDistance(double val) {
    AddSimpleAttribute<double>(AttributeType::LABELDISTANCE, val);
  }

  /**
   * If true, allows edge labels to be less constrained in position. In
   * particular, it may appear on top of other edges. 
   */
  void SetLabelFloat(bool val) {
    AddBoolAttribute(AttributeType::LABELFLOAT, val);
  }

  /**
   * Color used for headlabel and taillabel. If not set, defaults to edge's
   * fontcolor. 
   */
  void SetLabelFontColor(Color::e val) {
    AddEnumAttribute<Color::e, Color>(AttributeType::LABELFONTCOLOR, val);
  }

  /**
   * Font used for headlabel and taillabel. If not set, defaults to edge's
   * fontname. 
   */
  void SetLabelFontName(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::LABELFONTNAME, val);
  }

  /**
   * Font size, in points, used for headlabel and taillabel. If not set,
   * defaults to edge's fontsize. 
   */
  void SetLabelFontSize(double val) {
    AddSimpleAttribute<double>(AttributeType::LABELFONTSIZE, val);
  }

  /**
   * If the edge has a URL or labelURL attribute, this attribute determines
   * which window of the browser is used for the URL attached to the label.
   * Setting it to "_graphviz" will open a new window if it doesn't already
   * exist, or reuse it if it does. If undefined, the value of the target is
   * used. 
   */
  void SetLabelTarget(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::LABELTARGET, val);
  }

  /**
   * Tooltip annotation attached to label of an edge. This is used only if the
   * edge has a URL or labelURL attribute. 
   */
  void SetLabelTooltip(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::LABELTOOLTIP, val);
  }

  /**
   * Preferred edge length, in inches. 
   */
  void SetLen(double val) {
    AddSimpleAttribute<double>(AttributeType::LEN, val);
  }

  /**
   * Logical head of an edge. When compound is true, if lhead is defined and is
   * the name of a cluster containing the real head, the edge is clipped to the
   * boundary of the cluster.
   */
  void SetLHead(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::LHEAD, val);
  }

  /**
   * Label position, in points. The position indicates the center of the label.
   */
  void SetLP(double x, double y) {
    AddPointAttribute(AttributeType::LP, x, y);
  }

  /**
   * Logical tail of an edge. When compound is true, if ltail is defined and is
   * the name of a cluster containing the real tail, the edge is clipped to the
   * boundary of the cluster.
   */
  void SetLTail(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::LTAIL, val);
  }

  /**
   * Minimum edge length (rank difference between head and tail). 
   */
  void SetMinLen(int val) {
    AddSimpleAttribute<int>(AttributeType::MINLEN, val);
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
    AddBoolAttribute(AttributeType::NOJUSTIFY, val);
  }

  /**
   * Specifies the width of the pen, in points, used to draw lines and curves,
   * including the boundaries of edges and clusters. The value is inherited by
   * subclusters. It has no effect on text.
   *
   * Previous to 31 January 2008, the effect of penwidth=W was achieved by
   * including setlinewidth(W) as part of a style specification. If both are
   * used, penwidth will be used. 
   */
  void SetPenWidth(double val) {
    AddSimpleAttribute<double>(AttributeType::PENWIDTH, val);
  }

  /**
   * Position of node, or spline control points. For nodes, the position
   * indicates the center of the node. On output, the coordinates are in points.
   *
   * In neato and fdp, pos can be used to set the initial position of a node. By
   * default, the coordinates are assumed to be in inches. However, the -s
   * command line flag can be used to specify different units. As the output
   * coordinates are in points, feeding the output of a graph laid out by a
   * Graphviz program into neato or fdp will almost always require the -s flag.
   *
   * When the -n command line flag is used with neato, it is assumed the
   * positions have been set by one of the layout programs, and are therefore in
   * points. Thus, neato -n can accept input correctly without requiring a -s
   * flag and, in fact, ignores any such flag. 
   */
  void SetPos(double x, double y) {
    AddPointAttribute(AttributeType::POS, x, y);
  }
  void SetPos(SplineType::e val) {
    AddEnumAttribute<SplineType::e, SplineType>(AttributeType::POS, val);
  }
  void SetPos(QuadType::e val) {
    AddEnumAttribute<QuadType::e, QuadType>(AttributeType::POS, val);
  }

  /**
   * Edges with the same head and the same samehead value are aimed at the same
   * point on the head. This has no effect on loops.
   */
  void SetSameHead(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::SAMEHEAD, val);
  }

  /**
   * Edges with the same tail and the same sametail value are aimed at the same
   * point on the tail. This has no effect on loops.
   */
  void SetSameTail(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::SAMETAIL, val);
  }

  /**
   * Print guide boxes in PostScript at the beginning of routesplines if 1, or
   * at the end if 2. (Debugging) 
   */
  void SetShowBoxes(unsigned val) {
    if (val > 2) val = 2;
    AddSimpleAttribute<unsigned>(AttributeType::SHOWBOXES, val);
  }

  /**
   * Set style information for components of the graph. For cluster subgraphs,
   * if "filled", the cluster box's background is filled.
   *
   * Syntax is styleItem ( ',' styleItem )*
   * where styleItem = name or name'('args')'
   * and args  = name ( ',' name )*
   * and name can be any string of characters not containing a space, a left or
   * right parenthesis, or a comma. Note that whitespace characters are ignored.
   *
   * At present, the recognized style names are "dashed", "dotted", "solid",
   * "invis" and "bold" for nodes and edges, "tapered" for edges only, and
   * "filled", "diagonals" and "rounded" for nodes only. The styles "filled" and
   * "rounded" are recognized for clusters. The style "radial" is recognized for
   * nodes, clusters and graphs, and indicates a radial-style gradient fill if
   * applicable. Additional styles are available in device-dependent form. Style
   * lists are passed to device drivers, which can use this to generate
   * appropriate output.
   *
   * The setlinewidth style value can be used for more control over the width of
   * node borders and edges than is allowed by bold. This style value takes an
   * argument, specifying the width of the line in points. For example,
   * style="bold" is equivalent to style="setlinewidth(2)". The use of
   * setlinewidth is deprecated; one should use the penwidth attribute instead. 
   *
   * TODO(jvilk): Make a custom object for this for validation purposes.
   */
  void SetStyle(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::STYLE, val);
  }

  /**
   * If tailURL is defined, it is output as part of the tail label of the edge.
   * Also, this value is used near the tail node, overriding any URL value.
   */
  void SetTailURL(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::TAILURL, val);
  }

  /**
   * If true, the tail of an edge is clipped to the boundary of the tail node;
   * otherwise, the end of the edge goes to the center of the node, or the
   * center of a port, if applicable. 
   */
  void SetTailClip(bool val) {
    AddBoolAttribute(AttributeType::TAILCLIP, val);
  }

  /**
   * Text label to be placed near tail of edge.
   */
  void SetTailLabel(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::TAILLABEL, val);
  }

  /**
   * Indicates where on the tail node to attach the tail of the edge. 
   */
  void SetTailPort(CompassPoint::e val) {
    AddEnumAttribute<CompassPoint::e, CompassPoint>(AttributeType::TAILPORT,
      val);
  }

  /**
   * If the edge has a tailURL, this attribute determines which window of the
   * browser is used for the URL. Setting it to "_graphviz" will open a new
   * window if it doesn't already exist, or reuse it if it does. If undefined,
   * the value of the target is used. 
   */
  void SetTailTarget(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::TAILTARGET, val);
  }

  /**
   * Tooltip annotation attached to the tail of an edge. This is used only if
   * the edge has a tailURL attribute. 
   */
  void SetTailToolTip(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::TAILTOOLTIP, val);
  }  

  /**
   * If the object has a URL, this attribute determines which window of the
   * browser is used for the URL. See W3C documentation. 
   */
  void SetTarget(std::string val) {
    AddSimpleAttribute<std::string>(AttributeType::STYLESHEET, val);
  }

  /**
   * Tooltip annotation attached to the node or edge. If unset, Graphviz will
   * use the object's label if defined. Note that if the label is a record
   * specification or an HTML-like label, the resulting tooltip may be
   * unhelpful. In this case, if tooltips will be generated, the user should set
   * a tooltip attribute explicitly. 
   */
  void SetTooltip(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::TOOLTIP, val);
  }

  /**
   * Weight of edge. In dot, the heavier the weight, the shorter, straighter and
   * more vertical the edge is. N.B. Weights in dot must be integers. For other
   * layouts, a larger weight encourages the layout to make the edge length
   * closer to that specified by the len attribute. 
   */
  void SetWeight(double val) {
    AddSimpleAttribute<double>(AttributeType::WEIGHT, val);
  }

  /**
   * External label for a node or edge. For nodes, the label will be placed
   * outside of the node but near it. For edges, the label will be placed near
   * the center of the edge. This can be useful in dot to avoid the occasional
   * problem when the use of edge labels distorts the layout. For other layouts,
   * the xlabel attribute can be viewed as a synonym for the label attribute.
   *
   * These labels are added after all nodes and edges have been placed. The
   * labels will be placed so that they do not overlap any node or label. This
   * means it may not be possible to place all of them. To force placing all of
   * them, use the forcelabels attribute. 
   */
  void SetXLabel(std::string val) {
    val = SanitizeString(val);
    AddSimpleAttribute<std::string>(AttributeType::XLABEL, val);
  }
};

}  //namespace DotWriter

#endif