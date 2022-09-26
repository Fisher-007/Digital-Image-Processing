#pragma once
#ifndef PROCESSING_H
#define PROCESSING_H

#include "img.h"


class Processing {

public:
	void DisplayEffect(const Img& img_origin, const Img& img_processed);
	Img FourierTransform(const Img& img);
	Img FourierInverseTransform(const Img& img);
	Img HistogramEqualization(const Img& img);
};


#endif