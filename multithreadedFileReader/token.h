#pragma once
#include <string>
#include <vector>

class tokens final
{
	std::vector<std::string> tokens_{};
	bool valid_ = false;
public:
	tokens() = default;
	template<typename TokensList>
		requires std::is_convertible_v<TokensList, std::vector<std::string>>
	tokens(TokensList&& tokensList, const bool valid) :tokens_(std::forward<TokensList>(tokensList)), valid_(valid) {}

	[[nodiscard]] std::string& getTokens(const size_t index){return tokens_.at(index);}
	[[nodiscard]] const std::string& getTokens(const size_t index) const{return tokens_.at(index);}
	[[nodiscard]] bool isValid() const noexcept { return valid_; }
	[[nodiscard]] size_t size() const noexcept
	{
		return tokens_.size();
	}
};
