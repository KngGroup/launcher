#include <stdio.h>
#include <stdlib.h>
#include <libwebsockets.h>
#include <json-glib/json-glib.h>
#include <gobject/gtype.h>

int force_exit = 0;
char *json;

struct lws_context *context;

enum protocols {
    LAUNCHER = 0,
};

struct per_session_data {
    int number;
};

static const struct lws_extension exts[] = {
    { NULL, NULL, NULL /* terminator */ }
};

char *handle_request(char *json_request) {
    lwsl_notice("JSON: %s\n", json_request);
}

int callback_launcher(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 512];
    struct per_session_data *pss = (struct per_session_data *) user;
    unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
    int n, m;

    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            lwsl_notice("Client connected\n");
            pss->number = 0;
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            n = sprintf((char *) p, "%d", pss->number++);
            m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
            if (m < n) {
                lwsl_err("ERROR %d writing to di socket\n", n);
                return -1;
            }
            lws_rx_flow_control(wsi, 1);
            break;

        case LWS_CALLBACK_RECEIVE:
            lwsl_notice("Callback received: %s\n", in);
            handle_request(in);
            
            lws_rx_flow_control(wsi, 0);
            lws_callback_on_writable(wsi);
            break;

        default:
            break;
    }

    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "launcher-protocol",
        callback_launcher,
        sizeof (struct per_session_data),
        4096,
    },
    { NULL, NULL, 0, 0} /* terminator */
};

void sighandler(int sig) {
    force_exit = 1;
    lws_cancel_service(context);
}

int main(int argc, char **argv) {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);
    g_type_init();
    
    info.port                     = 9999;
    info.protocols                = protocols;
    info.ssl_cert_filepath        = NULL;
    info.ssl_private_key_filepath = NULL;
    info.extensions               = exts;

    info.gid = -1;
    info.uid = -1;
    info.max_http_header_pool = 1;
    info.options = 0;

    signal(SIGINT, sighandler);

    context = lws_create_context(&info);
    if (context == NULL) {
        lwsl_err("libwebsocket init failed\n");
        return -1;
    }

    while (lws_service(context, 100) >= 0 && !force_exit) {
    }

    lws_context_destroy(context);

    return 0;
}