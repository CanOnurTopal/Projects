#include "utf8string.h"

namespace utfstring {



    utf8string_char_reference utf8string::operator[](size_type index){
        return this->at(index);
    }

    const utf8string_char_reference utf8string::operator[](size_type index) const{
        return this->at(index);
    }

    //find
    utf8string::size_type utf8string::find (const utf8string& str, size_type pos) const{
        return this->find(str.data(), pos);
    }
    utf8string::size_type utf8string::find (const char32_t* str, size_type pos) const{
        data_container processed_str = utf32to8(str);
        return this->find(processed_str, pos);
    }

    utf8string::size_type utf8string::find (const char32_t str, size_type pos) const{
        data_container processed_str = utf32to8(str);
        return this->find(processed_str, pos);
    }

    std::vector<utf8string::size_type> utf8string::find_all (const char32_t str, size_type pos) const{
        data_container processed_str = utf32to8(str);
        return this->find_all(processed_str, pos);
    }

    std::vector<utf8string::size_type> utf8string::find_all (const utf8string& str, size_type pos) const{
        return this->find_all(str.data(), pos);
    }
    std::vector<utf8string::size_type> utf8string::find_all (const char32_t* str, size_type pos) const{
        data_container processed_str = utf32to8(str);
        return this->find_all(processed_str, pos);
    }

    utf8string::size_type utf8string::find (const data_container& str, size_type pos) const{
        pos = raw_index(pos);
        data_container::size_type result = raw_data.find(str,pos);
        if (result == data_container::npos) return utf8string::npos;
        else return real_index(result);
    }
    std::vector<utf8string::size_type> utf8string::find_all (const data_container& str, size_type pos) const{
        data_container::size_type last_found = raw_index(pos);
        std::vector<utf8string::size_type> found;
        while(true){
            last_found = find(str,last_found);
            if (last_found == utf8string::npos) break;
            else found.push_back(last_found); ++last_found;
        }
        return found;
    }

    //State
    bool utf8string::empty() const{

    }
    utf8string::size_type utf8string::capacity(size_type avg) const{return raw_data.capacity() / avg;}
    utf8string::size_type utf8string::max_size(size_type avg) const{return raw_data.max_size() / avg;}
    void utf8string::reserve(size_type n, size_type avg){raw_data.resize(n*avg);}
    void utf8string::clear(){
        multibytes.empty();
        multibytes_size = 0;
        raw_data.clear();
    }


    //Operator Overloads
    utf8string& utf8string::operator+= (const utf8string &str){
        return this->operator+=(str.data());
    }

    utf8string& utf8string::operator+= (const char32_t* str){
        data_container processed_str = utf32to8(str);
        return this->operator+=(processed_str);
    }

    utf8string& utf8string::operator+= (const data_container& str){
        this->raw_data += str;
        return *this;
    }

    utf8string utf8string::operator+ (const utf8string &str) const{
        return this->operator+(str.data());
    }

    utf8string utf8string::operator+ (const char32_t* str)const{
        data_container processed_str = utf32to8(str);
        return this->operator+(processed_str);
    }

    utf8string utf8string::operator+ (const data_container& str)const{
        data_container temp = this->data() + str;
        return utf8string(temp);
    }
    bool utf8string::operator!= (utf8string &comp) const {return !this->operator== (comp);}

    bool utf8string::operator!= (const char32_t comp[]) const {return !this->operator== (comp);}

    //replace
    utf8string& utf8string::replace(size_type pos, size_type len, size_type n,  char32_t c){
        data_container c8 = utf32to8(c);
        for (size_type x = 1; x < n; x++){
            c8 += c8;
        }
        return this->replace(pos, len, c8);
    }

    utf8string& utf8string::replace(utf8string::size_type pos, utf8string::size_type len, const utf8string::data_container& str){
        auto raw_idx = raw_index(pos);
        auto raw_len = raw_index(pos + len) - raw_idx;
        raw_data.replace(raw_idx, raw_len, str);
        detect_multibytes();
        return *this;
    }


    utf8string& utf8string::replace(utf8string::size_type pos, utf8string::size_type len, char32_t c){
        data_container c8 = utf32to8(c);
        return this->replace(pos,len,c8);
    }

