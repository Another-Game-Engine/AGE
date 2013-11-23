#include "System.h"

System::System()
{}

System::~System()
{}


void System::update(double time)
{
	updateBegin(time);
	mainUpdate(time);
	updateEnd(time);
}

void System::init()
{
	initialize();
}

const Barcode &System::getCode() const
{
	return _code;
}
