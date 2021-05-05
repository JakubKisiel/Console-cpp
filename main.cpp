#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace helper {
/* --------------------------------------------------------------------------*/
/**
 * @brief counts all occurences of std::regex expresion in  given string and
 * returns counter of occurences at the end of the function
 *
 * @Param const std::regex reference
 * @Param const std::string reference
 *
 * @Returns  long
 */
/* ----------------------------------------------------------------------------*/
auto inline count_all(const std::regex &expression, const std::string &to_check)
    -> long {
  long const match_count(std::distance(
      std::sregex_iterator(to_check.begin(), to_check.end(), expression),
      std::sregex_iterator()));
  return match_count;
}
/* --------------------------------------------------------------------------*/
/**
 * @brief function that checks if given string is palindrome
 *
 * @Param const std::string reference
 *
 * @Returns bool
 */
/* ----------------------------------------------------------------------------*/
auto inline check_if_palindrome(const std::string &word) -> bool {
  std::string reversedWord = std::string(word.rbegin(), word.rend());
  return !word.compare(reversedWord);
}
/* --------------------------------------------------------------------------*/
/**
 * @brief from the given iterator range checks if word is palindrom if yes
 * it's added to the std::unordered_set<std::string> that is returned at the
 * end of function
 *
 * @Param reference to iterator
 * @Param const reference to iterator
 *
 * @Returns std::unordered_set<std::string>
 */
/* ----------------------------------------------------------------------------*/
template <class It>
auto palindroms(It &begin_iterator, const It &end_iterator)
    -> std::unordered_set<std::string> {
  std::unordered_set<std::string> palindrom_set;
  while (begin_iterator != end_iterator) {
    if (check_if_palindrome(*begin_iterator))
      palindrom_set.insert(*begin_iterator);
    ++begin_iterator;
  }
  return palindrom_set;
}
/* --------------------------------------------------------------------------*/
/**
 * @brief Optimized for the usecase in this project since using the same
 * std::vector of anagrams
 *
 * @Param std::string argument
 * @Param const std::string reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
bool inline is_anagram_optimized(std::string word1, const std::string &word2) {
  std::sort(word1.begin(), word1.end());  // added
  return !word1.compare(word2);
}
auto change_line_to_words(const std::vector<std::string> &lines)
    -> std::vector<std::string> {
  for (const auto &line : lines) {
    std::vector<std::string> all_flags;
    std::istringstream buffer(line);
    std::copy(std::istream_iterator<std::string>(buffer),
              std::istream_iterator<std::string>(),
              std::back_inserter(all_flags));
  }
  // TODO: yield -> create generator so called Input Iterator
  return lines;
}
template <typename T>
auto print_vector(const std::vector<T> &vec) {
  for (const auto &e : vec) std::cout << e << std::endl;
}
}  // namespace helper
namespace file {
struct file_not_opened_exception : public std::exception {
  const char *what() const throw() { return "File didn't open"; }
};
struct cannot_open_file_exception : public std::exception {
  const char *what() const throw() {
    return "Error while trying to process file";
  }
};

/* --------------------------------------------------------------------------*/
/**
 * @brief struct that's purpose to keep the state of the file
 */
/* ----------------------------------------------------------------------------*/
struct manage_file {
 private:
  const std::string file_name;
  std::vector<std::string> my_lines;
  bool loaded;

 public:
  /* --------------------------------------------------------------------------*/
  /**
   * @brief Constructor that creates std::fstream with supplied file_name and
   * reads each line from file to vector of strings
   *
   * @Param const std::string reference
   */
  /* ----------------------------------------------------------------------------*/
  manage_file() : loaded(false){};
  manage_file(const std::string &file_name) : file_name(file_name) {
    std::fstream my_file(file_name);
    try {
      if (!my_file.is_open()) throw cannot_open_file_exception();
      while (my_file.peek() != EOF) {
        std::string row;
        getline(my_file, row);
        my_lines.push_back(std::move(row));
      }

      loaded = true;
    } catch (cannot_open_file_exception &e) {
      loaded = false;
      if (my_file.good())
        std::cerr << "There was an error when reading a file on line "
                  << my_lines.size() << std::endl;
      else
        std::cerr << "Wrong file name was provided" << std::endl;
      throw e;
    }
  }
  /* --------------------------------------------------------------------------*/
  /**
   * @brief returns std::vector<std::string> that contains all of the lines that
   * are present in the file(including empty ones)
   *
   *
   * @Returns std::vector<std::string>
   */
  /* ----------------------------------------------------------------------------*/
  auto get_all_lines() const -> std::vector<std::string> {
    if (!loaded) throw file_not_opened_exception();
    return my_lines;
  }

