#pragma once

#include "./../util/util.hpp"

#include <unordered_map>

enum class TokenType {
	Integer,
	Float, 
	Identifier, 
	OpenParen,			// (
	CloseParen, 		// )
	OpenCurlyBrac,		// {
	CloseCurlyBrac,		// }
	OpenSqrBrac,		// [
	CloseSqrBrac,		// ]
	BinaryOp, 			// +-*/%^
	Comma,
	Semicolon,
	Colon,
	Equals,
	EndOfFile,

	Let,
	Const,
};

static std::string TokenType_s[] = {
	"Integer", 
	"Float",
	"Identifier", 
	"OpenParen",			
	"CloseParen", 		
	"OpenCurlyBrac",	
	"CloseCurlyBrac",	
	"OpenSqrBrac",		
	"CloseSqrBrac",		
	"BinaryOp",
	"Comma",
	"Semicolon",
	"Colon", 	
	"Equals",
	"EndOfFile",

	"Let",
	"Const"
};

static std::unordered_map<std::string, TokenType> Keywords {
	{ "let", TokenType::Let },
	{ "const", TokenType::Const },
};

struct Token {
	TokenType type;
	std::string value;
};

std::ostream& operator<<(std::ostream& os, Token const& t) {
	os << "{ \"type\": \"" << TokenType_s[(size_t)t.type] << "\", \"value\": \"" << t.value << "\" }";	
	return os;
}

struct Tokenizer {
	Tokenizer() =default;
	std::vector<Token> tokenize (std::string const& src) {
		source_code = std::move(src);
		auto q = splitStringQ(source_code);
		while(!q.empty()) {
			if (q.front() == '(') 
				tokens.push_back({ TokenType::OpenParen, "(" });
			else if (q.front() == ')') 
				tokens.push_back({ TokenType::CloseParen, ")" });
			else if (q.front() == '{') 
				tokens.push_back({ TokenType::OpenCurlyBrac, "{" });
			else if (q.front() == '}') 
				tokens.push_back({ TokenType::CloseCurlyBrac, "}" });
			else if (q.front() == '[') 
				tokens.push_back({ TokenType::OpenSqrBrac, "[" });
			else if (q.front() == ']') 
				tokens.push_back({ TokenType::CloseSqrBrac, "]" });
			else if (q.front() == ':') 
				tokens.push_back({ TokenType::Colon, ":" });
			else if (q.front() == ';') 
				tokens.push_back({ TokenType::Semicolon, ";" });
			else if (q.front() == ',') 
				tokens.push_back({ TokenType::Comma, "," });
			else if (q.front() == '=') 
				tokens.push_back({ TokenType::Equals, "=" });
			else if (q.front() == '+' || q.front() == '-' 
					|| q.front() == '*' || q.front() == '/' 
					|| q.front() == '%' || q.front() == '^') 
				tokens.push_back({ TokenType::BinaryOp, {q.front()} });
			else { // multiple char token case.
				if (std::isdigit(q.front())) {
					std::string number;
					bool is_float = false;
					for (; std::isdigit(q.front()) || q.front() == '.'; number += q.front(), q.pop())
						if (q.front() == '.') is_float = true;
					tokens.push_back({ (is_float ? TokenType::Float : TokenType::Integer), number });
					continue;
				} else if(std::isalnum(q.front())) { // Identifier.
					std::string identifier;
					for (;std::isalnum(q.front()); identifier += q.front(), q.pop());
					
					// TODO: When I add variables, check identifier table.
					if (Keywords.count(identifier)) // First check if it's a reserved keyword.
						tokens.push_back({ Keywords[identifier], identifier });
					else tokens.push_back({ TokenType::Identifier, identifier });
					continue;
				} else if (isskippable(q.front())) {
					q.pop();
					continue;
				}
			}
			q.pop(); // deletes char registered.
		}
		tokens.push_back({ TokenType::EndOfFile, "EndOfFile" });
		return tokens;
	}
private:
	std::vector<Token> tokens;
	std::string source_code;
};