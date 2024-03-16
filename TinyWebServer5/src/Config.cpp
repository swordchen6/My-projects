#include "Config.h"

Config::Config()
{
	logOpen = true;
	maxQueueSize = 10000;

	maxSqlConn = 8;
	maxThreadNum = 8;

	port = 9006;
	optLinger = false;

}

void Config::parseArg(int argc,char* argv[])
{
	const char* p = "l:c:s:t:p:o:";
	int opt;
	while ((opt = getopt(argc, argv, p)) != -1)
	{
		switch (opt)
		{
		case 'l':
			logOpen = atoi(optarg);
			break;
		case 'c':
			maxQueueSize = atoi(optarg);
			break;
		case 's':
			maxSqlConn = atoi(optarg);
			break;
		case 't':
			maxThreadNum = atoi(optarg);
			break;
		case 'p':
			port = atoi(optarg);
		case 'o':
			optLinger = atoi(optarg);
			break;
		default:
			break;
		}
	}
}