  /* --------------------------------------------------------------------------*/
  /**
   * @brief returns std::vector<std::string> that contains all of the words that
   * are present in the file
   *
   *
   * @Returns std::vector<std::strings>
   */
  /* ----------------------------------------------------------------------------*/
  auto get_all_words() const -> std::vector<std::string> {
    if (!loaded) throw file_not_opened_exception();
    std::vector<std::string> all_words;
    for (const auto &line : my_lines) {
      std::istringstream buffer(line);
      std::copy(std::istream_iterator<std::string>(buffer),
                std::istream_iterator<std::string>(),
                std::back_inserter(all_words));
    }
    return all_words;
  }
  /* --------------------------------------------------------------------------*/
  /**
   * @brief return number of lines
   *
   * @Returns long long
   */
  /* ----------------------------------------------------------------------------*/
  auto get_lines_number() const -> long long {
    if (!loaded) throw file_not_opened_exception();
    return my_lines.size();
  }
};

}  // namespace file
namespace command {
struct parssing_error_exception : public std::exception {
  const char *what() const throw() { return "Error while parsing commands"; }
};
struct input_file_missing_exception : public std::exception {
  const char *what() const throw() { return "Wrong input file provided"; }
};

struct input_flag_exception : public std::exception {
  const char *what() const throw() {
    return "-i should be the only specified flag";
  }
};
auto manage_command(const std::vector<std::string> &command_vector,
                    bool = false) -> void;

/* --------------------------------------------------------------------------*/
/**
 * @brief specifies number that is assigned to each flag
 */
/* ----------------------------------------------------------------------------*/
std::unordered_map<std::string, int> flag_map = {
    {"", 0},   {"-f", 1}, {"-i", 2}, {"-n", 3}, {"-d", 4},   {"-dd", 5},
    {"-c", 6}, {"-a", 7}, {"-p", 8}, {"-s", 9}, {"-rs", 10}, {"-o", 11},
};
/* --------------------------------------------------------------------------*/
/**
 * @brief specifies aliasses for commands
 */
/* ----------------------------------------------------------------------------*/
std::unordered_map<std::string, std::string> alias_map = {
    {"--help", ""},
    {"--file", "-f"},
    {"--input", "-i"},
    {"--newlines", "-n"},
    {"--digits", "-d"},
    {"--numbers", "-dd"},
    {"--chars", "-c"},
    {"--angrams", "-a"},
    {"--palindroms", "-p"},
    {"--sorted", "-s"},
    {"--reverse-sorted", "-rs"},
    {"--output", "-o"},
};
/* --------------------------------------------------------------------------*/
/**
 * @brief prints help text to the console
 *
 */
/* ----------------------------------------------------------------------------*/
auto print_help() -> void {
  const char *text =
      "-----help-------\n"
      "This program needs to be specified with first flag -f and file name or "
      "just with the flag -i and name of the input file\n"
      "flag [||--help] shows this help menu\n"
      "flag [-f||--file] \"file_name\" specify the file that will be used for "
      "the rest of the program\n"
      "flag [-i||--input] \"file_name\" !!!should be the only specified flag "
      "in program!!! specify the file name that have a file "
      "with flags for the program \n"
      "flag [-n||--newlines] will output number of lines that are in the file\n"
      "flag [-d||--digits] will output number of digits in a file\n"
      "flag [-dd||--numbers] will output number of numbers in a file\n"
      "flag [-a||--anagrams] \"words words\" should be the last specified flag "
      "and after the flag all the words in the file will be checked against "
      "anagrams and the ones that match will be outputted\n"
      "flag [-p||--palindroms] \"words words\" should be the last specified "
      "flag and after the flag all the words that are palindroms and are in "
      "the file will be outputted\n"
      "flag [-s||--sorted] outputs all the words that are in the file in "
      "alphabetical order\n"
      "flag [-rs||--reverse-sorted] outputs all the words that are in the file "
      "in reversed alphabetical order\n"
      "flag [-o||--output] \"output_file_name\" outputs of the command are "
      "saved in specified file\n";
  std::cout << text << std::endl;
}
/* --------------------------------------------------------------------------*/
/**
 * @brief opens specified file and returns file::manage_file object that is used
 * for other commands
 *
 * @Param const std::string reference
 *
 * @Returns  file::manage_file
 */
/* ----------------------------------------------------------------------------*/
auto inline file_flag(const std::string &file_name)
    -> std::unique_ptr<file::manage_file> {
  return std::make_unique<file::manage_file>(file_name);
}
/* --------------------------------------------------------------------------*/
/**
 * @brief takes the name of file and treats lines in the file as command with
 * specified flags
 *
 * @Param const std::string reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
auto input_flag(const std::string &input_file_name) -> void {
  std::unique_ptr<file::manage_file> input_file;
  try {
    input_file = std::make_unique<file::manage_file>(input_file_name);
  } catch (std::exception &e) {
    throw input_file_missing_exception();
  }

  try {
    auto input_command_vector = (*input_file).get_all_lines();
    for (const auto &line : input_command_vector) {
      std::vector<std::string> all_flags;
      std::istringstream buffer(line);
      std::copy(std::istream_iterator<std::string>(buffer),
                std::istream_iterator<std::string>(),
                std::back_inserter(all_flags));
      manage_command(all_flags, true);
    }
  } catch (std::exception &e) {
    std::cerr << "Error while using command from input file\n";
  }
};
/* --------------------------------------------------------------------------*/
/**
 * @brief writes number of lines that are in the specified file argument to the
 * specified std::stringstrea output_stream
 *
 * @Param std::stringstream reference
 * @Param const file::manage_file reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
auto newline_flag(std::stringstream &output_stream,
                  const file::manage_file &file) -> void {
  output_stream << "Lines in file: " << file.get_lines_number() << "\n";
}
/* --------------------------------------------------------------------------*/
/**
 * @brief writes number of digits that are in the specified file argument to the
 * specified std::stringstrea output_stream
 *
 * @Param std::stringstream reference
 * @Param const file::manage_file reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
auto digits_flag(std::stringstream &output_stream,
                 const file::manage_file &file) -> void {
  long counter = 0;
  std::regex const digit("(\\d)");
  for (const auto &line : file.get_all_lines()) {
    counter += helper::count_all(digit, line);
  };
  output_stream << "Digits in file: " << counter << std::endl;
}
/* --------------------------------------------------------------------------*/
/**
 * @brief writes number of numbers that are in the specified file argument to
 * the specified std::stringstrea output_stream
 *
 * @Param std::stringstream reference
 * @Param const file::manage_file reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
auto numbers_flag(std::stringstream &output_stream,
                  const file::manage_file &file) -> void {
  long counter = 0;
  std::regex const digit("(^\\d+$)");
  for (const auto &word : file.get_all_words()) {
    counter += helper::count_all(digit, word);
  };
  output_stream << "Numbers in file: " << counter << std::endl;
}
/* --------------------------------------------------------------------------*/
/**
 * @brief writes number of chars that are in the specified file argument to the
 * specified std::stringstrea output_stream
 *
 * @Param std::stringstream reference
 * @Param const file::manage_file reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
auto chars_flag(std::stringstream &output_stream, const file::manage_file &file)
    -> void {
  long counter = 0;
  for (const auto &line : file.get_all_lines()) {
    for (const auto &c : line) counter++;
    counter++;
  }
  output_stream << "Chars in file: " << counter << std::endl;
}
/* --------------------------------------------------------------------------*/
/**
 * @brief outputs to output_stream words that are found in file and are anagrams
 * in the specified iterator range
 *
 *
 * @Param std::stringstream reference
 * @Param const file::manage_file reference
 * @Param iterator reference
 * @Param const iterator reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
template <typename It>
auto anagrams_flag(std::stringstream &output_stream,
                   const file::manage_file &file, It &word_iterator,
                   const It &end_iterator) -> void {
  output_stream << "Anagrams Found: " << std::endl;
  word_iterator++;
  std::vector<std::string> anagrams;
  while (word_iterator != end_iterator) {
    std::string anagram(*word_iterator);
    std::sort(anagram.begin(), anagram.end());
    anagrams.push_back(anagram);
    word_iterator++;
  }
  for (const auto &word : file.get_all_words()) {
    for (const auto &anagram : anagrams) {
      if (helper::is_anagram_optimized(word, anagram))
        output_stream << word << std::endl;
    }
  }
}
/* --------------------------------------------------------------------------*/
/**
 * @brief outputs to output_stream words that are found in file and are
 * palindroms in the specified iterator range
 *
 *
 * @Param std::stringstream reference
 * @Param const file::manage_file reference
 * @Param iterator reference
 * @Param const iterator reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
template <typename It>
auto palindroms_flag(std::stringstream &output_stream,
                     const file::manage_file &file, It &word_iterator,
                     const It &end_iterator) -> void {
  word_iterator++;
  auto palindrom_set = helper::palindroms(word_iterator, end_iterator);
  output_stream << "Palindroms found: " << std::endl;
  for (const auto &word : file.get_all_words()) {
    if (palindrom_set.contains(word)) output_stream << word << std::endl;
  }
}

/* --------------------------------------------------------------------------*/
/**
 * @brief outputs to the output_stream sorted words that are present in the file
 * in the alphabetical order
 *
 * @Param std::stringstream reference
 * @Param const file::manage_file reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
auto sorted_flag(std::stringstream &output_stream,
                 const file::manage_file &file) -> void {
  std::vector<std::string> sorted = file.get_all_words();
  std::sort(sorted.begin(), sorted.end());
  for (const auto &word : sorted) output_stream << word << std::endl;
}
/* --------------------------------------------------------------------------*/
/**
 * @brief outputs to the output_stream sorted words that are present in the file
 * in the reversed alphabetical order
 *
 * @Param std::stringstream reference
 * @Param const file::manage_file reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
auto reverse_sorted_flag(std::stringstream &output_stream,
                         const file::manage_file &file) -> void {
  std::vector<std::string> sorted = file.get_all_words();
  std::sort(sorted.rbegin(), sorted.rend());
  for (const auto &word : sorted) output_stream << word << std::endl;
}
/* --------------------------------------------------------------------------*/
/**
 * @brief specifies file for the output_stream with the specifed file_name
 *
 * @Param std::stringstream reference
 * @Param const file::manage_file reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
auto output_flag(std::ostream *&out, const std::string &file_name) -> void {
  out = new std::fstream(file_name);
  if (!out->good())
    dynamic_cast<std::fstream *>(out)->open(file_name, std::fstream::out);
};

/* --------------------------------------------------------------------------*/
/**
 * @brief checks if first argument iterator is not the last item in vector
 *
 * @Param iterator reference
 * @Param const iterator reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
template <typename It>
auto increment_iterator(It &increment_iterator, const It &end_iterator)
    -> void {
  ++increment_iterator;
  if (increment_iterator == end_iterator) throw parssing_error_exception();
}
/* --------------------------------------------------------------------------*/
/**
 * @brief Command parser per say it manages the flow of the command flags
 *
 * @Param const std::vector<std::string> reference
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
auto manage_command(const std::vector<std::string> &command_vector, bool input)
    -> void {
  if (command_vector.size() == 0 || command_vector[0].at(0) != '-')
    print_help();
  std::unique_ptr<file::manage_file> my_file;
  std::stringstream flags_output;
  std::string output_file;
  std::ostream *out = &std::cout;
  auto command_iterator = command_vector.begin();
  auto command_end = command_vector.end();
  bool output_to_console = true;
  try {
    while (command_iterator != command_end) {
      int flag_number = 0;
      std::string flag_string = *command_iterator;
      if (alias_map.contains(flag_string)) flag_string = alias_map[flag_string];

      if (flag_map.contains(flag_string))
        flag_number = flag_map[flag_string];
      else
        throw parssing_error_exception();
      switch (flag_number) {
        case 0:
          print_help();
          break;
        case 1:
          increment_iterator(command_iterator, command_end);
          my_file = file_flag(*command_iterator);
          break;
        case 2:
          increment_iterator(command_iterator, command_end);
          if (command_vector.size() != 2) throw input_flag_exception();
          input_flag(*command_iterator);
          break;
        case 3:
          newline_flag(flags_output, *my_file);
          break;
        case 4:
          digits_flag(flags_output, *my_file);
          break;
        case 5:
          numbers_flag(flags_output, *my_file);
          break;
        case 6:
          chars_flag(flags_output, *my_file);
          break;
        case 7:
          anagrams_flag(flags_output, *my_file, command_iterator, command_end);
          break;
        case 8:
          palindroms_flag(flags_output, *my_file, command_iterator,
                          command_end);
          break;
        case 9:
          sorted_flag(flags_output, *my_file);
          break;
        case 10:
          reverse_sorted_flag(flags_output, *my_file);
          break;
        case 11:
          increment_iterator(command_iterator, command_end);
          output_flag(out, *command_iterator);
          break;
        default:
          break;
      }
      if (command_end != command_iterator) ++command_iterator;
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    print_help();
    if (input) throw e;
  };
  *out << flags_output.str();
}
}  // namespace command
int main(int argc, char **argv) {
  std::vector<std::string> args(argv + 1, argv + argc);

  command::manage_command(args);
}