    utf8string& utf8string::replace(size_type pos, size_type len, const char32_t* str){
        data_container c8 = utf32to8(str);
        return this->replace(pos,len,c8);
    }
    utf8string& utf8string::replace(size_type pos, size_type len, const utf8string& str){
        data_container c8 = str.data();
        return this->replace(pos,len,c8);
    }


    //at
    utf8string_char_reference utf8string::at (utf8string::size_type idx){
        return utf8string_char_reference(idx, this);
    }

    const utf8string_char_reference utf8string::at (utf8string::size_type idx) const{
        auto const ref = utf8string_char_reference(idx, this);
        return ref;
    }

    char32_t utf8string::value_at(size_type index) const{
        if (index >= size()){
            throw std::out_of_range("utf8string out of range");
        }
        index = raw_index(index);
        utf8to32(index);
    }


    //Append
    utf8string& utf8string::append(const utf8string& str){
        raw_data.append(str.data());
        detect_multibytes();
        return *this;
    }



    const char32_t* utf8string::to_utf32() const {
        char32_t* utf32string = new char32_t[this->size() + 1];
        for (size_type x = 0; x < size(); ++x){
            utf32string[x] = this->value_at(x);
        }
        utf32string[size()] = '\0';
        return utf32string;
    }

    const utf8string::data_container& utf8string::data() const {return raw_data;}


    bool utf8string::operator== (utf8string &comp) const
    {
        return this->data() == comp.data();
    }

    bool utf8string::operator== (const char32_t comp[]) const
    {
        size_type current_size = size();
        size_type x;
        for (x = 0; comp[x] != '\0'; ++x){
            if (x < current_size && this->value_at(x) == comp[x]) continue;
            else return false;
        }
        if (x == current_size) return true;
        else return false;
    }



    //substr
    utf8string utf8string::substr(size_type pos, size_type len = utf8string::npos) const{
        if (pos >= size()){
            throw std::out_of_range("utf8string substring out of range");
        }

        auto start = raw_index(pos);
        if (len < size()){ //or statement necessary to avoid npos problems
            len = raw_index(pos + len) - start;
        }
        data_container temp(raw_data, start, len);
        return utf8string(temp);
    }


    //Constructors
    utf8string::utf8string(std::istream& input_stream){
            char c;
            while (input_stream.get(c)){raw_data.push_back(c);}
    }

    utf8string::utf8string(const char32_t* text){
        raw_data = utf32to8(text);
        detect_multibytes();
    }

    utf8string::utf8string(data_container& temp_data){
        raw_data.swap(temp_data);
        detect_multibytes();
    }


    //Data Manip
    utf8string::data_container utf8string::utf32to8(const char32_t* utf32text) const {//Could be optimized with null terminated arrays
        data_container result;
        for (int x = 0; utf32text[x] != '\0'; x++){

            if (utf32text[x] <= 127) result.push_back(utf32text[x]); //expressable by 7 bits
            else{
                for (int required_utf8 = 2; required_utf8 < 7; ++required_utf8){

                    char32_t capacity = (1 << (required_utf8 *5 + 1)) -1;
                    if (capacity >= utf32text[x]){ //if data is representable by required_utf8 amount of utf8 variables
                        for (int index = 0; index < required_utf8; ++index){
                            char32_t temp = utf32text[x] >> ((required_utf8 - index - 1) * 6);
                            if (index == 0){
                                char32_t leftmask = 0;
                                for(int bit_index = 7; bit_index > 7-required_utf8; --bit_index) leftmask = leftmask | (1 << bit_index); //generate the information bits
                                temp = temp | leftmask;
                            }
                            else{
                                constexpr char32_t trailer_mask = 63; // 0b00111111
                                constexpr char32_t trailed_info = 128; // 0b10000000
                                temp = temp & trailer_mask; //remove significant bits;
                                temp = temp | trailed_info;
                            }

                            result.push_back(temp);
                        }
                    break;
                    } //if (capacity >= utf32text[x])
                }
            }
        }
        return result;
    }

