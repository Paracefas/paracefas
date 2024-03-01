#pragma once

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <sstream>

std::ostream& operator<<(std::ostream& os, std::vector<auto> const& vs) {
	os << "{\n\t\"size\": " << vs.size() << ",\n\t\"content\": [ ";
	for (size_t i = 0; i < vs.size()-1; ++i) { os << vs[i] << ", "; }
	os << vs[vs.size()-1] << " ]\n}";
	return os;
}

std::ostream& operator<<(std::ostream& os, std::vector<auto*> const& vs) {
	os << "{\n\t\"size\": " << vs.size() << ",\n\t\"content\": [ ";
	for (size_t i = 0; i < vs.size()-1; ++i) { os << *vs[i] << ", "; }
	os << *vs[vs.size()-1] << " ]\n}";
	return os;
}

std::ostream& operator<<(std::ostream& os, std::queue<auto> q) {
	os << "{\n\t\"size\": " << q.size() << ",\n\t\"content\": [ ";
	for (size_t i = 0; i < q.size()-1; ++i) {
        os << q.front() << ", ";
        q.pop();
    }
    os << q.front() << " ]\n";
    q.pop();
    return os;
}

std::string read_file (std::string_view filePath) {
	std::ifstream file(std::string{filePath});

    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();
		return content;
	}

	return "Unable to open the file";
}

std::vector<std::string> splitString(const std::string& input, char delimiter = '\n') {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

std::queue<char> splitStringQ(const std::string& input) {
    std::queue<char> qo;
    for (char c : input) 
        qo.push(c);
    return qo;
}

template<typename T>
std::queue<T> vec2Q(const std::vector<T>& v) {
    std::queue<T> q;
    for (T const & el: v) q.push(el);
    return q;
}

inline bool isskippable(char c) { 
    return (c == ' ' ||c == '\n' ||c == '\0' ||c == '\r'); 
}

bool isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}