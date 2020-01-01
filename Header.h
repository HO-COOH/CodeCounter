#pragma once
#include "pch.h"

enum class Language { C, Cpp, PlainText, Unknown, Count };	//little trick to get the count of the enum
const std::unordered_map<Language, std::string> language_name
{
	{Language::C, "C"}, {Language::Cpp, "C++"}, {Language::PlainText, "plain text"}, {Language::Unknown, "unknown"}
};

const std::unordered_map<std::string, Language> file_postfix
{ {".c", Language::C},{".C", Language::C},{".h", Language::C},
	{".cpp", Language::Cpp},{".cxx", Language::Cpp},{".CPP", Language::Cpp},{".CXX",Language::Cpp}, {".hpp", Language::Cpp},
	{".txt", Language::PlainText}
};

struct Source_file
{
	std::string file_name;
	Language language;
	unsigned lines;
	unsigned code;
	unsigned comments;
	unsigned blanks;
};

std::ostream& operator<<(std::ostream& os, const Source_file& file_info);
void read_c(std::ifstream& file, Source_file& result);

void print_line();
void print_files(const std::vector<Source_file>& result);
void print_percentage(const std::vector<Source_file>& result);
void print_help();
