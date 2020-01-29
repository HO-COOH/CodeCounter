#pragma once
#include "pch.h"

//!!!!!!!!!!!!!!further language support add here!!!!!!!!!!!!!!!!!!!!
enum class Language { C, Cpp, Java, PlainText, Unknown, Count };	//little trick to get the count of the enum


//!!!!!!!!!!!!!!further language support add here!!!!!!!!!!!!!!!!!!!!
const std::unordered_map<Language, std::string> language_name
{
	{Language::C, "C"}, {Language::Cpp, "C++"},{Language::Java, "Java"} ,{Language::PlainText, "plain text"}, {Language::Unknown, "unknown"}
};

//!!!!!!!!!!!!!!further language support add here!!!!!!!!!!!!!!!!!!!!
const std::unordered_map<std::string, Language> file_postfix
{ {".c", Language::C},{".C", Language::C},{".h", Language::C},
	{".cpp", Language::Cpp},{".cxx", Language::Cpp},{".CPP", Language::Cpp},{".CXX",Language::Cpp}, {".hpp", Language::Cpp},
    {".java", Language::Java},
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

/*printing infos of one file record*/
std::ostream& operator<<(std::ostream& os, const Source_file& file_info);

/*Read the c-style comment & code*/
void read_c(std::ifstream& file, Source_file& result);

/*print a long line*/
void print_line();

/*print detailed info of all the files found in [result] using: std::ostream& operator<<(std::ostream& os, const Source_file& file_info)*/
void print_files(const std::vector<Source_file>& result);

/*print infos per language used*/
void print_percentage(const std::vector<Source_file>& result);

/*print helping message*/
void print_help();
