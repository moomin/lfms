#define LFMS_VERSION "1.0"
#define LFMS_CLIENT_STRING "tst"

typedef struct cfg {
    //command line parameters
    bool   display_version;
    bool   display_help;
    string config_file;
    string queue_file;
    string session_file;
    char   mode;

    //config file parameters
    string username;
    string password;
    string host;
    int    port;

    //static parameters
} lfmscfg;

typedef struct lastfm_session {
    bool is_active;
    string status;
    string id;
    string now_playing_url;
    string submission_url;
} lastfm_session;

typedef struct lastfm_track {
    string name;
} track;

int parse_command_line(int, char *[]);
int read_config(string *);
int read_session(string *);
int handshake();
int submit_track();
int now_playing();
int resolve_dirs();
