#include "../reduceBeginner.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

std::string process(std::string s) {
  std::string result;
  for (auto c : s)
    if (std::isalpha(c))
      result += std::tolower(c);
  return result;
}

int main() {
  reducio::MapReduce 
    <
      std::string, std::string,
      std::string, int, 
      std::string, int
    > 
  mapReduce
  (
  	[](std::string, std::string filename) {
  	  std::vector<std::pair<std::string, int>> result;
  	  std::ifstream in(filename);
  	  std::string s;
  	  while ( in >> s)
        result.push_back({process(s),1});
      return result;
  	},
  	[](std::string a, std::vector<int> v) {
  	  return std::make_pair(a, v.size());
  	}
  );
  auto result = mapReduce({{"a", "doc1.txt"},{"f", "doc2.txt"}});

  std::cout << result["the"];
}
