#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "node.h"
#include <queue>
#include <vector>

namespace zozulya
{
  using priority_queue_t = std::priority_queue< zozulya::pointer, std::vector< zozulya::pointer >, zozulya::Greater >;

  class Huffman
  {
  public:
    Huffman();
    Huffman(const std::string name);
    virtual ~Huffman() = default;
    void fillFrequency();
    void readFile();
    void printCodeDifference();
    void compress(std::string& path);
    void decompress();
  private:
    const std::string TYPE = ".hff";
    std::string file_name;
    std::string out_file_name;
    std::vector< int > frequency;
    std::vector< std::string > codes;
    priority_queue_t queue;
    zozulya::pointer root{ nullptr };
    std::string encode_message;
    std::string decode_message;
    std::string message;
    void encode_file();
    void fill_queue();
    void build_tree();
    void message2code(std::ifstream& input_file);
    void recursive_get_nodes(zozulya::Node::pointer& node, std::string str);
    void write_frequency(std::ofstream& output_file);
    void write_raw_message(std::ofstream& output_file);
    void write_encoding_file(std::string& path);
    void read_frequency(std::ifstream& input_file, zozulya::uchar& count);
    void read_raw_message(std::ifstream& input_file);
    void read_decoding_file();
    void check_file_name();
    void codes_of_nodes();
    void write_decoding_file();
  };
}
#endif
