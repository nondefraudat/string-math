#pragma once
#include <string>

class Tests {
public:
	Tests() noexcept;
	void add(const std::string& source,
			const std::string& target,
			const double result) noexcept;
	int getStatus() const noexcept;

private:
	bool status;
};
