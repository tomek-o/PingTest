//---------------------------------------------------------------------------

#ifndef PingH
#define PingH
//---------------------------------------------------------------------------

namespace Ping
{
	int init(void);
	int run(const char* ip, unsigned int requestSize);
	void shutdown(void);
}

#endif
