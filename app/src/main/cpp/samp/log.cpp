#include <string>
#include <sstream>
#include <list>
#include <vector>
#include "log.h"

#define MAX_TRACED_FUNCS 5

//std::list<TraceInfo> Log::m_listTracedFuncs;

void Log::traceLastFunc(const std::string& func_name)
{
	//LOGI("Traced: %s", func_name.c_str());

	//TraceInfo info;
	//info.name = func_name;

	//if (m_listTracedFuncs.size() > MAX_TRACED_FUNCS) m_listTracedFuncs.pop_front();
	//m_listTracedFuncs.push_back(info);
}