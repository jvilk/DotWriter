/**
 * Contains a definition for all of the base attribute types.
 * As a result, this is a large file. Sorry.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_ATTRIBUTE_H_
#define DOTWRITER_ATTRIBUTE_H_

#include <algorithm>
#include <string>
#include <ostream>
#include <vector>

#include "Enums.h"

namespace DotWriter {

/**
 * Attributes contain an attribute name and a value.
 */
class Attribute {
protected:
  /**
   * Print the name of this attribute to the given output stream.
   */
  virtual void PrintName(std::ostream& out) = 0;

  /**
   * Print the value of this attribute to the given output stream.
   */
  virtual void PrintValue(std::ostream& out) = 0;

public:
  Attribute() {}
  virtual ~Attribute() {}

  /**
   * Prints the DOT representation of this attribute to the output stream.
   * (e.g. Name = "Value")
   */
  virtual void Print(std::ostream& out) {
    PrintName(out);
    out << "=\"";
    PrintValue(out);
    out << "\"";
  }
};

/**
 * Standard attributes are officially documented on the GraphViz webpage
 * and have an AttributeType value.
 */
class StandardAttribute : public Attribute {
private:
  AttributeType::e _type;

protected:
  virtual void PrintName(std::ostream& out) {
    out << AttributeType::ToString(_type);
  }

public:
  /**
   * Constructs an attribute with the given type.
   */
  StandardAttribute(AttributeType::e type) : _type(type) {}
  virtual ~StandardAttribute() {}

  AttributeType::e GetType() {
    return _type;
  }
};

/**
 * A custom attribute is just two strings.
 */
class CustomAttribute : public Attribute {
private:
  std::string _type;
  std::string _value;

protected:
  virtual void PrintName(std::ostream& out) {
    out << _type;
  }

  virtual void PrintValue(std::ostream& out) {
    out << _value;
  }

public:
  /**
   * Constructs a custom attribute with the given type and value.
   */
  CustomAttribute(const std::string& type, const std::string& value) :
    _type(type), _value(value) {}

  virtual ~CustomAttribute() {}

  std::string GetName() {
    return _type;
  }
};

/**
 * Stores an enum attribute for the given enum.
 * T is the enum type, and F is the struct that houses its ToString() method.
 */
template <typename T, typename F>
class EnumAttribute : public StandardAttribute {
private:
  T _value;

protected:
  virtual void PrintValue(std::ostream& out) {
    out << F::ToString(_value);
  }

public:
  EnumAttribute(AttributeType::e type, T value) : StandardAttribute(type),
    _value(value) {
  }

  void SetValue(T value) {
    _value = value;
  }
};

/**
 * This attribute is represented as a list of enums.
 * T is the enum type, and F is the struct that houses its ToString() method.
 */
template <typename T, typename F>
class EnumListAttribute : public StandardAttribute {
private:
  std::vector<T> _values;

protected:
  virtual void PrintValue(std::ostream& out) {
    if (_values.empty()) return;

    // Print a colon-separated list of the values.
    typename std::vector<T>::iterator it;
    for (it = _values.begin(); it != _values.end(); it++) {
      T currentVal = *it;

      out << F::ToString(currentVal);

      // The last value does not need a colon after it.
      if (it+1 != _values.end())
        out << ":";
    }
  }

public:
  EnumListAttribute(AttributeType::e type, T value) : StandardAttribute(type) {
    AddValue(value);
  }

  EnumListAttribute(AttributeType::e type, const std::vector<T>& values) :
    StandardAttribute(type) {
    // Copies contents of the input vector.
    _values = values;
  }

  void RemoveValue(T value) {
    typename std::vector<T>::iterator loc =
      std::find(_values.begin(), _values.end(), value);

    if (loc != _values.end()) {
      _values.erase(loc);
    }
  }

  void AddValue(T value) {
    _values.push_back(value);
  }
};

/**
 * Used for boolean attributes
 */
class BoolAttribute : public StandardAttribute {
private:
  bool _value;

protected:
  virtual void PrintValue(std::ostream& out) {
    out << (_value ? "true" : "false");
  }

public:
  BoolAttribute(AttributeType::e type, bool value) : StandardAttribute(type),
    _value(value) {
  }

  bool GetValue() {
    return _value;
  }
};

/**
 * Used for attributes that can just be piped to an output stream with no fuss.
 */
template <typename T>
class SimpleAttribute : public StandardAttribute {
private:
  T _value;

protected:
  virtual void PrintValue(std::ostream& out) {
    out << _value;
  }

public:
  SimpleAttribute(AttributeType::e type, T value) : StandardAttribute(type),
    _value(value) {
  }

  T GetValue() {
    return _value;
  }
};

/**
 * Same as SimpleAttribute, but it's a list of those sort of attributes.
 */
template <typename T>
class SimpleListAttribute : public StandardAttribute {
private:
  std::vector<T> _values;

protected:
  virtual void PrintValue(std::ostream& out) {
    if (_values.empty()) return;

    // Print a colon-separated list of the values.
    typename std::vector<T>::iterator it;
    for (it = _values.begin(); it != _values.end(); it++) {
      T currentVal = *it;

      out << currentVal;

      // The last value does not need a colon after it.
      if (it+1 != _values.end())
        out << ":";
    }
  }

public:
  SimpleListAttribute(AttributeType::e type, T value) : StandardAttribute(type) {
    AddValue(value);
  }

  SimpleListAttribute(AttributeType::e type, const std::vector<T>& values) :
    StandardAttribute(type) {
    // Copies contents of the input vector.
    _values = values;
  }

  void RemoveValue(T value) {
    typename std::vector<T>::iterator loc =
      std::find(_values.begin(), _values.end(), value);

    if (loc != _values.end()) {
      _values.erase(loc);
    }
  }

  void AddValue(T value) {
    _values.push_back(value);
  }
};

class AddDoubleAttribute : public SimpleAttribute<double> {
protected:
  virtual void PrintValue(std::ostream& out) {
    out << "+";
    SimpleAttribute<double>::PrintValue(out);
  }

public:
  AddDoubleAttribute(AttributeType::e type, double value) :
    SimpleAttribute<double>(type, value) {
  }
};

class PointAttribute : public StandardAttribute {
private:
  double _x, _y;

protected:
  virtual void PrintValue(std::ostream& out) {
    out << _x << "," << _y;
  }

public:
  PointAttribute(AttributeType::e type, double x, double y) :
    StandardAttribute(type), _x(x), _y(y) {
  }
};

class AddPointAttribute : public PointAttribute {
protected:
  virtual void PrintValue(std::ostream& out) {
    out << "+";
    PointAttribute::PrintValue(out);
  }

public:
  AddPointAttribute(AttributeType::e type, double x, double y) :
    PointAttribute(type, x, y) {
  }
};

class PointListAttribute : public StandardAttribute {
private:
  std::vector<std::pair<double, double> > _values;

protected:
  virtual void PrintValue(std::ostream& out) {
    if (_values.empty()) return;

    // This is a *space* separated list.
    std::vector<std::pair<double, double> >::iterator it;
    for (it = _values.begin(); it != _values.end(); it++) {
      std::pair<double, double> currentVal = *it;
      out << currentVal.first << "," << currentVal.second;

      // The last value does not need a space after it.
      if (it+1 != _values.end()) {
        out << " ";
      }
    }
  }

public:
  PointListAttribute(AttributeType::e type, double x, double y) :
    StandardAttribute(type) {
    AddPoint(x,y);
  }

  void AddPoint(double x, double y) {
    _values.push_back(std::pair<double,double>(x, y));
  }
};

}  // namespace DotWriter

#endif
