#include <Windows.h>

class JsBuffer {
public:
	long m_Length;
	BYTE* m_Buffer;

	JsBuffer(BYTE* buffer, long length) : m_Length(0), m_Buffer(NULL)
	{
		if(length <= 0)
		{
			throw "Couldnt create buffer of length zero!";
		}

		if(buffer == NULL)
		{
			throw "Couldnt create buffer with ptr NULL!";
		}

		m_Buffer = buffer;
		m_Length = length;
	}

	~JsBuffer(){
		if(m_Buffer != NULL)
		{
			free(m_Buffer);
		}
	}
};