#pragma once 
#include "defines.h"

typedef enum log_severity {
    LOG_SEVERITY_TRACE,
    LOG_SEVERITY_DEBUG,
    LOG_SEVERITY_WARN,
    LOG_SEVERITY_ERROR,
} LogSeverity;

void _log_output(LogSeverity severity, const char* msg, ...);

#ifdef ENABLE_TRACE
#define LOG_TRACE(...) _log_output(LOG_SEVERITY_TRACE, __VA_ARGS__)
#else
#define LOG_TRACE(...) ((void)0)
#endif

#ifdef DEBUG
#define LOG_DEBUG(...) _log_output(LOG_SEVERITY_DEBUG, __VA_ARGS__)
#else
#define LOG_DEBUG(...) ((void)0)
#endif

#define LOG_WARN(...) _log_output(LOG_SEVERITY_WARN,  __VA_ARGS__)
#define LOG_ERROR(...) _log_output(LOG_SEVERITY_ERROR, __VA_ARGS__)

