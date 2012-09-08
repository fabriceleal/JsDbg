#ifndef __PEPARSER_H
#define __PEPARSER_H

#include "JsBuffer.h"

namespace PeParser {

	class PeFile {
	public:
		PeFile();
		~PeFile();
	};

	PeFile* ParseFromBuffer(JsBuffer* buf);
};

#endif