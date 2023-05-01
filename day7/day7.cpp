#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>
#include <set>

// Within the terminal output, lines that begin with $ are commands you executed, very much like some modern computers:
//
// cd means change directory. This changes which directory is the current directory, but the specific result depends on the argument:
//     cd x moves in one level: it looks in the current directory for the directory named x and makes it the current directory.
//     cd .. moves out one level: it finds the directory that contains the current directory, then makes that directory the current directory.
//     cd / switches the current directory to the outermost directory, /.
//
// ls means list. It prints out all of the files and directories immediately contained by the current directory:
//     123 abc means that the current directory contains a file named abc with size 123.
//     dir xyz means that the current directory contains a directory named xyz.

enum class TokenType {
  Command = 10,
  File = 20,
  Folder = 30,
};

struct Token {
  TokenType type;

  Token(TokenType _type): type(_type) {}
  ~Token() {}
};

struct Command : public Token {
  std::string command;
  std::vector<std::string> params;

  Command(std::string _command, std::vector<std::string> _params):
    Token{ TokenType::Command },
    command(_command),
    params(_params) {}
  ~Command() {}
};

struct File : public Token {
  std::string file_name;
  int file_size;

  File(std::string _file_name, int _file_size):
    Token{ TokenType::File },
    file_name(_file_name),
    file_size(_file_size) {}
  ~File() {}
};

struct Folder : public Token {
  std::string folder_name;
  std::map<std::string, Token*> children;
  bool is_visited = false;

  double total_files_size() {
    double total_size;

    for (auto child : children) {
      if (child.second->type == TokenType::File) {
        total_size += ((File*) child.second)->file_size;
      } else {
        total_size += ((Folder*) child.second)->total_files_size();
      }
    }

    return total_size;
  }

  void add_child(Token* child) {
    if (child->type == TokenType::File) {
      children[((File*) child)->file_name] = child;
    } else {
      children[((Folder*) child)->folder_name] = child;
    }
  }

  Token* get_child(std::string name) {
    if (!children.count(name)) {
      return nullptr;
    }

    return children[name];
  }

  Folder* get_child_folder(std::string name) {
    if (!children.count(name)) {
      throw std::invalid_argument("['" + name + "'] does not exist.");
    }

    if (children[name]->type != TokenType::Folder) {
      throw std::invalid_argument("['" + name + "'] is not a folder.");
    }

    return (Folder*) children[name];
  }

  Folder(std::string _folder_name):
    Token{ TokenType::Folder },
    folder_name(_folder_name) {}
  ~Folder() {}
};

std::vector<std::string> split_tokens(std::string line) {
  std::istringstream ss(line);
  std::string token;
  std::vector<std::string> tokens;

  while(std::getline(ss, token, ' ')) {
    tokens.push_back(token);
  }

  return tokens;
}

void debug(const Token* token) {
  switch (token->type)
  {
  case TokenType::Command: {
    auto it = ((Command*) token);

    if (it->command == "ls") {
      std::cout << it->command << '\n';
    } else {
      std::cout << it->command << " : " << it->params.back()<< '\n';
    }
    
    break;
  }
  case TokenType::Folder:
    std::cout << ((Folder*) token)->folder_name << '\n';
    break;
  case TokenType::File:
    std::cout << ((File*) token)->file_name << " <> " << ((File*) token)->file_size << '\n';
    break;
  
  default:
    break;
  }
}

void print_tree(Folder* root_folder, int indentation) {
  std::string contains = "";

  for (auto child : root_folder->children) {
    if (child.second->type == TokenType::File) {
      contains += " [File(" +
        ((File*) child.second)->file_name + ":" +
        std::to_string(((File*) child.second)->file_size) +
      ")]";
    }
    if (child.second->type == TokenType::Folder) {
      contains += " [Dir(" + ((Folder*) child.second)->folder_name + ")]";
    }
  }

  std::string indent(indentation, ' ');
  std::cout << indent << "> Folder: " << root_folder->folder_name << '\n';
  std::cout << indent << "- Content: " << contains << '\n';
  std::cout << indent << "- Total Size: " << root_folder->total_files_size() << '\n';
  std::cout << '\n';

  for (auto child : root_folder->children) {
    if (child.second->type == TokenType::Folder) {
      print_tree((Folder*) child.second, indentation + 4);
    }
  }
}

