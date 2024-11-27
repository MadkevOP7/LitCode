#include "lexer.h"
#include <cctype>
#include <stdexcept>
#include <iostream>

// Constructor
Lexer::Lexer(const std::string& sourceCode)
	: sourceCode(sourceCode), currentPos(0), line(1), column(0) {}

// Tokenize the source code
std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;

	while (currentPos < sourceCode.length()) {
		skipWhitespace();

		if (currentPos >= sourceCode.length())
			break;

		char currentChar = peek();

		if (std::isdigit(currentChar)) {
			tokens.push_back(parseNumber());
		}
		else if (std::isalpha(currentChar) || currentChar == '_') {
			tokens.push_back(parseIdentifierOrKeyword());
		}
		else if (currentChar == '"') {
			tokens.push_back(parseString());
		}
		else if (currentChar == '$') {
			skipComment(); // Ignore comments
		}
		else if (std::ispunct(currentChar)) {
			tokens.push_back(parseSymbol());
		}
		else {
			tokens.push_back({ TokenType::Unknown, std::string(1, currentChar), line, column });
			advance();
		}
	}

	tokens.push_back({ TokenType::EndOfFile, "", line, column });
	return tokens;
}

// Peek the current character without advancing
char Lexer::peek() const {
	if (currentPos >= sourceCode.length())
		return '\0';
	return sourceCode[currentPos];
}

// Advance to the next character
char Lexer::advance() {
	if (currentPos >= sourceCode.length())
		return '\0';
	char currentChar = sourceCode[currentPos];
	currentPos++;
	if (currentChar == '\n') {
		line++;
		column = 0;
	}
	else {
		column++;
	}
	return currentChar;
}

// Skip whitespace
void Lexer::skipWhitespace() {
	while (currentPos < sourceCode.length() && std::isspace(peek())) {
		advance();
	}
}

// Parse a number token
Token Lexer::parseNumber() {
	size_t start = currentPos;
	size_t startColumn = column;

	while (std::isdigit(peek())) {
		advance();
	}

	return { TokenType::Number, sourceCode.substr(start, currentPos - start), line, startColumn };
}

// Parse an identifier or keyword
Token Lexer::parseIdentifierOrKeyword() {
	size_t start = currentPos;
	size_t startColumn = column;

	while (std::isalnum(peek()) || peek() == '_') {
		advance();
	}

	std::string lexeme = sourceCode.substr(start, currentPos - start);

	static const std::unordered_map<std::string, TokenType> keywords = {
		{Lexer::KEY_IF, TokenType::Keyword},
		{Lexer::KEY_ELSE, TokenType::Keyword},
		{Lexer::KEY_RETURN, TokenType::Keyword},
	};

	auto it = keywords.find(lexeme);
	if (it != keywords.end()) {
		return { it->second, lexeme, line, startColumn };
	}

	return { TokenType::Identifier, lexeme, line, startColumn };
}

// Parse a symbol (punctuation)
Token Lexer::parseSymbol() {
	size_t startColumn = column;
	char symbol = advance();

	return { TokenType::Symbol, std::string(1, symbol), line, startColumn };
}

// Parse a string literal
Token Lexer::parseString() {
	size_t start = currentPos;
	size_t startColumn = column;

	advance(); // Skip the opening quote
	std::string value;

	while (peek() != '"' && currentPos < sourceCode.length()) {
		if (peek() == '\n') {
			throw std::runtime_error("Unterminated string literal at line " + std::to_string(line));
		}
		value += advance();
	}

	if (peek() == '"') {
		advance(); // Skip the closing quote
		return { TokenType::StringLiteral, value, line, startColumn };
	}
	else {
		throw std::runtime_error("Unterminated string literal at line " + std::to_string(line));
	}
}

// Skip single-line comment
void Lexer::skipComment() {
	advance(); // Skip the `$`
	while (peek() != '\n' && currentPos < sourceCode.length()) {
		advance();
	}
}