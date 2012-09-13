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
#include <sstream>
#include <vector>

#include "Enums.h"

namespace DotWriter {

/**
 * Attributes are just a double of an AttributeName enum
 * and an appropriate value. Type is encoded in subtype.
 */
class Attribute {
private:
  AttributeType::e _type;

public:
  /**
   * Constructs an attribute with the given type.
   */
  Attribute(AttributeType::e type) : _type(type) {
  }

  virtual ~Attribute() {};

  /**
   * Returns the name of the attribute.
   */
  const char* GetName() {
    return AttributeType::ToString(_type);
  }

  /**
   * Converts the attribute to string form for the DOT file, e.g.:
   * AttributeName = value
   */
  virtual std::string ToString() = 0;
};

/**
 * Stores an enum attribute for the given enum.
 * T is the enum type, and F is the struct that houses its ToString() method.
 */
template <typename T, typename F>
class EnumAttribute : public Attribute {
private:
  T _value;

public:
  EnumAttribute(AttributeType::e type, T value) : Attribute(type),
    _value(value) {
  }

  void SetValue(T value) {
    _value = value;
  }

  std::string ToString() {
    std::stringstream ss;
    ss << GetName() << "=" << F::ToString(_value);
    return ss.str();
  }
};

/**
 * This attribute is represented as a list of enums.
 * T is the enum type, and F is the struct that houses its ToString() method.
 */
template <typename T, typename F>
class EnumListAttribute : public Attribute {
private:
  std::vector<T> _values;

public:
  EnumListAttribute(AttributeType::e type, T value) : Attribute(type) {
    AddValue(value);
  }

  EnumListAttribute(AttributeType::e type, const std::vector<T>& values) :
    Attribute(type) {
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

  std::string ToString() {
    if (_values.empty()) return "";

    std::stringstream ss;
    ss << GetName() << "=";

    // Print a colon-separated list of the values.
    typename std::vector<T>::iterator it;
    for (it = _values.begin(); it != _values.end(); it++) {
      T currentVal = *it;
      
      ss << F::ToString(currentVal);

      // The last value does not need a colon after it.
      if (it+1 != _values.end())
        ss << ":";
    }

    return ss.str();
  }
};

/**
 * Represents an attribute for attributes that can be represented as first-class
 * datatypes in C++, e.g. int, double, etc.
 */
template <typename T>
class FirstClassAttribute : public Attribute {
private:
  T _value;

public:
  FirstClassAttribute(AttributeType::e type, T value) : Attribute(type),
    _value(value) {

  }

  T GetValue() {
    return _value;
  }

  std::string ToString() {
    std::stringstream ss;
    ss << GetName() << "=" << _value;
    return ss.str();
  }
};

/**
 * This attribute is represented as a list of first-class C++ datatypes.
 */
template <typename T>
class FirstClassListAttribute : public Attribute {
private:
  std::vector<T> _values;

public:
  FirstClassListAttribute(AttributeType::e type, T value) : Attribute(type) {
    AddValue(value);
  }

  FirstClassListAttribute(AttributeType::e type, const std::vector<T>& values) :
    Attribute(type) {
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

  std::string ToString() {
    if (_values.empty()) return "";

    std::stringstream ss;
    ss << GetName() << "=";

    // Print a colon-separated list of the values.
    typename std::vector<T>::iterator it;
    for (it = _values.begin(); it != _values.end(); it++) {
      T currentVal = *it;
      
      ss << currentVal;

      // The last value does not need a colon after it.
      if (it+1 != _values.end())
        ss << ":";
    }

    return ss.str();
  }
};

class AddDoubleAttribute : public Attribute {
private:
  double _value;

public:
  AddDoubleAttribute(AttributeType::e type, double value) : Attribute(type),
    _value(value) {
  }

  void SetValue(double value) {
    _value = value;
  }

  std::string ToString() {
    std::stringstream ss;
    ss << GetName() << "=+" << _value;
    return ss.str();
  }
};

class PointAttribute : public Attribute {
private:
  double _x, _y;

public:
  PointAttribute(AttributeType::e type, double x, double y) : Attribute(type),
    _x(x), _y(y) {
  }

  void SetX(double x) {
    _x = x;
  }

  void SetY(double y) {
    _y = y;
  }

  std::string ToString() {
    std::stringstream ss;
    ss << GetName() << "=" << _x << "," << _y;
    return ss.str();
  }
};

class AddPointAttribute : public Attribute {
private:
  double _x, _y;

public:
  AddPointAttribute(AttributeType::e type, double x, double y) : Attribute(type),
    _x(x), _y(y) {
  }

  void SetX(double x) {
    _x = x;
  }

  void SetY(double y) {
    _y = y;
  }

  std::string ToString() {
    std::stringstream ss;
    ss << GetName() << "=+" << _x << "," << _y;
    return ss.str();
  }
};

class PointListAttribute : public Attribute {
private:
  std::vector<std::pair<double, double> > _values;

public:
  PointListAttribute(AttributeType::e type, double x, double y) :
    Attribute(type)
  {
    AddPoint(x,y);
  }

  void AddPoint(double x, double y) {
    _values.push_back(std::pair<double,double>(x, y));
  }

  std::string ToString() {
    std::stringstream ss;
    ss << GetName() << "=";

    // This is a *space* separated list.
    std::vector<std::pair<double, double> >::iterator it;
    for (it = _values.begin(); it != _values.end(); it++) {
      std::pair<double, double> currentVal = *it;
      ss << currentVal.first << "," << currentVal.second;

      // The last value does not need a space after it.
      if (it+1 != _values.end()) {
        ss << " ";
      }
    }

    return ss.str();
  }
};

}  // namespace DotWriter

#endif