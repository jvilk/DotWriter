#ifndef DOTWRITER_IDMANAGER_H
#define DOTWRITER_IDMANAGER_H

#include <string>
#include <sstream>
#include <set>

namespace DotWriter {

/**
 * Ensures that no two IDs in a graph are the same.
 * This object also manages the memory for each ID.
 */
class IdManager {
private:
  unsigned long _nextNodeIdNum;
  unsigned long _nextSubgraphIdNum;
  // This is the next number to append to a non-unique custom ID supplied by the
  // user. Why just one count? It's simpler, and guarantees that I don't try the
  // same number twice. 
  // So if a user tries to create many nodes with the ID "foo", I don't retry
  // "foo0" as an alternative more than once across all attempts to use that ID.
  unsigned long _nextCustomIdNum;
  std::set<std::string> _existingIds;

  /** Use these functions to access the above ID counters within this class. **/

  unsigned long GetNextNodeIdNum() {
    return _nextNodeIdNum++;
  }

  unsigned long GetNextSubgraphIdNum() {
    return _nextSubgraphIdNum++;
  }

  unsigned long GetNextCustomIdNum() {
    return _nextCustomIdNum++;
  }

  /**
   * Registers the given ID with the ID manager.
   * Returns the string, and mutates success with whether or not the
   * registration succeeded.
   */
  const std::string& RegisterId(bool* success, const std::string& id) {
    std::pair<std::set<std::string>::iterator, bool> retVal =
        _existingIds.insert(id);

    *success = retVal.second;
    return *retVal.first;
  }

public:
  IdManager() : _nextNodeIdNum(0), _nextSubgraphIdNum(0), _nextCustomIdNum(0) {

  }

  virtual ~IdManager() {};

  /**
   * Get a unique node ID. Used when the user does not specify
   * an ID.
   */
  const std::string& GetNodeId();

  /**
   * Get a unique subgraph ID. Used when the user does not specify an
   * ID.
   */
  const std::string& GetSubgraphId();

  /**
   * This is used to validate IDs specified by the user of the API
   * (e.g. not retrieved through the GetId functions provided by this class).
   * This checks if the ID is unique. If it is not, it will append a number to
   * it until it is unique.
   */
  const std::string& ValidateCustomId(std::string customId);
};

}  // namespace DotWriter

#endif