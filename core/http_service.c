#include "fio_cli.h"
#include "main.h"
#include "fio_tls.h"
#include <stdio.h>
#include <string.h>

#define MAX_STRING_LEN 250

/* cleanup any leftovers */
static void cleanup(void);

/* reusable objects */
static FIOBJ HTTP_HEADER_SERVER;
static FIOBJ HTTP_VALUE_SERVER;


/* *****************************************************************************
Routing
***************************************************************************** */

/* adds a route to our simple router */
static void route_add(char *path, void (*handler)(http_s *));

/* routes a request to the correct handler */
static void route_perform(http_s *);

/* cleanup for our router */
static void route_clear(void);

/* *****************************************************************************
Request handlers
***************************************************************************** */

/* handles operational requests */
static void on_request_op(http_s *h);

/* handles key requests */
static void on_request_key(http_s *h);


/* starts a listeninng socket for HTTP connections. */
void initialize_http_service(void) {

  /* sertup routes */
  route_add("/op", on_request_op);
  route_add("/key", on_request_key);

  /* Server name and header */
  HTTP_HEADER_SERVER = fiobj_str_new("server", 6);
  HTTP_VALUE_SERVER = fiobj_str_new("mcp-keysafe " FIO_VERSION_STRING,
                                    strlen("mcp-keysafe " FIO_VERSION_STRING));

  fio_tls_s *tls = NULL;
  if (fio_cli_get_bool("-tls")) {
  char local_addr[1024];
  local_addr[fio_local_addr(local_addr, 1023)] = 0;
  tls = fio_tls_new(local_addr, NULL, NULL, NULL);
  }

  /* listen for inncoming connections */
  if (http_listen(fio_cli_get("-p"), fio_cli_get("-b"),
                  .on_request = route_perform,
                  .max_body_size = fio_cli_get_i("-maxbd") * 1024 * 1024,
                  .ws_max_msg_size = fio_cli_get_i("-max-msg") * 1024,
                  // .public_folder = "/home/charith/Desktop/Fraction/HAAS/project_temp/src/www/",
                  .log = fio_cli_get_bool("-log"),
                  .timeout = fio_cli_get_i("-keep-alive"), .tls = tls,
                  .ws_timeout = fio_cli_get_i("-ping")) == -1) {
    /* listen failed ?*/
    perror("ERROR: facil couldn't initialize HTTP service (already running?)");
    exit(1);
  }

  fio_start(.threads = fio_cli_get_i("-t"), .workers = fio_cli_get_i("-w"));
  fio_tls_destroy(tls);
}


/* *****************************************************************************
Request handlers
***************************************************************************** */

