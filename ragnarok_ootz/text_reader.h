#pragma once

#include <fstream>
#include <string>
#include <vector>

class TextReader
{
public:
    TextReader(std::string const& text_file_path)
    {
        _text_file_stream.open(text_file_path, std::ios::in);
    }

    ~TextReader()
    {
        _text_file_stream.close();
    }

    operator bool()
    {
        return _text_file_stream.is_open();
    }

    // read모드인지 확인하는 거 추가해야 함
    std::vector<std::string> getNextTokens()
    {
        commentDelete();
        std::string buffer;
        std::getline(_text_file_stream, buffer);
        return tokenize(buffer);
    }

private:
    void commentDelete()
    {
        while (_text_file_stream.peek() == '/')
        {
            std::string buffer;
            std::getline(_text_file_stream, buffer);
        }
    }

    std::vector<std::string> tokenize(std::string const& string)
    {
        std::vector<std::string> ret;
        size_t offset = 0;
        size_t count = string.find(' ');
        while (count != std::string::npos)
        {
            ret.emplace_back(string.substr(offset, count));
            offset = count + 1;
            count = string.find(' ', offset);
        }
        ret.emplace_back(string.substr(offset, count));
        return ret;
    }

    std::fstream _text_file_stream;
};
