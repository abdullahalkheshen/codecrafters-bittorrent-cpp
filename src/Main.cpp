#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

json decode_bencoded(const std::string& encoded_value) {

    if (std::isdigit(encoded_value[0])) {
        // Example: "5:hello" -> "hello"
        size_t colon_index = encoded_value.find(':');
        if (colon_index != std::string::npos) {
            std::string number_string = encoded_value.substr(0, colon_index);
            int64_t number = std::atoll(number_string.c_str());
            std::string str = encoded_value.substr(colon_index + 1, number);
            return json(str);
        }
        else {
            throw std::runtime_error("Invalid encoded value: " + encoded_value);
        }
    }
    else if (encoded_value[0] == 'i') {
        std::string number_string = encoded_value.substr(1, encoded_value.find('e'));
        int64_t number = stoll(number_string);
        return json(number);
    }
    else if (encoded_value[0] == 'l') {
        json list = json::array();
        size_t index = 1; // Start from the character after 'l'

        while (encoded_value[index] != 'e') {
            list.push_back(decode_bencoded(encoded_value.substr(index)));

            // Move index to the next element
            index += list.back().dump().size();
        }

        return list;
    }
    else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "decode") {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
            return 1;
        }
        // You can use print statements as follows for debugging, they'll be visible when running tests.
        //std::cout << "Logs from your program will appear here!" << std::endl;

         std::string encoded_value = argv[2];
         json decoded_value = decode_bencoded(encoded_value);
         std::cout << decoded_value.dump() << std::endl;
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
