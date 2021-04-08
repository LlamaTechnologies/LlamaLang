#include "VariableDefNode.hpp"
#include "AssignNode.hpp"

using namespace llang;

void ast::VariableDefNode::ToString(std::string &str, const int tabLevel) const
{
  auto tabs = GetTabs(tabLevel);
  str += tabs + Name + " : " + VarType;

  if( assignmentStmnt ) {
    str += "\n";
    assignmentStmnt->ToString(str, tabLevel);
  }
}
