#include "commands.hpp"

#include <iostream>
#include <string>

#include "parser.hpp"
#include "graph.hpp"
#include "graph_list.hpp"

bool alhimenko::correctInput(std::istream& in)
{
  std::istream::sentry sentry(in);
  if (!in && !in.eof())
  {
    skipcommand(in);
    return false;
  }
  return true;
}

void alhimenko::createGraph(std::istream& in, std::ostream& out)
{
  SkipWsFlagSaver saver(in);

  std::string name;
  in >> skipblank >> name;
  if (!correctInput(in))
  {
    return;
  }
  if (!checkName(name))
  {
    skipcommand(in);
    return;
  }

  try
  {
    std::list< Vertex_t > vertexes;
    Vertex_t v;
    bool last_vertex = false;
    while (!last_vertex)
    {
      v = readVertex(in, last_vertex);
      vertexes.push_back(v);
    }

    graphs.emplace(name, std::make_unique< Graph >(vertexes));
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << '\n';
  }
}

void alhimenko::printGraph(std::istream& in, std::ostream& out)
{
  SkipWsFlagSaver saver(in);

  std::string name;
  in >> skipblank >> name;
  if (!correctInput(in))
  {
    return;
  }
  if (!checkName(name))
  {
    skipcommand(in);
    return;
  }

  char separator;
  in >> skipblank >> separator;
  if (separator != ';')
  {
    skipcommand(in);
    return;
  }

  auto it = graphs.find(name);
  if (it != graphs.end())
  {
    std::cout << '\n';
    it->second->print(out);
  }
  else
  {
    skipcommand(in);
  }
}

void alhimenko::insertVertex(std::istream& in, std::ostream& out)
{
  SkipWsFlagSaver saver(in);

  std::string name;
  in >> skipblank >> name;
  if (!correctInput(in))
  {
    return;
  }
  if (!checkName(name))
  {
    skipcommand(in);
    return;
  }

  auto it = graphs.find(name);
  if (it == graphs.end())
  {
    skipcommand(in);
  }

  try
  {
    bool unuse = false;
    Vertex_t v = readVertex(in, unuse);

    it->second->insert(v);
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << '\n';
  }
}

void alhimenko::deleteVertex(std::istream& in, std::ostream& out)
{
  SkipWsFlagSaver saver(in);

  std::string name;
  in >> skipblank >> name;
  if (!correctInput(in))
  {
    return;
  }
  if (!checkName(name))
  {
    skipcommand(in);
    return;
  }

  auto it = graphs.find(name);
  if (it == graphs.end())
  {
    skipcommand(in);
  }

  try
  {
    uint32_t v_num;
    in >> skipblank >> v_num;
    if (!correctInput(in))
    {
      return;
    }

    char separator;
    in >> skipblank >> separator;
    if (separator != ';')
    {
      skipcommand(in);
      return;
    }

    it->second->erase(v_num);
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << '\n';
  }
}

void alhimenko::findVertex(std::istream& in, std::ostream& out)
{
  SkipWsFlagSaver saver(in);

  std::string name;
  in >> skipblank >> name;
  if (!correctInput(in))
  {
    return;
  }
  if (!checkName(name))
  {
    skipcommand(in);
    return;
  }

  auto it = graphs.find(name);
  if (it == graphs.end())
  {
    skipcommand(in);
    return;
  }

  try
  {
    uint32_t v_num;
    in >> skipblank >> v_num;
    if (!correctInput(in))
    {
      return;
    }

    char separator;
    in >> skipblank >> separator;
    if (separator != ';')
    {
      skipcommand(in);
      return;
    }

    out << '\n' << std::boolalpha << it->second->find(v_num) << '\n';
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << '\n';
  }
}

void alhimenko::checkEdge(std::istream& in, std::ostream& out)
{
  SkipWsFlagSaver saver(in);

  std::string name;
  in >> skipblank >> name;
  if (!correctInput(in))
  {
    return;
  }
  if (!checkName(name))
  {
    skipcommand(in);
    return;
  }

  auto it = graphs.find(name);
  if (it == graphs.end())
  {
    skipcommand(in);
    return;
  }

  try
  {
    uint32_t v_num1;
    in >> skipblank >> v_num1;
    if (!correctInput(in))
    {
      return;
    }

    uint32_t v_num2;
    in >> skipblank >> v_num2;
    if (!correctInput(in))
    {
      return;
    }

    char separator;
    in >> skipblank >> separator;
    if (separator != ';')
    {
      skipcommand(in);
      return;
    }

    out << '\n' << std::boolalpha << it->second->isEdge(v_num1, v_num2) << '\n';
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << '\n';
  }
}

void alhimenko::dfs(std::istream& in, std::ostream& out)
{
  SkipWsFlagSaver saver(in);

  std::string name;
  in >> skipblank >> name;
  if (!correctInput(in))
  {
    return;
  }

  if (!checkName(name))
  {
    skipcommand(in);
    return;
  }

  char separator;
  in >> skipblank >> separator;
  if (separator != ';')
  {
    skipcommand(in);
    return;
  }

  auto it = graphs.find(name);
  if (it != graphs.end())
  {
    std::cout << '\n';
    it->second->dfs(out);
    std::cout << '\n';
  }
  else
  {
    skipcommand(in);
  }
}

alhimenko::Vertex_t alhimenko::readVertex(std::istream& in, bool& last_vertex)
{
  SkipWsFlagSaver saver(in);

  uint32_t num;
  in >> skipblank >> num;
  if (!correctInput(in))
  {
    throw std::runtime_error("");
  }

  char separator;
  in >> skipblank >> separator;
  if (separator != ':')
  {
    skipcommand(in);
    throw std::runtime_error("\n<INCORRECT COMMAND>\n");
  }

  separator = ',';

  uint32_t edge_to;
  Vertex_t v;
  std::list<uint32_t> edges;
  while (separator == ',')
  {
    in >> skipblank >> edge_to;
    if (!in.good())
    {
      in.clear();
      in >> skipblank >> separator;

      if (separator == '|')
      {
        break;
      }
      else if(separator == ';')
      {
        last_vertex = true;
        break;
      }
      else
      {
        skipcommand(in);
      }
    }

    edges.push_back(edge_to);

    in >> skipblank >> separator;
    if (separator != ',' && separator != '|' && separator != ';')
    {
      skipcommand(in);
      throw std::runtime_error("\n<INCORRECT COMMAND>\n");
    }
    else if (separator == '|')
    {
      break;
    }
    else if (separator == ';')
    {
      last_vertex = true;
      break;
    }
  }
  return Vertex_t{ num, edges };
}

bool alhimenko::checkName(const std::string& name)
{
  for (auto&& i = 0; i < name.size(); ++i)
  {
    if (i == 0)
    {
      if (!std::isalpha(name[i]))
      {
        return false;
      }
    }
    else if (!std::isalpha(name[i]) && !std::isdigit(name[i]))
    {
      return false;
    }
  }

  return true;
}
