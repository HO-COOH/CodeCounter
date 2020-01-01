#include "pch.h"
#include "Header.h"
#include "Timer.h"

//-r: recursive
//-d: detailed file list
std::array<std::pair<const std::string, bool>, 3> options{ std::pair{"-d", true}, {"-r", true}, {"-h", false} };

std::ostream& operator<<(std::ostream& os, const Source_file& file_info)
{
	os << (file_info.file_name.size() > 17 ? file_info.file_name.substr(0, 17) + "..." : file_info.file_name) << (file_info.file_name.size() < 8 ? "\t\t\t" : file_info.file_name.size() < 16 ? "\t\t" : "\t")
		<< language_name.at(file_info.language) << (file_info.language == Language::PlainText ? "\t":"\t\t")
	<< file_info.lines << "\t" << file_info.code << "\t   " << file_info.comments << "\t\t  " << file_info.blanks << '\n';
	return os;
}

void read_c(std::ifstream& file, Source_file& result)
{
	std::string line;
	auto is_multi_line_comment = false;
	while (getline(file, line))
	{
		++result.lines;
		if (std::find_if_not(line.cbegin(), line.cend(), isspace) != line.cend())	//check if the line is blank first
		{
			if (const auto comment_pos = line.find("//");	comment_pos != std::string::npos)	//find "//" so a line of comment
			{
				++result.comments;
				if (std::find_if_not(line.cbegin(), line.cbegin() + comment_pos, isspace) != line.cbegin() + comment_pos)
					++result.code;	//if there is code before //, count as one line of code
			}
			else
			{
				if (line.find("/*") != std::string::npos)	//find "/*" so starting a multi-line comment
				{
					if (line.find("*/") == std::string::npos)
						is_multi_line_comment = true;
					++result.comments;
				}
				else
				{
					if (is_multi_line_comment)	//check if still in multi-line comment
					{
						++result.comments;
						if (const auto position = line.find("*/"); position != std::string::npos)//check if multi-line-comment is ended
						{
							is_multi_line_comment = false;	//check whether there are still code after this ended multi-line comment
							if (std::find_if_not(line.cbegin() + position + 2, line.cend(), isspace) != line.cend())
								++result.code;
						}
					}
					else
						++result.code;
				}
			}
		}
		else
			++result.blanks;
	}
}

/*Read the code inside a file*/
///file_name: the path of the file
///return a struct containing the info
Source_file read_source_file(const std::filesystem::path& file_path)
{
	std::ifstream file{ file_path, std::ios_base::in };
	if (file.is_open())//check if file is opened successfully
	{
		Source_file result{std::move(file_path.filename().string()) };
		const auto pair = file_postfix.find(file_path.extension().string());
		result.language = pair != file_postfix.cend() ? pair->second : Language::PlainText;
		if (result.language == Language::PlainText)
		{
			result.lines = static_cast<unsigned>(std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n'));
			return result;
		}
			
		switch (result.language)
		{
		case Language::C:
		case Language::Cpp:
			read_c(file, result);
			break;
		default:
			break;
		}
		return result;
	}
	return{file_path.filename().string(), Language::Unknown};
}

void print_line()
{
	std::cout << "-------------------------------------------------------------------------------\n";
}

void print_files(const std::vector<Source_file>& result)
{
	if (result.empty())
	{
		std::cout << "No source code is found!\n";
		return;
	}
	print_line();
	std::cout << "FileName\t\t Language\t Lines\t Code\t Comments\t Blanks\n";
	print_line();
	for (const auto& item : result)
		std::cout << item;
	print_line();
}

void print_percentage(const std::vector<Source_file>& result)
{
	if (result.empty())
	{
		std::cout << "No source code is found!\n";
		return;
	}
	std::cout	<< "Files total=" << result.size() << '\n'
				<< "Language\tPercentage\tFiles\tLines\tCode\tComments\tBlanks\n";
	print_line();
	
	std::array<unsigned, static_cast<size_t>(Language::Count)> language_count{ };
	std::array<unsigned, static_cast<size_t>(Language::Count)> language_lines{ };
	std::array<unsigned, static_cast<size_t>(Language::Count)> language_code{ };
	std::array<unsigned, static_cast<size_t>(Language::Count)> language_comments{ };
	std::array<unsigned, static_cast<size_t>(Language::Count)> language_blanks{ };
	for (const auto& each_file : result)
	{
		auto index = static_cast<size_t>(each_file.language);
		++language_count[index];
		language_lines[index] += each_file.lines;
		language_code[index] += each_file.code;
		language_comments[index] += each_file.comments;
		language_blanks[index] += each_file.blanks;
	}
	
	std::array<double, static_cast<size_t>(Language::Count)> language_percent{ };
	std::transform(language_count.begin(), language_count.end(), language_percent.begin(), [&](unsigned language_count) { return static_cast<double>(language_count) / result.size(); });
	auto language_name_iter = language_name.cbegin();
	std::cout.precision(3);
	for (size_t index=0; index<language_count.size(); ++index, ++language_name_iter)
	{
		std::cout << language_name_iter->second 
			<< (language_name_iter->second.size()>7? "\t":"\t\t")
			<< language_percent[index] * 100.0 << "%\t\t" 
			<< language_count[index]<<'\t'
			<< language_lines[index]<<'\t'
			<<language_code[index]<<'\t'
			<<language_comments[index]<<"\t\t"
			<<language_blanks[index]<<'\n';
	}
}

void print_help()
{
	std::cout << "Usage:\n"
		<< "-d:\t Print detailed file lists\n"
		<< "-r:\t Recursively read all files inside this directory\n"
		<< "-h:\t Print this help manual\n";
}

template<typename Dir_iter>
void process(Dir_iter&& directory_iter)
{
	std::vector<Source_file>files;
	for (const auto& item : directory_iter)
	{
		/*DEBUG*/
		//cout << item.path() << "\n";
		if (item.is_regular_file())
		{
			if (file_postfix.find(item.path().extension().string()) == file_postfix.cend())
				continue;
			files.emplace_back(read_source_file(item.path()));
		}
	}
	if(options[0].second)
		print_files(files);
	print_percentage(files);
}

int main(int argc, char** argv)
{
	Timer t;
	//std::ios::sync_with_stdio(false);
	//change mode according to argv
	std::filesystem::path path{ "." };
	if (argc != 1)
	{		
		if (std::filesystem::exists(std::filesystem::path(argv[argc])))
			path = argv[argc];
		while (--argc)	//argv[0] is skipped in the loop
		{
			auto option = std::find_if(options.begin(), options.end(), [&](const auto& p) { return std::string_view(argv[argc]) == p.first; });
			if (option != options.end())
				option->second = true;
			else
				std::cout << "Invalid argument: " << argv[argc] << '\n';
		}
	}
	if (!options[2].second)	//not including "-h" (help)
		options[1].second? process(std::filesystem::recursive_directory_iterator(path)) : process(std::filesystem::directory_iterator(path));
	else
		print_help();
}

