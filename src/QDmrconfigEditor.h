#ifndef QDMRCONFIGEDITOR_H
#define QDMRCONFIGEDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern "C" const char version[] = VERSION;
const char* copyright;

extern char* optarg;
extern int optind;

extern "C" void radio_connect();
extern "C" void radio_download(void);
extern "C" void radio_print_version(FILE* out);
extern "C" void radio_print_version(FILE* out);
extern "C" void radio_disconnect(void);
extern "C" void radio_save_image(const char* filename);
extern "C" void radio_print_config(FILE* out, int verbose);

#endif
