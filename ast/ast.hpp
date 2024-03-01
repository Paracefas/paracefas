#pragma once

#include "./../tokenizer/tokenizer.hpp"

#include <vector>
#include <string>

enum class NodeType {
    Program,
    IntegerLiteral,
    FloatLiteral,
    Identifier,
    BinaryExpression,


    Error,
};

static std::string NodeType_s[] = {
    "Program",
    "IntegerLiteral",
    "FloatLiteral",
    "Identifier",
    "BinaryExpression",


    "Error",
};

struct Statement {
    NodeType type;
    virtual ~Statement() {}
#ifdef _DEBUG
    virtual std::ostream& print(std::ostream& os) {
        os << "{ \"type\": " << NodeType_s[(size_t)type] << "\" }";
        return os;
    }
#endif
};

struct Program : Statement {
    std::vector<Statement*> body;
    Program() {
        // Statement::type;
        this->type = NodeType::Program;
    }
    ~Program() {
        for (auto& el : body) delete el;
    }
#ifdef _DEBUG
    virtual std::ostream& print(std::ostream& os) override {
        os << "{ \"type\": " << NodeType_s[(size_t)type] << "\""
           << ", \"body\": [";
        for (size_t i = 0; i < body.size()-1; ++i) { body[i]->print(os) << ", "; }
        body[body.size()-1]->print(os) << " ]\n}";

        return os;
    }
#endif
};

struct Expression : Statement {
};

struct BinaryExpression : Expression {
    Expression *lhs, *rhs;
    char const op;
    BinaryExpression(Expression* lhs, Expression* rhs, const char op)
        : lhs {lhs}, rhs{rhs}, op{op} {
        this->type = NodeType::BinaryExpression;
    }
#ifdef _DEBUG
    virtual std::ostream& print(std::ostream& os) override {
        os << "{ \"type\": " << NodeType_s[(size_t)type] << "\""
           << ", \"lhs\": "; lhs->print(os)
           << ", \"rhs\": "; rhs->print(os)
           << ", \"operator\": " << op << " }";
        return os;
    }
#endif
};

struct Identifier : Expression {
    std::string symbol;
    Identifier(std::string sym) 
        : symbol {sym} {
        this->type = NodeType::Identifier;
    }
#ifdef _DEBUG
    virtual std::ostream& print(std::ostream& os) override {
        os << "{ \"type\": " << NodeType_s[(size_t)type] << "\""
           << ", \"symbol\": \"" << symbol << "\" }";
        return os;
    }
#endif
};

struct IntegerLiteral : Expression {
    using Int = int;
    Int value;
    IntegerLiteral(Int i) 
        : value {i} {
        this->type = NodeType::IntegerLiteral;
    }
    
#ifdef _DEBUG
    virtual std::ostream& print(std::ostream& os) override {
        os << "{ \"type\": " << NodeType_s[(size_t)type] << "\""
           << ", \"value\": " << value << " }";
        return os;
    }
#endif
};

struct FloatLiteral : Expression {
    using Float = double;
    Float value;
    FloatLiteral(Float f) 
        : value {f} {
        this->type = NodeType::FloatLiteral;
    }
#ifdef _DEBUG
    virtual std::ostream& print(std::ostream& os) override {
        os << "{ \"type\": " << NodeType_s[(size_t)type] << "\""
           << ", \"value\": " << value << " }";
        return os;
    }
#endif
};

struct ErrorExpression : Expression {
    std::string errorMessage;
    ErrorExpression(std::string msg) 
        : errorMessage {msg} {
        this->type = NodeType::Error;
    }
#ifdef _DEBUG
    virtual std::ostream& print(std::ostream& os) override {
        os << "{ \"type\": " << NodeType_s[(size_t)type] << "\""
           << ", \"errorMessage\": " << errorMessage << " }";
        return os;
    }
#endif
};

template<typename Literal>
concept Literable = requires (Literal x) { x.type; x.value; };

template<typename T> requires Literable<T>
std::ostream& operator<<(std::ostream& os, T const& x) {
    os << "{ type: \"" << NodeType_s[(size_t)x.type] << "\", value: " << x.value << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, Statement* x) {
    return x->print(os);
}


class Parser {
    std::queue<Token> tokens;
    Program* program;
    bool isEndOfFile() {
        return tokens.front().type == TokenType::EndOfFile;
    }

    inline Token peek () { return tokens.front(); }
    Token advance () { 
        auto tk = tokens.front();
        if (!tokens.empty()) tokens.pop();
        return tk; 
    }

    Statement* parseStatement() {
        return parseExpression();
    }

    Expression* parseExpression() {
        return parseAdditiveExpression();
    }

    Expression* parseAdditiveExpression() {
        Expression* left = parsePrimaryExpression();
        while (peek().value == "+" || peek().value == "-") {
            const char op = advance().value.c_str()[0];
            Expression* right = parsePrimaryExpression();
            left = new BinaryExpression {left, right, op};
        }
        return left;
    }
    Expression* parsePrimaryExpression() {
        auto tk = peek();
        switch (tk.type) {
            case TokenType::Identifier: {
                return new Identifier{ advance().value };        
            } break;
            case TokenType::Float: {
                return new FloatLiteral{ std::stod(advance().value) };        
            } break;
            case TokenType::Integer: {
                return new IntegerLiteral{ std::stoi(advance().value) };        
            } break;
            default: {
                std::cerr << "[Parser - Error]: Unexpected token: `" << tk.value << "` was found.\n";
                return nullptr;
            }
        }
    }

public:
    Parser() =default;
    ~Parser() {
        delete program;
    }
    Program* generateAbstractSyntaxTree(std::string const& src) {
        program = new Program();
        Tokenizer t;
        tokens = vec2Q(t.tokenize(src));
        while (!isEndOfFile()) {
            program->body.push_back(parseStatement());
        }
        return program;
    }

    friend std::ostream& operator<<(std::ostream& os, const Parser& p) {
        os << p.tokens;
        return os;
    }
};