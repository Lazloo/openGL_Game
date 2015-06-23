#pragma once
#include "basicIncludes\basicModelOperations.h"
class invisibleBlock:public basicModelOperations
{
public:
	invisibleBlock(void);
	~invisibleBlock(void);

private:
	double ModelWidth;
	double ModelHeight;
};

