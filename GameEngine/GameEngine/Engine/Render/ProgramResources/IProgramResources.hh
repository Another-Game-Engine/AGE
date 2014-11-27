#pragma once

class IProgramResource
{
public:
	virtual IProgramResource &operator()() = 0;
};

typedef IProgramResource ResourceProgram;