// My input structure that stores my games movement data, so I can update it all in one go

#ifndef __userInputs__
#define __userInputs__

struct UserInputs
{
	bool leftKey,rightKey,upKey,downKey;
	bool forwardKey,backKey;
	bool fireKey;
    bool escapeKey;
};

#endif // __userInputs__