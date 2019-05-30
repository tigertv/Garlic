#include "clvpch.hpp"
#include "CloveException.hpp"

namespace clv{
	CloveException::CloveException(int lineNum, const char* file)
		: lineNum(lineNum)
		, file(file){
	}

	CloveException::~CloveException() = default;

	const char* CloveException::what() const noexcept{
		std::ostringstream oss;
		oss << getType() << std::endl
			<< getOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	std::string CloveException::getOriginString() const noexcept{
		std::ostringstream oss;
		oss << "[FILE] " << file << std::endl
			<< "[LINE] " << lineNum;
		return oss.str();
	}
}