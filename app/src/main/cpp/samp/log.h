#pragma once

#include <sstream>
#include <vector>
#include <android/log.h>
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "AX", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "AXL", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "AXL", __VA_ARGS__)

struct TraceInfo
{
	std::string name;
	std::vector<std::string> params;
};

class Log
{
public:
	static void traceLastFunc(const std::string& func_name);

	template<typename T>
	static void addParameter(const std::string& func_param_name, T value)
	{
		//auto params = m_listTracedFuncs.front().params;

		//std::stringstream ss;
		//ss << func_param_name << " = " << value;
		//params.push_back(ss.str());
		//LOGI(ss.str().c_str()); 
	};

private:

	//static std::list<TraceInfo> m_listTracedFuncs;
};