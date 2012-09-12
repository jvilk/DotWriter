/**
 * Stores a set of DOT attributes for various types of objects.
 * Handles validating input attributes to ensure they are appropriate
 * for the given object.
 *
 * Attribute descriptions lifted from:
 * http://www.graphviz.org/content/attrs
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

namespace DotWriter {

class InvalidAttributeValueException : public std::runtime_error
{
private:
  AttributeType::e _type;

public:
  InvalidAttributeValueException(AttributeType::e type,
    const std::string& message) : std::runtime_error(message),
    _type(type) {
  }

  virtual const char* what() const throw()
  {
    return "";
  }
};

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
  void SetLabelLoc(std::string val);
  void SetLandscape(bool val);
  //Layers
  //Layerselect
  //Layersep
  void SetLayout(std::string val);
  void SetLevels(int val);
  void SetLevelsGap(double val);
  void SetLHeight(double val);
  void SetLP(double x, double y);
  void SetLWidth(double val);
  void SetMargin(double val);
  void SetMargin(double x, double y);
  void SetMaxIter(int val);
  void SetMCLimit(double val);
  void SetMinDist(double val);
  void SetMode(std::string val);
  void SetModel(std::string val);
  void SetMosek(bool val);
  void SetNodeSep(double val);
  void SetNoJustify(bool val);
  void SetNormalize(bool val);
  void SetNsLimit(double val);
  void SetNsLimit1(double val);
  void SetOrdering(std::string val);
  void SetOrientation(std::string val);
  void SetOutputOrder(OutputMode::e val);
  void SetOverlap(std::string val);
  void SetOverlap(bool val);
  void SetOverlapScaling(double val);
  void SetOverlapScaling(bool val);
  void SetPack(bool val);
  void SetPack(int val);
  void SetPackMode(PackMode::e val);
  void SetPackMode(double val);
  void SetPad(double val);
  void SetPad(double x, double y);
  void SetPage(double val);
  void SetPage(double x, double y);
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