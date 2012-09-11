/**
 * Stores a set of DOT attributes for various types of objects.
 * Handles validating input attributes to ensure they are appropriate
 * for the given object.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_ATTRIBUTESET_H_
#define DOTWRITER_ATTRIBUTESET_H_

#include "Attribute.h"
#include "Enums.h"

namespace DotWriter {

class GraphAttributeSet {
private:
  std::vector<Attribute*> _attributes;

public:
  GraphAttributeSet() { };

  void SetDamping(double val);
  void SetK(double val);
  void SetURL(std::string val);
  void SetAspect(AspectType::e val);
  //void SetBB(Rect val);
  void SetBGColor(Color::e val);
  void SetBGColor(const std::vector<Color::e> vals);
  void ShouldCenter(bool val);
  void SetCharSet(std::string val);
  void SetClusterRank(ClusterMode::e val);
  void SetColorScheme(std::string val);
  void SetComment(std::string val);
  void ShouldCompound(bool val);
  void ShouldConcentrate(bool val);
  void SetDefaultDist(double val);
  void SetDim(int val);
  void SetDimen(int val);
  void SetDirEdgeConstraints(bool value);
  void SetDirEdgeConstraints(std::string val);
  void SetDPI(double value);
  void SetEpsilon(double value);
  void SetEsep(double value);
  void SetFontColor(Color::e value);
  void SetFontName(std::string value);
  void SetFontNames(std::string value);
  void SetFontPath(std::string value);
  void SetFontSize(double value);
  void ShouldForceLabels(bool value);
  void SetGradientAngle(int value);
  void SetHref(std::string value);
  //void SetId(std::string value);
  void SetImagePath(std::string value);
  //void SetLabel(std::string value);
  void SetLabelScheme(int value);
  void SetLabelJust(std::string value);
  void SetLabelLoc(std::string value);
  void SetLandscape(bool value);
  //Layers
  //Layerselect
  //Layersep
  void SetLayout(std::string value);
  void SetLevels(int value);
  void SetLevelsGap(double value);
  void SetLHeight(double value);
  void SetLP(double x, double y);
  void SetLWidth(double value);
  void SetMargin(double value);
  void SetMargin(double x, double y);
  
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