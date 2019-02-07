#pragma once

#ifdef DSENGINEFRAMEWORK_EXPORTS
#define DSENGINEFRAMEWORK_API __declspec(dllexport)
#else
#define DSENGINEFRAMEWORK_API __declspec(dllimport)
#endif

class DSENGINEFRAMEWORK_API DSFTestFramework
{
public:
	DSFTestFramework(int a, int b);
	~DSFTestFramework();

	int OutputAddResult() const;

private:
	int aVal;
	int bVal;
};

