#include "huffman.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <limits>

zozulya::Huffman::Huffman()
{}

zozulya::Huffman::Huffman(const std::string name):
  file_name(name), frequency(0x100, 0), codes(0x100, "")
{}

void zozulya::Huffman::fillFrequency()
{
  for (unsigned short i = 0; i < frequency.size(); i++)
  {
    if (frequency[i] != 0)
    {
      std::cout << "[" << static_cast< char >(i) << "] = " << frequency[i] << " \n";
    }
  }
  std::cout << std::endl;
}

void zozulya::Huffman::readFile()
{
  std::ifstream inputFile;
  inputFile.open(file_name);
  if (!inputFile)
  {
    throw std::logic_error("File opening error\n");
  }
  std::string message;
  while (!inputFile.eof())
  {
    std::getline(inputFile, message);
  }
  std::cout << message << std::endl;
}

void zozulya::Huffman::printCodeDifference()
{
  readFile();
  std::ifstream codeFile;
  codeFile.open(out_file_name);
  std::string message;
  while (!codeFile.eof())
  {
    std::getline(codeFile, message);
  }
  std::cout << "[" << message << "]\n";
}

void zozulya::Huffman::compress(std::string& path)
{
  try
  {
    encode_file();
    fill_queue();
    build_tree();
    root = queue.top();
    recursive_get_nodes(root, "");
    write_encoding_file(path);
  }
  catch (const std::logic_error& e)
  {
    std::cout << e.what() << std::endl;
  }
}

void zozulya::Huffman::decompress()
{
  check_file_name();
  decode_message = "";
  try
  {
    read_decoding_file();
    fill_queue();
    build_tree();
    root = queue.top();
    codes_of_nodes();
    write_decoding_file();
  }
  catch (const std::logic_error& e)
  {
    std::cout << e.what() << std::endl;
  }
}



void zozulya::Huffman::encode_file()
{
  std::ifstream is(file_name, std::ifstream::binary);
  if (!is)
  {
    throw std::logic_error("File opening error\n");
  }
  while (!is.eof())
  {
    char ch;
    is.read(&ch, 1);
    ++frequency[ch];
  }
}

void zozulya::Huffman::fill_queue()
{
  for (size_t i = 0; i < frequency.size(); ++i)
  {
    if (frequency[i] != 0)
    {
      zozulya::pointer n = std::make_shared< zozulya::Node >(i % 256, frequency[i]);
      queue.push(n);
    }
  }
}

void zozulya::Huffman::build_tree()
{

  while (queue.size() > 1)
  {
    zozulya::Node::pointer x = queue.top();
    queue.pop();

    zozulya::Node::pointer y = queue.top();
    queue.pop();

    std::string name = "";
    name += x->get_name();
    name += y->get_name();

    zozulya::Node::pointer z = std::make_shared< zozulya::Node >(name, *x + *y);
    z->left = x;
    z->right = y;

    x->parent = z;
    y->parent = z;

    queue.push(z);
  }
}

void zozulya::Huffman::message2code(std::ifstream& ifs)
{
  while (!ifs.eof())
  {
    char ch;
    ifs.read(&ch, 1);
    encode_message += codes[ch];
  }
}

void zozulya::Huffman::recursive_get_nodes(zozulya::Node::pointer& node, std::string str)
{
  if (node->left == nullptr && node->right == nullptr)
  {
    node->code(str);
    codes[node->get_byte()] = node->code();
    return;
  }

  if (node->left != nullptr)
  {
    recursive_get_nodes(node->left, str + "0");
  }

  if (node->right != nullptr)
  {
    recursive_get_nodes(node->right, str + "1");
  }
}

void zozulya::Huffman::write_frequency(std::ofstream& output_file)
{
  for (size_t j = 0; j < frequency.size(); ++j)
  {
    if (frequency[j])
    {
      output_file << static_cast< char >(j % 256);
      output_file << static_cast< char >(frequency[j] % 256);
    }
  }
}

