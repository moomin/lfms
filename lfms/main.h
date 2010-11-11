#define LFMS_VERSION "0.1"
#define LFMS_CLIENT_STRING "tst"

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

int read_session(string *);
int handshake();
int submit_track();
int now_playing();
