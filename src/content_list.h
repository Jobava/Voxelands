#ifndef _CONTENT_LIST_H
#define _CONTENT_LIST_H

#include <vector>
#include <stdint.h>

namespace lists {

struct ListData {
	content_t content;
	uint16_t count;
	uint16_t data;

	ListData()
	{}

	ListData(content_t c, uint16_t cc, uint16_t cd):
		content(c),
		count(cc),
		data(cd)
	{}
};

void add(std::string name, content_t c, uint16_t count=1, uint16_t data=0);
std::vector<ListData> &get(std::string name);

};

#endif
