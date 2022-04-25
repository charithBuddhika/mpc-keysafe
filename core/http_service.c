#include "fio_cli.h"
#include "main.h"
#include "fio_tls.h"

/* TODO: edit this function to handle HTTP data and answer Websocket requests.*/
static void on_http_request(http_s *h) {
  /* set a response and send it (finnish vs. destroy). */
  // printf("Test Print: %s \n", h->method);
  FIOBJ r = http_req2str(h);
  fprintf(stderr, "%s\n", fiobj_obj2cstr(r).data);
  http_send_body(h, "blob=123456789", 14);
}

/* starts a listeninng socket for HTTP connections. */
void initialize_http_service(void) {
  fio_tls_s *tls = NULL;
  if (fio_cli_get_bool("-tls")) {
  char local_addr[1024];
  char* public_key = {"-----BEGIN PUBLIC KEY-----MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAugDaVqkspCvi0kCo0dLYS6CgdL4WZEwoRbbwXCgyk8cNYcYOFIwYOWGbOBWsAsRHC2Z226OlniNOd/YOUmtTx6JL56UQaK6NRc0jOp2yucX8i99miTryijbV0LCya2+XZblKXJVX+mJ9nMkAG9VJGYrM3gmRT4g6s+aJdTnTZZRIKzVvP/i+k2F3zV7mVgtBPbPzaB2tM8Ovzf/1pi8OYIn1K1Lt9y51SGPG5aU8WgDdAuLYmYRLGnJkzgLhUxZJmEQGu0a2e+omGKQlSRos+8aOpXReKv6hbar9HzGLOzmtqDUVAhAUTmP5EfLYPRg7Z+LWZolcszuLFQaS7bLA/wIDAQAB-----END PUBLIC KEY-----"};
  char* private_key = {"-----BEGIN RSA PRIVATE KEY-----Proc-Type: 4,ENCRYPTEDDEK-Info: DES-EDE3-CBC,8025560CA986824C 27QFyUMSmtDMvg5zKjtoMmGw1SibgU/mnBfTxZWL31lrCoFC1c6XOT7Y9+gOa3zPIVP5wIQsKjLkM7q3HDF/zGnfCZhCV/Pk868WggdmDnboJLn6XfjxeNVLSfO/WQTFSgapmOm82GAXHMLOZxuo7wi6Oe7F3FJCzOQU9FpHCPLLLIsXMzzSbHgbi3FAeL+rCIJ2bUj4jDTVsdNRmqyWdKwvLUmI1/+BIVkiPM6lvUEUyqR+ewflOcdDO/nSX18RIQctILnWIUaDrlGZ7Hg15Yqu8tMWpotwt49Xbheknv2nU+p4cbMl3W0MLm7JtsuW+1O12mQngEap9wTsCGhEDrLFCboJFqFRbS0E5/rb1bQdH6mrpp6Yf8b/Q8zneHL00WI7geEP5bLbCOWzvDZ9DQzDUW9r0K9wz6oALfGxKdV1DoiuZ8wtI0gH4YVE/mDpkM+11JMHogqGZsFWC2vcS/MYf6h+tCCl6EFVixjBCdGrS7/YDd/zuBTkdSoIU3UeC2de7ga/P7p+hfPyPTFCobkNalcP1CjZQtwo3haf3eISbkHgx24BA/Hu1oGRcOwFvg/8VVpEH/hcCzG6Xr6lIJ77AL/2QLqcN8mxm9/1LbLRBndXlJHzwduLligkjHFPMvyCzk7QybCeQLlEa8jWqkLoJBxXLD0BX2B3iolgza/55W3YZs6jpT4QGmsn+o6x/rVOEqQWX5FlyLrP/ilGjBvOrv+8J131nmQ2uWPPCkLVK+re2NSod04eUQqJ8pjwTDvQ5eD2t8jLfvdbO4Pz0q7GA5k97JANR6fyAUJy1Cl/Uz4WJeUrS6HD6WiNmzda1h7czCz6ual/O+fkf1nuviZG1RSzSL+9biVVzluvoaVOaqQ2e1oOzC9fM9Ooqqp1UQgFDiVolrpUIYO/LbQc5/OHCSQgrkBm6uMhSv4vHZjxz/skPM30omq8KCle7qLFqqom2slFI12i91q0QPI1PJ78jFd+t7M/iXXg37mu0qRBZWzBddh8K6PIqQNQ7pZT/uFOWPq7zAs6jO5yj1zXxuEtwtarZQiHra4P4zx4M6iR6TrJj55p4/fcdqIpEkdEK4nrEBKcyrDmUPKEZ82rQZolfIiUv9IuTCHrfDXLN0UNDJpzJP+BNpZRN5E+TiYxeGXbWfeVJYeJATEm9mDeWDDgVGa6gqwY87wBIa9etLRocR5Ka5vNBwdQeKvl+hLLAkIi3Nj8OTQlQmccT7hBdkXLjnjjT5W0faPHvY76udaSq1+LcSvfrw5BE7+sxTMsePY1LkzM7RmGeo/lfbz+t/7mTVFZ2WiGkwXIQlW3D+avGAVhaRmHPQPlhw5vVaTzmnlrnzRT1WHQo+k6EGVUQQGlV5F4w/0DUUV6TestBg13VfuWubw3MmFAWNSNIKzA3e11MNuchzx+YCKUE8w9ctQYDnQaebB1Ktcggqipes07THeik9Fkn9rlL0TBKpR67fKM/aMI2eM1IzReGE65oAsCyw9ClK70Ii0JcQ57Q5VwaDd7+/TIM2CBwPk357TrxZatCCXzn2zJyOaPmHx9knepKaZsyUw5TetkSRwmtZRYdbudtouGokcUOhhtZ7NQ-----END RSA PRIVATE KEY-----"};
  local_addr[fio_local_addr(local_addr, 1023)] = 0;
  tls = fio_tls_new(local_addr, public_key, private_key, NULL);
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
