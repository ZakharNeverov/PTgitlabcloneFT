#include "commands.h"

mitkov::Command::Command(std::map< std::string, mitkov::orientedGraph< int > >& allGraphs, std::ostream& stream):
  stream(stream),
  allGraphs(allGraphs)
{
  using Pair = std::pair< std::string, void (mitkov::Command::*)(const std::list< std::string >& args) >;
  commands.insert(Pair("SEARCH", &mitkov::Command::search));
  commands.insert(Pair("GETMAXDEGREE", &mitkov::Command::getMaxDegree));
  commands.insert(Pair("TOPOLOGICSORT", &mitkov::Command::topologicSort));
  commands.insert(Pair("GETCYCLE", &mitkov::Command::getCycle));
  commands.insert(Pair("MERGE", &mitkov::Command::merge));
  commands.insert(Pair("CREATE", &mitkov::Command::addFirst));
}

void mitkov::Command::operator()(std::list<std::string>& cmdList)
{
  std::string command = cmdList.front();
  cmdList.pop_front();
  std::map< std::string, mitkov::orientedGraph< int > >::iterator it = allGraphs.find(command);
  if (allGraphs.find(command) != allGraphs.end()) {
    add(it->second, cmdList);
    return;
  }
  (this->*commands.find(command)->second)(cmdList);
}

void mitkov::Command::search(const std::list<std::string>& args)
{
  if (args.size() != 2) {
    throw std::invalid_argument("Not enough data");
  }
  std::list< std::string >::const_iterator it = args.begin();
  orientedGraph< int >& affected = allGraphs.find(*it++)->second;
  if (affected.search(std::stoi(*it))) {
    stream << "TRUE\n";
    return;
  }
  stream << "FALSE\n";
}

void mitkov::Command::getMaxDegree(const std::list<std::string>& args)
{
  if (args.size() != 2) {
    throw std::invalid_argument("Not enough data");
  }
  std::list< std::string >::const_iterator it = args.begin();
  orientedGraph< int >& affected = allGraphs.find(*it++)->second;
  stream << affected.getMaxDegree();
}

void mitkov::Command::topologicSort(const std::list<std::string>& args)
{
  if (args.size() != 1) {
    throw std::invalid_argument("Not enough data");
  }
  std::list< std::string >::const_iterator it = args.begin();
  orientedGraph< int >& affected = allGraphs.find(*it++)->second;

  affected.topologicSort();
  stream << "The graph was sorted\n";
}

void mitkov::Command::getCycle(const std::list<std::string>& args)
{
  if (args.size() != 1) {
    throw std::invalid_argument("Not enough data");
  }
  std::list< std::string >::const_iterator it = args.begin();
  orientedGraph< int >& affected = allGraphs.find(*it++)->second;
  orientedGraph< int > cycle = affected.getCycle();
  if (cycle.getSize() == 0) {
    stream << "<EMPTY>\n";
    return;
  }
  for (graph< int, true >::iterator it = cycle.begin(); it != cycle.end(); ++it) {
    if (it != cycle.begin()) {
      stream << ' ';
    }
    stream << *it;
  }
  stream << '\n';
}

void mitkov::Command::merge(const std::list<std::string>& args)
{
  if (args.size() != 4) {
    throw std::invalid_argument("Not enough data");
  }
  std::list< std::string >::const_iterator it = args.begin();
  orientedGraph< int >& affected1 = allGraphs.find(*it++)->second;
  orientedGraph< int >& affected2 = allGraphs.find(*it++)->second;
  int key1 = std::stoi(*it++);
  int key2 = std::stoi(*it);
  orientedGraph< int > result = affected1.merge(affected2, key1, key2);
  for (graph< int, true >::iterator it = result.begin(); it != result.end(); ++it) {
    if (it != result.begin()) {
      stream << ' ';
    }
    stream << *it;
  }
  stream << '\n';
}

void mitkov::Command::add(mitkov::orientedGraph< int >& target, const std::list<std::string>& args)
{
  if (args.size() != 5) {
    throw std::invalid_argument("Not enough data");
  }
  std::list< std::string >::const_iterator it = args.begin();
  if (*it++ != "{") {
    throw std::invalid_argument("Invalid syntax");
  }
  int one = std::stoi(*it++);
  std::string key = *it++;
  int two = std::stoi(*it++);

  if (*it != "}") {
    throw std::invalid_argument("Invalid syntax");
  }
  if (key == "->") {
    target.add(one, two);
  }
  if (key == "<-") {
    target.rAdd(one, two);
  }
}

void mitkov::Command::addFirst(const std::list<std::string>& args)
{
  if (args.size() != 2) {
    throw std::invalid_argument("Not enough data");
  }
  std::list< std::string >::const_iterator it = args.begin();
  std::string name = *it++;
  int key = std::stoi(*it);
  orientedGraph< int > a;
  a.addFirstVertex(key);
  allGraphs.insert(std::pair< std::string, orientedGraph< int > >(name, a));
}
