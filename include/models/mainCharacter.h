#pragma once
#include "basicIncludes\basicModelOperations.h"
class mainCharacter:public basicModelOperations
{
public:
	mainCharacter(void);
	~mainCharacter(void);

private:
	double ModelWidth;
	double ModelHeight;
};

