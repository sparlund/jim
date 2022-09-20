#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
// two below for get_window_size
#include <sys/ioctl.h>
#include <unistd.h>
// std::count
#include <algorithm>

// Returns 1) rows 2) columns
std::pair<size_t,size_t> get_window_size()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return std::pair<size_t,size_t>{w.ws_row,w.ws_col};
}

size_t number_of_line_in_files(std::ifstream& file)
{
    return std::count(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>(), '\n');
}

class Editor
{
public:
    size_t current_row = 0, current_column= 0, screenrows, screencolumns;
    Editor()
    {
        auto w = get_window_size();
        screenrows    = w.first;
        screencolumns = w.second;
    };
    std::string bottom_message = "\x1b[6nHEJ";
    ~Editor()= default;
};

int main(int argc, char* argv[])
{
    auto filename = std::string{argv[1]};
    std::cout << "filename = " << filename << "\n";
    std::ifstream file(filename);
    if (file.fail())
    {
        // file could not be opened
        exit(0);
    }
    auto a = number_of_line_in_files(file);
    std::cout << a << std::endl;
    std::vector<std::string> lines;
    std::string line;
    Editor e;
    // Read entire file.
    // In future only read a small part of file at a time to use less memory and increase speed?
    while (getline(file, line))
    {
        std::cout << line << std::endl;
        lines.push_back(line);
        if (file.eof())
        {
            break;
        }
    }
    // Print lines
    std::cout << "e.screenrows="<< e.screenrows << std::endl;
    std::cout << "lines.size()="<< lines.size() << std::endl;
    // while (true)
    // {
        // Keep drawing until user quits?
        for (size_t i = 0; i < lines.size(); i++)
        {
            // if(i < e.screenrows)
            // {
                std::cout << i <<  "\u2502" << lines[i] << '\n';
            // }
        }
    // }
    


}