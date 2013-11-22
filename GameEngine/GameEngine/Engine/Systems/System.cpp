#include "System.h"

System::System()
{}

System::~System()
{}


void System::update()
{}

void System::init()
{}

const Barcode &System::getCode() const
{
	return _code;
}