void zozulya::Huffman::write_raw_message(std::ofstream& output_file)
{
  size_t byte_round = encode_message.size() / 8u;
  size_t modulo = encode_message.size() % 8u;

  output_file << static_cast< char >(byte_round % 256);
  output_file << static_cast< char >(modulo % 256);

  size_t i;
  for (i = 0u; i < byte_round * 8u; i += 8u)
  {
    std::bitset< 8 > b(encode_message.substr(i, 8u));
    zozulya::uchar value = static_cast< unsigned char >(b.to_ulong());
    output_file << value;
  }

  if (modulo > 0u)
  {
    std::bitset< 8 > b(encode_message.substr(i, modulo));
    zozulya::uchar value = static_cast< unsigned char >(b.to_ulong());
    output_file << value;
  }
}

void zozulya::Huffman::write_encoding_file(std::string& path)
{
  out_file_name = path + ".hff";

  std::ifstream input_file(file_name, std::ifstream::binary);
  std::ofstream output_file(out_file_name, std::ofstream::binary);

  if (!input_file || !output_file)
  {
    throw std::logic_error("File opening error\n");
  }

  message2code(input_file);
  zozulya::uchar count = count_if(frequency.begin(), frequency.end(), [](const auto& value) { return value != 0; });

  output_file << count;

  write_frequency(output_file);
  write_raw_message(output_file);

  input_file.close();
  output_file.close();
}

void zozulya::Huffman::read_frequency(std::ifstream& input_file, zozulya::uchar& count)
{
  zozulya::uchar i = 0;
  while (i < count)
  {
    char index;
    input_file >> std::noskipws >> index >> std::skipws;

    char f;
    input_file >> f >> std::skipws;

    frequency[static_cast< size_t >(index)] = static_cast< int >(f);
    ++i;
  }
}

void zozulya::Huffman::read_raw_message(std::ifstream& input_file)
{
  char byte_round = 0;
  input_file >> std::noskipws >> byte_round;
  char modulo;
  input_file >> modulo;

  size_t entire_block_size = static_cast< size_t >(byte_round) * 8u;
  while (decode_message.size() < entire_block_size + modulo)
  {
    while (decode_message.size() < entire_block_size)
    {
      char ch;
      input_file >> ch;

      std::bitset< 8 > b(ch);

      decode_message += b.to_string();
    }
    char ch;
    input_file >> ch >> std::skipws;
    std::bitset< 8 > b(ch);
    decode_message += (b.to_string()).substr(8 - modulo, 8);
  }
}

void zozulya::Huffman::check_file_name()
{
  out_file_name = file_name.substr(0, file_name.size() - TYPE.size());
  out_file_name += ".1";
  std::cout << "Output file: " << out_file_name << std::endl;
}

void zozulya::Huffman::read_decoding_file()
{
  std::ifstream input_file(file_name, std::ifstream::binary);
  if (!input_file)
  {
    throw std::logic_error("File opening error\n");
  }

  zozulya::uchar count = 0;
  input_file >> std::noskipws >> count >> std::skipws;
  read_frequency(input_file, count);
  read_raw_message(input_file);
}


void zozulya::Huffman::codes_of_nodes()
{
  zozulya::Node::pointer node = root;

  auto found_the_letter = [this](zozulya::Node::pointer& n)
  {
    message += n->get_byte();
    n = root;
  };

  for (size_t i = 0; i < decode_message.size(); ++i)
  {
    char ch = decode_message[i];
    if (ch == '0')
    {
      if (node->left != nullptr)
      {
        node = node->left;
        if (node->is_leaf())
        {
          found_the_letter(node);
        }
      }
    }
    else if (ch == '1')
    {
      if (node->right != nullptr)
      {
        node = node->right;
        if (node->is_leaf())
        {
          found_the_letter(node);
        }
      }
    }
  }
}

void zozulya::Huffman::write_decoding_file()
{
  std::ofstream output_file(out_file_name);
  if (!output_file)
  {
    throw std::logic_error("File opening error\n");
  }
  output_file << message;
  output_file.close();
}