void traverse_folders(Folder* root_folder, int& total, int max_total_size) {
  auto size = root_folder->total_files_size();

  if (size < max_total_size) {
    total += size;
  }

  for (auto child : root_folder->children) {
    if (child.second->type == TokenType::Folder) {
      traverse_folders((Folder*) child.second, total, max_total_size);
    }
  }
}

int main()
{
  std::string line;
  std::ifstream data("../data-1.txt");

  std::cout << "Start counting." << '\n';

  if (!data.is_open())
  {
    std::cout << "File cannot be opened." << '\n';
    return 0;
  }

  try
  {
    std::queue<Token*> tokens;

    while (std::getline(data, line))
    {
      int len = line.length();
      if (len)
      {
        auto values = split_tokens(line);
        
        if (line.find("$") != std::string::npos) {
          std::vector<std::string> params;

          short len = values.size();
          for (short i = 2; i < len; ++i) {
            params.push_back(values[i]);
          }

          tokens.emplace(new Command(values[1], params));
        } else {
          if (line.find("dir") != std::string::npos) {
            tokens.emplace(new Folder(values[1]));
            // register folders: must be uniqie path ['a/b/c/', 'a/c/b/', 'a/a/']
          } else {
            tokens.emplace(new File(values[1], stoi(values[0])));
            // register files: must be uniqe file name: ['a/b/c/file.ext', 'a/b/file.ext']
          }
        }
      }
    }

    std::stack<Folder*> st;
    Folder* root_folder = new Folder("/");

    auto root = tokens.front();
    tokens.pop();
    st.push(root_folder);

    if (
      root->type != TokenType::Command ||
      ((Command*) root)->command != "cd" ||
      ((Command*) root)->params.back() != "/"
    ) {
      st.pop();
      throw std::invalid_argument("Invalid root command or params");
    }

    bool is_read_mode = false;
    while (!tokens.empty()) {
      auto token = tokens.front();
      tokens.pop();

      // debug(token);

      if (is_read_mode && !st.top()->is_visited) {
        if (token->type == TokenType::File) {
          st.top()->add_child(token);
        }

        if (token->type == TokenType::Folder) {
          st.top()->add_child(token);
        }
      }

      if (token->type == TokenType::Command) {
        auto command_token = ((Command*) token);

        if (command_token->command == "ls") {
          is_read_mode = true;
          continue;
        }

        if (command_token->command == "cd") {
          is_read_mode = false;
          std::string step = command_token->params.back();

          st.top()->is_visited = true;
          
          if (step == "..") {
            if (st.empty()) {
              throw std::out_of_range("Folders stack is empty");
            }
            
            st.pop();
          } else {
            auto folder = st.top()->get_child_folder(step);
            
            if (!folder) {
              throw std::invalid_argument("Folder not found");
            }
            
            st.push(folder);
          }
        }
      }
    }

    // print tree structure
    print_tree(root_folder, 0);

    // calculate needed size
    int total = 0;
    traverse_folders(root_folder, total, 100000);
    std::cout << "Total: " << total << '\n';
    std::cout << "Root: " << std::to_string((long) root_folder->total_files_size()) << '\n';

    // clear memory
    while (!tokens.empty()) {
      auto token = tokens.front();
      tokens.pop();
      delete token;
    }
    delete root_folder;
  }
  catch (const std::exception & ex)
  {
    std::cout << "Unexpected error occurred: " << ex.what() << '\n';
  }

  std::cout << '\n';

  data.close();
  return EXIT_SUCCESS;
}
