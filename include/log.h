/**
 * @file
 * Logging module header.
 *
 * The Logging module defines a set of functions that can be used to log
 * security events. It supports a hierarchy of logging levels which can be configured to determine
 * the severity of events that are logged, and those below the threshold that are discarded.
 * Implementors should use a well established logging library
 * as it is quite difficult to create a high-performance logger.
 *
 * The logging levels defined by this interface (in descending order) are:
 * <ul>
 * <li>fatal (highest value)</li>
 * <li>error</li>
 * <li>warning</li>
 * <li>info</li>
 * <li>debug</li>
 * <li>trace (lowest value)</li>
 * </ul>
 *
 * ESAPI also allows for the definition of the type of log event that is being generated. The Logging module
 * predefines 4 types of Log events: SECURITY_SUCCESS, SECURITY_FAILURE, EVENT_SUCCESS, EVENT_FAILURE.
 * Your implementation can extend or change this list if desired.
 *
 * This Logger allows callers to to submit events at different severity levels.<br>
 * <br>Implementors of this interface should:
 *
 * <ol>
 * <li>provide a mechanism for setting the logging level threshold that is currently enabled. This usually works by logging all
 * events at and above that severity level, and discarding all events below that level.
 * This is usually done via configuration, but can also be made accessible programmatically.</li>
 * <li>ensure that dangerous HTML characters are encoded before they are logged to defend against malicious injection into logs
 * that might be viewed in an HTML based log viewer.</li>
 * <li>encode any CRLF characters included in log data in order to prevent log injection attacks.</li>
 * <li>avoid logging the user's session ID. Rather, they should log something equivalent like a
 * generated logging session ID, or a hashed value of the session ID so they can track session specific
 * events without risking the exposure of a live session's ID.</li>
 * <li>record the following information with each event:</li>
 *   <ol type="a">
 *   <li>identity of the user that caused the event,</li>
 *   <li>a description of the event (supplied by the caller),</li>
 *   <li>whether the event succeeded or failed (indicated by the caller),</li>
 *   <li>severity level of the event (indicated by the caller),</li>
 *   <li>that this is a security relevant event (indicated by the caller),</li>
 *   <li>hostname or IP where the event occurred (and ideally the user's source IP as well),</li>
 *   <li>a time stamp</li>
 *   </ol>
 * </ol>
 *
 * Custom logger implementations might also:
 * <ol start="6">
 * <li>filter out any sensitive data specific to the current application or organization, such as credit cards,
 * social security numbers, etc.</li>
 * </ol>
 *
 * A Log4c based logger is the default implementation. This implementations implements requirements #1 thru #5 above.<br>
 * <br>
 * Customization: It is expected that most organizations will implement their own custom Logging module in
 * order to integrate ESAPI logging with their logging infrastructure. The ESAPI Reference Implementation
 * is intended to provide a simple functional example of an implementation.
 *
 * @since January 30, 2011
 */

#include <stdlib.h>
#include "log4c.h"
#include "user.h"

#ifndef _LOG_H_
#define _LOG_H_

#ifndef DEFAULT_LOG_CATEGORY_NAME
#define DEFAULT_LOG_CATEGORY_NAME "root"
#endif

#define SECURITY_LOG_PREFIX "SECURITY:"
#define MAX_MESSAGE_LEN 256

enum EVENT_TYPE {
	SECURITY_SUCCESS, SECURITY_FAILURE, EVENT_SUCCESS, EVENT_FAILURE
};

extern char *eventTypeNames[];

/**
 * Enables logging for this process.
 */
extern int esapi_open_log();

/**
 * Disables logging for this process.
 */
extern int esapi_close_log();

/**
 * Log a TRACE level message.
 */
extern void esapi_log_trace(const user *, int, const char *, ...);

/**
 * Log a DEBUG level message.
 */
extern void esapi_log_debug(const user *, int, const char *, ...);

/**
 * Log an INFO level message.
 */
extern void esapi_log_info(const user *, int, const char *, ...);

/**
 * Log a WARN level message.
 */
extern void esapi_log_warn(const user *, int, const char *, ...);

/**
 * Log an ERROR level message.
 */
extern void esapi_log_error(const user *, int, const char *, ...);

/**
 * Log a CRITICAL level message.
 */
extern void esapi_log_critical(const user *, int, const char *, ...);

/**
 * Log a FATAL level message.
 */
extern void esapi_log_fatal(const user *, int, const char *, ...);

#endif /* _LOG_H_ */
