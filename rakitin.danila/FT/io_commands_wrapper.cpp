#include "io_commands_wrapper.h"

#include <iomanip>

#include "dictionary_utils.h"
#include "iofmtguard.h"

void rakitin::executeAdd(dictionaries& dicts, std::ostream& out, std::istream& in)
{
  std::string path;
  std::string dataname;
  in >> path >> dataname;
  rakitin::add(dicts, dataname, path);
  out << "Dictionary " << dataname << " has been added\n";
}

void rakitin::executeFindWord(const dictionaries& dicts, std::ostream& out, std::istream& in)
{
  std::string word;
  std::string dataname;
  in >> word >> dataname;
  rakitin::ref_finder finder = rakitin::findWord(dicts, dataname, word);
  if (finder.second) {
    rakitin::printWord(finder.first, out);
    out << "\n";
  }
  else {
    out << "The word does not occur in the text\n";
  }
}

void rakitin::executePrintAll(const dictionaries& dicts, std::ostream& out, std::istream& in)
{
  std::string dataname;
  in >> dataname;
  rakitin::printAll(dicts, dataname, out);
  out << "\n";
}

void rakitin::executeMerge(dictionaries& dicts, std::ostream& out, std::istream& in)
{
  std::string dataname1, dataname2;
  in >> dataname1 >> dataname2;
  rakitin::merge(dicts, dataname1, dataname2);
  out << "Dictionaries successfully merged\n";
}

void rakitin::executeClear(dictionaries& dicts, std::ostream& out, std::istream& in)
{
  std::string dataname1;
  in >> dataname1;
  rakitin::clear(dicts, dataname1);
  out << "Dictionary was cleared\n";
}

void rakitin::executeIntersection(dictionaries& dicts, std::ostream& out, std::istream& in)
{
  std::string dataname1, dataname2;
  in >> dataname1 >> dataname2;
  rakitin::intersection(dicts, dataname1, dataname2);
  out << "Intersection dictionary was created with name " << dataname1 << "&" << dataname2 << "_intersection\n";
}

void rakitin::executeSimilarity(const dictionaries& dicts, std::ostream& out, std::istream& in)
{
  std::string dataname1, dataname2;
  in >> dataname1 >> dataname2;
  rakitin::iofmtguard guard(out);
  out << std::setprecision(1) << std::fixed;
  double sim = rakitin::similarity(dicts, dataname1, dataname2) * 100;
  out << "Similarity: " << sim << "%\n";
}

void rakitin::executeRename(dictionaries& dicts, std::ostream& out, std::istream& in)
{
  std::string dataname, newDataname;
  in >> dataname >> newDataname;
  rakitin::rename(dicts, dataname, newDataname);
  out << "Dictionary was renamed\n";
}

void rakitin::executePrintAllDatanames(const dictionaries& dicts, std::ostream& out)
{
  rakitin::printAllDatanames(dicts, out);
  out << "\n";
}

void rakitin::executeDelete(dictionaries& dicts, std::ostream& out, std::istream& in)
{
  std::string dataname1;
  in >> dataname1;
  rakitin::deleteDictionary(dicts, dataname1);
  out << "Dictionary was deleted\n";
}
