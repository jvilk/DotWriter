/**
 * Contains a definition for all of the base attribute types.
 * As a result, this is a large file. Sorry.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_ATTRIBUTE_H_
#define DOTWRITER_ATTRIBUTE_H_

#include <string>

namespace DotWriter {

/**
 * Attributes are just a double of an AttributeName enum
 * and an appropriate value. Type is encoded in subtype.
 */
class Attribute {
private:

protected:
  Attribute();

public:
  virtual std::string ToString() = 0;
};

// Have subtypes.
// Have a factory?
// User just wants to create a thing. Attribute::MakeDamping Attribute::MakeArrowSize.
// They alias methods n such.
// Probably should return *value*?  Otherwise user / program manages memory.
// Factory classes need to take in the appropriate type (double / int / etc).

/**
Attribute is a pairing of a ATTRIBUTE enum, and some value.

Layer-based attributes should be protected and only created by nodes n' such
at time of ToString.

Unique types:
  Internal      User-facing
* addDouble     double
* addPoint      Point (create class)
* arrowType     Enum
* aspectType    Enum
* bool          bool
* clusterMode   enum
* color         enum
* colorList     vector of enums
* dirType       ???
* double        double
* doubleList    vector of doubles
* escString     string 
* int           int
// Think about layers afterward.
* layerList     ???
* layerRange 

* lblString     Treat as escString.
* outputMode    enum
* packMode      enum
* pagedir       enum
* point         Point (create class)
* pointList     vector of points
* portPos       compass point for now (ignoring complex bs)
* quadType      enum
* rankdir       enum
* rankType      enum
* rect          Deal with later
* shape         Enum for now (could be user-defined; deal with later)
* smoothType    enum
* splineType    Deal with later
* startType     Enum and unsigned int. Both are optional
* style         list of enums
* string        string
* viewPort      Deal with later.


*/

}  // namespace DotWriter

#endif