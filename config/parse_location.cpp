#include "location.hpp"

void location::parse_location(location& loc, std::string line) {
    std::istringstream iss2(line);
    std::string key, value;
    iss2 >> key >> value;
    if (key == "location")
        loc.setLocation(value);
    if (key == "method"){
        std::vector<std::string> x;
        loc.setMethod(x);
        x.push_back(value);
        while (iss2 >> key){
            x.push_back(key);
        }
        loc.setMethod(x);
    } else if (key == "root"){
        loc.setRoot(value);
    } else if (key == "upload"){
        loc.setUpload(value);
    } else if (key == "autoindex"){
        loc.setAutoindex(value);
    } else if (key == "default"){
        loc.setDefault(value);
    } else if (key == "return"){
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
    std::cout << "return == " << _return << std::endl;
    std::cout << "************************************" << std::endl;
}