#pragma once

#include "ATypeCharacterNodeModel.hpp"
#include "NodeData.hpp"

class ATypeCharacterData : public NodeData {
  public:
  ATypeCharacterData() {}
  ATypeCharacterData(ATypeCharacterNodeModel *character) : _character(character) {}

  NodeDataType type() const override { return NodeDataType("atypeCharacter", "Atype Character"); }

  ATypeCharacterNodeModel *getCharacter() { return _character; }

  protected:
  ATypeCharacterNodeModel *_character;
};
