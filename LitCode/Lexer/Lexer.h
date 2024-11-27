#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType {
	Identifier,
	Number,
	Keyword,
	Symbol,
	StringLiteral,
	Comment,
	EndOfFile,
	Unknown
};

struct Token {
	TokenType type;
	std::string lexeme;
	size_t line;
	size_t column;
};

class Lexer {
public:
	Lexer(const std::string& sourceCode);
	std::vector<Token> tokenize();

private:
	std::string sourceCode;
	size_t currentPos;
	size_t line;
	size_t column;

	char peek() const;
	char advance();
	void skipWhitespace();
	Token parseNumber();
	Token parseIdentifierOrKeyword();
	Token parseSymbol();
	Token parseString();
	void skipComment();

	// DEFINES //
	const std::string KEY_IF = "fr";
	const std::string KEY_ELSE = "cap";
	const std::string KEY_RETURN = "yeet";
};

#endif // LEXER_H