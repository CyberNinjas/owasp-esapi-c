/**
 * @file
 * UNIX Command Shell Codec module header.
 *
 * @since January 30, 2011
 */

#ifndef _UNIX_CODEC_H
#define _UNIX_CODEC_H

extern char *encode_unix_char(char *, char);
extern char decode_unix_char(const char *, int *, char *);

#endif
