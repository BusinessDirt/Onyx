#include "stdafx.h"
#include "Util.h"

namespace Onyx
{
	bool Contains(const std::string& str, const std::string& substr)
	{
		return str.find(substr) != std::string::npos;
	}
}

