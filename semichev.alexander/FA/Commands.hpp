#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include "ForwardDeclaration.hpp"

namespace semichev
{
  class Command
  {
   public:
    virtual void operator()() = 0;
  };

  class Sheets : public Command
  {
   public:
    Sheets() = delete;
    Sheets(const Sheets& other) = default;
    Sheets(Sheets&& other) = default;
    Sheets(Spreadsheet& ws_, std::ostream& out);

    Sheets& operator=(const Sheets& other) = default;
    Sheets& operator=(Sheets&& other) = default;

    void operator()();

   private:
    Spreadsheet& ws_;
    std::ostream& out_;
  };

  class DeleteSheet : public Command
  {
   public:
    DeleteSheet() = delete;
    DeleteSheet(const DeleteSheet& other) = default;
    DeleteSheet(DeleteSheet&& other) = default;
    DeleteSheet(Spreadsheet& ws_, std::istream& in);

    DeleteSheet& operator=(const DeleteSheet& other) = default;
    DeleteSheet& operator=(DeleteSheet&& other) = default;

    void operator()();

  private:
      Spreadsheet& ws_;
      std::istream& in_;
  };

  class Enter: public Command
  {
  public:
      Enter() = delete;
      Enter(const Enter& other) = default;
      Enter(Enter&& other) = default;
      Enter(Spreadsheet& ws, std::istream& in);

      Enter& operator=(const Enter& other) = default;
      Enter& operator=(Enter&& other) = default;

      void operator()();

  private:
      Spreadsheet& ws_;
      std::istream& in_;
  };

  class Print: public Command
  {
  public:
      Print() = delete;
      Print(const Print& other) = default;
      Print(Print&& other) = default;
      Print(Spreadsheet& ws, std::istream& in, std::ostream& out);

      Print& operator=(const Print& other) = default;
      Print& operator=(Print&& other) = default;

      void operator()();

  private:
      Spreadsheet& ws_;
      std::istream& in_;
      std::ostream& out_;
  };

  class Peek : public Command
  {
  public:
      Peek() = delete;
      Peek(const Peek& other) = default;
      Peek(Peek&& other) = default;
      Peek(Spreadsheet& ws, std::istream& in, std::ostream& out);

      Peek& operator=(const Peek& other) = default;
      Peek& operator=(Peek&& other) = default;

      void operator()();
      void operator()(const std::string& str);

  private:
      Spreadsheet& ws_;
      std::istream& in_;
      std::ostream& out_;
  };

  class Copy : public Command
  {
  public:
      Copy() = delete;
      Copy(const Copy& other) = default;
      Copy(Copy&& other) = default;
      Copy(Spreadsheet& ws, std::istream& in);

      Copy& operator=(const Copy& other) = default;
      Copy& operator=(Copy&& other) = default;

      void operator()();

  private:
      Spreadsheet& ws_;
      std::istream& in_;
  };

  class Paste : public Command
  {
  public:
      Paste() = delete;
      Paste(const Paste& other) = default;
      Paste(Paste&& other) = default;
      Paste(Spreadsheet& ws, std::istream& in);

      Paste& operator=(const Paste& other) = default;
      Paste& operator=(Paste&& other) = default;

      void operator()();

  private:
      Spreadsheet& ws_;
      std::istream& in_;
  };
}

#endif
