#ifndef _CONTENT_LIST_H
#define _CONTENT_LIST_H

#include <vector>

namespace lists {

void add(std::string name, content_t c);
std::vector<content_t> &get(std::string name);

};

#endif
