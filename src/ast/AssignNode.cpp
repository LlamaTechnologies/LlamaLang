#include "AssignNode.hpp"
#include "VariableRefNode.hpp"

using namespace llang;

void ast::AssignNode::ToString(std::string &str, const int tabLevel) const
{
  auto tabs = GetTabs(tabLevel);
  str += tabs;
  Left->ToString(str, 0);
  str += " = ";
  Right->ToString(str, 0);
}
