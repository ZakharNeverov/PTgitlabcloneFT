#include "huffman.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <sstream>
#include <limits>

zozulya::Huffman::Huffman()
{}

zozulya::Huffman::Huffman(const std::string name):
  file_name(name), frequency(0x100, 0), codes(0x100, "")
{}

void zozulya::Huffman::fillFrequency()
{
  for (int i = 0; i < frequency.size(); i++)
  {
    if (frequency[i] != 0)
    {
      std::cout << "[" << (char) i << "] = " << frequency[i] << " \n";
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
    std::cerr << "Error in name of file." << std::endl;
  }
  else
  {
    std::cout << inputFile.rdbuf();
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void zozulya::Huffman::printCodeDifference()
{
  readFile();
  std::cout << "[" << std::ifstream(out_file_name).rdbuf() << "]";
  std::cout << std::endl;
  std::cout << std::endl;
}

int zozulya::Huffman::compress(std::string& path)
{
  int ret = encode_file();
  if (ret == -1)
  {
    std::cerr << "Error in encode file." << std::endl;
    return -1;
  }

  ret = fill_queue();
  if (ret == -1)
  {
    std::cerr << "Error in fill queue." << std::endl;
    return -1;
  }

  build_tree();
  root = queue.top();
  recursive_get_nodes(root, "");
  ret = write_encoding_file(path);
  if (ret == -1)
  {
    std::cerr << "Error in write encoding file." << std::endl;
    return -1;
  }
  return 0;
}

int zozulya::Huffman::decompress()
{
  check_file_name();

  decode_message = "";
  int ret = read_decoding_file();
  if (ret == -1)
  {
    std::cerr << "Error in read decoding file." << std::endl;
    return -1;
  }

  ret = fill_queue();
  if (ret == -1)
  {
    std::cerr << "Error in fill queue." << std::endl;
    return -1;
  }


  build_tree();
  root = queue.top();

  codes_of_nodes();

  ret = write_decoding_file();
  if (ret == -1)
  {
    std::cerr << "Error in writing decoding file." << std::endl;
    return -1;
  }
  return 0;
}



int zozulya::Huffman::encode_file()
{
  std::ifstream is(file_name, std::ifstream::binary);
  if (!is)
  {
    std::cerr << "File opening error: " << file_name << std::endl;
    return -1;
  }

  int i = 0;
  while (true)
  {
    char ch;
    is.read(&ch, 1);
    ++i;
    if (is.eof())
    {
      break;
    }
    ++frequency[static_cast<unsigned char>(ch)];
  }
  return 0;
}

int zozulya::Huffman::fill_queue()
{
  for (size_t i = 0; i < frequency.size(); ++i)
  {
    if (frequency[i] != 0)
    {
      zozulya::pointer n = std::make_shared< zozulya::Node >(static_cast<zozulya::uchar>(i), frequency[i]);
      queue.push(n);
    }
  }
  return 0;
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
  while (true)
  {
    char ch;
    ifs.read(&ch, 1);

    if (ifs.eof())
    {
      break;
    }
    encode_message += codes[static_cast< zozulya::uchar >(ch)];
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
      output_file.write((char*)&j, sizeof(zozulya::uchar));
      output_file.write((char*)&frequency[j], sizeof(int));
    }
  }
}

void zozulya::Huffman::write_raw_message(std::ofstream& output_file)
{
  int byte_round = encode_message.size() / CHAR_BIT;
  zozulya::uchar modulo = encode_message.size() % CHAR_BIT;

  output_file.write((char*)&byte_round, sizeof(byte_round));
  output_file.write((char*)&modulo, sizeof(zozulya::uchar));

  int i;
  for (i = 0; i < byte_round * CHAR_BIT; i += CHAR_BIT)
  {
    std::bitset< CHAR_BIT > b(encode_message.substr(i, CHAR_BIT));
    zozulya::uchar value = b.to_ulong();
    output_file.write((char*)&value, sizeof(zozulya::uchar));
  }

  if (modulo > 0)
  {
    std::bitset< CHAR_BIT > b(encode_message.substr(i, modulo));
    zozulya::uchar value = b.to_ulong();
    output_file.write((char*)&value, sizeof(char));
  }
}

int zozulya::Huffman::write_encoding_file(std::string& path)
{
  out_file_name = path + ".hff";

  std::ifstream input_file(file_name, std::ifstream::binary);
  std::ofstream output_file(out_file_name, std::ofstream::binary);

  if (!input_file)
  {
    std::cerr << "File opening error: " << file_name << std::endl;
    return -1;
  }

  message2code(input_file);
  zozulya::uchar count = count_if(frequency.begin(), frequency.end(), [](const auto& value) { return value != 0; });

  output_file.write((char*)&count, sizeof(count));

  write_frequency(output_file);

  write_raw_message(output_file);

  input_file.close();
  output_file.close();
  return 0;
}

void zozulya::Huffman::read_frequency(std::ifstream& input_file, zozulya::uchar& count)
{
  zozulya::uchar i = 0;
  while (i < count)
  {
    char index;
    input_file.read(&index, sizeof(index));

    int f;
    input_file.read(reinterpret_cast<char*>(&f), sizeof(int));

    frequency[static_cast< zozulya::uchar >(index)] = f;
    ++i;
  }
}

void zozulya::Huffman::read_raw_message(std::ifstream& input_file)
{
  int byte_round = 0;
  input_file.read(reinterpret_cast<char*>(&byte_round), sizeof(int));
  char modulo;
  input_file.read(&modulo, sizeof(modulo));

  std::stringstream ss;
  size_t entire_block_size = byte_round * CHAR_BIT;
  while (decode_message.size() < entire_block_size + modulo)
  {
    while (decode_message.size() < entire_block_size)
    {
      char ch;
      input_file.read(&ch, sizeof(ch));

      std::bitset< CHAR_BIT > b(ch);

      decode_message += b.to_string();
    }
    char ch;
    input_file.read(&ch, sizeof(ch));
    std::bitset< CHAR_BIT > b(ch);
    decode_message += (b.to_string()).substr(CHAR_BIT - modulo, CHAR_BIT);
  }
  std::cout << std::endl;
}

void zozulya::Huffman::check_file_name()
{
  out_file_name = file_name.substr(0, file_name.size() - TYPE.size());
  out_file_name += ".1";
  std::cout << "Output file: " << out_file_name << std::endl;
}

int zozulya::Huffman::read_decoding_file()
{
  std::ifstream input_file(file_name, std::ifstream::binary);
  if (!input_file)
  {
    std::cerr << "File opening error: " << file_name << std::endl;
    return -1;
  }

  zozulya::uchar count = 0;
  input_file.read(reinterpret_cast<char*>(&count), sizeof(count));

  read_frequency(input_file, count);
  read_raw_message(input_file);
  return 0;
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

int zozulya::Huffman::write_decoding_file()
{
  std::ofstream output_file(out_file_name);
  if (!output_file)
  {
    std::cerr << "Error in opening output file." << std::endl;
    return -1;
  }
  output_file << message;
  output_file.close();
  return 0;
}
