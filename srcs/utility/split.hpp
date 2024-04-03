#ifndef SPLITTED_STRING_H
#define SPLITTED_STRING_H

#include <string>
#include <vector>

class Split
{
  public:
    Split();
    Split(Split const &sp);
    Split& operator=(Split const &sp);
    ~Split();
    Split(char *str, std::string const &delimiter);
    Split(std::string const& str, std::string const &delimiter);
    Split(std::string const& str, std::string const &delimiter, bool multi_delimiter);
    Split(std::string const& str, std::string const &delimiter, bool multi_delimiter, bool d_quote);
    //Split(std::string const& str, std::string delimiter, bool rest_delimiter);
    static void split_cp(std::string const& str, std::string delimiter, std::vector<std::string>& dst, bool multi_delimiter);
    typedef std::vector<std::string>::iterator iterator;
    iterator begin();
    iterator end();
    size_t size() const;
    std::string const &operator[](size_t i) const;
    void concat(std::string& str, std::string delimiter);
    friend std::ostream& operator<<(std::ostream& os, Split& sp);

    void set_word(std::string &str, std::string const &delimiters);
    void print();

  private:
    //bool d_quote;
    std::vector<std::string> _splitted_string;
    //void split(std::string const& str, std::string delimiter, std::vector<std::string> &vec );
    void split_not_delete_delimiter(std::string const& str, std::string const &delimiter);
};

#endif /* SPLITTED_STRING_H */
