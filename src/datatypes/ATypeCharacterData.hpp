#pragma once

#include "ATypeCharacterNodeModel.hpp"
#include "NodeData.hpp"

class ATypeCharacterData : public NodeData {
  public:
  ATypeCharacterData() {}
  ATypeCharacterData(ATypeCharacterNodeModel *character) : _character(character) {}

  inline static const NodeDataType dataType = NodeDataType("atypeCharacter", "Atype Character");

  const NodeDataType &type() const override { return dataType; }

  ATypeCharacterNodeModel *getCharacter() { return _character; }

  protected:
  ATypeCharacterNodeModel *_character;
};
