#ifndef UTF8STRING_H
#define UTF8STRING_H
#include <exception>
#include <fstream>
#include <string>
#include <istream>
#include <utility>
#include <vector>

namespace utfstring {
class utf8string_char_reference;

class utf8string {

  public:
    // Definitions
    using b_count = char;
    using utf8 = char;
    using size_type = size_t;
    using multibyte_descriptor = std::pair<size_type, b_count>;
    using data_container = std::basic_string<utf8string::utf8>;
    static constexpr size_type npos = -1;

    //Constructors
    utf8string() = default;
    utf8string(std::istream& input_stream);
    utf8string(const char32_t* text);

    //Operators
    const utf8string_char_reference operator[](std::size_t idx) const;
    utf8string_char_reference operator[](std::size_t idx);
    bool operator== (utf8string &comp) const;
    bool operator== (const char32_t comp[]) const;
    bool operator!= (utf8string &comp) const;
    bool operator!= (const char32_t comp[]) const;
    utf8string& operator+= (const utf8string &str);
    utf8string& operator+= (const char32_t* str);
    utf8string operator+ (const utf8string &str) const;
    utf8string operator+ (const char32_t* str) const;

    //at
    char32_t value_at(size_type idx) const;
    utf8string_char_reference at(size_type);
    const utf8string_char_reference at(size_type) const;

    //Append
    utf8string& append(const utf8string& str);
    //utf8string& append(const char32_t* str); TODO
    //utf8string& append(size_type n, char32_t c); //Fill TODO

    //replace
    utf8string& replace(size_type pos, size_type len, size_type n, char32_t c); //fill
    utf8string& replace(size_type pos, size_type len, char32_t c);
    utf8string& replace(size_type pos, size_type len, const char32_t* c);
    utf8string& replace(size_type pos, size_type len, const utf8string& c);

    //State
    bool empty() const;
    size_type capacity(size_type avg = 4) const;
    size_type size() const;
    size_type max_size(size_type avg = 4) const;
    void reserve(size_type n=0, size_type avg = 4);
    void clear();

    //find
    size_type find (const char32_t str, size_type pos = 0) const;
    size_type find (const utf8string& str, size_type pos = 0) const;
    size_type find (const char32_t* str, size_type pos = 0) const;
    std::vector<utf8string::size_type> find_all (const char32_t str, size_type pos = 0) const;
    std::vector<utf8string::size_type> find_all (const utf8string& str, size_type pos = 0) const;
    std::vector<utf8string::size_type> find_all (const char32_t* str, size_type pos = 0) const;

    //Helper Functions
    utf8string substr(size_type pos, size_type len) const;
    const char32_t* to_utf32() const;
    const utf8* raw_utf8() const;
    const data_container& data() const;


  private:
    //Data
    data_container raw_data;
    std::vector<multibyte_descriptor> multibytes; //increasing index
    size_type multibytes_size = 0;// amount of utf8 variables taken by large characters

    //Binary character manipulation and Conversion

    char32_t utf8to32(size_type index) const;
    char32_t utf8to32(size_type index, const data_container& data_ref) const;
    char32_t utfchar32to8(const char32_t utf32text) const;
    utf8string::data_container utf32to8(const char32_t* text) const;
    utf8string::data_container utf32to8(const char32_t) const;

    //Process Data
    void detect_multibytes(); //data must be non-empty
    b_count char_bytes(const utf8& character) const;
    size_type raw_index(size_type char_index) const;
    size_type real_index(size_type raw_index) const;

    //Base Functions
    utf8string& replace(size_type pos, size_type len, const data_container& str);
    utf8string& operator+= (const data_container &str);
    utf8string operator+ (const data_container &str) const;
    size_type find (const data_container& str, size_type raw_pos) const;
    std::vector<size_type> find_all (const data_container& str, size_type raw_pos) const;

    //Private Constructors
    utf8string(data_container&);


};



    //Proxy Class------------------------------------------------------------------
    class utf8string_char_reference{
    public:
        utf8string_char_reference(utf8string::size_type idx, utf8string* owner):index(idx), data_owner(owner){}
        utf8string_char_reference(utf8string::size_type idx, const utf8string* owner):data_owner(nullptr){cache = owner->value_at(idx);}

        char32_t operator= (const char32_t & c){data_owner->replace(index, 1, c); return data();}
        char32_t operator= (const char32_t & c) const = delete; //Const delete here for compiler errors. Can be removed.
        char32_t operator() (const char32_t & c){return operator=(c);}
        char32_t operator() (const char32_t & c) const = delete;
        char32_t operator+= (const char32_t & c){data_owner->replace(index, 1, c + data()); return data();}
        char32_t operator+= (const char32_t & c) const = delete;
        char32_t operator-= (const char32_t & c){data_owner->replace(index, 1, c - data()); return data();}
        char32_t operator-= (const char32_t & c) const = delete;
        char32_t operator*= (const char32_t & c){data_owner->replace(index, 1, c * data()); return data();}
        char32_t operator*= (const char32_t & c) const = delete;
        char32_t operator/= (const char32_t & c){data_owner->replace(index, 1, c / data()); return data();}
        char32_t operator/= (const char32_t & c) const = delete;
        char32_t operator%= (const char32_t & c){data_owner->replace(index, 1, c % data()); return data();}
        char32_t operator%= (const char32_t & c) const = delete;
        char32_t operator|= (const char32_t & c){data_owner->replace(index, 1, c | data()); return data();}
        char32_t operator|= (const char32_t & c) const = delete;
        char32_t operator^= (const char32_t & c){data_owner->replace(index, 1, c ^ data()); return data();}
        char32_t operator^= (const char32_t & c) const = delete;
        char32_t operator<<= (const char32_t & c){data_owner->replace(index, 1, c << data()); return data();}
        char32_t operator<<= (const char32_t & c) const = delete;
        char32_t operator>>= (const char32_t & c){data_owner->replace(index, 1, c >> data()); return data();}
        char32_t operator>>= (const char32_t & c) const = delete;
        char32_t operator++ (){auto val = data(); data_owner->replace(index, 1, val + 1); return val + 1;}
        char32_t operator++ () const = delete;
        char32_t operator++ (int){auto val = data(); data_owner->replace(index, 1, val + 1); return val;}
        char32_t operator++ (int) const = delete;
        char32_t operator-- (){auto val = data(); data_owner->replace(index, 1, val - 1); return val - 1;}
        char32_t operator-- () const = delete;
        char32_t operator-- (int){auto val = data(); data_owner->replace(index, 1, val - 1); return val;}
        char32_t operator-- (int) const = delete;

        operator char32_t() const {return data();}

        char32_t data() const {
            if(data_owner != nullptr) return data_owner->value_at(index);
            else return cache;
        }
    private:
        utf8string::size_type index;
        utf8string* data_owner;
        char32_t cache;
    };
}


#endif // UTF8STRING_H
