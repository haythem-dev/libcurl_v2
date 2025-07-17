#pragma once

#include "mockcessor.h"
#include <libbasardbaspect.h>
#include <boost/shared_ptr.hpp>

class SelectStringsMockCessor;

typedef boost::shared_ptr<SelectStringsMockCessor> SelectStringsMockCessorPtr;


class SelectStringsMockCessor : public basar::mock::MockCessor
{
public:
    virtual ~SelectStringsMockCessor();

    SelectStringsMockCessor(const basar::VarString& accDefName);
};