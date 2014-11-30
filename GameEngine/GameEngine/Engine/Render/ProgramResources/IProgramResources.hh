#pragma once

class IProgramResources
{
public:
	virtual IProgramResources &operator()() = 0;
};

typedef IProgramResources ProgramResource;