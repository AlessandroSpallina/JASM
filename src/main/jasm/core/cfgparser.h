#ifndef __CONFIG_PARSER_H
#define __CONFIG_PARSER_H

#define __CONFIG_MAXCONN_DEFAULT 3

struct configval {
  int cfg_maxconn;
};

extern int get_values(struct configval* cfg);

#endif //__CONFIG_PARSER_H
