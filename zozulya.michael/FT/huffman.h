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
    const std::string TYPE = ".hff";
    int ratio{ 0 };
    int file_size{ 0 };
    int new_file_size{ 0 };
    std::string file_name;
    std::string out_file_name;
    std::vector<int> frequency;
    std::vector<std::string> codes;
    priority_queue_t queue;
    zozulya::pointer root{ nullptr };
    std::string encode_message;
    std::string decode_message;
    std::string message;

  public:
    Huffman();
    Huffman(const std::string name);
    virtual ~Huffman() = default;

    void fillFrequency();
    void readFile();
    void printCodeDifference();

    int compress(std::string& path);
    int decompress();

  private:
    int encode_file();
    int fill_queue();
    void build_tree();
    void message2code(std::ifstream& input_file);
    void recursive_get_nodes(zozulya::Node::pointer& node, std::string str);
    void write_frequency(std::ofstream& output_file);
    void write_raw_message(std::ofstream& output_file);
    int  write_encoding_file(std::string& path);
    void read_frequency(std::ifstream& input_file, zozulya::uchar& count);
    void read_raw_message(std::ifstream& input_file);
    int  read_decoding_file();
    void check_file_name();
    void codes_of_nodes();
    int  write_decoding_file();
  };
}
#endif
