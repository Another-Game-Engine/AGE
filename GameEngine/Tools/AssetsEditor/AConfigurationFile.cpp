#pragma once

namespace AGE
{
	class AConfigurationFile
	{
	public:
		// DTOR
		virtual ~AConfigurationFile() {};

		// Virtual Pure
		bool readFile();


	}; // class IConfigurationFile
} // namespace AGE