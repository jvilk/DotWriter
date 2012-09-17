#ifndef DOTWRITER_IDABLE_H_
#define DOTWRITER_IDABLE_H_

#include<string>

namespace DotWriter {

/**
 * Interface for idable objects.
 */
class Idable {
private:
  const std::string& _id;  // Managed by a IdManager created by a RootGraph

public:
  Idable(const std::string& id) : _id(id) {};

  const std::string& GetId() {
    return _id;
  }
};

}  // namespace DotWriter

#endif  // DOTWRITER_IDABLE_H_