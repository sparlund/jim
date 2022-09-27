#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <utility>
// two below for get_window_size
#include <sys/ioctl.h>
#include <unistd.h>
// std::count
#include <algorithm>
#include <termios.h>    // struct termios, tcgetattr(), tcsetattr()
#include <stdio.h>      // perror(), stderr, stdin, fileno()


// Returns 1) rows 2) columns
std::pair<size_t,size_t> get_window_size()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return std::pair<size_t,size_t>{w.ws_row,w.ws_col};
}

size_t number_of_line_in_files(std::ifstream& file)
{
    // auto numlines = std::count(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>(), '\n');

    // return ;
}

class Editor
{
public:
    size_t current_row = 0, current_column= 0, screenrows, screencolumns, row_offset=0;
    std::vector<std::string> lines;
    struct termios t;
    struct termios t_saved;
    // special keys enums
    Editor()
    {
        auto w = get_window_size();
        screenrows    = w.first;
        screencolumns = w.second;

        // Set terminal to single character mode.
        tcgetattr(fileno(stdin), &t);
        t_saved = t;
        t.c_lflag &= (~ICANON & ~ECHO);
        t.c_cc[VTIME] = 0;
        t.c_cc[VMIN] = 1;
        if (tcsetattr(fileno(stdin), TCSANOW, &t) < 0) {
            perror("Unable to set terminal to single character mode");
        }
    };
    std::string bottom_message = "\x1b[6nHEJ";
    ~Editor()
    {
        // Restore terminal mode.
        if (tcsetattr(fileno(stdin), TCSANOW, &t_saved) < 0)
        {
            perror("Unable to restore terminal mode");
        }
    };
    size_t getNumberOfDigitsInNumberOfLines(size_t i)
    {
        return i > 0 ? static_cast<size_t>(std::log10((double) i) + 1) : 1;
    }
    void refresh()
    {
        // update cursor position?
        size_t from = current_row - row_offset;
        size_t to   = current_row + (screenrows - row_offset);
        for (size_t i = std::max(from,(size_t)0); i < to ; i++)
        {
            if (i == current_row)
            {
                std::cout << "\033[;43m";
            }
            std::cout << std::setw(getNumberOfDigitsInNumberOfLines(lines.size())) << std::setfill(' ') << i <<  "\u2502"
            << lines[i]
            << std::setw(screencolumns-lines[i].size()-getNumberOfDigitsInNumberOfLines(lines.size())-1) << std::setfill(' ')
            << "\033[;39m" << '\n';
        }

    }
    void process_keys()
    {
        std::streambuf *pbuf = std::cin.rdbuf();
        bool done = false;
        while (!done)
        {
            char c;
            if (pbuf->sgetc() == EOF)
            {
                done = true;
            }
            c = pbuf->sbumpc();
            if (c == 0x1b)
            {
                done = true;
            }
            else if (c == 0x42)
            {
                (row_offset > screenrows)? row_offset=screenrows : row_offset++;
                current_row++;
                done = true;
            }
            // tab
            else if (c == 'a')
            {
                
            }
            else
            {
                // std::cout << "You entered character 0x" << std::setw(2) << std::setfill('0') << std::hex << int(c) << "'" << std::endl;
            }
            // if (c == "a")
            // {
            // }
        }
        // {
            // up
            // left
            // else if("[D")
            // {

            // }
            // // right
            // else if("[C")
            // {

            // }
            // // down
            // else if("[B")
            // {
            //     current_row++;
            // }
        // }
    }
};


int main(int argc, char* argv[])
{
    auto filename = std::string{argv[1]};
    std::ifstream file(filename);
    if (file.fail())
    {
        // file could not be opened
        exit(0);
    }
    // TODO: This doesn't close the file correctly or something!
    auto a = number_of_line_in_files(file);
    std::vector<std::string> lines;
    std::string line;
    Editor e;
    // Read entire file.
    // In future only read a small part of file at a time to use less memory and increase speed?
    while (getline(file, line))
    {
        e.lines.push_back(line);
        if (file.eof())
        {
            break;
        }
    }
    // Print lines
    // 2J means clear the entire screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    // H positions the cursor, takes 2 arguments, 0 used below
    write(STDOUT_FILENO, "\x1b[H", 3);
    // below would move cursor to row 12, column 40
    // [12;40H
    while (true)
    {
        e.refresh();
        // Wait for keypress before we re-draw
        e.process_keys();
    }
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);


}