    utf8string::data_container utf8string::utf32to8(const char32_t utf32text) const {
        data_container result;
            if (utf32text <= 127) result.push_back(utf32text); //expressable by 7 bits
            else{
                for (int required_utf8 = 2; required_utf8 < 7; ++required_utf8){

                    char32_t capacity = (1 << (required_utf8 *5 + 1)) -1;
                    if (capacity >= utf32text){ //if data is representable by required_utf8 amount of utf8 variables
                        for (int index = 0; index < required_utf8; ++index){
                            char32_t temp = utf32text >> ((required_utf8 - index - 1) * 6);
                            if (index == 0){
                                char32_t leftmask = 0;
                                for(int bit_index = 7; bit_index > 7-required_utf8; --bit_index) leftmask = leftmask | (1 << bit_index); //generate the information bits
                                temp = temp | leftmask;
                            }
                            else{
                                constexpr char32_t trailer_mask = 63; // 0b00111111
                                constexpr char32_t trailed_info = 128; // 0b10000000
                                temp = temp & trailer_mask; //remove significant bits;
                                temp = temp | trailed_info;
                            }

                            result.push_back(temp);
                        }
                    break;
                    } //if (capacity >= utf32text[x])
                }
            }
        return result;
    }


    void utf8string::detect_multibytes(){
        b_count skip = 0;
        multibytes_size = 0;
        if (!multibytes.empty()) multibytes.clear();
        for (size_type x = 0; x < raw_data.size() ; ++x){
            if (skip > 0){
                --skip;
                continue;
            }
            b_count continuation = char_bytes(raw_data[x]);
            if (continuation > 1){
                skip = continuation -1;
                multibytes_size += skip;
                multibytes.push_back(std::make_pair(x,skip));
            }
        }
    }


    //Following function assumes a little endian system
    //returns zero if the argument does not begin the character
    utf8string::b_count utf8string::char_bytes(const utf8& character) const {
        b_count byte_count = 1;
        utf8 comparison = 1 << (8 - byte_count); // comparison = 0b10000000
        if ((character & comparison) == 0) return byte_count; // if most significant bit of character == 0
        else //if most significant bit of character == 1
        {
            comparison = 1 << (8 - (byte_count + 1)); // comparison = 0b11000000

            if ((character & comparison) == 0) return 0;// if most significant two bits are not 11 argument is not the beginning of a utf-8 value
            else
            {
                utf8 comparison = 1 << (8 - byte_count); //reset comparison
                while (byte_count < 7){
                    comparison += 1 << (8 - (byte_count + 1));
                    if ((character & comparison) != comparison) return byte_count;
                    else byte_count++;
                }
            }
        }
        return -1; //Can put exception here
    }

    char32_t utf8string::utf8to32(size_type index) const{
        return utf8to32(index, raw_data);
    }


    char32_t utf8string::utf8to32(size_type index, const utf8string::data_container& data_ref) const{
        auto char_len = char_bytes(data_ref[index]);
        if (char_len == 1) return data_ref[index];
        constexpr utf8 mask = 127; //0b01111111
        char32_t value = 0;
        for (int x = char_len -1; x >= 0; --x){
            if (x == 0){
                utf8 leftmask = 0;
                for(int i = 0; i < (8-char_len- 1); i++){
                    utf8 kept_byte = 1 << i;
                    leftmask = leftmask | kept_byte;
                }
                char32_t temp = data_ref[index + x] & leftmask;
                temp = temp << ((char_len -1) * 6);
                value = value | temp;
            }
            else{
                char32_t temp = data_ref[x + index] & mask;
                temp = temp << ((char_len -1 - x) * 6);
                value = value | temp;
            }
        }
        return value;
    }


    //State
    utf8string::size_type utf8string::size() const{
        return raw_data.size() - multibytes_size;
    }

    utf8string::size_type utf8string::raw_index(size_type char_index) const {
        for (size_type x = 0; x < multibytes.size(); ++x){
            if (char_index > std::get<0>(multibytes[x])){
                char_index +=  std::get<1>(multibytes[x]);
            }
            else break;
        }
        return char_index;
    }

    utf8string::size_type utf8string::real_index(size_type raw_index) const {
        size_type subtract = 0;
        for (size_type x = 0; x < multibytes.size(); ++x){
            if (raw_index > std::get<0>(multibytes[x])){
                subtract +=  std::get<1>(multibytes[x]);
            }
            else break;
        }
        return raw_index - subtract;
    }

    const utf8string::utf8* utf8string::raw_utf8() const{
        return raw_data.data();
    }
}

