#pragma once
#ifndef BASE_IMG_H
#define BASE_IMG_H

#include <string>
// using namespace std;
using std::string;


typedef unsigned char uchar;


class BaseImg {

protected:
	string file_path;
	BaseImg();
	BaseImg(string file, string location = "data/");
	~BaseImg();
	friend class Processing;

public:
	const string get_file_path() const;
};

#endif