#include "location.hpp"

void location::parse_location(location& loc, std::string line) {
    std::istringstream iss2(line);
    std::string key, value;
    iss2 >> key >> value;
    if (key == "location")
        loc.setLocation(value);
    if (key == "method"){
        if (value.empty())
            throw MyException("Error The Method is Empty !");
        std::vector<std::string> x;
        loc.setMethod(x);
        x.push_back(value);
        while (iss2 >> key){
            x.push_back(key);
        }
        loc.setMethod(x);
    } else if (key == "root"){
        if (value.empty())
            throw MyException("Error root is Empty !");
        loc.setRoot(value);
    } else if (key == "upload"){
        if (value.empty())
            throw MyException("Error upload is Empty !");
        loc.setUpload(value);
    } else if (key == "autoindex"){
        if (value.empty())
            throw MyException("Error autoindex is Empty !");
        loc.setAutoindex(value);
    } else if (key == "default"){
        if (value.empty())
            throw MyException("Error default is Empty !");
        loc.setDefault(value);
    } else if (key == "redirection"){
        if (value.empty())
            throw MyException("Error redirection is Empty !");
        loc.setReturn(value);
    }
}

void location::print_location_elements(void){
    std::cout << "******* location elements *********" << std::endl;
    std::cout << "location path == " << location_path << std::endl;
    
    std::vector<std::string>::iterator it;
    int i = 1;
    for (it = method.begin(); it != method.end(); ++it){
        std::cout << "Method item == " << i << " == " << *it << std::endl;
        i++;
    }

    std::cout << "root == " << root << std::endl;
    std::cout << "upload == " << upload << std::endl;
    std::cout << "auto index == " << autoindex << std::endl;
    std::cout << "default == " << _default << std::endl;
    std::cout << "redirection == " << redirection << std::endl;
    std::cout << "************************************" << std::endl;
}

bool location::check_config_file(const std::string& filename, char c1, char c2) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return false;
    }
    int count_open = std::count(std::istreambuf_iterator<char>(file),
                                std::istreambuf_iterator<char>(), c1);
    file.clear();
    file.seekg(0, std::ios::beg);
    int count_close = std::count(std::istreambuf_iterator<char>(file),
                                 std::istreambuf_iterator<char>(), c2);
    if (count_open != count_close) {
        std::cerr << "Error: unmatched number of opening and closing braces" << std::endl;
        return false;
    }
    return true;
}
