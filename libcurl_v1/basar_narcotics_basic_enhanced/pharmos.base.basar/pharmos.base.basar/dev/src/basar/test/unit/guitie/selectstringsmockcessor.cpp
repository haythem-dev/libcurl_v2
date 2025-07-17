#include "libbasarguitie.h"
#include "selectstringsmockcessor.h"

SelectStringsMockCessor::~SelectStringsMockCessor()
{
}

SelectStringsMockCessor::SelectStringsMockCessor(const basar::VarString& accDefName) : basar::mock::MockCessor(accDefName)
{
}
