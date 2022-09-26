#pragma once
#ifndef BASE_IMG_H
#define BASE_IMG_H

#include <string>
using namespace std;


class BaseImg {

protected:
	string file_path;

public:
	BaseImg();
	BaseImg(string file, string location = "data/");
	const string get_file_path() const;
	~BaseImg();
};

#endif