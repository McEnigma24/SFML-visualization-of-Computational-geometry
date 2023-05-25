#pragma once
#include "Header.h"


ScopeInfo::ScopeInfo(string n) {};
ScopeInfo::~ScopeInfo()
{
	cout << name << endl;

	int i = 1;
	for (auto& triangle : info_triangle)
	{
		cout << i << endl; i++;
		triangle.infoAboutPosition();
		cout << endl;
	}
}