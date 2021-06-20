#include <iostream>
#include <assert.h>

#include <Core/Assertion.hpp>

using namespace Core::Debug;

bool Assertion::enabled = true;

void Assertion::assertTest(bool expression)
{
	if (enabled)
		assert(expression);
}