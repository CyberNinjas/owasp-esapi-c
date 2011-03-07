/**
 * @file
 * A Logging module implementation.
 *
 * @since January 30, 2011
 */

#include <stdlib.h>
#include <string.h>

#define DEFAULT_LOG_CATEGORY_NAME "log4c.category.default"

#include "log.h"

char *eventTypeNames[] = { "SECURITY_SUCCESS", "SECURITY_FAILURE",
		"EVENT_SUCCESS", "EVENT_FAILURE" };

int esapi_open_log() {
	return log4c_init();
}

int esapi_close_log() {
	return log4c_fini();
}

void _log(int priority, const user *u, int type, const char *fmt, ...) {
	va_list va;
	char *message;

	if (fmt) {
		va_start(va, fmt);
		if ((message = (char *) malloc(MAX_MESSAGE_LEN)) != 0) {
			if (vsnprintf(message, MAX_MESSAGE_LEN, fmt, va)) {
				char *username;
				if (u && u->name) {
					username = (char *) u->name;
				} else {
					username = "APPLICATION"; // non-attributable event
				}
				log4c_category_log(
						log4c_category_get(DEFAULT_LOG_CATEGORY_NAME),
						LOG4C_PRIORITY_DEBUG, "[%s %s] %s",
						eventTypeNames[type], username, message);
			}
		}
		free(message);
		va_end(va);
	}
}

/*
 * What about unattributed events?
 */
void esapi_log_trace(const user *u, int type, const char *fmt, ...) {
	va_list va;
	if ((fmt) && (u)) {
		va_start(va, fmt);
		_log(LOG4C_PRIORITY_TRACE, u, type, fmt, va);
		va_end(va);
	}
}

void esapi_log_debug(const user *u, int type, const char *fmt, ...) {
	va_list va;
	if ((fmt) && (u)) {
		va_start(va, fmt);
		_log(LOG4C_PRIORITY_DEBUG, u, type, fmt, va);
		va_end(va);
	}
}

void esapi_log_info(const user *u, int type, const char *fmt, ...) {
	va_list va;
	if ((fmt) && (u)) {
		va_start(va, fmt);
		_log(LOG4C_PRIORITY_INFO, u, type, fmt, va);
		va_end(va);
	}
}

void esapi_log_warn(const user *u, int type, const char *fmt, ...) {
	va_list va;
	if ((fmt) && (u)) {
		va_start(va, fmt);
		_log(LOG4C_PRIORITY_WARN, u, type, fmt, va);
		va_end(va);
	}
}

void esapi_log_error(const user *u, int type, const char *fmt, ...) {
	va_list va;
	if ((fmt) && (u)) {
		va_start(va, fmt);
		_log(LOG4C_PRIORITY_ERROR, u, type, fmt, va);
		va_end(va);
	}
}

void esapi_log_fatal(const user *u, int type, const char *fmt, ...) {
	va_list va;
	if ((fmt) && (u)) {
		va_start(va, fmt);
		_log(LOG4C_PRIORITY_FATAL, u, type, fmt, va);
		va_end(va);
	}
}
