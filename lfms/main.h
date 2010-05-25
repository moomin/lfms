typedef struct cfg {
  string username;
  string password;
} lfmscfg;


int parse_command_line(int, char *[], string *);
int read_config(string *);