/* handles operational requests */
static void on_request_op(http_s *h) {
  http_parse_body(h);
  fio_str_info_s req = fiobj_obj2cstr(h->body);
  fprintf(stderr, "%s\n", req.data);

  // Parsing the JSON
  FIOBJ data_obj = FIOBJ_INVALID;
  size_t consumed = fiobj_json2obj(&data_obj, req.data, req.len);
  if (!consumed || !data_obj) {
    printf("\nERROR, couldn't parse data.\n");
    exit(-1);
  }

  FIOBJ key_action = fiobj_str_new("Action", 6);
  FIOBJ key_blob = fiobj_str_new("Blob", 4);
  fio_str_info_s action;
  fio_str_info_s blob;
  char* blob_encoded;
  if (FIOBJ_TYPE_IS(data_obj, FIOBJ_T_HASH) && fiobj_hash_get(data_obj, key_action)) {   
    action.data = fiobj_obj2cstr(fiobj_hash_get(data_obj, key_action)).data;
  }
  else{
    printf("\nERROR, Action Key Couldn't Find.\n");
    http_send_body(h, "Action Key Couldn't Find.", 25);
  }

  if (FIOBJ_TYPE_IS(data_obj, FIOBJ_T_HASH) && fiobj_hash_get(data_obj, key_blob)) {   
    blob.data = fiobj_obj2cstr(fiobj_hash_get(data_obj, key_blob)).data;
    //base64 decoding
    printf("%s\n",blob.data);
    fio_base64_decode(NULL,blob.data,strlen(blob.data));
    printf("%s\n",blob.data);
    if(action.data[0] == 's')
    {
      //TODO:blob AES256 encryption
      printf("blob AES256 encryption\n");
    }
    else if(action.data[0] == 'u')
    {
      //TODO:blob AES256 decryption
      printf("blob AES256 decryption\n");
    }
    //base64 encoding
    blob_encoded = malloc(sizeof(uint8_t) * MAX_STRING_LEN);
    fio_base64_encode(blob_encoded,blob.data,strlen(blob.data));
    printf("%s\n",blob_encoded);

    http_set_header(h, HTTP_HEADER_CONTENT_TYPE, http_mimetype_find("json", 4));
    FIOBJ json;
    FIOBJ hash = fiobj_hash_new2(1);
    FIOBJ REQ_BLOB = fiobj_str_new("Blob", 4);
    FIOBJ REQ_BLOB_VAL = fiobj_str_new(blob_encoded, strlen(blob_encoded));
    fiobj_hash_set(hash, REQ_BLOB, fiobj_dup(REQ_BLOB_VAL));
    json = fiobj_obj2json(hash, 0);
    fio_str_info_s tmp = fiobj_obj2cstr(json);
    http_send_body(h, tmp.data, tmp.len);
    fiobj_free(hash);
    fiobj_free(json);
    fiobj_free(REQ_BLOB);
    fiobj_free(REQ_BLOB_VAL);

  }
  else{
    printf("\nERROR, Blob Key Couldn't Find.\n");
    http_send_body(h, "Blob Key Couldn't Find.", 23);
  }
  fiobj_free(key_action);
  fiobj_free(key_blob);
  fiobj_free(data_obj);
  free(blob_encoded);
}

/* handles key requests */
static void on_request_key(http_s *h) {
  http_send_body(h, "on_request_key", 14);
}

/* *****************************************************************************
Routing
***************************************************************************** */

typedef void (*fio_router_handler_fn)(http_s *);
#define FIO_SET_NAME fio_router
#define FIO_SET_OBJ_TYPE fio_router_handler_fn
#define FIO_SET_KEY_TYPE fio_str_s
#define FIO_SET_KEY_COPY(dest, obj) fio_str_concat(&(dest), &(obj))
#define FIO_SET_KEY_DESTROY(obj) fio_str_free(&(obj))
#define FIO_SET_KEY_COMPARE(k1, k2) fio_str_iseq(&(k1), &k2)
#define FIO_INCLUDE_STR
#define FIO_STR_NO_REF
#include <fio.h>
/* the router is a simple hash map */
static fio_router_s routes;

/* adds a route to our simple router */
static void route_add(char *path, void (*handler)(http_s *)) {
  /* add handler to the hash map */
  fio_str_s tmp = FIO_STR_INIT_STATIC(path);
  /* fio hash maps support up to 96 full collisions, we can use len as hash */
  fio_router_insert(&routes, fio_str_len(&tmp), tmp, handler, NULL);
}

/* routes a request to the correct handler */
static void route_perform(http_s *h) {
  /* add required Serevr header */
  http_set_header(h, HTTP_HEADER_SERVER, fiobj_dup(HTTP_VALUE_SERVER));
  /* collect path from hash map */
  fio_str_info_s tmp_i = fiobj_obj2cstr(h->path);
  fio_str_s tmp = FIO_STR_INIT_EXISTING(tmp_i.data, tmp_i.len, 0);
  fio_router_handler_fn handler = fio_router_find(&routes, tmp_i.len, tmp);
  /* forward request or send error */
  if (handler) {
    handler(h);
    return;
  }
  http_send_error(h, 404);
}

/* cleanup for our router */
static void route_clear(void) { fio_router_free(&routes); }


/* cleanup any leftovers */
static void cleanup(void) {
  fio_cli_end();
  fiobj_free(HTTP_HEADER_SERVER);
  fiobj_free(HTTP_VALUE_SERVER);

  route_clear();
}