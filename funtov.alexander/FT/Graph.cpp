#include "Graph.hpp"
#include <algorithm>
#include <istream>
#include <ostream>

funtov::Graph::Graph():
  plan(std::vector< std::vector< size_t > >(0)),
  count(0)
{}
std::ostream& funtov::operator<<(std::ostream& os, Graph& other)
{
  os << other.count << std::endl;
  for (size_t i = 0; i < other.plan.size(); ++i)
  {
    for (size_t j = 0; j < other.plan[0].size() - i; ++j)
    {
      if (other.plan[i][j] != 0U)
      {
        os << other.mp[i] << " <--> " << other.mp[j] << " = " << other.plan[i][j] << std::endl;
      }
    }
  }
  return os;
}
void funtov::Graph::addApex(const std::string& name)
{
  if (plan.empty())
  {
    count++;
    mp[count - 1] = name;
    pm[name] = count - 1;
    plan.emplace_back(1, 0);
    return;
  }
  count++;
  mp[count - 1] = name;
  pm[name] = count - 1;
  std::vector< std::vector< size_t > > temp =
    std::vector< std::vector< size_t > >(count, std::vector< size_t >(count, 0));
  for (size_t i = 0; i < count - 1; i++)
  {
    for (size_t j = 0; j < count - 1; j++)
    {
      temp[i][j] = plan[i][j];
    }
  }
  plan = temp;
}
void funtov::Graph::addJoint(const std::string& apexName1, const std::string& apexName2, size_t length)
{
  plan[pm[apexName1]][pm[apexName2]] = length;
  plan[pm[apexName2]][pm[apexName1]] = length;
}
std::istream& funtov::operator>>(std::istream& is, Graph& other)
{
  std::istream::sentry const sentry(is);
  if (!sentry)
  {
    return is;
  }

  size_t n = 0;
  is >> n;
  std::string name;
  for (size_t i = 0; i < n; i++)
  {
    is >> name;
    if (is.rdstate() != std::ios::failbit)
    {
      other.addApex(name);
    }
  }

  is >> n;
  std::string name1, name2;
  size_t length = 0;
  for (size_t i = 0; i < n; i++)
  {
    is >> name1 >> name2 >> length;
    if (is.rdstate() != std::ios::failbit)
    {
      other.addJoint(name1, name2, length);
    }
  }

  return is;
}
size_t funtov::Graph::getApexCnt() const
{
  return count;
}
size_t funtov::Graph::getLen(const std::string& first, const std::string& second)
{
  return plan[pm[first]][pm[second]];
}
void funtov::Graph::changeWay(const std::string& apexName1, const std::string& apexName2, size_t length)
{
  plan[pm[apexName1]][pm[apexName2]] = length;
}
void funtov::Graph::deleteWay(const std::string& apexName1, const std::string& apexName2)
{
  plan[pm[apexName1]][pm[apexName2]] = 0;
}
void funtov::Graph::deleteGraph()
{
  plan.clear();
  count = 0;
  pm.clear();
  mp.clear();
}
void funtov::Graph::checkOtherWays(std::ostream& os, const std::string& apexName)
{
  for (size_t i = 0; i < plan[pm[apexName]].size(); ++i)
  {
    if (plan[pm[apexName]][i] != 0U)
    {
      os << mp[pm[apexName]] << " <--> " << mp[i] << " = " << plan[pm[apexName]][i] << std::endl;
    }
  }
}
std::vector< size_t > funtov::Graph::topoSort()
{
  std::vector< size_t > ans(getApexCnt());
  std::vector< bool > used(getApexCnt());
  ans.clear();
  for (size_t i = 0; i < getApexCnt(); ++i)
  {
    if (!used[i])
    {
      dfs(i, used, ans);
    }
  }
  std::reverse(ans.begin(), ans.end());
  return ans;
}
std::vector< size_t > funtov::Graph::doDfs()
{
  std::vector< bool > used(getApexCnt());
  std::vector< size_t > ans(getApexCnt());
  dfs(0, used, ans);
  return ans;
}
void funtov::Graph::dfs(size_t v, std::vector< bool >& used, std::vector< size_t >& ans)
{
  used[v] = true;
  for (size_t i = 0; i < plan[v].size(); ++i)
  {
    if (plan[v][i] != 0U)
    {
      size_t const to = i;
      if (!used[to])
      {
        dfs(to, used, ans);
      }
    }
  }
  ans.push_back(v);
}
