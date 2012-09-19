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
   * Get the type of the attribute.
   */
  AttributeType::e GetType() {
    return _type;
  }

  /**
   * Prints the DOT representation of this attribute to the output stream.
   * (e.g. AttributeName = Value)
   */
  virtual void Print(std::ostream& out) = 0;
};

/**
 * A custom attribute is just two strings.
 */
class CustomAttribute : public Attribute {
private:
  std::string _type;
  std::string _value;

public:
  /**
   * Constructs a custom attribute with the given type and value.
   */
  CustomAttribute(const std::string& type, const std::string& value) :
    // TODO(jvilk): Make base Attribute class dumber; currently wasting
    // space on having an enum in there.
    Attribute(AttributeType::AREA), _type(type), _value(value) {
  }

  virtual ~CustomAttribute() {};

  /**
   * Prints the DOT representation of this attribute to the output stream.
   */
  virtual void Print(std::ostream& out) {
    out << _type << "=" << "\"" << _value << "\"";
  };
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

  virtual void Print(std::ostream& out) {
    if (_value != F::DEFAULT) {
      out << GetName() << "=\"" << F::ToString(_value) << "\"";
    }
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

  virtual void Print(std::ostream& out) {
    if (_values.empty()) return;

    out << GetName() << "=\"";

    // Print a colon-separated list of the values.
    typename std::vector<T>::iterator it;
    for (it = _values.begin(); it != _values.end(); it++) {
      T currentVal = *it;

      out << F::ToString(currentVal);

      // The last value does not need a colon after it.
      if (it+1 != _values.end())
        out << ":";
    }
    out << "\"";
  }
};

/**
 * Used for boolean attributes
 */
class BoolAttribute : public Attribute {
private:
  bool _value;

public:
  BoolAttribute(AttributeType::e type, bool value) : Attribute(type),
    _value(value) {

  }

  bool GetValue() {
    return _value;
  }

  virtual void Print(std::ostream& out) {
    out << GetName() << "=" << (_value ? "true" : "false");
  }
};

/**
 * Used for attributes that can just be piped to an output stream with no fuss.
 */
template <typename T>
class SimpleAttribute : public Attribute {
private:
  T _value;

public:
  SimpleAttribute(AttributeType::e type, T value) : Attribute(type),
    _value(value) {

  }

  T GetValue() {
    return _value;
  }

  virtual void Print(std::ostream& out) {
    out << GetName() << "=\"" << _value << "\"";
  }
};

/**
 * Same as SimpleAttribute, but it's a list of those sort of attributes.
 */
template <typename T>
class SimpleListAttribute : public Attribute {
private:
  std::vector<T> _values;

public:
  SimpleListAttribute(AttributeType::e type, T value) : Attribute(type) {
    AddValue(value);
  }

  SimpleListAttribute(AttributeType::e type, const std::vector<T>& values) :
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

  virtual void Print(std::ostream& out) {
    if (_values.empty()) return;

    out << GetName() << "=\"";

    // Print a colon-separated list of the values.
    typename std::vector<T>::iterator it;
    for (it = _values.begin(); it != _values.end(); it++) {
      T currentVal = *it;

      out << currentVal;

      // The last value does not need a colon after it.
      if (it+1 != _values.end())
        out << ":";
    }
    out << "\"";
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

  virtual void Print(std::ostream& out) {
    out << GetName() << "=+" << _value;
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

  virtual void Print(std::ostream& out) {
    out << GetName() << "=" << _x << "," << _y;
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

  virtual void Print(std::ostream& out) {
    out << GetName() << "=+" << _x << "," << _y;
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

  virtual void Print(std::ostream& out) {
    out << GetName() << "=\"";

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
    out << "\"";
  }
};

}  // namespace DotWriter

#endif
