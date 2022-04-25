#include "fio_cli.h"
#include "main.h"
#include "fio_tls.h"
#include <stdio.h>
#include <string.h>

#define MAX_STRING_LEN 250

static void on_http_request(http_s *h) {
  FIOBJ r = http_req2str(h);
  int blob_offset = 5;
  size_t blob_str_len;
  char* post_data;
  char* blob_str;
  char* blob_encoded;
  char* blob_final = NULL;

  post_data = malloc(strlen(fiobj_obj2cstr(r).data));
  post_data = fiobj_obj2cstr(r).data;

  fprintf(stderr, "%s\n", post_data);

  //extract the blob
  printf("Extracting Blob..\n");
  blob_str = malloc(sizeof(uint8_t) * MAX_STRING_LEN);
  blob_str = strstr(post_data, "Blob=") + blob_offset;
  blob_str_len = strlen(blob_str);

  //base64 decode
  printf("Base64 Decoding..\n");
  fio_base64_decode(NULL,blob_str,blob_str_len);
  printf("Base64 Decoding.. DONE\n");
  fprintf(stderr, "%s\n", blob_str); // Decoded String

  //Fetching the action
  printf("Fetching the Action..\n");
  blob_encoded = malloc(sizeof(uint8_t) * MAX_STRING_LEN);
  blob_final = malloc(sizeof(uint8_t) * MAX_STRING_LEN);
  strcat(blob_final,"Blob=");
 
  if(blob_str_len != 0)
  {
    if(strstr(post_data, "Action=seal") != NULL)
    {
      printf("Action = Seal..\n");
      //blob_str = SealFun(blob_str) // Encryption with AES256
    }
    else if(strstr(post_data, "Action=unseal") != NULL)
    {
      printf("Action = Unseal..\n");
      //blob_str = UnsealFun(blob_str) // Decryption with AES256
    }
    printf("Base64 Encoding..\n");
    fio_base64_encode(blob_encoded,blob_str,strlen(blob_str));
    strcat(blob_final,blob_encoded);
    printf("Base64 Encoding.. DONE\n");
    fprintf(stderr, "%s\n", blob_final); 
    http_send_body(h, blob_final, strlen(blob_final));
  }
  else{
    char* rq_err = "Empty Blob";
    http_send_body(h, rq_err, strlen(rq_err));
  }

  free(post_data);
  free(blob_str);
  free(blob_encoded);
  free(blob_final);
}

/* starts a listeninng socket for HTTP connections. */
void initialize_http_service(void) {
  fio_tls_s *tls = NULL;
  if (fio_cli_get_bool("-tls")) {
  char local_addr[1024];
  local_addr[fio_local_addr(local_addr, 1023)] = 0;
  tls = fio_tls_new(local_addr, NULL, NULL, NULL);
  }

  /* listen for inncoming connections */
  if (http_listen(fio_cli_get("-p"), fio_cli_get("-b"),
                  .on_request = on_http_request,
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
